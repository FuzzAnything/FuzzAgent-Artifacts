// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_text at pngget.c:1158:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
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

// Custom error handler to prevent crashes during fuzzing
static void custom_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, we might log this, but for fuzzing we just return
    // to allow the fuzzer to continue.
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file to simulate reading if needed
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, custom_error_handler, custom_warning_handler);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp to catch errors from libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // 1. Fuzz png_set_text
    // We need to construct some png_text structs from the input data if possible.
    std::vector<png_text> text_list;
    if (Size >= 4) {
        // Try to create at least one text entry
        png_text text_entry;
        text_entry.compression = (Data[0] % 4) - 1; // -1, 0, 1, 2
        text_entry.text_length = (Data[1] % 100);
        text_entry.key = nullptr;
        text_entry.text = nullptr;
        
        // Allocate key and text if length allows
        if (Size > 4) {
            size_t key_len = (Data[2] % (Size - 4));
            if (key_len > 0) {
                text_entry.key = (char*)malloc(key_len + 1);
                if (text_entry.key) {
                    memcpy(text_entry.key, Data + 3, key_len);
                    text_entry.key[key_len] = '\0';
                }
            }
            
            size_t text_len = (Data[3] % (Size - 4 - key_len));
            if (text_len > 0 && text_entry.key) {
                text_entry.text = (char*)malloc(text_len + 1);
                if (text_entry.text) {
                    memcpy(text_entry.text, Data + 3 + key_len, text_len);
                    text_entry.text[text_len] = '\0';
                }
            }
        }
        text_list.push_back(text_entry);
    }

    if (!text_list.empty()) {
        png_set_text(png_ptr, info_ptr, text_list.data(), static_cast<int>(text_list.size()));
    }

    // 2. Fuzz png_set_invalid
    // Generate a random mask from input data
    png_uint_32 mask = 0;
    if (Size > 4) {
        mask = (png_uint_32)Data[4] | ((png_uint_32)Data[5] << 8) | ((png_uint_32)Data[6] << 16) | ((png_uint_32)Data[7] << 24);
    }
    png_set_invalid(png_ptr, info_ptr, mask);

    // 3. Fuzz png_get_text
    // The signature is: png_get_text(png_ptr, info_ptr, png_textp *text_ptr, int *num_text)
    // text_ptr should be a pointer to a png_textp (which is png_text*).
    // So we need a variable of type png_textp (png_text*), and pass its address.
    png_textp text_ptr = nullptr;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    // Note: png_get_text returns the count. We don't need to free text_ptr here
    // as it points to internal data managed by libpng.

    // 4. Fuzz png_get_valid
    png_uint_32 flag = 0;
    if (Size > 8) {
        flag = (png_uint_32)Data[8] | ((png_uint_32)Data[9] << 8) | ((png_uint_32)Data[10] << 16) | ((png_uint_32)Data[11] << 24);
    }
    png_uint_32 valid_result = png_get_valid(png_ptr, info_ptr, flag);
    (void)valid_result; // Use result to avoid unused warning

    // 5. Fuzz png_set_unknown_chunks
    // Construct a simple unknown chunk
    std::vector<png_unknown_chunk> unknown_list;
    if (Size > 12) {
        png_unknown_chunk chunk;
        memset(chunk.name, 0, 5);
        // Use first 4 bytes as name (ensure non-zero length)
        if (Size > 16) {
            memcpy(chunk.name, Data + 12, 4);
        } else {
            chunk.name[0] = 'X'; chunk.name[1] = 'P'; chunk.name[2] = 'R'; chunk.name[3] = 'I';
        }
        chunk.name[4] = '\0';
        
        chunk.data = nullptr;
        chunk.size = 0;
        chunk.location = Data[16] % 16; // Random location flag

        // Try to allocate some data if possible
        if (Size > 20) {
            size_t data_size = Data[17] % (Size - 20);
            if (data_size > 0) {
                chunk.data = (png_byte*)malloc(data_size);
                if (chunk.data) {
                    memcpy(chunk.data, Data + 18, data_size);
                    chunk.size = data_size;
                }
            }
        }
        unknown_list.push_back(chunk);
    }

    if (!unknown_list.empty()) {
        png_set_unknown_chunks(png_ptr, info_ptr, unknown_list.data(), static_cast<int>(unknown_list.size()));
    }

    // 6. Fuzz png_free_data
    // Free text data if we set it
    png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, -1);
    
    // Free unknown chunks if we set them
    png_free_data(png_ptr, info_ptr, PNG_FREE_UNKN, -1);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Clean up manually allocated memory for text entries
    for (auto &t : text_list) {
        if (t.key) free(t.key);
        if (t.text) free(t.text);
    }

    // Clean up manually allocated memory for unknown chunks
    for (auto &u : unknown_list) {
        if (u.data) free(u.data);
    }

    return 0;
}