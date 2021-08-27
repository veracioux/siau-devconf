/**
  * @file file.h
  */
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
 * Populate the template from file `in` with device `data` and write the result
 * to the file `out`.
 *
 * @note `in` and `out` are file paths.
 *
 * **Example:**
 * ~~~
 * Device data = jsonParseDevice("factory_device.json");
 * write(data, "device.h.in", "device.h");
 * ~~~
 */
void write(const Device &data, const QString &in, const QString &out);

#endif // FILE_H
