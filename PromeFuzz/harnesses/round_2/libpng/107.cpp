// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that just fills with zeros
    memset(data, 0, length);
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler - do nothing to avoid crashing on errors
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Initialize PNG I/O
    png_init_io(png_ptr, fp);

    // Use fuzzer data to determine which functions to call and with what parameters
    // Use first byte to select function combination
    uint8_t selector = Data[0] % 8;
    
    // Use subsequent bytes for parameters (with bounds checking)
    size_t param_index = 1;
    
    // Call various API functions based on selector
    switch (selector) {
        case 0:
            png_set_gray_to_rgb(png_ptr);
            break;
            
        case 1: {
            png_uint_32 filler = 0;
            int flags = 0;
            if (param_index < Size) {
                filler = Data[param_index++];
                if (param_index < Size) {
                    flags = Data[param_index++] % 3; // 0, 1, or 2
                }
            }
            png_set_add_alpha(png_ptr, filler, flags);
            break;
        }
            
        case 2: {
            int error_action = 0;
            png_fixed_point red = 0, green = 0;
            if (param_index < Size) {
                error_action = Data[param_index++] % 2; // 0 or 1
                if (param_index + 1 < Size) {
                    red = (Data[param_index] << 8) | Data[param_index + 1];
                    param_index += 2;
                }
                if (param_index + 1 < Size) {
                    green = (Data[param_index] << 8) | Data[param_index + 1];
                    param_index += 2;
                }
            }
            png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
            // Also test the getter function
            png_get_rgb_to_gray_status(png_ptr);
            break;
        }
            
        case 3: {
            int mode = 0;
            double output_gamma = 1.0;
            if (param_index < Size) {
                mode = Data[param_index++] % 4; // 0-3 for different modes
                if (param_index < Size) {
                    // Use byte value to create a small gamma value
                    output_gamma = 0.5 + (Data[param_index++] / 255.0);
                }
            }
            png_set_alpha_mode(png_ptr, mode, output_gamma);
            break;
        }
            
        case 4: {
            int mode = 0;
            png_fixed_point output_gamma = 0;
            if (param_index < Size) {
                mode = Data[param_index++] % 4;
                if (param_index + 1 < Size) {
                    output_gamma = (Data[param_index] << 8) | Data[param_index + 1];
                    param_index += 2;
                }
            }
            // Note: png_set_alpha_mode_fixed is available when PNG_FIXED_POINT_SUPPORTED is defined
            // We'll call it conditionally
            #ifdef PNG_FIXED_POINT_SUPPORTED
            png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);
            #endif
            break;
        }
            
        case 5: {
            int error_action = 0;
            double red = 0.0, green = 0.0;
            if (param_index < Size) {
                error_action = Data[param_index++] % 2;
                if (param_index < Size) {
                    red = Data[param_index++] / 255.0;
                }
                if (param_index < Size) {
                    green = Data[param_index++] / 255.0;
                }
            }
            png_set_rgb_to_gray(png_ptr, error_action, red, green);
            // Also test the getter function
            png_get_rgb_to_gray_status(png_ptr);
            break;
        }
            
        case 6:
            png_set_bgr(png_ptr);
            break;
            
        case 7:
            // Test multiple functions in sequence
            png_set_gray_to_rgb(png_ptr);
            if (param_index < Size) {
                png_set_add_alpha(png_ptr, Data[param_index] % 256, Data[param_index] % 3);
                param_index++;
            }
            png_set_bgr(png_ptr);
            break;
    }

    // Try to read the PNG header to exercise the transformations
    png_read_info(png_ptr, info_ptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}