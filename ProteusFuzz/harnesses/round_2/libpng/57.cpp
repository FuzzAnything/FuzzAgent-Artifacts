// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

static void
fuzz_png_get_uint_31(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    // Create a minimal png_struct for the context required by png_get_uint_31
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;
    
    // Create an info struct as well, though not strictly needed for this function
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    // We need to provide a valid buffer of at least 4 bytes
    png_byte buf[4];
    std::memcpy(buf, Data, 4);
    
    // Set up error handling to prevent fuzzer termination on invalid data
    // Use a longjmp handler to catch errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }
    
    // Invoke the target function
    png_uint_32 result = png_get_uint_31(png_ptr, buf);
    (void)result; // Use result to avoid unused variable warning
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

static void
fuzz_png_get_uint_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    png_byte buf[4];
    std::memcpy(buf, Data, 4);
    
    // Invoke the target function
    png_uint_32 result = png_get_uint_32(buf);
    (void)result;
}

static void
fuzz_png_save_uint_32(const uint8_t *Data, size_t Size) {
    png_byte buf[4];
    png_uint_32 value = 0;
    
    // Extract a value from the input data if available
    if (Size >= 4) {
        std::memcpy(&value, Data, 4);
    } else if (Size > 0) {
        value = Data[0];
    }
    
    // Invoke the target function
    png_save_uint_32(buf, value);
}

static void
fuzz_png_save_uint_16(const uint8_t *Data, size_t Size) {
    png_byte buf[2];
    unsigned int value = 0;
    
    // Extract a value from the input data if available
    if (Size >= 2) {
        std::memcpy(&value, Data, 2);
    } else if (Size > 0) {
        value = Data[0];
    }
    
    // Invoke the target function
    png_save_uint_16(buf, value);
}

static void
fuzz_png_get_int_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    png_byte buf[4];
    std::memcpy(buf, Data, 4);
    
    // Invoke the target function
    png_int_32 result = png_get_int_32(buf);
    (void)result;
}

static void
fuzz_png_save_int_32(const uint8_t *Data, size_t Size) {
    png_byte buf[4];
    png_int_32 value = 0;
    
    // Extract a value from the input data if available
    if (Size >= 4) {
        std::memcpy(&value, Data, 4);
    } else if (Size > 0) {
        value = (png_int_32)Data[0];
    }
    
    // Invoke the target function
    png_save_int_32(buf, value);
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz each target function
    fuzz_png_get_uint_31(Data, Size);
    fuzz_png_get_uint_32(Data, Size);
    fuzz_png_save_uint_32(Data, Size);
    fuzz_png_save_uint_16(Data, Size);
    fuzz_png_get_int_32(Data, Size);
    fuzz_png_save_int_32(Data, Size);
    
    return 0;
}