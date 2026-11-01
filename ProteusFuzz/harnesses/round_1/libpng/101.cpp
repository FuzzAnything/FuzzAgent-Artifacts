// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_progressive_frame_fn at pngpread.c:1125:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <setjmp.h>

// Static helper to create a valid PNG header for testing progressive reading
static void write_dummy_png_header(const uint8_t *Data, size_t Size, FILE *fp) {
    // Write a minimal valid PNG signature
    const uint8_t png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_signature, 1, 8, fp);
    
    // Write IHDR chunk (13 bytes data)
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; // RGB
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    
    uint32_t chunk_length = 13;
    const char chunk_type[4] = {'I', 'H', 'D', 'R'};
    
    fwrite(&chunk_length, 4, 1, fp);
    fwrite(chunk_type, 1, 4, fp);
    fwrite(&width, 4, 1, fp);
    fwrite(&height, 4, 1, fp);
    fwrite(&bit_depth, 1, 1, fp);
    fwrite(&color_type, 1, 1, fp);
    fwrite(&compression, 1, 1, fp);
    fwrite(&filter, 1, 1, fp);
    fwrite(&interlace, 1, 1, fp);
    
    // CRC (dummy value)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, fp);
    
    // Write IDAT chunk (minimal compressed data)
    uint32_t idat_len = (Size > 100) ? 100 : Size;
    if (idat_len == 0) idat_len = 10;
    
    chunk_length = idat_len;
    const char idat_type[4] = {'I', 'D', 'A', 'T'};
    
    fwrite(&chunk_length, 4, 1, fp);
    fwrite(idat_type, 1, 4, fp);
    fwrite(Data, 1, idat_len, fp);
    fwrite(&crc, 4, 1, fp);
    
    // Write IEND chunk
    chunk_length = 0;
    const char iend_type[4] = {'I', 'E', 'N', 'D'};
    
    fwrite(&chunk_length, 4, 1, fp);
    fwrite(iend_type, 1, 4, fp);
    fwrite(&crc, 4, 1, fp);
}

// Callbacks for progressive reading
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void progressive_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// Frame callbacks for APNG (signature: void (*)(png_structp, png_uint_32))
static void progressive_frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    (void)png_ptr;
    (void)frame_num;
}

static void progressive_frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    (void)png_ptr;
    (void)frame_num;
}

// User transform callback (signature: void (*)(png_structp, png_row_infop, png_bytep))
static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for progressive reading
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    write_dummy_png_header(Data, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Set up progressive reading
    png_set_progressive_read_fn(png_ptr, (void*)fp, 
        progressive_info_callback, 
        progressive_row_callback, 
        progressive_end_callback);
    
    // Set progressive frame functions (APNG support)
    // Note: Callbacks must match signature: void (*)(png_structp, png_uint_32)
    png_set_progressive_frame_fn(png_ptr, 
        progressive_frame_info_callback, 
        progressive_frame_end_callback);

    // Set user transform function and pointer
    // Correct function name is png_set_read_user_transform_fn
    png_set_read_user_transform_fn(png_ptr, user_transform_callback);
    // Set the user pointer associated with the transform
    // Note: libpng doesn't have a direct setter for user transform ptr in all versions,
    // but png_get_user_transform_ptr exists. We assume the context is handled internally
    // or via the function pointer if the library supports it. 
    // Since there is no png_set_user_transform_ptr in standard libpng API exposed in the error,
    // we skip setting it explicitly if the API doesn't exist, but we can test the getter.

    // Test png_get_user_transform_ptr
    void *user_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_ptr; // Suppress unused warning

    // Test png_get_progressive_ptr
    void *prog_ptr = png_get_progressive_ptr(png_ptr);
    (void)prog_ptr; // Suppress unused warning

    // Read data from file and feed to png_process_data
    uint8_t buffer[1024];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        png_process_data(png_ptr, info_ptr, buffer, bytes_read);
    }

    // Test png_start_read_image
    // Wrap in setjmp to catch potential errors if called at wrong time
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }
    
    png_start_read_image(png_ptr);

    // Test png_progressive_combine_row
    // Allocate dummy rows
    png_byte *old_row = (png_byte*)malloc(1024);
    png_byte *new_row = (png_byte*)malloc(1024);
    if (old_row && new_row) {
        memset(old_row, 0, 1024);
        memset(new_row, 0, 1024);
        png_progressive_combine_row(png_ptr, old_row, new_row);
        // Test with NULL new_row
        png_progressive_combine_row(png_ptr, old_row, NULL);
        free(old_row);
        free(new_row);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    return 0;
}