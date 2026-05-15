// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct FuzzData {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing, errors are handled by setjmp
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from the fuzzer input
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FuzzData* fuzz_data = static_cast<FuzzData*>(io_ptr);
    size_t to_copy = length;
    if (fuzz_data->offset + to_copy > fuzz_data->size) {
        to_copy = fuzz_data->size - fuzz_data->offset;
    }
    
    if (to_copy > 0) {
        memcpy(data, fuzz_data->data + fuzz_data->offset, to_copy);
        fuzz_data->offset += to_copy;
    }
    
    // Fill remaining bytes with zeros if needed
    if (to_copy < length) {
        memset(data + to_copy, 0, length - to_copy);
    }
}

extern "C" int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    // Write data to file as backup for file-based reading
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }
    
    // Create png_struct for reading
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, 
                                                 dummy_error_fn, 
                                                 dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    // Create png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function using fuzzer input
    FuzzData fuzz_data = {Data, Size, 0};
    png_set_read_fn(png_ptr, &fuzz_data, dummy_read_fn);
    
    // Try different sequences of API calls
    int variant = Size > 0 ? Data[0] % 4 : 0;
    
    switch (variant) {
        case 0: {
            // Normal sequence: read_info -> set_packing -> update_info -> read_row -> read_end
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            // Try to read rows if we have valid dimensions
            png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
            png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
            png_byte color_type = png_get_color_type(png_ptr, info_ptr);
            png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
            
            if (width > 0 && height > 0 && width < 10000 && height < 10000) {
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                if (rowbytes > 0 && rowbytes < 100000) {
                    png_bytep row = static_cast<png_bytep>(png_malloc(png_ptr, rowbytes));
                    if (row) {
                        for (png_uint_32 y = 0; y < height; y++) {
                            png_read_row(png_ptr, row, nullptr);
                        }
                        png_free(png_ptr, row);
                    }
                }
            }
            
            png_read_end(png_ptr, info_ptr);
            break;
        }
        
        case 1: {
            // Try read_info -> read_row (without update_info)
            png_read_info(png_ptr, info_ptr);
            
            png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
            png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
            
            if (width > 0 && height > 0 && width < 10000 && height < 10000) {
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                if (rowbytes > 0 && rowbytes < 100000) {
                    png_bytep row = static_cast<png_bytep>(png_malloc(png_ptr, rowbytes));
                    if (row) {
                        for (png_uint_32 y = 0; y < height && y < 10; y++) {
                            png_read_row(png_ptr, row, nullptr);
                        }
                        png_free(png_ptr, row);
                    }
                }
            }
            
            png_read_end(png_ptr, info_ptr);
            break;
        }
        
        case 2: {
            // Try read_info -> update_info -> read_end (without reading rows)
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            png_read_end(png_ptr, info_ptr);
            break;
        }
        
        case 3: {
            // Try read_info -> read_frame_head (for APNG)
            png_read_info(png_ptr, info_ptr);
            png_read_frame_head(png_ptr, info_ptr);
            break;
        }
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}