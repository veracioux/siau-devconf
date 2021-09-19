#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

bool verifyFloatPayloadSize(size_t len);
bool verifyIntPayloadSize(size_t len);
bool verifyBoolPayloadSize(size_t len);
bool verifyVoidPayloadSize(size_t len);
bool verifyEnumPayloadSize(size_t len);

/*** Message handler declarations ***/

#endif // MESSAGE_HANDLER_H
