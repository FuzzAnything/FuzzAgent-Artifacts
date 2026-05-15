// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include "png.h"

static void test_png_get_uint_16(const uint8_t* data, size_t size) {
    if (size < 2) return;
    png_get_uint_16(reinterpret_cast<const png_byte*>(data));
}

static void test_png_get_uint_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_get_uint_32(reinterpret_cast<const png_byte*>(data));
}

static void test_png_save_uint_16(const uint8_t* data, size_t size) {
    if (size < 2) return;
    png_byte buf[2];
    unsigned int val = (data[0] << 8) | data[1];
    png_save_uint_16(buf, val);
}

static void test_png_save_int_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    png_int_32 val;
    memcpy(&val, data, sizeof(val));
    png_save_int_32(buf, val);
}

static void test_png_save_uint_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    png_uint_32 val;
    memcpy(&val, data, sizeof(val));
    png_save_uint_32(buf, val);
}

static void test_png_get_int_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_get_int_32(reinterpret_cast<const png_byte*>(data));
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *data, size_t size) {
    // Test all API functions with the fuzzer input
    test_png_get_uint_16(data, size);
    test_png_get_uint_32(data, size);
    test_png_save_uint_16(data, size);
    test_png_save_int_32(data, size);
    test_png_save_uint_32(data, size);
    test_png_get_int_32(data, size);
    
    return 0;
}