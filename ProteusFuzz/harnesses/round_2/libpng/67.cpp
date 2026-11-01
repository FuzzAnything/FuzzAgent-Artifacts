// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_bKGD at pngget.c:502:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
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
#include <setjmp.h>

// Static helper to set up error handling for libpng
static void fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Static helper to initialize PNG structures safely
static bool init_png_structs(png_structp *png_ptr_out, png_infop *info_ptr_out,
                             const uint8_t *data, size_t size) {
    if (!data || size == 0) return false;

    // Create png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                  fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return false;

    // Create png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    // We need to provide a read callback that reads from the provided buffer
    // We'll use a custom approach: store the data pointer and offset in the io_ptr
    // and implement a custom read function
    
    // For simplicity in fuzzing, we can use png_set_read_fn with a FILE*
    // But since we have a memory buffer, we need a custom read function
    
    // Let's use a simple approach: create a temporary file or use memory mapping
    // However, the cleanest way for libpng is to use png_set_read_fn with a custom callback
    
    // We'll use a static buffer approach for the callback
    static const uint8_t *g_data = nullptr;
    static size_t g_size = 0;
    static size_t g_offset = 0;
    
    g_data = data;
    g_size = size;
    g_offset = 0;
    
    // Set the read function
    png_set_read_fn(png_ptr, (png_voidp)(intptr_t)0, [](png_structp png_ptr, png_bytep data, png_size_t length) {
        (void)png_ptr;
        static const uint8_t *g_data = nullptr;
        static size_t g_size = 0;
        static size_t g_offset = 0;
        
        // This won't work directly because we need to access the static variables
        // Let's use a different approach with a struct
    });
    
    // Actually, let's use a simpler approach: use a FILE pointer to a temporary file
    // Or better yet, use the memory-based approach with a custom struct
    
    // Let's implement a proper memory-based reader
    struct MemoryReader {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    static MemoryReader reader;
    reader.data = data;
    reader.size = size;
    reader.offset = 0;
    
    png_set_read_fn(png_ptr, (png_voidp)&reader, [](png_structp png_ptr, png_bytep data, png_size_t length) {
        (void)png_ptr;
        MemoryReader *reader = (MemoryReader*)png_get_io_ptr(png_ptr);
        if (reader->offset + length > reader->size) {
            length = reader->size - reader->offset;
        }
        if (length > 0) {
            memcpy(data, reader->data + reader->offset, length);
            reader->offset += length;
        }
    });

    *png_ptr_out = png_ptr;
    *info_ptr_out = info_ptr;
    return true;
}

// Static helper to read the PNG header
static bool read_png_header(png_structp png_ptr, png_infop info_ptr) {
    if (setjmp(png_jmpbuf(png_ptr))) {
        return false;
    }

    // Read the header
    png_read_info(png_ptr, info_ptr);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Initialize structures
    if (!init_png_structs(&png_ptr, &info_ptr, Data, Size)) {
        return 0;
    }

    // Attempt to read the header
    if (!read_png_header(png_ptr, info_ptr)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // --- Fuzzing Target Functions ---

    // 1. png_get_tRNS
    png_byte *trans_alpha = NULL;
    int num_trans = 0;
    png_color_16 *trans_color = NULL;
    png_uint_32 trns_result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    (void)trns_result; // Suppress unused variable warning

    // 2. png_get_bKGD
    png_color_16 *background = NULL;
    png_uint_32 bgkd_result = png_get_bKGD(png_ptr, info_ptr, &background);
    (void)bgkd_result;

    // 3. png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_uint_32 ihdr_result = png_get_IHDR(png_ptr, info_ptr, &width, &height,
                                           &bit_depth, &color_type,
                                           &interlace_method, &compression_method, &filter_method);
    (void)ihdr_result;

    // 4. png_get_num_plays
    png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
    (void)num_plays;

    // 5. png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // 6. png_get_valid
    // Test various flags. Note: PNG_INFO_IHDR is not a valid flag for png_get_valid 
    // as IHDR is always present if info is valid. We use valid flags like tRNS, bKGD, PLTE.
    png_uint_32 valid_trns = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_uint_32 valid_bgkd = png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD);
    png_uint_32 valid_plte = png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_uint_32 valid_idat = png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT);
    (void)valid_trns;
    (void)valid_bgkd;
    (void)valid_plte;
    (void)valid_idat;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}