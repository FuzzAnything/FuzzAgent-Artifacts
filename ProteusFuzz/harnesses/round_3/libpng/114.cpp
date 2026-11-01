// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

// Helper to safely call png_get_uint_31 without triggering abort
// We cannot easily mock png_error, so we avoid calling this with invalid data
// or we ensure the png_ptr is valid and we control the input to stay within limits.
// Since png_get_uint_31 calls png_error on overflow which aborts, 
// we will only call it with values <= PNG_UINT_31_MAX to prevent crash in fuzzing.
// Alternatively, we can skip fuzzing the error path for this specific function 
// to keep the fuzzer running, or we can construct a valid png_struct.
// Constructing a full valid png_struct is complex. 
// Strategy: Only call png_get_uint_31 if the input buffer represents a value <= PNG_UINT_31_MAX.
// Or, simpler: Do not call png_get_uint_31 in a way that triggers error.
// We will construct a minimal valid png_struct if possible, but it's risky.
// Better approach: Just test the logic with safe inputs. If the fuzzer provides > 31 bits, skip.

static void test_png_save_uint_16(const uint8_t *data, size_t size) {
    if (size < 2) return;
    png_byte buf[2];
    // Use first 2 bytes as input value (big endian interpretation for variety)
    unsigned int val = (data[0] << 8) | data[1];
    // Also try direct casting
    png_save_uint_16(buf, val);
    // Try with a random value from data if available
    if (size >= 4) {
        unsigned int val2 = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
        png_save_uint_16(buf, val2);
    }
}

static void test_png_get_uint_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    const png_byte *buf = data;
    png_uint_32 val = png_get_uint_32(buf);
    (void)val; // Use return value to avoid unused warning
}

static void test_png_save_int_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    // Construct a 32-bit signed int from input
    int32_t val = (int32_t)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    png_save_int_32(buf, val);
    
    // Try with negative values explicitly if data suggests
    if (size >= 8) {
        int32_t val2 = (int32_t)((data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7]);
        png_save_int_32(buf, val2);
    }
}

static void test_png_save_uint_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    png_byte buf[4];
    uint32_t val = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    png_save_uint_32(buf, val);
    
    if (size >= 8) {
        uint32_t val2 = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
        png_save_uint_32(buf, val2);
    }
}

static void test_png_get_int_32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    const png_byte *buf = data;
    png_int_32 val = png_get_int_32(buf);
    (void)val;
}

static void test_png_get_uint_31(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Check if the value in the buffer exceeds PNG_UINT_31_MAX
    // PNG_UINT_31_MAX is 0x7FFFFFFF (2^31 - 1)
    // We must ensure the high bit is 0 to avoid png_error being called.
    if (data[0] & 0x80) {
        // High bit set, would trigger error. Skip to avoid abort.
        return;
    }
    
    // We need a valid png_ptr. Creating a minimal one is hard without full init.
    // However, png_get_uint_31 only checks the value and calls png_error if too big.
    // If we ensure the value is small, it might not even use png_ptr deeply?
    // Looking at libpng source, png_get_uint_31 checks the value first.
    // If value > PNG_UINT_31_MAX, it calls png_error(png_ptr, "chunk length exceeds PNG_UINT_31_MAX").
    // If we ensure value <= PNG_UINT_31_MAX, it returns the value.
    // It does NOT seem to dereference png_ptr if the check passes? 
    // Actually, let's look at the logic: 
    // png_uint_32 png_get_uint_31(png_const_structrp png_ptr, png_const_bytep buf)
    // {
    //    png_uint_32 val = png_get_uint_32(buf);
    //    if (val > PNG_UINT_31_MAX) {
    //       png_error(png_ptr, "chunk length exceeds PNG_UINT_31_MAX");
    //    }
    //    return val;
    // }
    // So if val <= PNG_UINT_31_MAX, it returns val without using png_ptr.
    // So we can pass NULL or a dummy pointer safely as long as the value is safe.
    
    const png_byte *buf = data;
    png_uint_32 val = png_get_uint_31(nullptr, buf);
    (void)val;
}

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test png_save_uint_16
    test_png_save_uint_16(Data, Size);

    // Test png_get_uint_32
    test_png_get_uint_32(Data, Size);

    // Test png_save_int_32
    test_png_save_int_32(Data, Size);

    // Test png_save_uint_32
    test_png_save_uint_32(Data, Size);

    // Test png_get_int_32
    test_png_get_int_32(Data, Size);

    // Test png_get_uint_31 (with safety check)
    test_png_get_uint_31(Data, Size);

    return 0;
}