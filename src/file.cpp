#include "file.h"
#include "devlib/util.h"
#include <QDir>
#include <QFileInfo>
#include <fstream>
#include <iostream>
#include "generator.h"

// helpers
void openOutputFile(QFile &out)
{
    if (!out.open(QIODevice::WriteOnly))
        throw std::runtime_error(QStringLiteral("Could not open output file '%1'")
                                 .arg(out.fileName()).toStdString());
}
void openInOutFiles(QFile &in, QFile &out)
{
    if (!in.open(QIODevice::ReadOnly))
        throw std::runtime_error(QStringLiteral("Could not open input file '%1'")
                                 .arg(in.fileName()).toStdString());
    openOutputFile(out);
}
QString substituteInLine(QString line, const QMap<QString, QString> &substitutions)
{
    // This code will be explained by an example assuming line == "void $name()".
    // Also assume that device.getName() == "MyDevice".
    QRegExp validIds = validIdentifiers();
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == '$') {
            // restOfLine = "name()"
            QString restOfLine = line.mid(i+1);
            // The matched substring should be "$name"
            int firstMatch = validIds.indexIn(restOfLine);
            if (firstMatch != 0)
                continue;
            // match = "name"
            QString match = validIds.cap();
            // len = 4
            int len = validIds.matchedLength();
            QString replacement;
            if (substitutions.contains(match))
                replacement = substitutions[match];
            else
                std::cerr << QStringLiteral("devconf: warning: $%1 not found, expanding to empty string")
                             .arg(match).toStdString() << std::endl;
            // line = "void " + "MyDevice" + "()"
            line = line.left(i) + replacement + line.mid(i + len + 1);
            i += replacement.length();
        }
    }
    return line;
}
bool matchesPlaceholder(const QString &str, const QString &placeholder)
{
    QRegExp regex(QStringLiteral("\\s*/\\*\\*\\* %1 \\*\\*\\*/\\s*").arg(placeholder));
    return regex.exactMatch(str);
}
/**
 * @brief Find all enum ValueSpecs in the `device` and return them.
 */
QList<ValueSpec> sweepDeviceEnums(const Device &device)
{
    // We use QMap so we can discard duplicates
    QMap<QString, ValueSpec> enums;
    for (auto *fun : device.getFunctions()) {
        for (auto *sfun : fun->getSingleFunctions()) {
            const ValueSpec &spec = sfun->getValueSpec();
            if (spec.isCustomEnum())
                enums[spec.getValueType()] = spec;
        }
    }
    for (auto *data : device.getData()) {
        const ValueSpec &spec = data->getValueSpec();
        if (spec.isCustomEnum())
            enums[spec.getValueType()] = spec;
    }
    return enums.values();
}
QList<const SingleFunction*> sweepDeviceFunctions(const Device &device)
{
    QList<const SingleFunction*> functions;
    for (auto *fun : device.getFunctions()) {
        for (auto *sfun : fun->getSingleFunctions()) {
            functions << sfun;
        }
    }
    return functions;
}

/**
 * Copy the file `source` into the file `destination`, thereby creating the
 * necessary output directory structure if necessary.
 */
bool copyFile(const QString& source, const QString& dest)
{
    QString destPath = QFileInfo(dest).absolutePath();
    if (!QDir(destPath).exists() && !QDir().mkpath(destPath))
        return false;

    return QFile(source).copy(dest);
}

/**
 * @brief Copy `source` file to `dest` file with `substitutions` applied.
 * @param substitutions The keys are names of the substitution variables and the
 * values are the strings with which to substitute them. Substitution variables
 * must start with a '$' in the `source` file.
 */
void substituteInFile(const QString &source, const QString &dest,
                      const QMap<QString, QString> &substitutions)
{
    QFile inFile(source);
    QFile outFile(dest);

    openInOutFiles(inFile, outFile);
    QTextStream inStream(&inFile), outStream(&outFile);

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        outStream << substituteInLine(line, substitutions) << Qt::endl;
    }

    inFile.close();
    outFile.close();
}

