// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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
#include <cstdio>
#include <cstring>
#include <vector>

static void write_data(png_structp png_ptr, png_bytep data, size_t length) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

static void write_data_flush(png_structp png_ptr) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fflush(fp);
    }
}

static void write_data_error(png_structp png_ptr) {
    // No-op for fuzzing
}

static void write_data_warning(png_structp png_ptr, png_const_charp message) {
    // No-op for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Create a temporary file for output
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    // Initialize libpng structures
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

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_set_write_fn(png_ptr, fp, write_data, write_data_flush);

    // Basic image parameters from input data
    uint32_t width = (Data[0] & 0x0F) + 1; // 1-16
    uint32_t height = (Data[1] & 0x0F) + 1; // 1-16
    uint32_t bit_depth = 8;
    uint32_t color_type = Data[2] & 0x07; // Limit to valid color types
    uint32_t interlace_type = (Data[3] & 0x01); // 0 or 1

    // Ensure valid color type
    if (color_type == 0 || color_type == 2 || color_type == 3 || color_type == 4 || color_type == 6) {
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Write signature
        png_write_sig(png_ptr);

        // Write info
        png_write_info(png_ptr, info_ptr);

        // Prepare row data
        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        std::vector<png_byte> row_data(row_bytes);
        size_t data_idx = 4;
        size_t data_len = Size - data_idx;

        // Fuzz png_write_row
        for (uint32_t y = 0; y < height; ++y) {
            if (data_len > 0) {
                size_t fill_len = (data_len > row_bytes) ? row_bytes : data_len;
                memcpy(row_data.data(), Data + data_idx, fill_len);
                if (fill_len < row_bytes) {
                    memset(row_data.data() + fill_len, 0, row_bytes - fill_len);
                }
                data_idx += fill_len;
                data_len -= fill_len;

                png_write_row(png_ptr, row_data.data());
                
                // Occasionally flush
                if (y % 2 == 0) {
                    png_write_flush(png_ptr);
                }
            } else {
                // Fill with zeros if no data left
                memset(row_data.data(), 0, row_bytes);
                png_write_row(png_ptr, row_data.data());
            }
        }

        // Fuzz custom chunk writing
        if (data_len > 4) {
            png_byte chunk_name[5] = {'t', 'E', 'X', 't', 0};
            size_t chunk_data_len = data_len - 4;
            if (chunk_data_len > 1024) chunk_data_len = 1024; // Limit chunk size
            
            // Write chunk using png_write_chunk
            png_write_chunk(png_ptr, chunk_name, Data + data_idx, chunk_data_len);
            
            // Write chunk using start/data method
            if (data_len > 10) {
                png_write_chunk_start(png_ptr, chunk_name, 5);
                png_write_chunk_data(png_ptr, Data + data_idx + chunk_data_len, 5);
                // Note: We don't write the CRC here as libpng expects it to be handled internally
                // This is a simplified fuzzing approach
            }
        }

        // End the image
        png_write_end(png_ptr, info_ptr);
    }

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return 0;
}