// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

#include "code/png.h"

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler that longjmps back to the setjmp point
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that reads from the input buffer
    static const uint8_t* buffer = nullptr;
    static png_size_t buffer_size = 0;
    static png_size_t offset = 0;
    
    if (data == nullptr) {
        // Reset buffer
        buffer = nullptr;
        buffer_size = 0;
        offset = 0;
        return;
    }
    
    if (buffer == nullptr) {
        // Initialize buffer from user data
        buffer = (const uint8_t*)png_get_io_ptr(png_ptr);
        if (buffer == nullptr) {
            memset(data, 0, length);
            return;
        }
        
        // Get buffer size from user data
        const uint8_t* size_ptr = buffer + sizeof(void*);
        memcpy(&buffer_size, size_ptr, sizeof(png_size_t));
        offset = 0;
    }
    
    png_size_t to_read = length;
    if (offset + to_read > buffer_size) {
        to_read = buffer_size - offset;
    }
    
    if (to_read > 0) {
        memcpy(data, buffer + offset, to_read);
        offset += to_read;
    }
    
    // Fill remaining with zeros if needed
    if (to_read < length) {
        memset(data + to_read, 0, length - to_read);
    }
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function - do nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function - do nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_341(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the input data
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) {
        remove("./dummy_file");
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Set up custom read function to read from memory
    struct mem_io_data {
        const uint8_t* data;
        png_size_t size;
    };
    
    mem_io_data io_data = {Data, static_cast<png_size_t>(Size)};
    png_set_read_fn(png_ptr, &io_data, dummy_read_fn);
    
    // Reset the dummy read function state
    dummy_read_fn(png_ptr, nullptr, 0);
    
    // Try to read PNG info - this may fail if data is not a valid PNG
    png_read_info(png_ptr, info_ptr);
    
    // Get IHDR information
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // Test png_set_sBIT and png_get_sBIT
    png_color_8 sig_bit;
    if (Size >= 5) {
        sig_bit.red = Data[0] % 16;
        sig_bit.green = Data[1] % 16;
        sig_bit.blue = Data[2] % 16;
        sig_bit.gray = Data[3] % 16;
        sig_bit.alpha = Data[4] % 16;
    } else {
        sig_bit.red = sig_bit.green = sig_bit.blue = sig_bit.gray = sig_bit.alpha = 0;
    }
    
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    
    png_color_8p retrieved_sig_bit = nullptr;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &retrieved_sig_bit);
    (void)sbit_result;
    
    // Test png_set_rgb_to_gray_fixed
    if (Size >= 8) {
        png_set_rgb_to_gray_fixed(png_ptr, Data[5] % 3, 
                                  static_cast<png_fixed_point>(Data[6] * 1000),
                                  static_cast<png_fixed_point>(Data[7] * 1000));
    } else {
        png_set_rgb_to_gray_fixed(png_ptr, 0, 0, 0);
    }
    
    // Get RGB to gray status
    png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_ptr);
    (void)rgb_to_gray_status;
    
    // Test png_set_shift
    png_color_8 true_bits;
    if (Size >= 13) {
        true_bits.red = Data[8] % 9;
        true_bits.green = Data[9] % 9;
        true_bits.blue = Data[10] % 9;
        true_bits.gray = Data[11] % 9;
        true_bits.alpha = Data[12] % 9;
    } else {
        true_bits.red = true_bits.green = true_bits.blue = true_bits.gray = true_bits.alpha = 0;
    }
    
    png_set_shift(png_ptr, &true_bits);
    
    // Create write structure for additional testing
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            // Set up error handling for write structure
            if (setjmp(png_jmpbuf(write_ptr))) {
                // Error occurred during write operations
                png_destroy_write_struct(&write_ptr, &write_info);
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                remove("./dummy_file");
                return 0;
            }
            
            // Set up dummy write functions
            png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
            
            // Test png_set_sBIT on write structure
            png_color_8 write_sig_bit;
            if (Size >= 18) {
                write_sig_bit.red = Data[13] % 16;
                write_sig_bit.green = Data[14] % 16;
                write_sig_bit.blue = Data[15] % 16;
                write_sig_bit.gray = Data[16] % 16;
                write_sig_bit.alpha = Data[17] % 16;
            } else {
                write_sig_bit.red = write_sig_bit.green = write_sig_bit.blue = write_sig_bit.gray = write_sig_bit.alpha = 0;
            }
            
            png_set_sBIT(write_ptr, write_info, &write_sig_bit);
            
            // Clean up write structures
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}