// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
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
#include <vector>

#include "png.h"

// Custom read function for memory buffer
struct PngReadState {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    PngReadState* state = (PngReadState*)png_get_io_ptr(png_ptr);
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read error");
        return;
    }
    memcpy(data, state->data + state->offset, length);
    state->offset += length;
}

// Progressive read callbacks
static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing
}

static void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // For progressive combine test
    png_byte* old_row = (png_byte*)png_get_progressive_ptr(png_ptr);
    if (old_row && new_row) {
        png_progressive_combine_row(png_ptr, old_row, new_row);
    }
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Test 1: Normal PNG reading with png_read_rows and png_read_row
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            return 0;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            goto progressive_test;
        }

        // Set up custom read function
        PngReadState state{Data, Size, 0};
        png_set_read_fn(png_ptr, &state, user_read_data);

        // Read PNG info
        png_read_info(png_ptr, info_ptr);

        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        // Limit dimensions to avoid excessive memory usage
        if (width > 4096 || height > 4096) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            goto progressive_test;
        }

        // Transformations
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png_ptr);
        }
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png_ptr);
        }
        if (bit_depth == 16) {
            png_set_strip_16(png_ptr);
        }
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png_ptr);
        }

        png_read_update_info(png_ptr, info_ptr);

        png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        if (rowbytes == 0 || rowbytes > 1048576) { // Limit row size to 1MB
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            goto progressive_test;
        }

        // Test png_read_rows
        std::vector<png_bytep> row_pointers(height);
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)malloc(rowbytes);
            if (!row_pointers[y]) {
                for (png_uint_32 i = 0; i < y; i++) {
                    free(row_pointers[i]);
                }
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                goto progressive_test;
            }
        }

        // Read multiple rows at once
        png_uint_32 rows_to_read = height > 10 ? 10 : height;
        if (rows_to_read > 0) {
            png_read_rows(png_ptr, row_pointers.data(), nullptr, rows_to_read);
        }

        // Test png_read_row in a loop
        for (png_uint_32 y = rows_to_read; y < height; y++) {
            png_read_row(png_ptr, row_pointers[y], nullptr);
        }

        // Test png_get_current_pass_number
        png_byte pass_num = png_get_current_pass_number(png_ptr);
        (void)pass_num; // Use the value

        // Cleanup
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

progressive_test:
    // Test 2: Progressive PNG reading
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            return 0;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            return 0;
        }

        // Set up progressive callbacks
        png_set_progressive_read_fn(png_ptr, nullptr, info_callback, row_callback, end_callback);

        // Process data in chunks
        size_t chunk_size = 1024;
        size_t processed = 0;
        
        while (processed < Size) {
            size_t to_process = (Size - processed) > chunk_size ? chunk_size : (Size - processed);
            png_process_data(png_ptr, info_ptr, (png_bytep)(Data + processed), to_process);
            processed += to_process;
            
            // Test png_get_progressive_ptr
            void* prog_ptr = png_get_progressive_ptr(png_ptr);
            (void)prog_ptr; // Use the value
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

    return 0;
}