// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_399(const uint8_t *Data, size_t Size) {
    // Create dummy file for reading if needed
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Test png_set_sRGB with various intents
    if (Size > 0) {
        int srgb_intent = Data[0] % 4; // PNG_sRGB_INTENT values: 0-3
        png_set_sRGB(png_ptr, info_ptr, srgb_intent);
        
        // Test with NULL pointers (should be safely ignored)
        png_set_sRGB(nullptr, nullptr, srgb_intent);
    }
    
    // Test png_get_iCCP
    if (Size > 1) {
        char* name = nullptr;
        int compression_type = 0;
        png_byte* profile = nullptr;
        png_uint_32 proflen = 0;
        
        png_uint_32 result = png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
        
        // Free allocated memory if any
        if (name) png_free(png_ptr, name);
        if (profile) png_free(png_ptr, profile);
    }
    
    // Test png_set_cHRM_XYZ with random floating point values
    if (Size >= 9 * sizeof(double)) {
        double values[9];
        memcpy(values, Data, 9 * sizeof(double));
        
        png_set_cHRM_XYZ(png_ptr, info_ptr,
                        values[0], values[1], values[2],   // red X,Y,Z
                        values[3], values[4], values[5],   // green X,Y,Z
                        values[6], values[7], values[8]);  // blue X,Y,Z
    }
    
    // Test png_set_iCCP with random profile data
    if (Size > 10) {
        // Use first 10 bytes for name (ensure null termination)
        char name[11];
        size_t name_len = Size > 10 ? 10 : Size;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        
        // Use remaining data as profile
        size_t profile_offset = name_len + 1;
        if (profile_offset < Size) {
            png_uint_32 proflen = Size - profile_offset;
            const png_byte* profile = Data + profile_offset;
            int compression_type = 0; // PNG_COMPRESSION_TYPE_BASE
            
            png_set_iCCP(png_ptr, info_ptr, name, compression_type, profile, proflen);
        }
    }
    
    // Test png_free with various pointers
    if (Size > 0) {
        // Allocate some memory using png_malloc
        void* allocated_ptr = png_malloc(png_ptr, 100);
        if (allocated_ptr) {
            png_free(png_ptr, allocated_ptr);
        }
        
        // Test with NULL pointer (should be safely ignored)
        png_free(png_ptr, nullptr);
        png_free(nullptr, allocated_ptr);
        png_free(nullptr, nullptr);
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}