// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>

static void
fuzz_read_png_from_file(const uint8_t *data, size_t size) {
    if (size == 0) return;

    // Write fuzz input to a dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;

    // Open the dummy file
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    // Create read struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Apply the target transformation functions
    png_set_palette_to_rgb(png_ptr);
    png_set_invert_alpha(png_ptr);
    png_set_strip_16(png_ptr);
    png_set_invert_mono(png_ptr);
    png_set_expand_gray_1_2_4_to_8(png_ptr);
    png_set_strip_alpha(png_ptr);

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Read the image data
    png_bytep row_pointers[png_get_image_height(png_ptr, info_ptr)];
    for (size_t y = 0; y < png_get_image_height(png_ptr, info_ptr); ++y) {
        row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    // Read end info
    png_read_end(png_ptr, info_ptr);

    // Cleanup
    for (size_t y = 0; y < png_get_image_height(png_ptr, info_ptr); ++y) {
        free(row_pointers[y]);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Basic sanity check
    if (Size < 8) return 0;

    // Fuzz the read process with the target transformations
    fuzz_read_png_from_file(Data, Size);

    return 0;
}