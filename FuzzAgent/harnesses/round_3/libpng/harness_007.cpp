#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_007 {

// Structure to hold memory writing callback data
struct png_write_memory_data {
    uint8_t* buffer;
    size_t capacity;
    size_t size;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    if (write_data->size + length > write_data->capacity) {
        // Reallocate buffer if needed
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->size + length) {
            new_capacity = write_data->size + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (new_buffer == NULL) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->size, data, length);
    write_data->size += length;
}

// Flush callback function (optional)
static void png_flush_memory_callback(png_structp png_ptr) {
    // Nothing to do for memory-based output
}

// Structure to hold memory reading callback data
struct png_read_memory_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byteCountToRead > read_data->size) {
        png_error(png_ptr, "Insufficient data in read callback");
        return;
    }
    
    memcpy(outBytes, read_data->data + read_data->offset, byteCountToRead);
    read_data->offset += byteCountToRead;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for meaningful text chunk testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // PHASE 1: WRITE TEXT CHUNKS TO PNG IMAGE
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Validate color_type and bit_depth combinations
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && 
                bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;  // Default to 8 bits
            }
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_PALETTE:
            if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        default:
            color_type = PNG_COLOR_TYPE_RGB;
            bit_depth = 8;
    }
    
    // 1. Initialize write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (write_ptr == nullptr) {
        return 0;
    }
    
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (write_info_ptr == nullptr) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    // 2. Set up memory writing callback
    png_write_memory_data write_data;
    write_data.buffer = (uint8_t*)malloc(4096);
    write_data.capacity = 4096;
    write_data.size = 0;
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    png_set_write_fn(write_ptr, &write_data, png_write_memory_callback, png_flush_memory_callback);
    
    // 3. Set basic image parameters
    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // 4. Add multiple text chunks with png_set_text
    // Determine number of text chunks to create (1-5)
    int num_text_chunks = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<png_text> text_chunks(num_text_chunks);
    std::vector<std::string> keywords(num_text_chunks);
    std::vector<std::string> texts(num_text_chunks);
    std::vector<std::string> langs(num_text_chunks);
    std::vector<std::string> lang_keys(num_text_chunks);
    
    for (int i = 0; i < num_text_chunks; i++) {
        // Consume text chunk type from fuzzer input
        int compression_type = fdp.ConsumeIntegralInRange<int>(-1, 2);
        
        // Consume keyword (1-79 characters as per PNG spec)
        size_t keyword_len = fdp.ConsumeIntegralInRange<size_t>(1, 79);
        keywords[i] = fdp.ConsumeBytesAsString(keyword_len);
        if (keywords[i].empty()) keywords[i] = "Keyword";
        
        // Consume text content
        size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        texts[i] = fdp.ConsumeBytesAsString(text_len);
        
        // For iTXt chunks, also consume language and translated keyword
        if (compression_type == PNG_ITXT_COMPRESSION_NONE || 
            compression_type == PNG_ITXT_COMPRESSION_zTXt) {
            size_t lang_len = fdp.ConsumeIntegralInRange<size_t>(0, 79);
            langs[i] = fdp.ConsumeBytesAsString(lang_len);
            
            size_t lang_key_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
            lang_keys[i] = fdp.ConsumeBytesAsString(lang_key_len);
        }
        
        // Setup png_text structure
        text_chunks[i].compression = compression_type;
        text_chunks[i].key = const_cast<char*>(keywords[i].c_str());
        text_chunks[i].text = const_cast<char*>(texts[i].c_str());
        text_chunks[i].text_length = texts[i].size();
        
        if (compression_type == PNG_ITXT_COMPRESSION_NONE || 
            compression_type == PNG_ITXT_COMPRESSION_zTXt) {
            text_chunks[i].itxt_length = texts[i].size();
            text_chunks[i].lang = const_cast<char*>(langs[i].c_str());
            text_chunks[i].lang_key = const_cast<char*>(lang_keys[i].c_str());
        } else {
            text_chunks[i].itxt_length = 0;
            text_chunks[i].lang = nullptr;
            text_chunks[i].lang_key = nullptr;
        }
    }
    
    // Set the text chunks
    png_set_text(write_ptr, write_info_ptr, text_chunks.data(), num_text_chunks);
    
    // 5. Write image header with text chunks
    png_write_info(write_ptr, write_info_ptr);
    
    // Write a simple image (just write dummy row data)
    png_bytep row = (png_bytep)malloc(png_get_rowbytes(write_ptr, write_info_ptr));
    if (row) {
        for (png_uint_32 y = 0; y < height; y++) {
            // Fill row with pattern
            for (size_t x = 0; x < png_get_rowbytes(write_ptr, write_info_ptr); x++) {
                row[x] = (png_byte)((x + y) % 256);
            }
            png_write_row(write_ptr, row);
        }
        free(row);
    }
    
    png_write_end(write_ptr, write_info_ptr);
    
    // 6. Cleanup write structures
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    
    // PHASE 2: READ TEXT CHUNKS FROM PNG IMAGE
    
    // 7. Initialize read structures
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                   nullptr, nullptr, nullptr);
    if (read_ptr == nullptr) {
        free(write_data.buffer);
        return 0;
    }
    
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    if (read_info_ptr == nullptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // Set up error handling for read
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // 8. Set up memory reading callback
    png_read_memory_data read_data;
    read_data.data = write_data.buffer;
    read_data.size = write_data.size;
    read_data.offset = 0;
    
    png_set_read_fn(read_ptr, &read_data, png_read_memory_callback);
    
    // 9. Read image header and text chunks
    png_read_info(read_ptr, read_info_ptr);
    
    // 10. Retrieve and verify text chunks with png_get_text
    png_textp retrieved_texts = nullptr;
    int num_retrieved_texts = 0;
    
    int get_text_result = png_get_text(read_ptr, read_info_ptr, &retrieved_texts, &num_retrieved_texts);
    
    // Verify we got some text back (might be 0 if no text chunks were written)
    if (get_text_result != 0 && retrieved_texts != nullptr && num_retrieved_texts > 0) {
        // Access the retrieved text chunks
        for (int i = 0; i < num_retrieved_texts; i++) {
            // Verify key is not null (should always be a valid string per PNG spec)
            if (retrieved_texts[i].key != nullptr) {
                // Access key to ensure it's valid
                volatile char first_char = retrieved_texts[i].key[0];
                (void)first_char;  // Avoid unused variable warning
            }
            
            // Verify text is not null (should always be a valid string, possibly empty)
            if (retrieved_texts[i].text != nullptr) {
                // Access text to ensure it's valid
                volatile char first_text_char = retrieved_texts[i].text[0];
                (void)first_text_char;  // Avoid unused variable warning
            }
        }
    }
    
    // 11. Cleanup read structures
    png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
    
    // Cleanup memory buffer
    free(write_data.buffer);
    
    return 0;
}

} // namespace harness_007
