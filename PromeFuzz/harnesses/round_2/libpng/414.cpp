// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return nullptr;
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    return info_ptr;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr, FILE* fp) {
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
    if (fp) fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_414(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr, fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr, fp);
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG info (but not the full image yet)
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // Test png_get_iCCP
    char* name = nullptr;
    int compression_type = 0;
    png_byte* profile = nullptr;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    
    // Test png_get_sPLT - fixed: pass entries directly, not &entries
    png_sPLT_tp entries = nullptr;
    png_get_sPLT(png_ptr, info_ptr, &entries);
    
    // Test png_get_hIST
    png_uint_16* hist = nullptr;
    png_get_hIST(png_ptr, info_ptr, &hist);
    
    // Test png_set_sPLT with dummy data
    if (Size > 0) {
        // Create a minimal sPLT entry
        png_sPLT_t dummy_splt;
        dummy_splt.name = (char*)"dummy";
        dummy_splt.depth = 8;
        dummy_splt.nentries = 1;
        
        // Allocate one entry
        png_sPLT_entry* splt_entries = (png_sPLT_entry*)malloc(sizeof(png_sPLT_entry));
        if (splt_entries) {
            // Initialize with fuzzing data if available
            if (Size >= 10) {
                splt_entries[0].red = (png_uint_16)((Data[0] << 8) | Data[1]);
                splt_entries[0].green = (png_uint_16)((Data[2] << 8) | Data[3]);
                splt_entries[0].blue = (png_uint_16)((Data[4] << 8) | Data[5]);
                splt_entries[0].alpha = (png_uint_16)((Data[6] << 8) | Data[7]);
                splt_entries[0].frequency = (png_uint_16)((Data[8] << 8) | Data[9]);
            } else {
                splt_entries[0].red = 0;
                splt_entries[0].green = 0;
                splt_entries[0].blue = 0;
                splt_entries[0].alpha = 0;
                splt_entries[0].frequency = 0;
            }
            dummy_splt.entries = splt_entries;
            
            // Call png_set_sPLT
            png_set_sPLT(png_ptr, info_ptr, &dummy_splt, 1);
            
            free(splt_entries);
        }
    }
    
    // Test png_read_png with different transforms
    // Reset file pointer to beginning
    rewind(fp);
    
    // Create new info struct for reading
    png_infop end_info = png_create_info_struct(png_ptr);
    if (end_info) {
        // Try reading with different transforms
        int transforms = PNG_TRANSFORM_IDENTITY;
        png_read_png(png_ptr, info_ptr, transforms, nullptr);
        
        // Try with different transform combinations
        if (Size > 0) {
            transforms = Data[0] % 16;  // Use fuzzing data to select transforms
            rewind(fp);
            png_read_png(png_ptr, info_ptr, transforms, nullptr);
        }
        
        png_destroy_info_struct(png_ptr, &end_info);
    }
    
    // Cleanup
    cleanup_png(png_ptr, info_ptr, fp);
    return 0;
}