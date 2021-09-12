/**
 * @file file.h
 */
#ifndef FILE_H
#define FILE_H

#include "devlib/device.h"
#include <QFile>
#include <QString>

using namespace Devlib;

bool copyFile(const QString& source, const QString& destination);
void writeDevice(const Device& device, const QString& in, const QString& out);

#endif // FILE_H
