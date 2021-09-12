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
void writeDevice(const Device& device, const QString& in, const QString& out)
{
    using namespace std;
    ifstream in_file(in.toStdString());
    ofstream out_file(out.toStdString());
    string wordToReplace[] = { "$vendorId", "$model", "$serialNo", "$name" };

    QString wordToReplaceWith[] = {
        device.getVendorId(), device.getModel(), device.getSerialNo(), device.getName()
    };

    if (!in_file)
        throw std::runtime_error("Could not open input file");

    if (!out_file)
        throw std::runtime_error("Could not open output file");

    string line;

    while (getline(in_file, line)) {
        out_file << processLine(QString::fromStdString(line), device).toStdString() << endl;
    }

    in_file.close();
    out_file.close();
}
