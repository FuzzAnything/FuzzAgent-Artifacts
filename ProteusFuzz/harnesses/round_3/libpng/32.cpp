// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_valid at pngget.c:19:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
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
#include <stddef.h>
#include <cstring>
#include <vector>
#include <cstdio>

// Custom error handler that does nothing to prevent crashes during fuzzing
static void dummy_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing, just return to avoid aborting
}

// Custom warning handler that does nothing
static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Do nothing
}

// Helper to create a png_struct for fuzzing
static png_structp create_png_struct(const uint8_t *data, size_t size) {
    if (!data || size == 0) {
        return nullptr;
    }
    
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_handler, dummy_warning_handler);
    if (!png_ptr) {
        return nullptr;
    }
    
    return png_ptr;
}

// Helper to create a png_info struct
static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) {
        return nullptr;
    }
    return png_create_info_struct(png_ptr);
}

// Helper to destroy structures
static void destroy_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
    }
}

static void destroy_png_info(png_infop info_ptr) {
    if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

// Fuzz driver entry point
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) {
        return 0;
    }

    // Create png_struct
    png_structp png_ptr = create_png_struct(Data, Size);
    if (!png_ptr) {
        return 0;
    }

    // Create png_info
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        destroy_png_struct(png_ptr);
        return 0;
    }

    // Test png_get_valid with various flags
    // Using known valid flags from libpng
    png_uint_32 valid_flags[] = {
        PNG_INFO_tRNS,
        PNG_INFO_PLTE,
        PNG_INFO_gAMA,
        PNG_INFO_cHRM,
        PNG_INFO_sRGB,
        PNG_INFO_iCCP,
        PNG_INFO_sBIT,
        PNG_INFO_hIST,
        PNG_INFO_pHYs,
        PNG_INFO_oFFs,
        PNG_INFO_bKGD,
        PNG_INFO_sPLT,
        PNG_INFO_eXIf,
        PNG_INFO_sCAL,
        PNG_INFO_pCAL,
        0xFFFFFFFF // Invalid flag
    };

    size_t num_flags = sizeof(valid_flags) / sizeof(valid_flags[0]);
    for (size_t i = 0; i < num_flags; i++) {
        png_get_valid(png_ptr, info_ptr, valid_flags[i]);
    }

    // Test png_write_info_before_PLTE
    // We need to set up a valid file pointer to avoid crashes
    // Create a dummy file for writing
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        png_init_io(png_ptr, dummy_file);
        
        // Set minimal required info to avoid errors in png_write_IHDR
        // This prevents the error that was causing the crash
        png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                     PNG_FILTER_TYPE_DEFAULT);
        
        png_write_info_before_PLTE(png_ptr, info_ptr);
        fclose(dummy_file);
    }

    // Test png_set_sPLT and png_get_sPLT
    // Create some dummy sPLT entries
    std::vector<png_sPLT_t> splt_entries;
    if (Size > 0) {
        int num_entries = (Size % 10) + 1; // 1 to 10 entries
        splt_entries.reserve(num_entries);
        
        for (int i = 0; i < num_entries; i++) {
            png_sPLT_t entry;
            entry.name = const_cast<char*>("fuzz_palette");
            entry.depth = (Data[i % Size] % 9) + 1; // 1-8 bits
            entry.nentries = 256;
            
            // Allocate entries array to avoid invalid sPLT error
            if (entry.nentries > 0) {
                entry.entries = static_cast<png_sPLT_entryp>(malloc(entry.nentries * sizeof(png_sPLT_entry)));
                if (entry.entries) {
                    for (int j = 0; j < entry.nentries; j++) {
                        entry.entries[j].red = Data[(i * 256 + j) % Size];
                        entry.entries[j].green = Data[(i * 256 + j + 1) % Size];
                        entry.entries[j].blue = Data[(i * 256 + j + 2) % Size];
                        entry.entries[j].alpha = Data[(i * 256 + j + 3) % Size];
                        entry.entries[j].frequency = 10000;
                    }
                }
            } else {
                entry.entries = nullptr;
            }
            
            splt_entries.push_back(entry);
        }
        
        png_set_sPLT(png_ptr, info_ptr, splt_entries.data(), num_entries);
        
        // Clean up allocated memory
        for (int i = 0; i < num_entries; i++) {
            if (splt_entries[i].entries) {
                free(splt_entries[i].entries);
            }
        }
    }

    // Test png_get_sPLT
    png_sPLT_t *spalettes = nullptr;
    int count = png_get_sPLT(png_ptr, info_ptr, &spalettes);
    // spalettes is now set if there were sPLT chunks

    // Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette; // Suppress unused warning

    // Test with null pointers (should handle gracefully)
    png_get_valid(nullptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, nullptr, PNG_INFO_tRNS);
    png_get_sPLT(nullptr, info_ptr, &spalettes);
    png_get_sPLT(png_ptr, nullptr, &spalettes);
    png_get_palette_max(nullptr, info_ptr);
    png_get_palette_max(png_ptr, nullptr);

    // Cleanup
    destroy_png_info(info_ptr);
    destroy_png_struct(png_ptr);

    return 0;
}