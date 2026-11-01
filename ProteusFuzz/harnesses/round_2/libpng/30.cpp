// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_png at pngread.c:957:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

// Global jump buffer to handle libpng errors since png_struct is opaque
static jmp_buf g_jmp_buf;

// Static helper to handle errors and aborts during fuzzing
static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(g_jmp_buf, 1);
}

static void fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Static helper to write dummy data to a file if needed
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file for file-based operations
    write_dummy_file(Data, Size);

    // Allocate memory for png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, fuzz_error_handler, fuzz_warning_handler);

    // Use setjmp to catch libpng errors
    if (setjmp(g_jmp_buf)) {
        // Error occurred, cleanup and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Attempt to initialize reading from the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr, fp);
        
        // Try to read the info
        png_read_info(png_ptr, info_ptr);
        
        // Close file after reading info
        fclose(fp);
    } else {
        // If file cannot be opened, we still test the APIs with potentially uninitialized info
        // This is safe as libpng functions handle NULL or invalid states gracefully
    }

    // Test png_set_invalid with various masks
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_tIME);
    png_set_invalid(png_ptr, info_ptr, 0);
    png_set_invalid(NULL, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, NULL, PNG_INFO_IDAT);

    // Test png_get_palette_max
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max;

    // Test png_get_tIME
    // The function expects png_timep * which is png_time *
    // So we need to pass the address of a png_time variable (pointer to pointer)
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    png_uint_32 tIME_result = png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    (void)tIME_result;
    (void)mod_time_ptr;

    // Test png_get_PLTE
    png_colorp palette = NULL;
    int num_palette = 0;
    png_uint_32 PLTE_result = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    (void)PLTE_result;
    (void)palette;
    (void)num_palette;

    // Test png_read_png (this might fail if the file is not a valid PNG, which is expected in fuzzing)
    // We use a dummy file, so this is likely to trigger errors, which are caught by setjmp
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Test png_get_text
    png_textp text_ptr = NULL;
    int num_text = 0;
    int text_result = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    (void)text_result;
    (void)text_ptr;
    (void)num_text;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}