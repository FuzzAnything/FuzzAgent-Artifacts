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

extern "C" {
#include "png.h"
}

static void test_png_get_uint_16(const uint8_t* data, size_t size) {
    if (size < 2) return;
    png_get_uint_16((const png_byte*)data);
}

static void test_png_get_uint_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_get_uint_32((const png_byte*)data);
}

static void test_png_save_uint_16(const uint8_t* data, size_t size) {
    if (size < 2) return;
    png_byte buf[2];
    png_uint_16 value = (png_uint_16)((data[0] << 8) | data[1]);
    png_save_uint_16(buf, value);
}

static void test_png_save_int_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    png_int_32 value = (png_int_32)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    png_save_int_32(buf, value);
}

static void test_png_save_uint_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    png_uint_32 value = (png_uint_32)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    png_save_uint_32(buf, value);
}

static void test_png_get_int_32(const uint8_t* data, size_t size) {
    if (size < 4) return;
    png_get_int_32((const png_byte*)data);
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    test_png_get_uint_16(data, size);
    test_png_get_uint_32(data, size);
    test_png_save_uint_16(data, size);
    test_png_save_int_32(data, size);
    test_png_save_uint_32(data, size);
    test_png_get_int_32(data, size);
    
    return 0;
}