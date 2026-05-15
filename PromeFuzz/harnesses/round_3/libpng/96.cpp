// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
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

#include "code/png.h"

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Silent error handler to avoid output during fuzzing
    (void)png_ptr;
    (void)error_msg;
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Silent warning handler
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Open the dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, error_handler, warning_handler);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize PNG I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Extract PNG info to determine what transformations are applicable
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Use fuzzer data to decide which functions to call
    // We'll use the first few bytes to determine which transformations to apply
    size_t decision_index = 0;
    
    // Apply transformations based on fuzzer input
    if (Size > decision_index && (Data[decision_index] & 0x01)) {
        png_set_packing(png_ptr);
    }
    decision_index++;
    
    if (Size > decision_index && (Data[decision_index] & 0x02)) {
        png_set_invert_mono(png_ptr);
    }
    decision_index++;
    
    if (Size > decision_index && (Data[decision_index] & 0x04)) {
        png_set_swap(png_ptr);
    }
    decision_index++;
    
    if (Size > decision_index && (Data[decision_index] & 0x08)) {
        png_set_swap_alpha(png_ptr);
    }
    decision_index++;
    
    if (Size > decision_index && (Data[decision_index] & 0x10)) {
        png_set_packswap(png_ptr);
    }
    decision_index++;
    
    if (Size > decision_index && (Data[decision_index] & 0x20)) {
        png_set_bgr(png_ptr);
    }
    
    // Update the PNG info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Try to read some image data to exercise the transformations
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes > 0 && height > 0) {
        png_bytep row = (png_bytep)png_malloc(png_ptr, rowbytes);
        if (row) {
            // Read a few rows to exercise the transformations
            for (png_uint_32 row_num = 0; row_num < height && row_num < 10; row_num++) {
                png_read_row(png_ptr, row, nullptr);
            }
            png_free(png_ptr, row);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}