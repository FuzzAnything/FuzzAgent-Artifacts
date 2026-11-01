// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_filler at pngtrans.c:119:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include <fuzzer/FuzzedDataProvider.h>

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Too small to be a valid PNG

    FuzzedDataProvider data_provider(Data, Size);
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Initialize libpng structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Initialize the read process with the fuzzed data
    {
        static size_t bytes_read = 0;
        static const uint8_t *fuzz_data = nullptr;
        static size_t fuzz_size = 0;
        
        fuzz_data = Data;
        fuzz_size = Size;
        bytes_read = 0;
        
        auto read_callback = [](png_structp png_ptr, png_bytep out, png_size_t length) {
            if (fuzz_data == nullptr || length == 0) return;
            size_t remaining = fuzz_size - bytes_read;
            if (length > remaining) length = remaining;
            memcpy(out, fuzz_data + bytes_read, length);
            bytes_read += length;
        };
        
        png_set_read_fn(png_ptr, nullptr, read_callback);
        
        // Read the IHDR chunk
        png_read_info(png_ptr, info_ptr);
    }
    
    // --- Target API Functions ---
    
    // 1. png_set_gray_to_rgb
    // Must be called after png_read_info and before png_read_update_info
    png_set_gray_to_rgb(png_ptr);
    
    // 2. png_set_user_transform_info
    // Sets custom transform parameters. 
    void *user_transform_ptr = nullptr;
    if (data_provider.ConsumeBool()) {
        static char dummy_buffer[64];
        user_transform_ptr = dummy_buffer;
    }
    int user_transform_depth = data_provider.ConsumeIntegralInRange<int>(1, 16);
    int user_transform_channels = data_provider.ConsumeIntegralInRange<int>(1, 4);
    
    png_set_user_transform_info(png_ptr, user_transform_ptr, user_transform_depth, user_transform_channels);
    
    // 3. png_get_IHDR
    // Retrieve image header metadata
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // 4. png_set_filler
    // Insert a filler byte. 
    png_uint_32 filler = data_provider.ConsumeIntegralInRange<png_uint_32>(0, 65535);
    int flags = data_provider.ConsumeBool() ? PNG_FILLER_BEFORE : PNG_FILLER_AFTER;
    
    png_set_filler(png_ptr, filler, flags);
    
    // 5. png_set_add_alpha
    // Add an alpha channel.
    png_uint_32 alpha_filler = data_provider.ConsumeIntegralInRange<png_uint_32>(0, 65535);
    int alpha_flags = data_provider.ConsumeBool() ? PNG_FILLER_BEFORE : PNG_FILLER_AFTER;
    
    png_set_add_alpha(png_ptr, alpha_filler, alpha_flags);
    
    // 6. png_set_packswap
    // Swap pixel bit order.
    png_set_packswap(png_ptr);
    
    // Perform png_read_update_info to finalize transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Read the image data to trigger the transformations
    // We need to allocate rows
    png_bytep *row_pointers = nullptr;
    if (height > 0) {
        row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
        if (row_pointers) {
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
            }
            
            png_read_image(png_ptr, row_pointers);
            
            // Cleanup rows
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    // End of read
    png_read_end(png_ptr, nullptr);
    
    cleanup_png(&png_ptr, &info_ptr);
    
    return 0;
}