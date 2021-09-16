#include "file.h"
#include "devlib/util.h"
#include <QDir>
#include <QFileInfo>
#include <fstream>
#include <iostream>

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

QString processLine(QString line, const Device &device)
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
            if (Device::textualAttributeNames().contains(match))
                replacement = device[match];
            else
                ; // TODO log
            // line = "void " + "MyDevice" + "()"
            line = line.left(i) + replacement + line.mid(i + len + 1);
            i += replacement.length();
        }
    }
    return line;
}

// Helper functions

QString indented(QString str)
{
    str.replace("\n", "\n    ");
    return "    " + str;
}

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

// C++ symbol writers

QString enumDefinition(const ValueSpec &spec)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("enum %1 {\n").arg(spec.getValueType());
    stream << indented(spec.getValueRange().join(", "));
    stream << "\n};\n";
    return str;
}

QString functionDeclaration(const SingleFunction &fun)
{
    QString str;
    QTextStream stream(&str);
    ValueSpec spec = fun.getValueSpec();
    stream << QStringLiteral("void %1(%2 value);\n")
              .arg(fun.getName())
              .arg(spec.getValueType());
    return str;
}

QString functionDefinition(const SingleFunction &fun)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("void %1(%2 value)\n")
              .arg(fun.getName())
              .arg(fun.getValueSpec().getValueType())
           << indented("// TODO implement")
           << "\n}\n";
    return str;
}

QString dataDeclaration(const Data &data)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("%1 %2() const;\n")
              .arg(data.getValueSpec().getValueType())
              .arg(data.getName());
    return str;
}

QString dataDefinition(const Data &data)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("%1 %2()\n")
              .arg(data.getValueSpec().getValueType())
              .arg(data.getName())
           << indented("// TODO implement")
           << "\n}\n";
    return str;
}

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

// BULK WRITERS

void writeEnums(QTextStream &stream, QList<ValueSpec> enums)
{
    for (auto &e : enums) {
        stream << enumDefinition(e);
        stream << "\n";
    }
}

void writeFunctionDeclarations(QTextStream &stream, QList<const SingleFunction*> functions)
{
    for (auto *f : functions)
        stream << functionDeclaration(*f);
}

void writeDataDeclarations(QTextStream &stream, QList<const Data*> functions)
{
    for (auto *d : functions)
        stream << dataDeclaration(*d);
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
 * write(device, "device.h.in", "device.h");
 * ~~~
 */
void writeDeviceHeader(const Device& device, const QString& in, const QString& out)
{
    QFile inFile(in);
    QFile outFile(out);

    if (!inFile.open(QIODevice::ReadOnly))
        throw std::runtime_error("Could not open input file");
    if (!outFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("Could not open output file");

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
            outStream << indented(str);
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
            outStream << processLine(line, device) << Qt::endl;
    }

    inFile.close();
    outFile.close();
}

void writeDeviceImpl(const Device &device, const QString &out)
{
    QFile outFile(out);

    if (!outFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("Could not open output file");

    auto functions = sweepDeviceFunctions(device);
    auto data = device.getData();

    QTextStream outStream(&outFile);
    outStream << "#include \"device.h\"\n\n";

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
