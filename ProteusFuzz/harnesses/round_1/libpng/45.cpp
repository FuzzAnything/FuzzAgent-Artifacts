// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static void setup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) {
        *info_ptr = NULL;
        return;
    }
    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
    }
}

static void cleanup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr != NULL) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

static png_fixed_point extract_fixed_point(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(png_fixed_point) > size) {
        return 0;
    }
    png_fixed_point val;
    memcpy(&val, data + *offset, sizeof(png_fixed_point));
    *offset += sizeof(png_fixed_point);
    return val;
}

static png_uint_32 extract_uint_32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(png_uint_32) > size) {
        return 0;
    }
    png_uint_32 val;
    memcpy(&val, data + *offset, sizeof(png_uint_32));
    *offset += sizeof(png_uint_32);
    return val;
}

static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

static png_byte extract_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(png_byte) > size) {
        return 0;
    }
    png_byte val = data[*offset];
    *offset += sizeof(png_byte);
    return val;
}

// Custom error handler to prevent libpng from aborting
static jmp_buf jump_buffer;
static void custom_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    longjmp(jump_buffer, 1);
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == NULL || info_ptr == NULL) {
        return 0;
    }

    // Set custom error handler to prevent crashes on invalid input
    png_set_error_fn(png_ptr, NULL, custom_error_handler, NULL);

    size_t offset = 0;
    int choice = Data[offset++] % 6;

    int result = 0;
    if (setjmp(jump_buffer) == 0) {
        switch (choice) {
            case 0: { // png_get_sCAL_fixed
                int unit;
                png_fixed_point width, height;
                png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
                break;
            }
            case 1: { // png_set_mDCV_fixed
                if (offset + 10 * sizeof(png_fixed_point) + 2 * sizeof(png_uint_32) > Size) break;
                png_fixed_point int_white_x = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_white_y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_red_x = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_red_y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_green_x = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_green_y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_blue_x = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_blue_y = extract_fixed_point(Data, Size, &offset);
                png_uint_32 max_lum = extract_uint_32(Data, Size, &offset);
                png_uint_32 min_lum = extract_uint_32(Data, Size, &offset);
                png_set_mDCV_fixed(png_ptr, info_ptr, int_white_x, int_white_y,
                                   int_red_x, int_red_y, int_green_x, int_green_y,
                                   int_blue_x, int_blue_y, max_lum, min_lum);
                break;
            }
            case 2: { // png_get_gAMA_fixed
                png_fixed_point int_file_gamma;
                png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
                break;
            }
            case 3: { // png_set_cHRM_XYZ_fixed
                if (offset + 9 * sizeof(png_fixed_point) > Size) break;
                png_fixed_point int_red_X = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_red_Y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_red_Z = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_green_X = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_green_Y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_green_Z = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_blue_X = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_blue_Y = extract_fixed_point(Data, Size, &offset);
                png_fixed_point int_blue_Z = extract_fixed_point(Data, Size, &offset);
                png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                       int_red_X, int_red_Y, int_red_Z,
                                       int_green_X, int_green_Y, int_green_Z,
                                       int_blue_X, int_blue_Y, int_blue_Z);
                break;
            }
            case 4: { // png_get_cHRM_fixed
                png_fixed_point int_white_x, int_white_y;
                png_fixed_point int_red_x, int_red_y;
                png_fixed_point int_green_x, int_green_y;
                png_fixed_point int_blue_x, int_blue_y;
                png_get_cHRM_fixed(png_ptr, info_ptr,
                                   &int_white_x, &int_white_y,
                                   &int_red_x, &int_red_y,
                                   &int_green_x, &int_green_y,
                                   &int_blue_x, &int_blue_y);
                break;
            }
            case 5: { // png_get_cHRM_XYZ_fixed
                png_fixed_point int_red_X, int_red_Y, int_red_Z;
                png_fixed_point int_green_X, int_green_Y, int_green_Z;
                png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
                png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                       &int_red_X, &int_red_Y, &int_red_Z,
                                       &int_green_X, &int_green_Y, &int_green_Z,
                                       &int_blue_X, &int_blue_Y, &int_blue_Z);
                break;
            }
        }
    } else {
        // Error occurred, jump here
        result = 1;
    }

    cleanup_png_structs(&png_ptr, &info_ptr);
    return 0;
}