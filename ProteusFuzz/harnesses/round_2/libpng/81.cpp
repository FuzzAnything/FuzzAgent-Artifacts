// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <setjmp.h>

// Helper to create a minimal valid png_struct for png_get_uint_31
// We use setjmp/longjmp to catch errors triggered by png_get_uint_31
static png_structp create_minimal_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return nullptr;
    
    // Initialize info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }
    
    // Set custom error handlers that use longjmp
    // We do not set custom handlers here; we rely on the default png_error which calls longjmp(png_jmpbuf(png_ptr))
    // The caller will use setjmp(png_jmpbuf(png_ptr)) to catch the error.
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a buffer for saving operations
    uint8_t save_buf[8];
    
    // Test png_get_uint_32 (requires 4 bytes)
    if (Size >= 4) {
        png_uint_32 val32 = png_get_uint_32(Data);
        (void)val32; // Use value to avoid unused warning
    }
    
    // Test png_get_int_32 (requires 4 bytes)
    if (Size >= 4) {
        png_int_32 val_int32 = png_get_int_32(Data);
        (void)val_int32;
    }
    
    // Test png_save_uint_32
    if (Size >= 4) {
        png_uint_32 input_val = png_get_uint_32(Data);
        png_save_uint_32(save_buf, input_val);
    }
    
    // Test png_save_uint_16 (requires 2 bytes for reading, 2 for writing)
    // Ensure we have at least 4 bytes to safely read a uint32 for the input value
    if (Size >= 4) {
        png_uint_32 input_val16 = png_get_uint_32(Data);
        // Truncate to 16 bits for saving
        unsigned int val16 = (unsigned int)(input_val16 & 0xFFFF);
        png_save_uint_16(save_buf, val16);
    }
    
    // Test png_save_int_32
    if (Size >= 4) {
        png_int_32 input_int32 = png_get_int_32(Data);
        png_save_int_32(save_buf, input_int32);
    }
    
    // Test png_get_uint_31 (requires png_struct and 4 bytes)
    if (Size >= 4) {
        png_structp png_ptr = create_minimal_png_struct();
        if (png_ptr) {
            // Wrap the call in a try-catch equivalent using setjmp/longjmp
            // png_get_uint_31 calls png_error if value > PNG_UINT_31_MAX
            // The default error handler triggers longjmp(png_jmpbuf(png_ptr))
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                png_uint_32 val31 = png_get_uint_31(png_ptr, Data);
                (void)val31;
            } else {
                // Error occurred, longjmp was triggered
                // Clean up and continue
            }
            
            // Clean up png_struct
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
    }
    
    return 0;
}