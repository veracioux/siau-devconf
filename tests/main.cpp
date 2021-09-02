#include <QtTest/QtTest>

#include "file.h"
#include "devlib/device.h"
#include <iostream>
#include <QDir>
#include <QFile>

class TestWrite : public QObject
{
    Q_OBJECT
private slots:
    void testBasicWrite()
    {
        Device data;
        data.setName("MyDevice");
        data.setVendorId("MyVendor");
        data.setModel("MyModel");
        data.setSerialNo("XYZ-UVW");
        QDir("_out").removeRecursively();
        QDir().mkpath("_out");
        write(data, "files/basic.h.in", "_out/basic.h");

        QFile file1("_out/basic.h"), file2("files/basic.h");
        QVERIFY(file1.open(QIODevice::ReadOnly));
        QVERIFY(file2.open(QIODevice::ReadOnly));
        QCOMPARE(file1.readAll(), file2.readAll());
    }
};

QTEST_MAIN(TestWrite)

#include "main.moc"
