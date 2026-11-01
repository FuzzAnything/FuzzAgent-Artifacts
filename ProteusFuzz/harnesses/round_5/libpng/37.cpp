// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_text at pngset.c:912:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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

// Suppress libpng errors for fuzzing to prevent aborts
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void
fuzz_png_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void
fuzz_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
fuzz_png_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create png_struct with error handling
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (png_ptr == nullptr) return 0;

    // Create png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up dummy callbacks to avoid crashes during internal operations
    png_set_error_fn(png_ptr, nullptr, fuzz_png_error, fuzz_png_warning);
    png_set_read_fn(png_ptr, nullptr, fuzz_png_read_data);
    png_set_write_fn(png_ptr, nullptr, fuzz_png_write_data, fuzz_png_flush);

    // Parse input data to drive fuzzing logic
    int choice = Data[0] % 10;
    int num_text = 0;
    int num_unknown = 0;
    png_uint_32 free_me = 0;
    int num_idx = 0;
    png_uint_32 flag = 0;
    int mask = 0;

    // Helper to safely read integers from buffer
    auto read_int = [&](size_t &offset) -> int {
        if (offset + sizeof(int) > Size) return 0;
        int val = 0;
        std::memcpy(&val, Data + offset, sizeof(int));
        offset += sizeof(int);
        return val;
    };

    size_t offset = 1;
    num_text = read_int(offset);
    num_unknown = read_int(offset);
    free_me = read_int(offset);
    num_idx = read_int(offset);
    flag = read_int(offset);
    mask = read_int(offset);

    // Ensure non-negative and reasonable bounds
    if (num_text < 0) num_text = 0;
    if (num_unknown < 0) num_unknown = 0;
    if (num_idx < 0) num_idx = 0;
    
    // Limit array sizes to prevent OOM or stack overflow
    if (num_text > 100) num_text = 100;
    if (num_unknown > 100) num_unknown = 100;

    // Allocate and initialize text chunks
    // We must allocate memory for key and text so libpng can free them safely later
    std::vector<png_text> text_chunks;
    if (num_text > 0 && offset + num_text * sizeof(png_text) <= Size) {
        text_chunks.resize(num_text);
        for (int i = 0; i < num_text; ++i) {
            text_chunks[i].compression = Data[offset++] % 4 - 1;
            
            // Allocate memory for key and text to simulate real usage
            char *key_str = (char*)malloc(64);
            char *text_str = (char*)malloc(64);
            
            if (key_str && text_str) {
                std::snprintf(key_str, 64, "FuzzKey%d", i);
                std::snprintf(text_str, 64, "FuzzText%d", i);
                text_chunks[i].key = key_str;
                text_chunks[i].text = text_str;
                text_chunks[i].text_length = std::strlen(text_str);
                
                // Handle iTXt fields if present (simplified)
                #if PNG_LIBPNG_VER >= 10500
                text_chunks[i].itxt_length = 0;
                text_chunks[i].lang = nullptr;
                text_chunks[i].lang_key = nullptr;
                #endif
            } else {
                // Cleanup if allocation failed
                free(key_str);
                free(text_str);
                text_chunks[i].key = nullptr;
                text_chunks[i].text = nullptr;
                text_chunks[i].text_length = 0;
            }
        }
    }

    // Allocate and initialize unknown chunks
    std::vector<png_unknown_chunk> unknown_chunks;
    if (num_unknown > 0 && offset + num_unknown * sizeof(png_unknown_chunk) <= Size) {
        unknown_chunks.resize(num_unknown);
        for (int i = 0; i < num_unknown; ++i) {
            std::memcpy(unknown_chunks[i].name, "FUZZ", 4);
            unknown_chunks[i].name[4] = '\0';
            // Allocate dummy data for unknown chunks
            size_t data_size = (Data[offset++] % 256) + 1;
            unknown_chunks[i].data = (png_byte*)malloc(data_size);
            if (unknown_chunks[i].data) {
                std::memset(unknown_chunks[i].data, 0, data_size);
            }
            unknown_chunks[i].size = data_size;
            unknown_chunks[i].location = Data[offset++] % 16;
        }
    }

    // Execute target API functions based on choice
    switch (choice) {
        case 0: {
            // png_free_data
            // First set some data to free
            if (!text_chunks.empty()) {
                png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            }
            // Ensure we don't free with invalid index if no data was set
            if (num_text > 0) {
                // FIX: Validate num_idx to ensure it is within bounds [0, num_text-1]
                // or -1 to free all.
                if (num_idx < 0 || num_idx >= num_text) {
                    num_idx = -1;
                }
                png_free_data(png_ptr, info_ptr, free_me, num_idx);
            } else {
                // Safe fallback: free everything if no specific index logic
                png_free_data(png_ptr, info_ptr, free_me, -1);
            }
            break;
        }
        case 1: {
            // png_get_valid
            // Set some data first to make it valid
            if (!text_chunks.empty()) {
                png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            }
            if (!unknown_chunks.empty()) {
                png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks.data(), num_unknown);
            }
            png_uint_32 result = png_get_valid(png_ptr, info_ptr, flag);
            (void)result;
            break;
        }
        case 2: {
            // png_set_unknown_chunks
            png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks.data(), num_unknown);
            break;
        }
        case 3: {
            // png_set_invalid
            // Set some data first
            if (!text_chunks.empty()) {
                png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            }
            png_set_invalid(png_ptr, info_ptr, mask);
            break;
        }
        case 4: {
            // png_set_text
            png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            break;
        }
        case 5: {
            // png_get_text
            // Set some data first
            if (!text_chunks.empty()) {
                png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            }
            png_textp text_ptr = nullptr;
            int num_text_out = 0;
            int result = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text_out);
            (void)result;
            (void)text_ptr;
            (void)num_text_out;
            break;
        }
        case 6: {
            // Combined: set text, get valid, then free
            if (!text_chunks.empty()) {
                png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
            }
            // Use PNG_INFO_tEXt if defined, otherwise fallback to 0
            #ifdef PNG_INFO_tEXt
            png_uint_32 valid = png_get_valid(png_ptr, info_ptr, PNG_INFO_tEXt);
            #else
            png_uint_32 valid = png_get_valid(png_ptr, info_ptr, 0);
            #endif
            (void)valid;
            // Only free if we actually set text
            if (!text_chunks.empty()) {
                png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, -1);
            }
            break;
        }
        case 7: {
            // Combined: set unknown, get valid, then set invalid
            if (!unknown_chunks.empty()) {
                png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks.data(), num_unknown);
            }
            png_uint_32 valid = png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP);
            (void)valid;
            png_set_invalid(png_ptr, info_ptr, PNG_INFO_iCCP);
            break;
        }
        case 8: {
            // Stress test: multiple set_text calls
            for (int i = 0; i < 3; ++i) {
                if (!text_chunks.empty()) {
                    png_set_text(png_ptr, info_ptr, text_chunks.data(), num_text);
                }
            }
            break;
        }
        case 9: {
            // Stress test: multiple unknown chunk operations
            for (int i = 0; i < 3; ++i) {
                if (!unknown_chunks.empty()) {
                    png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks.data(), num_unknown);
                }
            }
            break;
        }
        default:
            break;
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}