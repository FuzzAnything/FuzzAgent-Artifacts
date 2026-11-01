// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_image at pngread.c:696:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file with the fuzzed data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Apply transformations based on fuzz input diversity
    // Try png_set_invert_mono
    png_set_invert_mono(png_ptr);

    // Try png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr);

    // Try png_set_expand
    png_set_expand(png_ptr);

    // Check RGB to gray status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Suppress unused variable warning

    // Get image dimensions
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

    // Calculate row size and allocate memory
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate array of pointers for rows
    png_byte **row_pointers = static_cast<png_byte **>(malloc(height * sizeof(png_byte *)));
    if (!row_pointers) {
        fclose(fp);
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = static_cast<png_byte *>(malloc(rowbytes));
        if (!row_pointers[y]) {
            // Cleanup allocated rows
            for (png_uint_32 i = 0; i < y; ++i) {
                free(row_pointers[i]);
            }
            free(row_pointers);
            fclose(fp);
            cleanup_png(png_ptr, info_ptr);
            return 0;
        }
    }

    // Read the image using png_read_image
    png_read_image(png_ptr, row_pointers);

    // To test png_read_row, we need to reset the stream and re-read
    // Reset file pointer to re-read for row-by-row test
    rewind(fp);
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    // Re-apply transforms
    png_set_invert_mono(png_ptr);
    png_set_palette_to_rgb(png_ptr);
    png_set_expand(png_ptr);
    
    // Update info after transforms
    png_read_update_info(png_ptr, info_ptr);

    // Allocate buffers for row reading
    png_byte *raw_row = static_cast<png_byte *>(malloc(rowbytes));
    png_byte *display_row = static_cast<png_byte *>(malloc(rowbytes));
    
    if (raw_row && display_row) {
        // Read a single row to test png_read_row
        // Note: This only reads the first row of the current pass
        png_read_row(png_ptr, raw_row, display_row);
        free(raw_row);
        free(display_row);
    }

    // Cleanup
    png_read_end(png_ptr, nullptr);
    fclose(fp);

    // Free row pointers
    for (png_uint_32 y = 0; y < height; ++y) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    cleanup_png(png_ptr, info_ptr);
    return 0;
}