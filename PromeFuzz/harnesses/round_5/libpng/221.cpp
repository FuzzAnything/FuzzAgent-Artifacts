// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warn_fn(png_structp, png_const_charp) {}

extern "C" int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    // Initialize dummy png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, dummy_error_fn, dummy_warn_fn);
    if (!png_ptr) return 0;
    
    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Use fuzzer input for various parameters
    if (Size > 0) {
        // Split input for different parameters
        size_t name_len = Size > 10 ? 10 : Size;
        size_t profile_len = Size > name_len ? Size - name_len : 0;
        
        // Prepare name (null-terminated)
        char name[11];
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        
        // Prepare profile data
        const png_byte* profile = profile_len > 0 ? Data + name_len : nullptr;
        
        // Test png_set_iCCP with various compression types
        int compression_type = Size % 3; // 0, 1, or 2
        png_set_iCCP(png_ptr, info_ptr, name, compression_type, profile, 
                     static_cast<png_uint_32>(profile_len));
        
        // Test png_set_invalid with different masks
        int mask = Size % 256;
        png_set_invalid(png_ptr, info_ptr, mask);
        
        // Test png_data_freer with different modes
        int freer_mode = Size % 2; // 0 or 1
        png_uint_32 freer_mask = static_cast<png_uint_32>(Size);
        png_data_freer(png_ptr, info_ptr, freer_mode, freer_mask);
        
        // Test png_free_data with different masks and nums
        png_uint_32 free_mask = static_cast<png_uint_32>(Size);
        int free_num = static_cast<int>(Size % 100) - 50; // -50 to 49
        png_free_data(png_ptr, info_ptr, free_mask, free_num);
        
        // Test png_free with allocated memory
        if (Size > 20) {
            void* allocated_mem = png_malloc(png_ptr, 100);
            if (allocated_mem) {
                png_free(png_ptr, allocated_mem);
            }
        }
    }
    
    // Test png_destroy_info_struct
    png_destroy_info_struct(png_ptr, &info_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}