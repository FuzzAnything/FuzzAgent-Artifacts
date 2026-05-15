// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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
    if (size >= 2) {
        png_get_uint_16(reinterpret_cast<const png_byte*>(data));
    }
}

static void test_png_get_uint_32(const uint8_t* data, size_t size) {
    if (size >= 4) {
        png_get_uint_32(reinterpret_cast<const png_byte*>(data));
    }
}

static void test_png_save_uint_16(const uint8_t* data, size_t size) {
    if (size >= 2) {
        png_byte buf[2];
        unsigned int value = (data[0] << 8) | data[1];
        png_save_uint_16(buf, value);
    }
}

static void test_png_save_int_32(const uint8_t* data, size_t size) {
    if (size >= 4) {
        png_byte buf[4];
        png_int_32 value = static_cast<png_int_32>(
            (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
        png_save_int_32(buf, value);
    }
}

static void test_png_save_uint_32(const uint8_t* data, size_t size) {
    if (size >= 4) {
        png_byte buf[4];
        png_uint_32 value = static_cast<png_uint_32>(
            (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
        png_save_uint_32(buf, value);
    }
}

static void test_png_get_int_32(const uint8_t* data, size_t size) {
    if (size >= 4) {
        png_get_int_32(reinterpret_cast<const png_byte*>(data));
    }
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test all functions with the input data
    test_png_get_uint_16(Data, Size);
    test_png_get_uint_32(Data, Size);
    test_png_save_uint_16(Data, Size);
    test_png_save_int_32(Data, Size);
    test_png_save_uint_32(Data, Size);
    test_png_get_int_32(Data, Size);

    // Test with different offsets to explore more states
    if (Size > 1) {
        test_png_get_uint_16(Data + (Size / 2), Size / 2);
    }
    if (Size > 3) {
        test_png_get_uint_32(Data + (Size / 4), Size - (Size / 4));
        test_png_get_int_32(Data + (Size / 4), Size - (Size / 4));
    }

    // Test edge cases with small values
    if (Size >= 2) {
        png_byte buf16[2];
        png_save_uint_16(buf16, 0);
        png_save_uint_16(buf16, 65535);
    }
    
    if (Size >= 4) {
        png_byte buf32[4];
        png_save_int_32(buf32, 0);
        png_save_int_32(buf32, -1);
        png_save_int_32(buf32, 2147483647);
        png_save_int_32(buf32, -2147483648);
        
        png_save_uint_32(buf32, 0);
        png_save_uint_32(buf32, 4294967295U);
    }

    return 0;
}