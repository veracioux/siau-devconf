#include "userdata.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

QString &UserData::operator[](const QString &attr)
{
    if (!attributes.contains(attr))
        throw std::logic_error(
          QStringLiteral("Invalid user device attribute: '%1'")
            .arg(attr)
            .toStdString());
    return attributes[attr];
}

QMap<QString, QString> UserData::getAttributes() const
{
    return attributes;
}

QString UserData::operator[](const QString &attr) const
{
    return (*const_cast<UserData*>(this))[attr];
}

UserData UserData::fromJson(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error(QStringLiteral("Could not open file '%1'")
                                 .arg(path).toStdString());
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = document.object();
    file.close();

    UserData data;
    for (const QString &key : obj.keys()) {
        QJsonValue val = obj.value(key);
        if (!val.isString())
            throw std::runtime_error("All user_device.json attributes must be strings");
        data[key] = val.toString();
    }

    return data;
}
