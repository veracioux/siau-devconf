#ifndef USERDATA_H
#define USERDATA_H

#include <QMap>

/**
 * Data for a concrete device (its personal ID).
 * This is usually specified in a `user_device.json` file.
 */
class UserData
{
    QMap<QString, QString> attributes = {
        {"name", ""},
        {"serialNo", ""},
        {"location", ""},
        {"ip", "192.168.1.100"},
        {"subnetMask", "255.255.255.0"},
        {"defaultGateway", "192.168.1.1"}
    };

public:
    QString &operator[](const QString& attr);
    QString operator[](const QString& attr) const;
    QMap<QString, QString> getAttributes() const;

    static UserData fromJson(const QString &path);
};

#endif // USERDATA_H
