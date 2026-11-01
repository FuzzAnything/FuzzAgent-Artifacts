// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_image at pngread.c:696:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    } else {
        // Write a minimal valid PNG header if input is empty to ensure file exists
        const char png_header[] = {
            static_cast<char>(0x89), static_cast<char>(0x50), static_cast<char>(0x4E), static_cast<char>(0x47),
            static_cast<char>(0x0D), static_cast<char>(0x0A), static_cast<char>(0x1A), static_cast<char>(0x0A)
        };
        file.write(png_header, sizeof(png_header));
    }
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file for file-based operations
    write_dummy_png(Data, Size);

    // Allocate buffers for row data
    // We use a fixed small size for safety in fuzzing context, 
    // but scale slightly with input size if reasonable.
    size_t max_rows = 100; 
    if (Size > 100) max_rows = 100; // Cap to avoid OOM
    
    std::vector<png_byte*> row_ptrs(max_rows);
    std::vector<png_byte*> display_ptrs(max_rows);
    std::vector<std::vector<png_byte>> row_buffers(max_rows);
    std::vector<std::vector<png_byte>> display_buffers(max_rows);

    // Initialize buffers
    for (size_t i = 0; i < max_rows; ++i) {
        row_buffers[i].resize(1024); // Fixed width for simplicity
        display_buffers[i].resize(1024);
        row_ptrs[i] = row_buffers[i].data();
        display_ptrs[i] = display_buffers[i].data();
    }

    // --- Test Reading APIs ---
    {
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        FILE *fp = fopen("./dummy_file", "rb");
        
        if (fp) {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (png_ptr) {
                info_ptr = png_create_info_struct(png_ptr);
                if (info_ptr) {
                    if (!setjmp(png_jmpbuf(png_ptr))) {
                        png_init_io(png_ptr, fp);

                        // Attempt to read signature
                        png_byte sig[8];
                        size_t read_sig = fread(sig, 1, 8, fp);
                        if (png_sig_cmp(sig, 0, read_sig) == 0) {
                            png_set_sig_bytes(png_ptr, 8);
                        }

                        // Try to read info
                        png_read_info(png_ptr, info_ptr);
                        
                        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                        int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                        int color_type = png_get_color_type(png_ptr, info_ptr);
                        int channels = png_get_channels(png_ptr, info_ptr);
                        png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
                        
                        // Adjust buffer sizes if valid image found
                        if (width > 0 && height > 0 && row_bytes > 0) {
                            // Resize buffers to actual row bytes
                            for (size_t i = 0; i < max_rows; ++i) {
                                row_buffers[i].resize(row_bytes);
                                display_buffers[i].resize(row_bytes);
                                row_ptrs[i] = row_buffers[i].data();
                                display_ptrs[i] = display_buffers[i].data();
                            }
                        }

                        // Call png_read_image
                        if (height > 0) {
                            png_read_image(png_ptr, row_ptrs.data());
                        }

                        // Call png_read_rows (read a few rows)
                        png_uint_32 rows_to_read = (height > 10) ? 10 : height;
                        if (rows_to_read > 0) {
                            png_read_rows(png_ptr, row_ptrs.data(), display_ptrs.data(), rows_to_read);
                        }

                        // Call png_read_row (single row)
                        if (height > 0) {
                            png_read_row(png_ptr, row_ptrs[0], display_ptrs[0]);
                        }

                        // Check pass info
                        png_byte pass = png_get_current_pass_number(png_ptr);
                        (void)pass;
                        png_uint_32 row_num = png_get_current_row_number(png_ptr);
                        (void)row_num;

                        png_read_end(png_ptr, info_ptr);
                    }
                }
            }
            fclose(fp);
        }

        // Cleanup read structures
        if (info_ptr) png_destroy_info_struct(png_ptr, &info_ptr);
        if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

    // --- Test Writing APIs ---
    // Note: Writing requires a valid PNG structure initialized for writing.
    // Since we don't have a valid image to write from the fuzz input easily,
    // we create a minimal write structure to test the API calls safely.
    {
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        FILE *fp = fopen("./dummy_file", "wb"); // Overwrite for writing test

        if (fp) {
            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (png_ptr) {
                info_ptr = png_create_info_struct(png_ptr);
                if (info_ptr) {
                    if (!setjmp(png_jmpbuf(png_ptr))) {
                        png_init_io(png_ptr, fp);
                        
                        // Create a minimal valid header
                        png_set_IHDR(png_ptr, info_ptr, 10, 10, 8, PNG_COLOR_TYPE_RGB, 
                                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                                     PNG_FILTER_TYPE_DEFAULT);
                        png_write_info(png_ptr, info_ptr);

                        // Prepare dummy row data
                        for (size_t i = 0; i < max_rows; ++i) {
                            memset(row_ptrs[i], 0, 1024);
                        }

                        // Call png_write_rows
                        // We limit to 10 rows to avoid excessive data
                        png_write_rows(png_ptr, row_ptrs.data(), 10);

                        png_write_end(png_ptr, info_ptr);
                    }
                }
            }
            fclose(fp);
        }

        // Cleanup write structures
        if (info_ptr) png_destroy_info_struct(png_ptr, &info_ptr);
        if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    return 0;
}