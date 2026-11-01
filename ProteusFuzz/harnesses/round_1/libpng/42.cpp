// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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

static void write_data_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Use longjmp to handle errors gracefully instead of aborting
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, error_callback, nullptr);
    png_init_io(png_ptr, fp);

    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t bit_depth = 8;
    uint32_t color_type = PNG_COLOR_TYPE_RGB;
    uint32_t compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    uint32_t filter_method = PNG_FILTER_TYPE_DEFAULT;
    uint32_t interlace_method = PNG_INTERLACE_NONE;

    if (Size > 0) width = Data[0] % 100 + 1;
    if (Size > 1) height = Data[1] % 100 + 1;
    if (Size > 2) bit_depth = (Data[2] % 8) + 1;
    if (Size > 3) color_type = Data[3] % 10;
    if (Size > 4) compression_type = Data[4] % 2;
    if (Size > 5) filter_method = Data[5] % 6;
    if (Size > 6) interlace_method = Data[6] % 2;

    if (color_type > PNG_COLOR_TYPE_PALETTE) color_type = PNG_COLOR_TYPE_PALETTE;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_type, filter_method);

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    std::vector<png_bytep> row_pointers(height);
    for (uint32_t i = 0; i < height; ++i) {
        row_pointers[i] = (png_bytep)malloc(row_bytes);
        if (!row_pointers[i]) {
            for (uint32_t j = 0; j < i; ++j) {
                free(row_pointers[j]);
            }
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(fp);
            return 0;
        }
        memset(row_pointers[i], 0, row_bytes);
        size_t data_offset = 7 + i * (row_bytes > Size - 7 ? Size - 7 : row_bytes);
        if (data_offset < Size) {
            size_t copy_size = std::min(row_bytes, Size - data_offset);
            memcpy(row_pointers[i], Data + data_offset, copy_size);
        }
    }

    png_write_info(png_ptr, info_ptr);

    if (Size > 10) {
        uint32_t num_frames = (Data[7] % 100) + 1;
        uint32_t num_plays = (Data[8] % 100) + 1;
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
    }

    if (Size > 20) {
        uint32_t frame_width = (Data[9] % width) + 1;
        uint32_t frame_height = (Data[10] % height) + 1;
        uint32_t x_offset = (Data[11] % width);
        uint32_t y_offset = (Data[12] % height);
        uint16_t delay_num = (Data[13] % 100) + 1;
        uint16_t delay_den = (Data[14] % 100) + 1;
        png_byte dispose_op = Data[15] % 3;
        png_byte blend_op = Data[16] % 2;

        png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height,
                                x_offset, y_offset, delay_num, delay_den,
                                dispose_op, blend_op);
    }

    if (Size > 25) {
        uint32_t head_width = (Data[17] % width) + 1;
        uint32_t head_height = (Data[18] % height) + 1;
        uint32_t head_x_offset = (Data[19] % width);
        uint32_t head_y_offset = (Data[20] % height);
        uint16_t head_delay_num = (Data[21] % 100) + 1;
        uint16_t head_delay_den = (Data[22] % 100) + 1;
        png_byte head_dispose_op = Data[23] % 3;
        png_byte head_blend_op = Data[24] % 2;

        png_write_frame_head(png_ptr, info_ptr, &row_pointers[0],
                             head_width, head_height,
                             head_x_offset, head_y_offset,
                             head_delay_num, head_delay_den,
                             head_dispose_op, head_blend_op);
    }

    if (Size > 30) {
        uint32_t num_rows = (Data[25] % height) + 1;
        png_write_rows(png_ptr, &row_pointers[0], num_rows);
    }

    if (Size > 35) {
        png_byte chunk_name[4] = {'t', 'E', 'X', 't'};
        uint32_t chunk_length = (Data[26] % 100) + 1;
        png_write_chunk_start(png_ptr, chunk_name, chunk_length);
        // Write dummy data for the chunk to satisfy the length requirement
        std::vector<png_byte> dummy_data(chunk_length);
        for (uint32_t i = 0; i < chunk_length; ++i) {
            dummy_data[i] = Data[27 + (i % (Size - 27))];
        }
        // Use fwrite to write raw bytes since png_write_data is not a standard API
        fwrite(dummy_data.data(), 1, chunk_length, fp);
        png_write_chunk_end(png_ptr);
    }

    if (Size > 40) {
        png_write_frame_tail(png_ptr, info_ptr);
    }

    // Ensure at least one row is written to satisfy IDAT requirement if not already done
    // If the previous conditions didn't write rows, we write one row now to prevent "No IDATs" error
    if (Size <= 30 && height > 0) {
        png_write_rows(png_ptr, &row_pointers[0], 1);
    }

    png_write_end(png_ptr, info_ptr);

    for (uint32_t i = 0; i < height; ++i) {
        free(row_pointers[i]);
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}