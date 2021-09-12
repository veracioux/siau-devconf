#include "file.h"
#include <QDir>
#include <QFileInfo>
#include <fstream>
#include <iostream>

bool copyFile(const QString& source, const QString& destination)
{
    QString destPath = QFileInfo(destination).absolutePath();
    if (!QDir(destPath).exists() && !QDir().mkpath(destPath))
        return false;

    return QFile(source).copy(destination);
}

void write(const Device& data, const QString& in, const QString& out)
{
    using namespace std;
    ifstream in_file(in.toStdString());
    ofstream out_file(out.toStdString());
    string wordToReplace[] = { "$vendorId", "$model", "$serialNo", "$name" };

    QString wordToReplaceWith[] = {
        data.getVendorId(), data.getModel(), data.getSerialNo(), data.getName()
    };

    if (!in_file)
        throw std::runtime_error("Could not open input file");

    if (!out_file)
        throw std::runtime_error("Could not open output file");

    string line;

    while (getline(in_file, line)) {

        for (int i = 0; i < 5; i++) {
            while (true) {
                size_t pos = line.find(wordToReplace[i]);
                if (pos != string::npos) { // word found
                    line.replace(pos,
                                 wordToReplace[i].length(),
                                 wordToReplaceWith[i].toStdString());
                } else
                    break;
            }
        }

        out_file << line << endl;
    }

    in_file.close();
    out_file.close();
}
