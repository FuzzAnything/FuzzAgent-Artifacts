// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
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

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr, png_bytep row_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    if (row_ptr) {
        free(row_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_bytep row_ptr = nullptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Allocate a dummy row pointer to prevent crashes during potential read operations
    // The size is arbitrary but must be non-zero to simulate a valid row buffer
    // png_bytep is unsigned char*, so we allocate enough bytes for a typical row
    row_ptr = (png_bytep)malloc(1024 * sizeof(png_byte));
    if (!row_ptr) {
        cleanup_png_struct(png_ptr, info_ptr, nullptr);
        return 0;
    }

    // Use setjmp to handle libpng errors safely
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_struct(png_ptr, info_ptr, row_ptr);
        return 0;
    }

    // Initialize a dummy file stream to satisfy libpng requirements
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Re-open for reading
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr, fp);

        // Invoke the target API functions
        // 1. png_set_expand_16
        png_set_expand_16(png_ptr);

        // 2. png_set_swap
        png_set_swap(png_ptr);

        // 3. png_set_invert_alpha
        png_set_invert_alpha(png_ptr);

        // 4. png_set_packing
        png_set_packing(png_ptr);

        // 5. png_set_expand_gray_1_2_4_to_8
        png_set_expand_gray_1_2_4_to_8(png_ptr);

        // 6. png_set_strip_16
        png_set_strip_16(png_ptr);

        // Attempt to read the IHDR to trigger state changes
        // This is necessary because some transformations depend on image info
        png_read_info(png_ptr, info_ptr);

        // Read a single row to trigger the transformations
        // We use a dummy row pointer; if the image is larger, this might fail safely via setjmp
        // Fixed: row_ptr is now png_bytep (unsigned char*) as required by png_read_row
        png_read_row(png_ptr, row_ptr, nullptr);

        fclose(fp);
    }

    cleanup_png_struct(png_ptr, info_ptr, row_ptr);
    return 0;
}