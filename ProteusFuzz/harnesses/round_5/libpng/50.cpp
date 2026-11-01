// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdio>

static void write_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid PNG signature
    const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal)
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t ihdr_len = 13;
    uint8_t ihdr_type[] = {'I', 'H', 'D', 'R'};
    
    fwrite(&ihdr_len, 4, 1, f);
    fwrite(ihdr_type, 1, 4, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // Simple CRC (not accurate, but sufficient for fuzzing to trigger parsing)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write acTL chunk if we have enough data to construct it
    if (size >= 8) {
        uint32_t num_frames = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        uint32_t num_plays = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
        
        if (num_frames > 0 && num_frames < 0x80000000) {
            uint32_t actl_len = 8;
            uint8_t actl_type[] = {'a', 'c', 'T', 'L'};
            
            fwrite(&actl_len, 4, 1, f);
            fwrite(actl_type, 1, 4, f);
            fwrite(&num_frames, 4, 1, f);
            fwrite(&num_plays, 4, 1, f);
            fwrite(&crc, 4, 1, f);
        }
    }
    
    // Write IDAT chunk (minimal)
    uint32_t idat_len = 1;
    uint8_t idat_type[] = {'I', 'D', 'A', 'T'};
    uint8_t idat_data[] = {0};
    
    fwrite(&idat_len, 4, 1, f);
    fwrite(idat_type, 1, 4, f);
    fwrite(idat_data, 1, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    uint32_t iend_len = 0;
    uint8_t iend_type[] = {'I', 'E', 'N', 'D'};
    
    fwrite(&iend_len, 4, 1, f);
    fwrite(iend_type, 1, 4, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test with file-based reading
    write_dummy_png_file("./dummy_file", Data, Size);
    
    // Test 1: Reading with png_get_acTL
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
        
        FILE *fp = fopen("./dummy_file", "rb");
        if (!fp) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
        
        png_init_io(png_ptr, fp);
        
        // Try to read info
        png_read_info(png_ptr, info_ptr);
        
        // Test png_get_acTL
        png_uint_32 num_frames = 0, num_plays = 0;
        png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
        
        // Test other getter functions
        png_get_next_frame_y_offset(png_ptr, info_ptr);
        png_get_next_frame_width(png_ptr, info_ptr);
        png_get_num_frames(png_ptr, info_ptr);
        png_get_num_plays(png_ptr, info_ptr);
        
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    
    // Test 2: Writing with png_set_acTL
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        // Extract values from fuzz data
        png_uint_32 num_frames = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_uint_32 num_plays = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
        
        // Set acTL chunk
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
        
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
    
    // Test 3: Edge cases with NULL pointers
    {
        png_uint_32 num_frames = 0, num_plays = 0;
        png_get_acTL(NULL, NULL, &num_frames, &num_plays);
        png_get_acTL(NULL, NULL, NULL, NULL);
        png_get_next_frame_y_offset(NULL, NULL);
        png_get_next_frame_width(NULL, NULL);
        png_get_num_frames(NULL, NULL);
        png_get_num_plays(NULL, NULL);
    }
    
    // Test 4: Invalid values for png_set_acTL
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        // Test with zero frames
        png_set_acTL(png_ptr, info_ptr, 0, 1);
        
        // Test with very large values
        png_set_acTL(png_ptr, info_ptr, 0xFFFFFFFF, 0xFFFFFFFF);
        
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
    
    return 0;
}