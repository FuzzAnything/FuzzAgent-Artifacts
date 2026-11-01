// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
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
#include <cstdlib>
#include <vector>
#include <fstream>

// Custom error handler to prevent abort() during fuzzing
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Simply return to allow fuzzing to continue instead of aborting
    return;
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings
    return;
}

static void
cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
        *png_ptr = nullptr;
    }
    // Note: png_destroy_info_struct is typically called via png_destroy_write_struct
    // If called separately, it requires png_const_structrp and png_infopp
    if (info_ptr && *info_ptr) {
        // Only destroy if png_ptr is still valid or if we are in a read context
        // In write context, png_destroy_write_struct handles info cleanup
        // We skip separate call here to avoid double free or invalid pointer issues
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for writing
    const char *dummy_file = "./dummy_file";
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (!ofs) return 0;
    ofs.close();

    // Initialize PNG write structure
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create write struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return 0;

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set custom error handlers to avoid abort
    png_set_error_fn(png_ptr, nullptr, fuzz_png_error, fuzz_png_warning);

    // Initialize dummy file I/O
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzz text compression settings
    // Extract values from input data if available, otherwise use defaults
    int level = 0;
    int mem_level = 1;
    int window_bits = 8;
    int method = 8;

    if (Size > 0) level = Data[0] % 10; // 0-9
    if (Size > 1) mem_level = (Data[1] % 9) + 1; // 1-9
    if (Size > 2) window_bits = (Data[2] % 8) + 8; // 8-15
    if (Size > 3) method = (Data[3] % 2) == 0 ? 8 : 99; // 8 or invalid

    // Call target APIs
    png_set_text_compression_level(png_ptr, level);
    png_set_text_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_method(png_ptr, method);

    // Prepare text chunks to test png_set_text and png_get_text
    std::vector<png_text> text_chunks;
    int num_text = 0;

    // Parse input to create text chunks
    size_t offset = 4;
    while (offset + 4 <= Size) {
        size_t key_len = Data[offset];
        size_t text_len = Data[offset + 1];
        offset += 2;

        if (offset + key_len + text_len > Size) break;

        png_text text;
        text.compression = (Data[offset + key_len] % 3) - 1; // -1, 0, 1
        text.key = (char*)malloc(key_len + 1);
        if (!text.key) break;
        std::memcpy(text.key, Data + offset, key_len);
        text.key[key_len] = '\0';

        text.text_length = text_len;
        text.text = (char*)malloc(text_len + 1);
        if (!text.text) {
            free(text.key);
            break;
        }
        std::memcpy(text.text, Data + offset + key_len, text_len);
        text.text[text_len] = '\0';

        // Set iTXt fields if supported (optional, ignored if not compiled)
        text.itxt_length = 0;
        text.lang = nullptr;
        text.lang_key = nullptr;

        text_chunks.push_back(text);
        num_text++;
        offset += key_len + text_len;
    }

    // Test png_set_text
    if (num_text > 0) {
        png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
    }

    // Test png_get_text 
    // Note: png_get_text expects png_textp * (which is png_text**), not png_textp **
    png_textp text_ptr = nullptr;
    int retrieved_num = 0;
    int result = png_get_text(png_ptr, info_ptr, &text_ptr, &retrieved_num);
    (void)result; // Suppress unused warning

    // Cleanup allocated text data
    for (auto &t : text_chunks) {
        free(t.key);
        free(t.text);
    }

    // Finalize and cleanup
    fclose(fp);
    cleanup_png(&png_ptr, &info_ptr);

    // Remove dummy file
    std::remove(dummy_file);

    return 0;
}