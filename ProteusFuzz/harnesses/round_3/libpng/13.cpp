// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
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
#include <setjmp.h>

// Suppress warnings for unused parameters
#ifdef _MSC_VER
#pragma warning(disable: 4100) // unreferenced formal parameter
#endif

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to create a minimal PNG file for reading
static bool create_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return false;
    
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t ihdr_len = 13;
    
    fwrite(&ihdr_len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    uint32_t crc = 0; 
    fwrite(&crc, 4, 1, f);
    
    uint32_t idat_len = 10;
    fwrite(&idat_len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    uint8_t dummy_data[10] = {0x78, 0x9c, 0x63, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00};
    fwrite(dummy_data, 1, 10, f);
    fwrite(&crc, 4, 1, f);
    
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
    return true;
}

// Helper to create an APNG file with acTL chunk
static bool create_dummy_apng_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return false;
    
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t ihdr_len = 13;
    
    fwrite(&ihdr_len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    uint32_t num_frames = 2;
    uint32_t num_plays = 1;
    uint32_t actl_len = 8;
    
    fwrite(&actl_len, 4, 1, f);
    fwrite("acTL", 4, 1, f);
    fwrite(&num_frames, 4, 1, f);
    fwrite(&num_plays, 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    uint32_t fctl_len = 26;
    uint32_t seq_num = 0;
    uint32_t frame_width = 1, frame_height = 1, frame_x = 0, frame_y = 0;
    uint16_t delay_num = 1, delay_den = 100;
    uint8_t dispose_op = 0, blend_op = 0;
    
    fwrite(&fctl_len, 4, 1, f);
    fwrite("fcTL", 4, 1, f);
    fwrite(&seq_num, 4, 1, f);
    fwrite(&frame_width, 4, 1, f);
    fwrite(&frame_height, 4, 1, f);
    fwrite(&frame_x, 4, 1, f);
    fwrite(&frame_y, 4, 1, f);
    fwrite(&delay_num, 2, 1, f);
    fwrite(&delay_den, 2, 1, f);
    fwrite(&dispose_op, 1, 1, f);
    fwrite(&blend_op, 1, 1, f);
    fwrite(&crc, 4, 1, f);
    
    uint32_t idat_len = 10;
    fwrite(&idat_len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    uint8_t dummy_data[10] = {0x78, 0x9c, 0x63, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00};
    fwrite(dummy_data, 1, 10, f);
    fwrite(&crc, 4, 1, f);
    
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int scenario = Data[0] % 4;
    
    if (scenario == 0) {
        if (!create_dummy_png_file("./dummy_file", Data, Size)) return 0;
        
        FILE *fp = fopen("./dummy_file", "rb");
        if (!fp) return 0;
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 0;
        }
        
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        
        png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
        png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
        png_uint_32 width = png_get_next_frame_width(png_ptr, info_ptr);
        
        png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
        png_uint_32 frames_out, plays_out;
        png_get_acTL(png_ptr, info_ptr, &frames_out, &plays_out);
        
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }
    else if (scenario == 1) {
        if (!create_dummy_apng_file("./dummy_file", Data, Size)) return 0;
        
        FILE *fp = fopen("./dummy_file", "rb");
        if (!fp) return 0;
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 0;
        }
        
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        
        png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
        png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
        png_uint_32 width = png_get_next_frame_width(png_ptr, info_ptr);
        png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
        
        png_uint_32 frames_out, plays_out;
        png_get_acTL(png_ptr, info_ptr, &frames_out, &plays_out);
        
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }
    else if (scenario == 2) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (!fp) return 0;
        
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            fclose(fp);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(fp);
            return 0;
        }
        
        png_init_io(png_ptr, fp);
        
        png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        
        // Safe access to Data[1] and Data[2] with bounds checking
        png_uint_32 num_frames = 1;
        png_uint_32 num_plays = 1;
        if (Size > 1) num_frames = Data[1] + 1;
        if (Size > 2) num_plays = Data[2] + 1;
        
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
        
        png_write_info(png_ptr, info_ptr);
        
        png_byte row[3] = {0, 0, 0};
        png_write_row(png_ptr, row);
        
        png_write_end(png_ptr, info_ptr);
        
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }
    else {
        png_structp png_ptr = NULL;
        png_infop info_ptr = NULL;
        
        png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
        png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
        png_uint_32 width = png_get_next_frame_width(png_ptr, info_ptr);
        png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
        
        png_uint_32 frames_out, plays_out;
        png_get_acTL(png_ptr, info_ptr, &frames_out, &plays_out);
        
        png_set_acTL(png_ptr, info_ptr, 0, 0);
        
        png_structp valid_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (valid_png) {
            png_set_acTL(valid_png, NULL, 1, 1);
            png_get_num_frames(valid_png, NULL);
            png_get_next_frame_y_offset(valid_png, NULL);
            png_get_next_frame_width(valid_png, NULL);
            png_get_num_plays(valid_png, NULL);
            png_get_acTL(valid_png, NULL, &frames_out, &plays_out);
            png_destroy_read_struct(&valid_png, NULL, NULL);
        }
        
        return 0;
    }
}