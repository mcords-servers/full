#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"

void build_varint(Buffer *out, int value) {
    char temp[5];  // use char[] instead of uint8_t[] to avoid warnings
    int len = 0;

    do {
        uint8_t byte = value & 0x7F;
        value >>= 7;
        if (value != 0)
            byte |= 0x80;
        temp[len++] = (char)byte;
    } while (value != 0 && len < 5);

    append_to_buffer(out, temp, len);
}

void build_string(Buffer *out, const char *str) {
    int len = strlen(str);

    // Write length prefix
    build_varint(out, len);

    // Write UTF-8 bytes (without null terminator)
    append_to_buffer(out, str, len);
}

void build_integer(Buffer *out, int64_t value, int bytes, int is_signed) {
    char temp[8] = {0};

    if (is_signed) {
        for (int i = 0; i < bytes; i++) {
            temp[bytes - 1 - i] = (char)((value >> (i * 8)) & 0xFF);
        }
    } else {
        uint64_t uval = (uint64_t)value;
        for (int i = 0; i < bytes; i++) {
            temp[bytes - 1 - i] = (char)((uval >> (i * 8)) & 0xFF);
        }
    }

    append_to_buffer(out, temp, bytes);
}

void build_float(Buffer *out, float value) {
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));  // reinterpret IEEE-754

    char temp[4];
    temp[0] = (bits >> 24) & 0xFF;
    temp[1] = (bits >> 16) & 0xFF;
    temp[2] = (bits >> 8)  & 0xFF;
    temp[3] = bits & 0xFF;

    append_to_buffer(out, temp, 4);
}

void build_double(Buffer *out, double value) {
    uint64_t bits;
    memcpy(&bits, &value, sizeof(bits));  // reinterpret IEEE-754

    char temp[8];
    temp[0] = (bits >> 56) & 0xFF;
    temp[1] = (bits >> 48) & 0xFF;
    temp[2] = (bits >> 40) & 0xFF;
    temp[3] = (bits >> 32) & 0xFF;
    temp[4] = (bits >> 24) & 0xFF;
    temp[5] = (bits >> 16) & 0xFF;
    temp[6] = (bits >> 8)  & 0xFF;
    temp[7] = bits & 0xFF;

    append_to_buffer(out, temp, 8);
}

// void build_mcjson(Buffer *out, char *str) {
//     char stack[256] = {0};
//     char string[65536];
//     void parse_tag() {


//         if (false) parse_tag();
//     }
//     parse_tag();
// }