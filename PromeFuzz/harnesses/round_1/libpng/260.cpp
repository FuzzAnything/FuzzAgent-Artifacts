// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error function
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning function
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write fuzzer data to dummy file
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Create read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_read) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up read structure
    png_init_io(png_ptr_read, fp);
    
    // Create write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                        nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up write structure with dummy I/O
    png_set_write_fn(png_ptr_write, nullptr, dummy_write_data, dummy_flush_data);
    
    // Use fuzzer data to derive parameter values
    // Ensure we have enough data for all parameters
    if (Size >= 20) {
        // Extract parameters from fuzzer data
        int strategy = Data[0] % 5;  // ZLIB strategies: 0-4
        int level = Data[1] % 10;    // Compression levels: 0-9
        int method = Data[2] % 9;    // Compression methods: 0-8 (only 8 is valid)
        int mem_level = (Data[3] % 9) + 1;  // Memory levels: 1-9
        
        // Extract buffer size (ensure it's in valid range 1-2^31-1)
        uint32_t buffer_size_raw;
        memcpy(&buffer_size_raw, Data + 4, sizeof(uint32_t));
        size_t buffer_size = (buffer_size_raw % 2147483647) + 1;  // 2^31-1 = 2147483647
        
        // Test with read structure
        png_set_compression_buffer_size(png_ptr_read, buffer_size);
        size_t read_buffer_size = png_get_compression_buffer_size(png_ptr_read);
        (void)read_buffer_size;  // Use result to avoid unused variable warning
        
        // Test with write structure
        png_set_compression_strategy(png_ptr_write, strategy);
        png_set_compression_level(png_ptr_write, level);
        png_set_compression_method(png_ptr_write, method);
        png_set_text_compression_mem_level(png_ptr_write, mem_level);
        png_set_compression_buffer_size(png_ptr_write, buffer_size);
        size_t write_buffer_size = png_get_compression_buffer_size(png_ptr_write);
        (void)write_buffer_size;  // Use result to avoid unused variable warning
        
        // Test edge cases
        if (Size >= 24) {
            // Test with different parameter combinations
            int alt_strategy = Data[8] % 5;
            int alt_level = Data[9] % 10;
            
            png_set_compression_strategy(png_ptr_write, alt_strategy);
            png_set_compression_level(png_ptr_write, alt_level);
            
            // Test buffer size edge cases
            if (Data[10] % 2 == 0) {
                png_set_compression_buffer_size(png_ptr_write, 1);  // Minimum
            } else {
                png_set_compression_buffer_size(png_ptr_write, 2147483647);  // Maximum
            }
        }
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    fclose(fp);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}