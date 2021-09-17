#include <QtTest/QtTest>

#include "devlib/device.h"
#include "file.h"
#include <QDir>
#include <QFile>
#include <iostream>

extern QString processLine(QString, const Device&);

class TestWrite : public QObject
{
    Q_OBJECT
private slots:
    void testProcessLine() {
        Device device;
        device.setName("MyDevice");
        device.setModel("MyModel");
        QCOMPARE(processLine("class $name", device), "class MyDevice");
        QCOMPARE(processLine("class $name : $model", device), "class MyDevice : MyModel");
        QCOMPARE(processLine("string \"$name\"", device), "string \"MyDevice\"");
        QCOMPARE(processLine("string \"$name\" aftertext $model", device),
                 "string \"MyDevice\" aftertext MyModel");

        // Degenerate cases
        QCOMPARE(processLine("", device), ""); // empty string
        QCOMPARE(processLine("$", device), "$");
        QCOMPARE(processLine("$nonexistent", device), "");
        QCOMPARE(processLine("$nonexistent $model", device), " MyModel");
    }

    void testBasicWrite()
    {
        // Populate the device with data
        Device device;
        device.setName("MyDevice");
        device.setVendorId("MyVendor");
        device.setModel("MyModel");
        device.setSerialNo("XYZ-UVW");

        // Write the file
        QDir("_out").removeRecursively();
        QDir().mkpath("_out");
        writeDeviceHeader(device, "files/basic.h.in", "_out/basic.h");

        // Compare the output file with expected output
        QFile file1("_out/basic.h"), file2("files/basic.h");
        QVERIFY(file1.open(QIODevice::ReadOnly));
        QVERIFY(file2.open(QIODevice::ReadOnly));
        QCOMPARE(file1.readAll(), file2.readAll());
    }
};

QTEST_MAIN(TestWrite)

#include "main.moc"
