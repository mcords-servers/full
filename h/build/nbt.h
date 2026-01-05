#ifndef NBT_H
#define NBT_H

#include "kit.h"

#define TAG_End(buf) build_varint(buf, 0)
#define TAG_Byte(buf, val) build_integer(buf, val, 1, 1)
#define TAG_Short(buf, val) build_integer(buf, val, 2, 1)
#define TAG_Int(buf, val) build_integer(buf, val, 4, 1)
#define TAG_Long(buf, val) build_integer(buf, val, 8, 1)
#define TAG_Float(buf, val) build_float(buf, val)
#define TAG_Double(buf, val) build_double(buf, val)

#endif