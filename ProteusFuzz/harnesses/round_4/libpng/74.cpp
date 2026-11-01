// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdint.h>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Create a temporary file for libpng to read/write
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    // Write fuzzing data to the file
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (!read_ptr) return 0;
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        return 0;
    }

    // Initialize PNG write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, NULL);
        return 0;
    }

    // Open file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }

    // Set up read pointers
    png_init_io(read_ptr, fp);

    // Set up write pointers (using a dummy file for writing)
    const char *filename_out = "./dummy_file_out";
    FILE *fp_write = fopen(filename_out, "wb");
    if (fp_write) {
        png_init_io(write_ptr, fp_write);
    }

    // Allocate row buffers
    // Limit buffer size to avoid excessive allocation
    size_t max_row_size = 4096;
    png_byte *row = (png_byte *)malloc(max_row_size);
    png_byte *display_row = (png_byte *)malloc(max_row_size);
    
    // Allocate image array (limit rows to 100 to prevent memory exhaustion)
    size_t max_rows = 100;
    png_byte **image = (png_byte **)malloc(max_rows * sizeof(png_byte *));
    png_byte **row_array = (png_byte **)malloc(max_rows * sizeof(png_byte *));

    if (!row || !display_row || !image || !row_array) {
        free(row); free(display_row); free(image); free(row_array);
        fclose(fp);
        if (fp_write) fclose(fp_write);
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }

    // Initialize row pointers
    for (size_t i = 0; i < max_rows; i++) {
        image[i] = (png_byte *)malloc(max_row_size);
        row_array[i] = (png_byte *)malloc(max_row_size);
        if (!image[i] || !row_array[i]) {
            // Cleanup on allocation failure
            for (size_t j = 0; j <= i; j++) {
                free(image[j]);
                free(row_array[j]);
            }
            free(row); free(display_row); free(image); free(row_array);
            fclose(fp);
            if (fp_write) fclose(fp_write);
            png_destroy_read_struct(&read_ptr, &read_info, NULL);
            png_destroy_write_struct(&write_ptr, &write_info);
            return 0;
        }
        memset(image[i], 0, max_row_size);
        memset(row_array[i], 0, max_row_size);
    }

    // Use setjmp to catch longjmp from libpng errors
    if (setjmp(png_jmpbuf(read_ptr)) == 0) {
        // Attempt to read image info
        // This might trigger a longjmp if the file is invalid
        png_read_info(read_ptr, read_info);

        // Get image dimensions safely
        png_uint_32 width = png_get_image_width(read_ptr, read_info);
        png_uint_32 height = png_get_image_height(read_ptr, read_info);
        
        // Limit dimensions to prevent excessive allocation and processing
        if (width > 1024 || height > 1024) {
            width = 1024;
            height = 1024;
        }

        // Test png_set_swap
        png_set_swap(read_ptr);
        png_set_swap(write_ptr);

        // Test png_set_swap_alpha
        png_set_swap_alpha(read_ptr);
        png_set_swap_alpha(write_ptr);

        // Test png_read_image
        png_read_image(read_ptr, image);

        // Test png_read_row (single row)
        // Ensure we have valid data after reading
        png_read_row(read_ptr, row, display_row);

        // Test png_read_rows (multiple rows)
        png_uint_32 num_rows = (height > max_rows) ? max_rows : height;
        if (num_rows > 0) {
            png_read_rows(read_ptr, row_array, NULL, num_rows);
        }

        // Test png_write_rows (write to dummy file)
        if (fp_write) {
            // Set minimal IHDR for writing
            png_set_IHDR(write_ptr, write_info, width, height, 8, PNG_COLOR_TYPE_RGB, 
                         PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            png_write_info(write_ptr, write_info);
            if (num_rows > 0) {
                png_write_rows(write_ptr, row_array, num_rows);
            }
            png_write_end(write_ptr, NULL);
        }
    } else {
        // Longjmp was triggered, handle cleanup
        // The error was caught, so we just proceed to cleanup
    }

    // Cleanup
    fclose(fp);
    if (fp_write) fclose(fp_write);

    for (size_t i = 0; i < max_rows; i++) {
        free(image[i]);
        free(row_array[i]);
    }
    free(row);
    free(display_row);
    free(image);
    free(row_array);

    png_destroy_read_struct(&read_ptr, &read_info, NULL);
    png_destroy_write_struct(&write_ptr, &write_info);

    return 0;
}