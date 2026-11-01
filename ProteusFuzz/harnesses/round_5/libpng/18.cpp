// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
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

static void
write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int
LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: png_get_color_type and png_get_tRNS with NULL pointers (safe guards)
    png_byte color_type_null = png_get_color_type(NULL, NULL);
    (void)color_type_null;

    png_byte *trans_alpha = NULL;
    int num_trans = 0;
    png_color_16 *trans_color = NULL;
    png_uint_32 trns_null = png_get_tRNS(NULL, NULL, &trans_alpha, &num_trans, &trans_color);
    (void)trns_null;

    // Test 2: png_set_tRNS with NULL pointers (safe guards)
    png_set_tRNS(NULL, NULL, NULL, 0, NULL);

    // Test 3: png_process_data with NULL pointers (safe guards)
    png_process_data(NULL, NULL, NULL, 0);

    // Test 4: png_set_expand with NULL pointer (safe guards)
    png_set_expand(NULL);

    // Test 5: png_destroy_info_struct with NULL pointers (safe guards)
    png_destroy_info_struct(NULL, NULL);

    // Test 6: Full read workflow with fuzzed data
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Write dummy file for file-based reading
    write_dummy_file(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read signature and info
    png_byte sig[8];
    size_t sig_read = fread(sig, 1, sizeof(sig), fp);
    if (sig_read == sizeof(sig) && png_sig_cmp(sig, 0, sizeof(sig)) == 0) {
        png_set_sig_bytes(png_ptr, sizeof(sig));
        png_read_info(png_ptr, info_ptr);

        // Call target APIs after header read
        png_byte ct = png_get_color_type(png_ptr, info_ptr);
        (void)ct;

        png_byte *ta = NULL;
        int nt = 0;
        png_color_16 *tc = NULL;
        png_uint_32 trns = png_get_tRNS(png_ptr, info_ptr, &ta, &nt, &tc);
        (void)trns;

        // Test png_set_expand
        png_set_expand(png_ptr);

        // Test png_set_tRNS with dummy data
        png_byte dummy_alpha = 128;
        png_color_16 dummy_trans_color = {0, 0, 0, 0, 0};
        png_set_tRNS(png_ptr, info_ptr, &dummy_alpha, 1, &dummy_trans_color);

        // Read image data if possible
        png_read_image(png_ptr, NULL);
        png_read_end(png_ptr, NULL);
    }

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Test 7: Push reader workflow (png_process_data)
    // Note: png_create_push_struct is deprecated/removed in newer libpng versions.
    // We use png_create_read_struct and manually call png_process_data to simulate push reading.
    png_structp png_push = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_push) return 0;

    png_infop info_push = png_create_info_struct(png_push);
    if (!info_push) {
        png_destroy_read_struct(&png_push, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_push))) {
        png_destroy_read_struct(&png_push, &info_push, NULL);
        return 0;
    }

    // Initialize push reader with dummy file
    FILE *fp_push = fopen("./dummy_file", "rb");
    if (fp_push) {
        png_init_io(png_push, fp_push);

        // Feed data in chunks
        png_byte buffer[256];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp_push)) > 0) {
            png_process_data(png_push, info_push, buffer, bytes_read);
        }
        fclose(fp_push);
    }

    png_destroy_read_struct(&png_push, &info_push, NULL);

    // Test 8: Write workflow with target APIs
    png_structp png_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_write) return 0;

    png_infop info_write = png_create_info_struct(png_write);
    if (!info_write) {
        png_destroy_write_struct(&png_write, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_write))) {
        png_destroy_write_struct(&png_write, &info_write);
        return 0;
    }

    FILE *fp_write = fopen("./dummy_file", "wb");
    if (!fp_write) {
        png_destroy_write_struct(&png_write, &info_write);
        return 0;
    }

    png_init_io(png_write, fp_write);

    // Set minimal image info
    png_set_IHDR(png_write, info_write, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_set_tRNS in write context
    png_byte write_alpha = 255;
    png_color_16 write_trans = {0, 255, 0, 0, 0};
    png_set_tRNS(png_write, info_write, &write_alpha, 1, &write_trans);

    // Test png_set_expand (no-op in write context but safe)
    png_set_expand(png_write);

    png_write_info(png_write, info_write);

    // Write dummy row
    png_byte row[3] = {255, 0, 0};
    png_bytep row_ptr[1] = {row};
    png_write_rows(png_write, row_ptr, 1);

    png_write_end(png_write, info_write);
    fclose(fp_write);

    png_destroy_write_struct(&png_write, &info_write);

    return 0;
}