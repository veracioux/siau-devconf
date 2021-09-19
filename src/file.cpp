#include "file.h"
#include "devlib/util.h"
#include <QDir>
#include <QFileInfo>
#include <fstream>
#include <iostream>
#include "generator.h"

/**
 * Copy the file `source` into the file `destination`, thereby creating the
 * necessary output directory structure if necessary.
 */
bool copyFile(const QString& source, const QString& destination)
{
    QString destPath = QFileInfo(destination).absolutePath();
    if (!QDir(destPath).exists() && !QDir().mkpath(destPath))
        return false;

    return QFile(source).copy(destination);
}

QString processLine(QString line, const QMap<QString, QString> &attributes)
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
            if (attributes.contains(match))
                replacement = attributes[match];
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

// Helper functions

bool matchesPlaceholder(const QString &str, const QString &placeholder)
{
    QRegExp regex(QStringLiteral("\\s*/\\*\\*\\* %1 \\*\\*\\*/\\s*").arg(placeholder));
    return regex.exactMatch(str);
}

QList<const SingleFunction*> extractSingleFunctions(const Function *fun)
{
    QList<const SingleFunction*> singleFuncs;

    if (fun->isSingleFunction())
        singleFuncs = { (SingleFunction*) fun };
    else
        singleFuncs = ((const MultiFunction*) fun)->getSubfunctions();

    return singleFuncs;
}

// C++ symbol sweepers

/**
 * @brief Find all enum ValueSpecs in the `device` and return them.
 */
QList<ValueSpec> sweepDeviceEnums(const Device &device)
{
    // We use QMap so we can discard duplicates
    QMap<QString, ValueSpec> enums;
    for (auto *fun : device.getFunctions()) {
        for (auto *sfun : extractSingleFunctions(fun)) {
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
        for (auto *sfun : extractSingleFunctions(fun)) {
            functions << sfun;
        }
    }
    return functions;
}

// helper

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
            outStream << processLine(line, device.getAttributes()) << Qt::endl;
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
    outStream << "#include \"iot_device.h\"\n\n";

    outStream << "// Device functions\n\n";
    for (auto *f : functions) {
        outStream << functionDefinition(*f)
                  << "\n";
    }

    outStream << "// Device data\n\n";
    for (auto *d : data) {
        outStream << dataDefinition(*d)
                  << "\n";
    }

    outFile.close();
    return;
}

void writeMqttImpl(const QString &in, const QString &out)
{
    QFile inFile(in);
    QFile outFile(out);

    openInOutFiles(inFile, outFile);

    QString mqttTopic;

    inFile.close();
    outFile.close();
}

void writeUserDeviceData(const UserData &data, const QString &in, const QString &out)
{
    QFile inFile(in);
    QFile outFile(out);

    openInOutFiles(inFile, outFile);
    QTextStream inStream(&inFile), outStream(&outFile);

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        outStream << processLine(line, data.getAttributes()) << Qt::endl;
    }

    inFile.close();
    outFile.close();
}
