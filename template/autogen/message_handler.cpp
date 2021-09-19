#include "message_handler.h"

bool verifyFloatPayloadSize(size_t len)
{
    return $sizeFloat;
}
bool verifyIntPayloadSize(size_t len)
{
    return $sizeInt;
}
bool verifyBoolPayloadSize(size_t len)
{
    return $sizeBool;
}
bool verifyVoidPayloadSize(size_t len)
{
    return 0;
}
bool verifyEnumPayloadSize(size_t len)
{
    return $sizeEnum;
}