/**
 * Populate the template from file `in` with the data from `device` and write
 * the result to the file `out`.
 *
 * @note `in` and `out` are file paths.
 *
 * **Example:**
 * ~~~
 * Device device = jsonParseDevice("factory_device.json");
 * write(device, "iot_device.h.in", "iot_device.h");
 * ~~~
 */
void writeDeviceHeader(const Device& device, const QString& in, const QString& out)
{
    QFile inFile(in);
    QFile outFile(out);

    openInOutFiles(inFile, outFile);

    auto enums = sweepDeviceEnums(device);
    auto functions = sweepDeviceFunctions(device);
    auto data = device.getData();

    QTextStream inStream(&inFile), outStream(&outFile);

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        QString str;
        QTextStream stream(&str);
        if (matchesPlaceholder(line, "Enums")) {
            stream << "// Enums\n";
            writeEnums(stream, enums);
            outStream << str;
        } else if (matchesPlaceholder(line, "Function declarations")) {
            stream << "// Device functions\n";
            writeFunctionDeclarations(stream, functions);
            outStream << indented(str);
        } else if (matchesPlaceholder(line, "Data declarations")) {
            stream << "// Device data\n";
            writeDataDeclarations(stream, data);
            outStream << indented(str);
        }
        else
            outStream << substituteInLine(line, device.getAttributes()) << Qt::endl;
    }

    inFile.close();
    outFile.close();
}

void writeDeviceImpl(const Device &device, const QString &out)
{
    QFile outFile(out);

    openOutputFile(outFile);

    auto functions = sweepDeviceFunctions(device);
    auto data = device.getData();

    QTextStream outStream(&outFile);
    outStream << "#include \"iot_device.h\"\n"
              << "#include <mbed.h>\n\n";

    outStream << "// Device functions\n\n";
    for (auto *f : functions)
        outStream << functionDefinition(*f)
                  << "\n";

    outStream << "// Device data\n\n";
    for (auto *d : data)
        outStream << dataDefinition(*d)
                  << "\n";

    outStream << "void Device::initialize()\n"
                 "{\n"
                 "    // TODO\n"
                 "}\n";

    outFile.close();
    return;
}

void writeMessageHandlers(const QString &in, const QString &out, const Device &device)
{
    QFile inFile(in);
    QFile outFile(out);

    openInOutFiles(inFile, outFile);
    QTextStream inStream(&inFile), outStream(&outFile);

    auto functions = sweepDeviceFunctions(device);
    auto data = device.getData();

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        if (matchesPlaceholder(line, "Message handler declarations")) {
            outStream << "// Message handlers\n";
            for (auto *f : functions)
                outStream << messageHandlerDeclaration(*f);
            for (auto *d : data)
                outStream << messageHandlerDeclaration(*d);
        } else if (matchesPlaceholder(line, "Message handler implementations")) {
            outStream << "// Message handlers\n";
            for (auto *f : functions)
                outStream << messageHandlerDefinition(*f) << "\n";
            for (auto *d : data)
                outStream << messageHandlerDefinition(*d) << "\n";
        }
        else {
            outStream << line << Qt::endl;
        }
    }

    inFile.close();
    outFile.close();
}

void writeMqttWrapperImpl(const QString &in, const QString &out, const Device &device)
{
    QFile inFile(in);
    QFile outFile(out);
    openInOutFiles(inFile, outFile);
    QTextStream inStream(&inFile), outStream(&outFile);

    auto functions = sweepDeviceFunctions(device);
    auto data = device.getData();

    QStringList cppFunctions;
    cppFunctions.reserve(functions.size() + data.size());
    for (const auto *f : functions)
        cppFunctions.append(f->getName());
    for (const auto *d : data)
        cppFunctions.append(d->getName());

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        if (matchesPlaceholder(line, "mqttAutoSubscribe"))
            outStream << mqttAutoSubscribeImpl(cppFunctions);
        else {
            // Substitute max number of subscribe handlers
            QString num = QString::number(cppFunctions.size() + 5);
            outStream << substituteInLine(line, {{"maxSubscriptions", num}})
                      << Qt::endl;
        }
    }

    inFile.close();
    outFile.close();
}
