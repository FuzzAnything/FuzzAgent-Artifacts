// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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

// Define a local buffer for setjmp/longjmp since png_struct members are opaque in newer libpng
static jmp_buf g_jmp_buf;

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(g_jmp_buf, 1);
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize png_struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling using the global buffer
    if (setjmp(g_jmp_buf)) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Set write function (dummy)
    png_set_write_fn(png_ptr, nullptr, dummy_write_data_fn, nullptr);

    // Initialize basic info for IHDR (required before writing info)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // 1. Test png_write_info_before_PLTE
    // This writes signature, IHDR, etc.
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 2. Test png_set_sPLT
    // Create a dummy sPLT entry
    png_sPLT_t splt_entry;
    memset(&splt_entry, 0, sizeof(splt_entry));
    
    // Allocate name
    size_t name_len = (Size > 10) ? 10 : Size;
    splt_entry.name = (char *)malloc(name_len + 1);
    if (splt_entry.name) {
        memcpy(splt_entry.name, Data, name_len);
        splt_entry.name[name_len] = '\0';
    } else {
        splt_entry.name = nullptr;
    }

    splt_entry.depth = 8;
    splt_entry.nentries = (Size > 2) ? (int)(Data[0] % 10) + 1 : 1;

    // Allocate entries
    splt_entry.entries = (png_sPLT_entryp)malloc(splt_entry.nentries * sizeof(png_sPLT_entry));
    if (splt_entry.entries) {
        for (int i = 0; i < splt_entry.nentries; ++i) {
            splt_entry.entries[i].red = Data[(i + 1) % Size];
            splt_entry.entries[i].green = Data[(i + 2) % Size];
            splt_entry.entries[i].blue = Data[(i + 3) % Size];
            splt_entry.entries[i].alpha = Data[(i + 4) % Size];
            splt_entry.entries[i].frequency = (png_uint_16)(Data[(i + 5) % Size] * 256);
        }
    }

    png_set_sPLT(png_ptr, info_ptr, &splt_entry, 1);

    // 3. Test png_get_sPLT
    png_sPLT_t *spalettes = nullptr;
    int nentries = png_get_sPLT(png_ptr, info_ptr, &spalettes);
    (void)nentries; // Use result to suppress unused warning
    // Note: spalettes is a pointer to internal data, do not free it manually

    // 4. Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette;

    // 5. Test png_set_invalid
    // Mark sPLT as invalid
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_sPLT);
    
    // Mark other chunks as invalid for diversity
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_tRNS);

    // 6. Test png_get_valid
    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_sPLT);
    (void)valid_flags;

    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE);
    (void)valid_flags;

    // Cleanup
    if (splt_entry.name) free(splt_entry.name);
    if (splt_entry.entries) free(splt_entry.entries);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}