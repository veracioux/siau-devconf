#include "file.h"
#include <QFileInfo>
#include <QDir>

bool copyFile(const QString &source, const QString &destination)
{
    QString destPath = QFileInfo(destination).absolutePath();
    if (!QDir(destPath).exists() && !QDir().mkpath(destPath))
        return false;

    return QFile(source).copy(destination);
}

void write(const Device &data, const QString &in, const QString &out)
{
}
