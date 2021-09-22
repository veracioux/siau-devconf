#include "generator.h"
#include "devlib/device.h"
#include <QTextStream>

enum What {
    Definition, Declaration
};

// helpers
QString indented(QString str)
{
    str.replace("\n", "\n    ");
    return "    " + str;
}
QString cppFunction(What what, const QString &retType, const QString &name,
                    const QString &paramType, const QString &paramName,
                    const QString &content = "// TODO")
{
    QString str;
    QTextStream stream(&str);
    ValueSpec spec;

    QString param;
    if (paramType != "void")
        param = paramType + " " + paramName;

    stream << QStringLiteral("%1 %2(%3)").arg(retType).arg(name).arg(param);
    if (what == Declaration) {
        stream << ";\n";
        return str;
    } else {
        stream << "\n{\n" << indented(content) << "\n}\n";
    }
    return str;
}

QString enumDefinition(const ValueSpec &spec)
{
    QString str;
    QTextStream stream(&str);
    stream << QStringLiteral("typedef enum {\n");
    stream << indented(spec.getValueRange().join(", "));
    stream << QStringLiteral("\n} %1;\n").arg(spec.getValueType());
    return str;
}

QString functionDeclaration(const SingleFunction &fun)
{
    return cppFunction(Declaration, "void", fun.getName(),
                       fun.getValueSpec().getValueType(), "value");
}

QString functionDefinition(const SingleFunction &fun)
{
    return cppFunction(Definition, "void", fun.getName(),
                       fun.getValueSpec().getValueType(), "value");
}

QString dataDeclaration(const Data &data)
{
    return cppFunction(Declaration, data.getValueSpec().getValueType(),
                       data.getName(), "void", "");
}

QString dataDefinition(const Data &data)
{
    return cppFunction(Definition, data.getValueSpec().getValueType(),
                       data.getName(), "void", "");
}

QString messageHandlerDeclaration(const SingleFunction &fun)
{
    return cppFunction(Declaration, "void", fun.getName() + "_handler",
                       "MQTT::MessageData&", "msg");
}

QString messageHandlerDeclaration(const Data &data)
{
    return cppFunction(Declaration, "void", data.getName() + "_handler",
                       "MQTT::MessageData&", "msg");
}

QString messageHandlerDefinition(const SingleFunction &fun)
{
    const ValueSpec& valueSpec = fun.getValueSpec();
    QString content;
    QTextStream stream(&content);

    if (valueSpec.isVoid())
        stream << "Device::" << fun.getName() << "();";
    else {
        // TODO log an error or something
        // TODO do not hardcode to size 4
        stream << "if (msg.message.payloadlen != 4)\n    return;\n";
        stream << "Device::" << fun.getName()
               << QStringLiteral("(*((%1*) msg.message.payload));")
                    .arg(valueSpec.getValueType());
    }

    return cppFunction(Definition, "void", fun.getName() + "_handler",
                       "MQTT::MessageData&", "msg", content);
}

QString messageHandlerDefinition(const Data &data)
{
    QString content;
    QTextStream stream(&content);

    const ValueSpec &valueSpec = data.getValueSpec();
    QString actualValueType;
    // TODO change size for bool type
    if (valueSpec.isCustomEnum() || valueSpec.isBool())
        actualValueType = "int";
    else
        actualValueType = valueSpec.getValueType();

    stream << QStringLiteral("%1 payload = Device::%2();\n")
                .arg(actualValueType)
                .arg(data.getName())
           << QStringLiteral("mqttPublish(TOPIC_PREFIX \"%1/response\", &payload, 4);")
                .arg(data.getName());

    return cppFunction(Definition, "void", data.getName() + "_handler",
                       "MQTT::MessageData&", "msg", content);
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
