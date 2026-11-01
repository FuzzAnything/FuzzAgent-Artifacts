// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>
#include <png.h>

// Helper to create a dummy PNG file for reading tests
static void create_dummy_png_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    // Minimal valid PNG signature
    const uint8_t signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, f);

    // IHDR chunk (13 bytes data)
    uint32_t width = 10, height = 10;
    uint8_t bit_depth = 8, color_type = 2; // RGB
    uint8_t compression = 0, filter = 0, interlace = 0;
    uint32_t len = 13;
    uint32_t type = 0x49484452; // "IHDR"
    
    fwrite(&len, 4, 1, f);
    fwrite(&type, 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // CRC (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);

    // acTL chunk (8 bytes data) - Required for APNG
    uint32_t num_frames = 1;
    uint32_t num_plays = 0;
    len = 8;
    type = 0x6163544C; // "acTL"
    
    fwrite(&len, 4, 1, f);
    fwrite(&type, 4, 1, f);
    fwrite(&num_frames, 4, 1, f);
    fwrite(&num_plays, 4, 1, f);
    fwrite(&crc, 4, 1, f);

    // IDAT chunk (dummy compressed data)
    len = 10;
    type = 0x49444154; // "IDAT"
    uint8_t dummy_data[10] = {0x78, 0x9C, 0x63, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00};
    
    fwrite(&len, 4, 1, f);
    fwrite(&type, 4, 1, f);
    fwrite(dummy_data, 1, 10, f);
    fwrite(&crc, 4, 1, f);

    // IEND chunk
    len = 0;
    type = 0x49454E44; // "IEND"
    
    fwrite(&len, 4, 1, f);
    fwrite(&type, 4, 1, f);
    fwrite(&crc, 4, 1, f);

    fclose(f);
}

// Custom read function for libpng
static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr) {
        FILE *fp = (FILE *)io_ptr;
        if (fread(data, 1, length, fp) != length) {
            png_error(png_ptr, "Read Error");
        }
    }
}

// Custom write function for libpng
static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr) {
        FILE *fp = (FILE *)io_ptr;
        if (fwrite(data, 1, length, fp) != length) {
            png_error(png_ptr, "Write Error");
        }
    }
}

// Error handlers
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Use png_longjmp which is the correct API for libpng error handling
    png_longjmp(png_ptr, 1);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Ensure we have enough data to extract parameters
    png_uint_32 width = Data[0] + 1;
    png_uint_32 height = Data[1] + 1;
    png_uint_32 x_offset = Data[2];
    png_uint_32 y_offset = Data[3];
    png_uint_16 delay_num = Data[4] + 1;
    png_uint_16 delay_den = Data[5] + 1;
    png_byte dispose_op = Data[6] & 0x03; // Valid dispose ops: 0-3
    png_byte blend_op = Data[7] & 0x01;   // Valid blend ops: 0-1
    png_byte is_hidden = Data[8] & 0x01;

    // Create dummy file for read tests
    create_dummy_png_file("./dummy_file");

    // --- Reading Path ---
    png_structp png_read_ptr = NULL;
    png_infop info_read_ptr = NULL;
    FILE *fp_read = fopen("./dummy_file", "rb");

    if (fp_read) {
        png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_read_ptr) {
            fclose(fp_read);
            return 0;
        }
        
        info_read_ptr = png_create_info_struct(png_read_ptr);
        if (!info_read_ptr) {
            png_destroy_read_struct(&png_read_ptr, NULL, NULL);
            fclose(fp_read);
            return 0;
        }

        // Use png_set_jmpbuf to set up the jump buffer correctly for the current libpng version
        if (setjmp(png_jmpbuf(png_read_ptr))) {
            png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
            fclose(fp_read);
            return 0;
        }

        png_set_error_fn(png_read_ptr, NULL, dummy_error_fn, dummy_warning_fn);
        png_set_read_fn(png_read_ptr, fp_read, dummy_read_data);

        // Read header
        png_read_info(png_read_ptr, info_read_ptr);

        // Fuzz png_get_next_frame_fcTL
        png_uint_32 w, h, xo, yo;
        png_uint_16 dn, dd;
        png_byte do_op, bo_op;
        png_get_next_frame_fcTL(png_read_ptr, info_read_ptr, &w, &h, &xo, &yo, &dn, &dd, &do_op, &bo_op);

        // Fuzz png_get_next_frame_x_offset
        png_uint_32 x_off = png_get_next_frame_x_offset(png_read_ptr, info_read_ptr);
        (void)x_off;

        // Fuzz png_get_next_frame_height
        png_uint_32 h_val = png_get_next_frame_height(png_read_ptr, info_read_ptr);
        (void)h_val;

        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
        fclose(fp_read);
    }

    // --- Writing Path ---
    png_structp png_write_ptr = NULL;
    png_infop info_write_ptr = NULL;
    FILE *fp_write = fopen("./dummy_file", "wb");

    if (fp_write) {
        png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_write_ptr) {
            fclose(fp_write);
            return 0;
        }

        info_write_ptr = png_create_info_struct(png_write_ptr);
        if (!info_write_ptr) {
            png_destroy_write_struct(&png_write_ptr, NULL);
            fclose(fp_write);
            return 0;
        }

        // Use png_set_jmpbuf to set up the jump buffer correctly
        if (setjmp(png_jmpbuf(png_write_ptr))) {
            png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
            fclose(fp_write);
            return 0;
        }

        png_set_error_fn(png_write_ptr, NULL, dummy_error_fn, dummy_warning_fn);
        png_set_write_fn(png_write_ptr, fp_write, dummy_write_data, NULL);

        // Initialize basic image info
        png_set_IHDR(png_write_ptr, info_write_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Fuzz png_set_first_frame_is_hidden
        png_set_first_frame_is_hidden(png_write_ptr, info_write_ptr, is_hidden);

        // Fuzz png_set_next_frame_fcTL
        png_set_next_frame_fcTL(png_write_ptr, info_write_ptr, width, height, x_offset, y_offset,
                                delay_num, delay_den, dispose_op, blend_op);

        // Prepare dummy row pointers for png_write_frame_head
        png_byte **row_pointers = NULL;
        if (height > 0 && width > 0) {
            size_t row_size = png_get_rowbytes(png_write_ptr, info_write_ptr);
            row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
            if (row_pointers) {
                for (uint32_t i = 0; i < height; ++i) {
                    row_pointers[i] = (png_byte *)malloc(row_size);
                    if (row_pointers[i]) {
                        memset(row_pointers[i], 0xFF, row_size);
                    }
                }
            }
        }

        // Fuzz png_write_frame_head
        if (row_pointers) {
            png_write_frame_head(png_write_ptr, info_write_ptr, row_pointers,
                                 width, height, x_offset, y_offset,
                                 delay_num, delay_den, dispose_op, blend_op);
            
            // Cleanup row pointers
            for (uint32_t i = 0; i < height; ++i) {
                free(row_pointers[i]);
            }
            free(row_pointers);
        }

        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        fclose(fp_write);
    }

    return 0;
}