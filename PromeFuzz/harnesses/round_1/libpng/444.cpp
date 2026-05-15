// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <fstream>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_444(const uint8_t *Data, size_t Size) {
    // Prepare dummy file for any file operations
    write_dummy_file(Data, Size);
    
    // Initialize png_struct for writing
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        dummy_error_fn, 
        dummy_warning_fn
    );
    
    if (!png_ptr) {
        return 0;
    }
    
    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Test png_write_chunk_start and png_write_chunk_data
    if (Size >= 8) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data, 4);
        png_uint_32 length = Size - 4;
        if (length > PNG_UINT_31_MAX) {
            length = PNG_UINT_31_MAX;
        }
        
        // Open file for writing
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            png_init_io(png_ptr, fp);
            
            png_write_chunk_start(png_ptr, chunk_name, length);
            
            if (Size > 4) {
                png_write_chunk_data(png_ptr, Data + 4, length);
            } else {
                png_write_chunk_data(png_ptr, nullptr, 0);
            }
            
            fclose(fp);
        }
    }
    
    // Test png_get_uint_31
    if (Size >= 4) {
        png_byte buf[4];
        memcpy(buf, Data, 4);
        png_uint_32 result = png_get_uint_31(png_ptr, buf);
        (void)result; // Prevent unused variable warning
    }
    
    // Test png_save_uint_16
    if (Size >= 2) {
        png_byte buf16[2];
        unsigned int value16 = (Data[0] << 8) | Data[1];
        png_save_uint_16(buf16, value16);
    }
    
    // Test png_save_uint_32
    if (Size >= 4) {
        png_byte buf32[4];
        png_uint_32 value32 = 0;
        for (size_t i = 0; i < 4 && i < Size; ++i) {
            value32 = (value32 << 8) | Data[i];
        }
        png_save_uint_32(buf32, value32);
    }
    
    // Test png_save_int_32
    if (Size >= 4) {
        png_byte buf_int32[4];
        png_int_32 value_int32 = 0;
        for (size_t i = 0; i < 4 && i < Size; ++i) {
            value_int32 = (value_int32 << 8) | Data[i];
        }
        png_save_int_32(buf_int32, value_int32);
    }
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}