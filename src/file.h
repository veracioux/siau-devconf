/**
 * @file file.h
 */
#ifndef FILE_H
#define FILE_H

#include "devlib/device.h"
#include <QFile>
#include <QString>
#include "userdata.h"

using namespace Devlib;

bool copyFile(const QString& source, const QString& dest);
void substituteInFile(const QString& source, const QString& dest,
                      const QMap<QString, QString> &substitutions);

void writeDeviceHeader(const Device& device, const QString& in, const QString& out);
void writeDeviceImpl(const Device& device, const QString& out);

void writeMessageHandlers(const QString& in, const QString& out, const Device& device);
void writeMqttWrapperImpl(const QString& in, const QString& out, const Device& device);

#endif // FILE_H
