#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_012 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom write function for memory buffer
struct png_memory_buffer {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t offset;
};

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->offset + length) {
            new_capacity = buffer->offset + length;
        }
        buffer->data = (uint8_t*)realloc(buffer->data, new_capacity);
        buffer->capacity = new_capacity;
    }
    
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
    buffer->size = buffer->offset;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Do nothing
}

// Custom read function for memory buffer
static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG text chunk operations
    // We need enough data for:
    // - Basic image parameters: width, height, bit_depth, color_type: ~20 bytes
    // - Text chunk parameters: compression settings, text count: ~10 bytes
    // - Text data: keywords and text strings: variable
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PART 1: WRITE TEXT CHUNKS TO PNG
    // ============================================
    
    // Step 1: Create PNG write structure
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!write_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp for write operations
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_info_struct(write_ptr, &write_info_ptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Step 3: Set basic image parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.PickValueInArray({8, 16});
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_RGB, 
                                           PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_method = PNG_INTERLACE_NONE;
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Step 4: Set up output buffer for writing
    png_memory_buffer write_buffer;
    write_buffer.data = (uint8_t*)malloc(4096);
    write_buffer.capacity = 4096;
    write_buffer.size = 0;
    write_buffer.offset = 0;
    
    png_set_write_fn(write_ptr, &write_buffer, pngtest_write_data, pngtest_flush_data);
    
    // Step 5: Configure text compression settings
    // Use fuzzer input to vary compression parameters
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int compression_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int compression_method_zlib = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    int compression_mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    
    // PRIMARY TARGET: png_set_text_compression_* functions
    png_set_text_compression_level(write_ptr, compression_level);
    png_set_text_compression_window_bits(write_ptr, compression_window_bits);
    png_set_text_compression_method(write_ptr, compression_method_zlib);
    png_set_text_compression_strategy(write_ptr, compression_strategy);
    png_set_text_compression_mem_level(write_ptr, compression_mem_level);
    
    // Step 6: Create and set text chunks
    // Determine number of text chunks (1-5)
    int num_text_chunks = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<png_text> text_chunks(num_text_chunks);
    
    // Store string data to keep it alive until png_set_text copies it
    std::vector<std::string> keyword_storage(num_text_chunks);
    std::vector<std::string> text_storage(num_text_chunks);
    std::vector<std::string> lang_storage(num_text_chunks);
    std::vector<std::string> lang_key_storage(num_text_chunks);
    
    for (int i = 0; i < num_text_chunks; i++) {
        // Consume text compression type
        int compression_type = fdp.PickValueInArray({
            PNG_TEXT_COMPRESSION_NONE,    // tEXt - uncompressed
            PNG_TEXT_COMPRESSION_zTXt,    // zTXt - compressed
            PNG_ITXT_COMPRESSION_NONE,    // iTXt - uncompressed international
            PNG_ITXT_COMPRESSION_zTXt     // iTXt - compressed international
        });
        
        // Consume keyword (1-79 characters as per PNG spec)
        keyword_storage[i] = fdp.ConsumeRandomLengthString(78);
        if (keyword_storage[i].empty()) keyword_storage[i] = "Keyword";
        
        // Consume text content
        text_storage[i] = fdp.ConsumeRandomLengthString(500);
        
        // Consume language and translated keyword for iTXt chunks
        if (compression_type == PNG_ITXT_COMPRESSION_NONE || 
            compression_type == PNG_ITXT_COMPRESSION_zTXt) {
            lang_storage[i] = fdp.ConsumeRandomLengthString(78);
            lang_key_storage[i] = fdp.ConsumeRandomLengthString(200);
        }
        
        // Fill png_text structure
        text_chunks[i].compression = compression_type;
        text_chunks[i].key = const_cast<char*>(keyword_storage[i].c_str());
        text_chunks[i].text = const_cast<char*>(text_storage[i].c_str());
        text_chunks[i].text_length = text_storage[i].length();
        
        if (compression_type == PNG_ITXT_COMPRESSION_NONE || 
            compression_type == PNG_ITXT_COMPRESSION_zTXt) {
            text_chunks[i].itxt_length = text_storage[i].length();
            text_chunks[i].lang = const_cast<char*>(lang_storage[i].c_str());
            text_chunks[i].lang_key = const_cast<char*>(lang_key_storage[i].c_str());
        } else {
            text_chunks[i].itxt_length = 0;
            text_chunks[i].lang = NULL;
            text_chunks[i].lang_key = NULL;
        }
    }
    
    // PRIMARY TARGET: png_set_text
    png_set_text(write_ptr, write_info_ptr, text_chunks.data(), num_text_chunks);
    // Step 7: Write the PNG image with embedded text
    png_write_info(write_ptr, write_info_ptr);
    
    // Create dummy image data
    png_bytep row_pointers[height];
    size_t row_bytes = png_get_rowbytes(write_ptr, write_info_ptr);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(row_bytes);
        memset(row_pointers[y], 0, row_bytes);
    }
    
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, nullptr);
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    
    // ============================================
    // PART 2: READ TEXT CHUNKS FROM PNG
    // ============================================
    
    // Step 1: Create PNG read structure
    png_structp read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!read_ptr) {
        png_destroy_info_struct(write_ptr, &write_info_ptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Step 2: Create info structure
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        png_destroy_info_struct(write_ptr, &write_info_ptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Set error handling with setjmp for read operations
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        png_destroy_info_struct(write_ptr, &write_info_ptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Step 3: Set up read buffer with written PNG data
    png_memory_buffer read_buffer;
    read_buffer.data = write_buffer.data;
    read_buffer.size = write_buffer.size;
    read_buffer.capacity = write_buffer.capacity;
    read_buffer.offset = 0;
    
    png_set_read_fn(read_ptr, &read_buffer, pngtest_read_data);
    
    // Step 4: Read image info (this should parse our text chunks)
    png_read_info(read_ptr, read_info_ptr);
    
    // Step 5: Retrieve embedded text chunks
    png_textp retrieved_text_ptr = nullptr;
    int num_retrieved_text = 0;
    
    // PRIMARY TARGET: png_get_text
    int result = png_get_text(read_ptr, read_info_ptr, &retrieved_text_ptr, &num_retrieved_text);
    
    // Note: retrieved_text_ptr points to internal libpng memory, don't free it
    
    // ============================================
    // CLEANUP
    // ============================================
    
    png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
    png_destroy_info_struct(write_ptr, &write_info_ptr);
    png_destroy_write_struct(&write_ptr, nullptr);
    free(write_buffer.data);
    
    return 0;
}

} // namespace harness_012
