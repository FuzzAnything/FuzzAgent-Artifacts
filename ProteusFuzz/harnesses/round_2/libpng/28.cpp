// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <cstring>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid PNG header
    static const uint8_t png_sig[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal 1x1 grayscale)
    uint32_t width = 1, height = 1;
    uint8_t bit_depth = 8, color_type = 0; // Grayscale
    uint8_t compression = 0, filter = 0, interlace = 0;
    
    uint8_t ihdr_data[13];
    memcpy(ihdr_data, &width, 4);
    memcpy(ihdr_data + 4, &height, 4);
    ihdr_data[8] = bit_depth;
    ihdr_data[9] = color_type;
    ihdr_data[10] = compression;
    ihdr_data[11] = filter;
    ihdr_data[12] = interlace;
    
    uint32_t chunk_len = 13;
    
    fwrite(&chunk_len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(ihdr_data, 13, 1, f);
    
    // Write IDAT chunk with provided data (or minimal data if size is 0)
    uint8_t *idat_data = (uint8_t *)data;
    uint32_t idat_len = size > 0 ? size : 1;
    if (idat_len == 0) {
        idat_data = (uint8_t *)"\x00";
        idat_len = 1;
    }
    
    fwrite(&idat_len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    fwrite(idat_data, idat_len, 1, f);
    
    // Write IEND chunk
    chunk_len = 0;
    fwrite(&chunk_len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    
    fclose(f);
}

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr, FILE *fp) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    if (fp) {
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a dummy PNG file
    const char *filename = "./dummy_file";
    write_dummy_png(Data, Size, filename);
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;
    
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
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr, fp);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read the signature manually to check validity
    png_byte signature[8];
    if (fread(signature, 1, 8, fp) != 8) {
        safe_cleanup(png_ptr, info_ptr, fp);
        return 0;
    }
    
    // Check signature using png_sig_cmp (correct signature: 3 args)
    if (png_sig_cmp(signature, 0, 8)) {
        safe_cleanup(png_ptr, info_ptr, fp);
        return 0;
    }
    
    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);
    
    // Apply the target API functions
    // 1. png_set_bgr
    png_set_bgr(png_ptr);
    
    // 2. png_set_expand_16
    png_set_expand_16(png_ptr);
    
    // 3. png_set_expand
    png_set_expand(png_ptr);
    
    // 4. png_set_invert_alpha
    png_set_invert_alpha(png_ptr);
    
    // 5. png_set_swap
    png_set_swap(png_ptr);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // 6. png_start_read_image
    png_start_read_image(png_ptr);
    
    // Read a single row to trigger processing
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    if (row) {
        png_read_row(png_ptr, row, NULL);
        png_free(png_ptr, row);
    }
    
    // Clean up
    safe_cleanup(png_ptr, info_ptr, fp);
    
    return 0;
}