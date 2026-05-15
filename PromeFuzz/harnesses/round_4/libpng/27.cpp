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
#include "png.h"
#include <cstdint>
#include <cstddef>
#include <cstring>

static void test_png_get_uint_16(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    png_uint_16 result = png_get_uint_16(reinterpret_cast<const png_byte*>(data));
    (void)result; // Use result to avoid unused variable warning
}

static void test_png_get_uint_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    png_uint_32 result = png_get_uint_32(reinterpret_cast<const png_byte*>(data));
    (void)result;
}

static void test_png_save_uint_16(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    png_byte buf[2];
    png_uint_16 value = png_get_uint_16(reinterpret_cast<const png_byte*>(data));
    png_save_uint_16(buf, value);
}

static void test_png_save_int_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    png_byte buf[4];
    png_int_32 value = png_get_int_32(reinterpret_cast<const png_byte*>(data));
    png_save_int_32(buf, value);
}

static void test_png_save_uint_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    png_byte buf[4];
    png_uint_32 value = png_get_uint_32(reinterpret_cast<const png_byte*>(data));
    png_save_uint_32(buf, value);
}

static void test_png_get_int_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    png_int_32 result = png_get_int_32(reinterpret_cast<const png_byte*>(data));
    (void)result;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Test all API functions with the fuzzer input
    test_png_get_uint_16(Data, Size);
    test_png_get_uint_32(Data, Size);
    test_png_save_uint_16(Data, Size);
    test_png_save_int_32(Data, Size);
    test_png_save_uint_32(Data, Size);
    test_png_get_int_32(Data, Size);
    
    // Test with different offsets to explore more states
    if (Size > 4) {
        // Test with offset 1
        test_png_get_uint_16(Data + 1, Size - 1);
        test_png_get_uint_32(Data + 1, Size - 1);
        test_png_get_int_32(Data + 1, Size - 1);
    }
    
    if (Size > 8) {
        // Test with offset 2
        test_png_get_uint_16(Data + 2, Size - 2);
        test_png_get_uint_32(Data + 2, Size - 2);
        test_png_get_int_32(Data + 2, Size - 2);
    }
    
    // Test with dummy file if needed (though not required for these functions)
    // The functions don't require file operations, so we skip file creation
    
    return 0;
}