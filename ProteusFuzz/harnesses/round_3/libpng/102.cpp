// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>

static void dummy_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    // This callback is not used for reading from the fuzzer buffer directly in this setup
    // as we use png_set_sig_bytes and png_read_info which expect a file or stream.
    // For fuzzing, we will write the input to a temporary file and read from it.
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) {
        return 0;
    }

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Initialize PNG structures
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

    // Set up error handling to avoid aborts
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Try to read the signature bytes first if available
    // We check the first 8 bytes of input to see if they look like a PNG signature
    // If not, we might still want to test the functions with partial data or invalid data
    // However, png_read_info expects a valid header.
    // Let's attempt to read the info. If it fails, the setjmp will catch it.
    
    // We can try to skip signature bytes if we know them, but for fuzzing we let libpng handle it.
    // If the input is too short, png_read_info will fail safely via setjmp.
    
    png_read_info(png_ptr, info_ptr);

    // Now call the target API functions
    // 1. png_get_signature
    const png_byte *sig = png_get_signature(png_ptr, info_ptr);
    if (sig) {
        // Just read the data to ensure it's accessible
        volatile png_byte dummy = sig[0];
        (void)dummy;
    }

    // 2. png_get_interlace_type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    // 3. png_get_compression_type
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    // 4. png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // 5. png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // 6. png_get_filter_type
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}