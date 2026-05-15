// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

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

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid data
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent the default behavior of aborting
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    // Create a dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    
    // Write the fuzzer data to the dummy file
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Reopen the file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    
    // Initialize PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Initialize PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }
    
    // Set up custom write functions
    png_set_write_fn(png_ptr, fp, dummy_write_data, dummy_flush_data);
    
    // Set image parameters (minimal valid parameters)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Try different function calls based on input data
    // Use first byte to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        // Always try to write signature
        png_write_sig(png_ptr);
        
        // Try writing info before PLTE
        png_write_info_before_PLTE(png_ptr, info_ptr);
        
        // Try writing a chunk if we have enough data
        if (Size >= 9) {  // Need at least 4 bytes for chunk name + 4 bytes for length + 1 byte data
            const png_byte* chunk_name = Data + 1;
            size_t chunk_length = Data[5] % 256;  // Use a small length to avoid excessive memory usage
            const png_byte* chunk_data = Data + 6;
            
            if (Size >= 6 + chunk_length) {
                png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_length);
            }
        }
        
        // Try writing chunk end (even without start, to test error handling)
        png_write_chunk_end(png_ptr);
        
        // Try writing a row if we have data
        if (Size >= 10) {
            const png_byte* row_data = Data + (Size > 100 ? 100 : Size - 1);
            png_write_row(png_ptr, row_data);
        }
        
        // Try flushing
        png_write_flush(png_ptr);
        
        // Try different combinations based on selector
        if (selector & 0x01) {
            // Call functions in different order
            png_write_flush(png_ptr);
            png_write_row(png_ptr, Data);
        }
        
        if (selector & 0x02) {
            // Try with different chunk names
            png_byte alt_chunk_name[4] = {'t', 'E', 'S', 't'};
            png_write_chunk(png_ptr, alt_chunk_name, Data, Size > 100 ? 100 : Size);
        }
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    return 0;
}