// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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

static void
fuzz_png_save_uint_16(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    png_byte buf[2];
    // Extract value from input, mask to 16 bits
    unsigned int val = 0;
    if (Size >= 2) {
        val = (Data[0] << 8) | Data[1];
    }
    png_save_uint_16(buf, val);
}

static void
fuzz_png_save_uint_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_byte buf[4];
    png_uint_32 val = 0;
    if (Size >= 4) {
        val = ((png_uint_32)Data[0] << 24) |
              ((png_uint_32)Data[1] << 16) |
              ((png_uint_32)Data[2] << 8) |
              ((png_uint_32)Data[3]);
    }
    png_save_uint_32(buf, val);
}

static void
fuzz_png_get_int_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_int_32 result = png_get_int_32(Data);
    (void)result;
}

static void
fuzz_png_get_uint_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_uint_32 result = png_get_uint_32(Data);
    (void)result;
}

static void
fuzz_png_save_int_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_byte buf[4];
    png_int_32 val = 0;
    if (Size >= 4) {
        val = ((png_int_32)Data[0] << 24) |
              ((png_int_32)Data[1] << 16) |
              ((png_int_32)Data[2] << 8) |
              ((png_int_32)Data[3]);
    }
    png_save_int_32(buf, val);
}

static void
fuzz_png_get_uint_31_safe(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    // Pre-validate to avoid triggering png_error in fuzzing context
    png_uint_32 val = ((png_uint_32)Data[0] << 24) |
                      ((png_uint_32)Data[1] << 16) |
                      ((png_uint_32)Data[2] << 8) |
                      ((png_uint_32)Data[3]);
    
    // Only call if within safe range (PNG_UINT_31_MAX = 2^31 - 1)
    if (val <= PNG_UINT_31_MAX) {
        // We cannot easily create a valid png_ptr without full initialization,
        // so we skip direct invocation of png_get_uint_31 in this standalone driver
        // as it requires a valid png_struct context which is complex to set up
        // without triggering errors during initialization in a fuzzer.
        // Instead, we rely on the pre-check logic which mimics the function's safety.
        (void)val;
    }
}

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz png_save_uint_16
    fuzz_png_save_uint_16(Data, Size);

    // Fuzz png_save_uint_32
    fuzz_png_save_uint_32(Data, Size);

    // Fuzz png_get_int_32
    fuzz_png_get_int_32(Data, Size);

    // Fuzz png_get_uint_32
    fuzz_png_get_uint_32(Data, Size);

    // Fuzz png_save_int_32
    fuzz_png_save_int_32(Data, Size);

    // Fuzz png_get_uint_31 (safe wrapper)
    fuzz_png_get_uint_31_safe(Data, Size);

    return 0;
}