// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <png.h>

// Static helper to create a dummy PNG file in memory or on disk
static void create_dummy_png_file() {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;

    // Minimal valid PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);

    // IHDR chunk
    uint32_t length = 13;
    fwrite(&length, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; // RGB
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;

    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // Dummy CRC
    uint32_t dummy_crc = 0;
    fwrite(&dummy_crc, 4, 1, f);

    // PLTE chunk
    length = 3;
    fwrite(&length, 4, 1, f);
    fwrite("PLTE", 4, 1, f);
    uint8_t palette[3] = {0, 0, 0};
    fwrite(palette, 3, 1, f);
    fwrite(&dummy_crc, 4, 1, f);

    // sPLT chunk
    length = 12;
    fwrite(&length, 4, 1, f);
    fwrite("sPLT", 4, 1, f);
    
    const char *name = "test";
    fwrite(name, 4, 1, f);
    uint8_t depth = 8;
    fwrite(&depth, 1, 1, f);
    uint32_t nentries = 1;
    fwrite(&nentries, 4, 1, f);
    
    uint32_t alpha = 65535;
    uint16_t red = 0, green = 0, blue = 0;
    fwrite(&alpha, 4, 1, f);
    fwrite(&red, 2, 1, f);
    fwrite(&green, 2, 1, f);
    fwrite(&blue, 2, 1, f);
    fwrite(&dummy_crc, 4, 1, f);

    // IDAT chunk
    length = 1;
    fwrite(&length, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    uint8_t dummy_data = 0x00;
    fwrite(&dummy_data, 1, 1, f);
    fwrite(&dummy_crc, 4, 1, f);

    // IEND chunk
    length = 0;
    fwrite(&length, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&dummy_crc, 4, 1, f);

    fclose(f);
}

// Custom error handler to prevent libpng from calling longjmp/abort on errors
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    create_dummy_png_file();

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_read_info(png_ptr, info_ptr);
    }

    // Test png_get_palette_max
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max;

    // Test png_get_valid
    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE);
    (void)valid_flags;

    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_sPLT);
    (void)valid_flags;

    // Test png_get_sPLT
    // png_sPLT_tpp is defined as png_sPLT_t **
    // The function expects a pointer to the array of pointers (i.e., png_sPLT_t ***)
    // Wait, signature: png_get_sPLT(..., png_sPLT_tpp entries)
    // png_sPLT_tpp is typedef png_sPLT_t **
    // So the argument is png_sPLT_t **.
    // We need to declare a variable of type png_sPLT_t ** and pass it.
    png_sPLT_t *splt_entries_local = NULL;
    png_sPLT_t **splt_entries_ptr = &splt_entries_local;
    
    int splt_count = png_get_sPLT(png_ptr, info_ptr, splt_entries_ptr);
    (void)splt_count;
    
    // splt_entries_local is now a pointer to an array of png_sPLT_t structs (not pointers to structs)
    // The type of splt_entries_local is png_sPLT_t *
    // So splt_entries_local[i] is a png_sPLT_t struct, not a pointer.
    if (splt_entries_local && splt_count > 0) {
        for (int i = 0; i < splt_count; ++i) {
            // Access members directly using dot operator
            (void)splt_entries_local[i].name;
            (void)splt_entries_local[i].depth;
            (void)splt_entries_local[i].nentries;
            // Check entries array if needed
            if (splt_entries_local[i].entries != NULL) {
                (void)splt_entries_local[i].entries[0].alpha;
            }
        }
    }

    // Test png_set_sPLT
    png_sPLT_t entry;
    entry.name = (char*)"fuzz_palette";
    entry.depth = 8;
    entry.nentries = 1;
    
    png_sPLT_entryp entry_data = (png_sPLT_entryp)malloc(sizeof(png_sPLT_entry));
    if (entry_data) {
        entry_data[0].alpha = 65535;
        entry_data[0].red = 255;
        entry_data[0].green = 0;
        entry_data[0].blue = 0;
        entry.entries = entry_data;
        
        png_set_sPLT(png_ptr, info_ptr, &entry, 1);
        
        free(entry_data);
    }

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}