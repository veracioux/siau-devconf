#include <QtTest/QtTest>

#include "devlib/device.h"
#include "file.h"
#include <QDir>
#include <QFile>
#include <iostream>

extern QString processLine(QString, const QMap<QString, QString> &attr);

QString _processLine(QString line, const Device& device)
{
    return processLine(line, device.getAttributes());
}

class TestWrite : public QObject
{
    Q_OBJECT
private slots:
    void testProcessLine() {
        Device device;
        device.setName("MyDevice");
        device.setModel("MyModel");
        QCOMPARE(_processLine("class $name", device), "class MyDevice");
        QCOMPARE(_processLine("class $name : $model", device), "class MyDevice : MyModel");
        QCOMPARE(_processLine("string \"$name\"", device), "string \"MyDevice\"");
        QCOMPARE(_processLine("string \"$name\" aftertext $model", device),
                 "string \"MyDevice\" aftertext MyModel");

        // Degenerate cases
        QCOMPARE(_processLine("", device), ""); // empty string
        QCOMPARE(_processLine("$", device), "$");
        QCOMPARE(_processLine("$nonexistent", device), "");
        QCOMPARE(_processLine("$nonexistent $model", device), " MyModel");
    }

    void testBasicWrite()
    {
        // Populate the device with data
        Device device;
        device.setName("MyDevice");
        device.setVendorId("MyVendor");
        device.setModel("MyModel");
        device.setDescription("A test device");

        // Write the file
        QDir("_out").removeRecursively();
        QDir().mkpath("_out");
        writeDeviceHeader(device, "files/basic.h.in", "_out/basic.h");

        // Compare the output file with expected output
        QFile file1("_out/basic.h"), file2("files/basic.h");
        QVERIFY(file1.open(QIODevice::ReadOnly));
        QVERIFY(file2.open(QIODevice::ReadOnly));
        // NOTE Parentheses are a cheap trick to align Actual-Expected in output
        QCOMPARE(((((((file1.readAll())))))), file2.readAll());
    }
};

QTEST_MAIN(TestWrite)

#include "main.moc"
