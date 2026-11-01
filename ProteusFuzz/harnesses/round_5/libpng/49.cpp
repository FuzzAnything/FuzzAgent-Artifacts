// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <cstdint>
#include <vector>

static void write_to_file(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Ensure minimum size to safely access indices up to 17
    if (Size < 20) return 0;

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

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

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, fp, write_to_file, nullptr);

    // Extract parameters from fuzz input with bounds checking
    png_uint_32 width = (Data[0] | (Data[1] << 8)) % 1000 + 1;
    png_uint_32 height = (Data[2] | (Data[3] << 8)) % 1000 + 1;
    png_byte bit_depth = (Data[4] % 8) + 1; // 1 to 8
    png_byte color_type = Data[5] % 10; // 0 to 9
    png_byte interlace_type = Data[6] % 2; // 0 or 1
    png_byte compression_type = Data[7] % 2; // 0 or 1
    png_byte filter_type = Data[8] % 6; // 0 to 5
    png_byte chunk_method = Data[9] % 2; // 0 or 1

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);

    // Fuzz png_set_cICP
    png_byte colour_primaries = Data[10] % 256;
    png_byte transfer_function = Data[11] % 256;
    png_byte matrix_coefficients = Data[12] % 256;
    png_byte video_full_range_flag = Data[13] % 2;
    png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                 matrix_coefficients, video_full_range_flag);

    // Fuzz png_set_eXIf_1
    // Fixed: Check if (Size - 20) is greater than 0 before modulo to avoid FPE
    png_uint_32 num_exif = 0;
    if (Size > 20) {
        num_exif = (Data[14] | (Data[15] << 8) | (Data[16] << 16) | (Data[17] << 24)) % (Size - 20) + 1;
    }
    
    if (num_exif > 0 && num_exif <= Size - 20) {
        png_byte *exif_data = new png_byte[num_exif];
        memcpy(exif_data, Data + 20, num_exif);
        png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
        delete[] exif_data;
    }

    // Prepare row pointers for png_set_rows and png_write_image
    png_byte **row_pointers = new png_byte*[height];
    for (png_uint_32 y = 0; y < height; ++y) {
        png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        row_pointers[y] = new png_byte[row_bytes];
        // Use modulo to safely wrap around the input data for row content
        if (Size > 20) {
            size_t offset = 20 + (y * 100) % (Size - 20);
            size_t copy_len = std::min(row_bytes, Size - offset);
            if (copy_len > 0) {
                memcpy(row_pointers[y], Data + offset, copy_len);
                if (row_bytes > copy_len) {
                    memset(row_pointers[y] + copy_len, 0, row_bytes - copy_len);
                }
            } else {
                memset(row_pointers[y], 0, row_bytes);
            }
        } else {
            memset(row_pointers[y], 0, row_bytes);
        }
    }

    png_set_rows(png_ptr, info_ptr, row_pointers);

    // Invoke target functions
    png_write_info_before_PLTE(png_ptr, info_ptr);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);

    // Cleanup
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    for (png_uint_32 y = 0; y < height; ++y) {
        delete[] row_pointers[y];
    }
    delete[] row_pointers;
    fclose(fp);

    return 0;
}