// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>
#include <png.h>

// Custom error handler to prevent crashes during fuzzing
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Use the standard longjmp mechanism provided by libpng
    png_longjmp(png_ptr, 1);
}

// Custom warning handler
static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to initialize PNG structures safely
static bool
init_png_structs(png_structp *png_ptr, png_infop *info_ptr,
                 const uint8_t *data, size_t size) {
    if (!png_ptr || !info_ptr) return false;

    // Create png_struct
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                      fuzz_png_error, fuzz_png_warning);
    if (!*png_ptr) return false;

    // Create png_info
    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        return false;
    }

    // Set up error handling using the standard libpng longjmp mechanism
    if (setjmp(png_jmpbuf(*png_ptr))) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        return false;
    }

    // Create a temporary file to simulate reading from a file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        return false;
    }

    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);

    if (written != size) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        return false;
    }

    // Re-open for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        return false;
    }

    png_init_io(*png_ptr, fp);

    // Attempt to read the signature
    if (size >= 8 && png_sig_cmp(data, 0, 8)) {
        // Not a valid PNG signature, but we still try to read header to trigger
        // error handling paths in the library
        png_set_sig_bytes(*png_ptr, 0);
    } else {
        png_set_sig_bytes(*png_ptr, (size >= 8) ? 8 : (png_uint_32)size);
    }

    // Read the IHDR chunk (header)
    // We use a separate setjmp to catch errors during header reading
    if (setjmp(png_jmpbuf(*png_ptr))) {
        fclose(fp);
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        return false;
    }

    png_read_info(*png_ptr, *info_ptr);
    fclose(fp);

    return true;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Initialize structures
    if (!init_png_structs(&png_ptr, &info_ptr, Data, Size)) {
        // Initialization failed, which is a valid test case for error handling
        return 0;
    }

    // Ensure we have valid pointers before calling APIs
    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // 1. png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    (void)exif_result; // Suppress unused warning
    (void)num_exif;
    (void)exif;

    // 2. png_get_compression_type
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    // 3. png_get_first_frame_is_hidden
    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;

    // 4. png_get_next_frame_blend_op
    png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    (void)blend_op;

    // 5. png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // 6. png_get_next_frame_dispose_op
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}