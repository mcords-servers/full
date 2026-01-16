#ifndef NBT_H
#define NBT_H

#include "kit.h"

#define TAG_Name(buf, val) build_integer(buf, strlen(val), 2, 1); append_to_buffer(buf, val, strlen(val));
#define TAG_NAME(buf, name) if (name) {TAG_Name(buf,name)}

#define TAG_End(buf) do {build_varint(buf, 0);} while(0)
#define TAG_Byte(buf, name, val) do {build_varint(buf, 1); TAG_NAME(buf, name); build_integer(buf, val, 1, 1);} while(0)
#define TAG_Short(buf, name, val) do {build_varint(buf, 2); TAG_NAME(buf, name); build_integer(buf, val, 2, 1);} while(0)
#define TAG_Int(buf, name, val) do {build_varint(buf, 3); TAG_NAME(buf, name); build_integer(buf, val, 4, 1);} while(0)
#define TAG_Long(buf, name, val) do {build_varint(buf, 4); TAG_NAME(buf, name); build_integer(buf, val, 8, 1);} while(0)
#define TAG_Float(buf, name, val) do {build_varint(buf, 5); TAG_NAME(buf, name); build_float(buf, val);} while(0)
#define TAG_Double(buf, name, val) do {build_varint(buf, 6); TAG_NAME(buf, name); build_double(buf, val);} while(0)
#define TAG_Byte_Array(buf, name, val, len) do {build_varint(buf, 7); TAG_NAME(buf, name); build_integer(buf, len, 4, 1); append_to_buffer(buf, (const char *)val, len);} while(0)
#define TAG_String(buf, name, val) do {build_varint(buf, 8); TAG_NAME(buf, name); build_integer(buf, strlen(val), 2, 1); append_to_buffer(buf, val, strlen(val));} while(0)
#define TAG_List(buf, name, type, len) do {build_varint(buf, 9); TAG_NAME(buf, name); build_integer(buf, type, 1, 0); build_integer(buf, len, 4, 1);} while(0)
#define TAG_Compound(buf, name) do {build_varint(buf, 10); TAG_NAME(buf, name);} while(0)
// #define TAG_List(buf, name, type, len) do {build_varint(buf, 11); TAG_NAME(buf, name); build_integer(buf, type, 1, 0); build_integer(buf, len, 4, 1);} while(0)
// #define TAG_List(buf, name, type, len) do {build_varint(buf, 12); TAG_NAME(buf, name); build_integer(buf, type, 1, 0); build_integer(buf, len, 4, 1);} while(0)

#endif