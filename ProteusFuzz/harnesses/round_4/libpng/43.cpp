// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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

static void
fuzz_error_callback(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
}

static void
fuzz_warning_callback(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void
fuzz_write_data(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static uint8_t safe_read(const uint8_t *Data, size_t Size, size_t index) {
    if (index >= Size) {
        return 0;
    }
    return Data[index];
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_error_callback, fuzz_warning_callback);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up write callbacks
    png_set_write_fn(png_ptr, NULL, fuzz_write_data, NULL);

    // Basic setup for writing
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Create a minimal palette
    png_color palette[256];
    int num_palette = 1;
    if (Size > 0) {
        num_palette = (safe_read(Data, Size, 0) % 256) + 1;
    }
    for (int i = 0; i < num_palette; ++i) {
        palette[i].red = safe_read(Data, Size, (size_t)((size_t)i * 3));
        palette[i].green = safe_read(Data, Size, (size_t)((size_t)i * 3 + 1));
        palette[i].blue = safe_read(Data, Size, (size_t)((size_t)i * 3 + 2));
    }
    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

    // Test 1: png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Test 2: png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette;

    // Parse input to create sPLT data
    int nentries = 0;
    if (Size > 10) {
        nentries = (safe_read(Data, Size, 0) % 10) + 1; // 1 to 10 entries
    }

    png_sPLT_t *splt_entries = NULL;
    if (nentries > 0) {
        splt_entries = (png_sPLT_t *)malloc(nentries * sizeof(png_sPLT_t));
        if (!splt_entries) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        for (int i = 0; i < nentries; ++i) {
            // Generate a valid name
            char name_buf[80];
            size_t name_len = (safe_read(Data, Size, 1 + i) % 20) + 1;
            if (name_len > 79) name_len = 79;
            
            snprintf(name_buf, sizeof(name_buf), "splt%d", i);
            size_t actual_len = strlen(name_buf);
            
            if (actual_len > 79) actual_len = 79;
            
            splt_entries[i].name = (char *)malloc(actual_len + 1);
            if (!splt_entries[i].name) {
                for (int j = 0; j < i; ++j) {
                    free(splt_entries[j].name);
                }
                free(splt_entries);
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return 0;
            }
            memcpy(splt_entries[i].name, name_buf, actual_len);
            splt_entries[i].name[actual_len] = '\0';

            // Depth: 1 to 8
            splt_entries[i].depth = (safe_read(Data, Size, 2 + i) % 8) + 1;

            // Entries count
            splt_entries[i].nentries = (safe_read(Data, Size, 3 + i) % 20) + 1;

            // Allocate palette entries
            splt_entries[i].entries = (png_sPLT_entryp)malloc(splt_entries[i].nentries * sizeof(png_sPLT_entry));
            if (!splt_entries[i].entries) {
                for (int j = 0; j <= i; ++j) {
                    free(splt_entries[j].name);
                }
                free(splt_entries);
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return 0;
            }

            // Fill entries with dummy data
            for (int k = 0; k < splt_entries[i].nentries; ++k) {
                splt_entries[i].entries[k].red = safe_read(Data, Size, 4 + i + k) % 256;
                splt_entries[i].entries[k].green = safe_read(Data, Size, 5 + i + k) % 256;
                splt_entries[i].entries[k].blue = safe_read(Data, Size, 6 + i + k) % 256;
                splt_entries[i].entries[k].alpha = safe_read(Data, Size, 7 + i + k) % 256;
                splt_entries[i].entries[k].frequency = safe_read(Data, Size, 8 + i + k) % 100000;
            }
        }
    }

    // Test 3: png_set_sPLT
    if (splt_entries) {
        png_set_sPLT(png_ptr, info_ptr, splt_entries, nentries);
    }

    // Test 4: png_write_info
    png_write_info(png_ptr, info_ptr);

    // Test 5: png_get_sPLT
    png_sPLT_t *spalettes = NULL;
    int num_spalettes = png_get_sPLT(png_ptr, info_ptr, &spalettes);
    (void)num_spalettes;
    (void)spalettes; 

    // Test 6: png_get_palette_max again
    max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette;

    // Cleanup sPLT entries we allocated
    if (splt_entries) {
        for (int i = 0; i < nentries; ++i) {
            free(splt_entries[i].name);
            free(splt_entries[i].entries);
        }
        free(splt_entries);
    }

    // Cleanup libpng structures
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}