#include "file.h"
#include <QDir>
#include <QFileInfo>
#include <fstream>
#include <iostream>

bool copyFile(const QString &source, const QString &destination) {
    QString destPath = QFileInfo(destination).absolutePath();
    if (!QDir(destPath).exists() && !QDir().mkpath(destPath))
        return false;

    return QFile(source).copy(destination);
}

void write(const Device &data, const QString &in, const QString &out) {
    using namespace std;
    ifstream in_file(in.toStdString());
    ofstream out_file(out.toStdString());
    string wordToReplace[] = {"$vendorId", "$model", "$serialNo", "$name"};

    // DODATI U device.h  getModel i getSerialNo...
    // a u datoteku device.h.in dodati gettere getName() i getDeviceId()

    QString wordToReplaceWith[] = {data.getVendorId()}; //, data.getModel(),
                                  //data.getSerialNo(), data.getName()};

    if (!in_file) {
        cerr << "Could not open!"
             << "\n";
        return;
    }

    if (!out_file) {
        cerr << "Could not open!"
             << "\n";
        return;
    }
    string line;

    while (getline(in_file, line)) {

        for (int i = 0; i < 5; i++) {
            while (true) {
                size_t pos = line.find(wordToReplace[i]);
                if (pos != string::npos)
                    line.replace(pos, wordToReplace[i].length(),
                                 wordToReplaceWith[i].toStdString());
                else
                    break;
            }
        }

        out_file << line << '\n';
    }
}
