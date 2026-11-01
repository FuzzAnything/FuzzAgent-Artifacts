// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;

    // Create a dummy file for writing operations
    fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    // Initialize png_struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error, dummy_warning);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Initialize write interface
    png_init_io(png_ptr, fp);

    // Fuzzing Logic

    // 1. png_set_eXIf_1
    // Extract EXIF data from input if available
    if (Size >= 4) {
        png_uint_32 num_exif = 0;
        png_byte *exif_data = nullptr;
        
        // Parse size from input
        if (Size >= 4) {
            num_exif = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
            if (num_exif > 0 && num_exif <= Size - 4) {
                exif_data = (png_byte *)(Data + 4);
                png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
            }
        }
    }

    // 2. png_set_unknown_chunks
    // Create unknown chunks from input
    if (Size >= 9) {
        int num_unknowns = 1;
        png_unknown_chunk unknown_chunk;
        memset(&unknown_chunk, 0, sizeof(unknown_chunk));
        
        // Create chunk name (4 bytes + null)
        if (Size >= 5) {
            memcpy(unknown_chunk.name, Data, 4);
            unknown_chunk.name[4] = '\0';
            
            // Set data pointer and size
            if (Size > 5) {
                size_t data_size = Size - 5;
                if (data_size > 0) {
                    unknown_chunk.data = (png_byte *)(Data + 5);
                    unknown_chunk.size = data_size;
                    unknown_chunk.location = (Data[5] % 4) + 1; // Valid location range
                    
                    png_set_unknown_chunks(png_ptr, info_ptr, &unknown_chunk, num_unknowns);
                    
                    // 3. png_set_unknown_chunk_location
                    // Try setting location with various values
                    int chunk_idx = 0;
                    int location = Data[6] % 4;
                    png_set_unknown_chunk_location(png_ptr, info_ptr, chunk_idx, location);
                }
            }
        }
    }

    // 4. png_write_info_before_PLTE
    // This should be called after setting up info
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 5. png_get_eXIf_1
    // Try to retrieve EXIF data (might fail if not set correctly)
    png_uint_32 retrieved_num = 0;
    png_byte *retrieved_exif = nullptr;
    png_uint_32 result = png_get_eXIf_1(png_ptr, info_ptr, &retrieved_num, &retrieved_exif);
    (void)result; // Suppress unused variable warning

    // 6. png_destroy_info_struct
    // Clean up info structure
    png_destroy_info_struct(png_ptr, &info_ptr);

    // Clean up png_struct
    png_destroy_write_struct(&png_ptr, nullptr);
    
    fclose(fp);
    return 0;
}