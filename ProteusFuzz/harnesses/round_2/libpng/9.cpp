// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>

static void setup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) return;
    
    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
        return;
    }
}

static void cleanup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    // Initialize PNG structures
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == NULL || info_ptr == NULL) {
        return 0;
    }
    
    // Test png_set_invalid with various masks
    // PNG_INFO_IDAT, PNG_INFO_PLTE are standard valid flags
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    // PNG_INFO_tEXt is often available, but to be safe we stick to core ones
    // PNG_INFO_IHDR is not a valid mask for png_set_invalid in many versions as IHDR is critical
    // We use PNG_INFO_IDAT and PNG_INFO_PLTE which are safe
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT | PNG_INFO_PLTE);
    png_set_invalid(png_ptr, info_ptr, 0);
    png_set_invalid(NULL, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, NULL, PNG_INFO_IDAT);
    
    // Test png_data_freer with different modes and masks
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_IDAT);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_PLTE);
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_IDAT | PNG_INFO_PLTE);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, 0);
    
    // Test png_free_data with different masks and indices
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_free_data(png_ptr, info_ptr, PNG_FREE_PLTE, 0);
    png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, 0);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ICCP, 0);
    png_free_data(png_ptr, info_ptr, PNG_FREE_HIST, 0);
    png_free_data(NULL, info_ptr, PNG_FREE_ALL, -1);
    png_free_data(png_ptr, NULL, PNG_FREE_ALL, -1);
    
    // Test png_get_rows
    png_byte **rows = png_get_rows(png_ptr, info_ptr);
    (void)rows; // Suppress unused variable warning
    
    // Test png_get_PLTE
    png_colorp palette = NULL;
    int num_palette = 0;
    png_uint_32 plte_result = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    (void)plte_result; // Suppress unused variable warning
    
    // Test png_get_text
    png_textp text_ptr = NULL;
    int num_text = 0;
    int text_result = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    (void)text_result; // Suppress unused variable warning
    
    // Cleanup
    cleanup_png_structs(&png_ptr, &info_ptr);
    
    return 0;
}