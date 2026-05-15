// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_pCAL at pngset.c:482:1 in png.h
// png_get_pCAL at pngget.c:994:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "/promefuzz/database/libpng/latest/code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct() {
    return png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                  dummy_error_fn, dummy_warning_fn);
}

static png_infop create_info_struct(png_structp png_ptr) {
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Test png_set_sCAL_fixed and png_get_sCAL_fixed
    if ((selector & 0x01) && fuzz_size >= 9) {
        int unit = (fuzz_data[0] & 0x01) ? 1 : 2; // Only valid units 1 or 2
        png_fixed_point width = *reinterpret_cast<const png_int_32*>(fuzz_data + 1);
        png_fixed_point height = *reinterpret_cast<const png_int_32*>(fuzz_data + 5);
        
        // Ensure positive values as required
        if (width > 0 && height > 0) {
            png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);
            
            int retrieved_unit;
            png_fixed_point retrieved_width, retrieved_height;
            png_uint_32 result = png_get_sCAL_fixed(png_ptr, info_ptr, 
                                                    &retrieved_unit, 
                                                    &retrieved_width, 
                                                    &retrieved_height);
            (void)result; // Suppress unused variable warning
        }
    }

    // Test png_set_sCAL_s and png_get_sCAL_s
    if ((selector & 0x02) && fuzz_size >= 4) {
        int unit = (fuzz_data[0] & 0x01) ? 1 : 2;
        
        // Create valid floating-point strings from fuzz data
        std::string swidth = "1.0";
        std::string sheight = "1.0";
        
        if (fuzz_size >= 8) {
            // Use fuzz data to create numeric strings (avoiding negative signs)
            uint32_t width_val = *reinterpret_cast<const uint32_t*>(fuzz_data + 1);
            uint32_t height_val = *reinterpret_cast<const uint32_t*>(fuzz_data + 5);
            swidth = std::to_string(width_val % 1000) + "." + std::to_string(width_val % 100);
            sheight = std::to_string(height_val % 1000) + "." + std::to_string(height_val % 100);
        }
        
        png_set_sCAL_s(png_ptr, info_ptr, unit, swidth.c_str(), sheight.c_str());
        
        int retrieved_unit;
        char *retrieved_swidth = nullptr;
        char *retrieved_sheight = nullptr;
        png_uint_32 result = png_get_sCAL_s(png_ptr, info_ptr, 
                                            &retrieved_unit, 
                                            &retrieved_swidth, 
                                            &retrieved_sheight);
        (void)result;
        
        // DO NOT free strings returned by png_get_sCAL_s - they are managed by libpng
        // and will be freed by png_destroy_info_struct
    }

    // Test png_set_pCAL and png_get_pCAL
    if ((selector & 0x04) && fuzz_size >= 20) {
        const char* purpose = "fuzz";
        png_int_32 X0 = *reinterpret_cast<const png_int_32*>(fuzz_data);
        png_int_32 X1 = *reinterpret_cast<const png_int_32*>(fuzz_data + 4);
        int type = static_cast<int>(fuzz_data[8]) % 4; // Valid types 0-3
        int nparams = 2;
        const char* units = "units";
        
        // Create parameter array
        char* params[2];
        char param1[32], param2[32];
        if (fuzz_size >= 28) {
            uint32_t param1_val = *reinterpret_cast<const uint32_t*>(fuzz_data + 12);
            uint32_t param2_val = *reinterpret_cast<const uint32_t*>(fuzz_data + 16);
            snprintf(param1, sizeof(param1), "%u", param1_val % 1000);
            snprintf(param2, sizeof(param2), "%u", param2_val % 1000);
            params[0] = param1;
            params[1] = param2;
            
            png_set_pCAL(png_ptr, info_ptr, purpose, X0, X1, type, 
                        nparams, units, params);
            
            // Test retrieval
            char *retrieved_purpose = nullptr;
            png_int_32 retrieved_X0, retrieved_X1;
            int retrieved_type, retrieved_nparams;
            char *retrieved_units = nullptr;
            char **retrieved_params = nullptr;
            
            png_uint_32 result = png_get_pCAL(png_ptr, info_ptr, 
                                            &retrieved_purpose,
                                            &retrieved_X0, &retrieved_X1,
                                            &retrieved_type, &retrieved_nparams,
                                            &retrieved_units, &retrieved_params);
            (void)result;
            
            // DO NOT free strings returned by png_get_pCAL - they are managed by libpng
            // and will be freed by png_destroy_info_struct
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}