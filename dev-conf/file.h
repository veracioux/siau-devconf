#ifndef FILE_H
#define FILE_H

#include "devlib/device.h"
#include <QFile>
#include <QString>

/**
 * Copy the file `source` into the file `destination`, thereby creating the
 * necessary output directory structure if it does not exist.
 */
bool copyFile(const QString &source, const QString &destination);
/**
 * Substitute device `data` into the contents of file `in` and write the result
 * to the file `out`.
 */
void write(const Device &data, const QString &in, const QString &out);

#endif // FILE_H