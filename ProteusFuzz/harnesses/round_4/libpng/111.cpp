// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
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

static void safe_png_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_read_struct(NULL, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create a dummy file for file-based operations
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    // Write input data to the file
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Test Case 1: png_set_first_frame_is_hidden and png_get_first_frame_is_hidden (Write context)
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        // Initialize minimal write state
        // We don't actually open a file for writing to avoid complex setup, 
        // but we test the API logic which operates on the struct flags.
        
        png_byte is_hidden = (Data[0] % 2);
        png_uint_32 ret = png_set_first_frame_is_hidden(png_ptr, info_ptr, is_hidden);
        (void)ret; // Suppress unused variable warning if optimization is on

        png_byte check_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
        (void)check_hidden;

        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    // Test Case 2: png_read_frame_head, png_get_next_frame_dispose_op, png_get_next_frame_blend_op (Read context)
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }

        // Attempt to open the dummy file
        FILE *fp_read = fopen(filename, "rb");
        if (fp_read) {
            png_init_io(png_ptr, fp_read);
            
            // We expect the dummy file to likely be invalid PNG, triggering errors.
            // We wrap the read calls in try-catch logic via setjmp/longjmp.
            
            // Try to read start info
            png_read_info(png_ptr, info_ptr);
            
            // Attempt to read frame head (will likely fail on invalid file, which is fine for fuzzing)
            png_read_frame_head(png_ptr, info_ptr);
            
            // Get dispose op
            png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
            (void)dispose_op;

            // Get blend op
            png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
            (void)blend_op;

            fclose(fp_read);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }

    // Test Case 3: png_write_frame_tail (Write context)
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        // Call tail directly. Note: In a real scenario, this requires head and data to be written first.
        // Fuzzing allows calling in unexpected states to check for robustness.
        png_write_frame_tail(png_ptr, info_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}