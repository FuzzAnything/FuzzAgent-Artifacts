// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
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

static void fuzz_png_getters(const uint8_t *data, size_t size) {
    if (size == 0) return;

    // Create a temporary file for the fuzzed input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) return;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Attempt to read the signature
    // Fixed: png_get_signature returns const png_byte *, so we must use that type
    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    if (signature) {
        // Check if the signature is valid PNG magic
        if (!png_sig_cmp(signature, 0, 8)) {
            png_set_sig_bytes(png_ptr, 8);
        }
    }

    // Try to read the header (IHDR)
    // We use png_read_info to populate info_ptr
    // If the file is malformed, the setjmp will catch it
    png_read_info(png_ptr, info_ptr);

    // Close the file as we have read the header
    fclose(fp);

    // Now invoke the target API functions
    // These functions are safe to call even if pointers are null, 
    // but here we pass valid pointers to test the logic after header read.
    
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;

    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    const png_byte *sig = png_get_signature(png_ptr, info_ptr);
    (void)sig;

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Fuzz the main logic
    fuzz_png_getters(Data, Size);
    
    // Additional edge case: Test with NULL pointers directly if the API allows
    // The documentation says they return 0 safely, so we can test that path too.
    // Note: png_ptr and info_ptr are not initialized here, but the API handles NULL.
    // To be safe and avoid undefined behavior with uninitialized memory in some 
    // implementations, we rely on the fuzzed path above for valid pointers.
    // But we can explicitly test the NULL handling if we want to ensure robustness.
    // Since the API is designed to be safe, we can call them with NULL.
    
    // Testing NULL pointer handling explicitly
    (void)png_get_color_type(NULL, NULL);
    (void)png_get_filter_type(NULL, NULL);
    (void)png_get_compression_type(NULL, NULL);
    (void)png_get_interlace_type(NULL, NULL);
    (void)png_get_signature(NULL, NULL);
    (void)png_get_bit_depth(NULL, NULL);

    return 0;
}