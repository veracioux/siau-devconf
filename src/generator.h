/** @file generator.h
 * C++ code generator
 */
#ifndef BLAH_H
#define BLAH_H

#include "devlib/device.h"
#include <QString>

using namespace Devlib;

QString enumDefinition(const ValueSpec &spec);
QString functionDeclaration(const SingleFunction &fun);
QString functionDefinition(const SingleFunction &fun);
QString dataDeclaration(const Data &data);
QString dataDefinition(const Data &data);
QString messageHandlerDeclaration(const SingleFunction &fun);
QString messageHandlerDeclaration(const Data &data);
QString messageHandlerDefinition(const SingleFunction &fun);
QString messageHandlerDefinition(const Data &data);
QString mqttAutoSubscribeImpl(const QStringList &names);

// BULK WRITERS
void writeEnums(QTextStream &stream, QList<ValueSpec> enums);
void writeFunctionDeclarations(QTextStream &stream, QList<const SingleFunction*> functions);
void writeDataDeclarations(QTextStream &stream, QList<const Data*> functions);
void writeMessageHandlers(QTextStream &stream, QList<const SingleFunction*> functions);
void writeMessageHandlers(QTextStream &stream, QList<const Data*> data);

// HELPERS
QString indented(QString str);

#endif // BLAH_H
