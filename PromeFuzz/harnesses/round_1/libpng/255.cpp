// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr) {
        // Just fill with zeros
        memset(data, 0, length);
    }
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function - do nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing to avoid aborting
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_read) return 0;

    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }

    // Set up custom I/O
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    png_init_io(png_ptr_read, fp);

    // Try to read PNG header
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        // PNG read error - clean up and continue
        fclose(fp);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        
        // Try write operations instead
        png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                           nullptr, dummy_error_fn, dummy_warning_fn);
        if (!png_ptr_write) return 0;
        
        png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
        if (!info_ptr_write) {
            png_destroy_write_struct(&png_ptr_write, nullptr);
            return 0;
        }
        
        // Set up write I/O
        png_set_write_fn(png_ptr_write, nullptr, dummy_write_fn, dummy_flush_fn);
        
        // Test png_set_PLTE with random data
        if (Size >= sizeof(png_color) * 3) {
            png_color palette[3];
            for (int i = 0; i < 3 && Size >= sizeof(png_color) * (i+1); i++) {
                palette[i].red = Data[i * sizeof(png_color)];
                palette[i].green = Data[i * sizeof(png_color) + 1];
                palette[i].blue = Data[i * sizeof(png_color) + 2];
            }
            int num_palette = Size < 3 ? Size : 3;
            png_set_PLTE(png_ptr_write, info_ptr_write, palette, num_palette);
        }
        
        // Test png_set_hIST with random data
        if (Size >= sizeof(png_uint_16) * 3) {
            png_uint_16 hist[3];
            for (size_t i = 0; i < 3 && i * sizeof(png_uint_16) < Size; i++) {
                hist[i] = *reinterpret_cast<const png_uint_16*>(Data + i * sizeof(png_uint_16));
            }
            png_set_hIST(png_ptr_write, info_ptr_write, hist);
        }
        
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }

    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);

    // Test png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                 &color_type, &interlace_method, &compression_method, &filter_method);

    // Test png_get_palette_max
    png_get_palette_max(png_ptr_read, info_ptr_read);

    // Test png_get_sPLT - fix: use correct type
    png_sPLT_tp entries = nullptr;  // Changed from png_sPLT_tpp to png_sPLT_tp
    png_get_sPLT(png_ptr_read, info_ptr_read, &entries);

    // Test png_get_hIST
    png_uint_16* hist = nullptr;
    png_get_hIST(png_ptr_read, info_ptr_read, &hist);

    // Clean up read structures
    fclose(fp);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);

    // Now test write operations
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_write) return 0;
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Set up write I/O
    png_set_write_fn(png_ptr_write, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Set IHDR first for write operations
    png_set_IHDR(png_ptr_write, info_ptr_write, 
                 width > 0 ? width : 100, height > 0 ? height : 100,
                 bit_depth > 0 ? bit_depth : 8,
                 color_type >= 0 && color_type <= 6 ? color_type : PNG_COLOR_TYPE_RGB,
                 interlace_method >= 0 && interlace_method <= 1 ? interlace_method : PNG_INTERLACE_NONE,
                 compression_method >= 0 && compression_method <= 1 ? compression_method : PNG_COMPRESSION_TYPE_BASE,
                 filter_method >= 0 && filter_method <= 1 ? filter_method : PNG_FILTER_TYPE_BASE);
    
    // Test png_set_PLTE with data from fuzzer input
    if (Size >= sizeof(png_color)) {
        // Create palette from fuzzer data
        size_t palette_size = Size / sizeof(png_color);
        if (palette_size > 256) palette_size = 256;
        
        png_color* palette = static_cast<png_color*>(malloc(palette_size * sizeof(png_color)));
        if (palette) {
            for (size_t i = 0; i < palette_size; i++) {
                size_t offset = i * sizeof(png_color);
                palette[i].red = offset < Size ? Data[offset] : 0;
                palette[i].green = offset + 1 < Size ? Data[offset + 1] : 0;
                palette[i].blue = offset + 2 < Size ? Data[offset + 2] : 0;
            }
            png_set_PLTE(png_ptr_write, info_ptr_write, palette, static_cast<int>(palette_size));
            free(palette);
        }
    }
    
    // Test png_set_hIST with data from fuzzer input
    if (Size >= sizeof(png_uint_16)) {
        size_t hist_size = Size / sizeof(png_uint_16);
        if (hist_size > 256) hist_size = 256;
        
        png_uint_16* hist_data = static_cast<png_uint_16*>(malloc(hist_size * sizeof(png_uint_16)));
        if (hist_data) {
            for (size_t i = 0; i < hist_size; i++) {
                size_t offset = i * sizeof(png_uint_16);
                if (offset + sizeof(png_uint_16) <= Size) {
                    hist_data[i] = *reinterpret_cast<const png_uint_16*>(Data + offset);
                } else {
                    hist_data[i] = 0;
                }
            }
            png_set_hIST(png_ptr_write, info_ptr_write, hist_data);
            free(hist_data);
        }
    }
    
    // Clean up write structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}