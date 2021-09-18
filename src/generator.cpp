#include "generator.h"
#include "devlib/device.h"
#include <QTextStream>

QString indented(QString str)
{
    str.replace("\n", "\n    ");
    return "    " + str;
}

QString enumDefinition(const ValueSpec &spec)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("enum %1 {\n").arg(spec.getValueType());
    stream << indented(spec.getValueRange().join(", "));
    stream << "\n};\n";
    return str;
}

QString functionDeclaration(const SingleFunction &fun)
{
    QString str;
    QTextStream stream(&str);
    ValueSpec spec = fun.getValueSpec();
    stream << QStringLiteral("void %1(%2 value);\n")
              .arg(fun.getName())
              .arg(spec.getValueType());
    return str;
}

QString functionDefinition(const SingleFunction &fun)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("void %1(%2 value)\n{\n")
              .arg(fun.getName())
              .arg(fun.getValueSpec().getValueType())
           << indented("// TODO implement")
           << "\n}\n";
    return str;
}

QString dataDeclaration(const Data &data)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("%1 %2() const;\n")
              .arg(data.getValueSpec().getValueType())
              .arg(data.getName());
    return str;
}

QString dataDefinition(const Data &data)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("%1 %2()\n{\n")
              .arg(data.getValueSpec().getValueType())
              .arg(data.getName())
           << indented("// TODO implement")
           << "\n}\n";
    return str;
}

// BULK WRITERS

void writeEnums(QTextStream &stream, QList<ValueSpec> enums)
{
    for (auto &e : enums) {
        stream << enumDefinition(e);
        stream << "\n";
    }
}

void writeFunctionDeclarations(QTextStream &stream, QList<const SingleFunction*> functions)
{
    for (auto *f : functions)
        stream << functionDeclaration(*f);
}

void writeDataDeclarations(QTextStream &stream, QList<const Data*> functions)
{
    for (auto *d : functions)
        stream << dataDeclaration(*d);
}
