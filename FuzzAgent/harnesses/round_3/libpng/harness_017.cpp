#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_017 {

// Custom write callback structure for png_set_write_fn
struct png_write_memory_data {
    uint8_t* buffer;
    size_t size;
    size_t offset;
    size_t capacity;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (write_data->offset + length > write_data->capacity) {
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->offset + length) {
            new_capacity = write_data->offset + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (!new_buffer) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->offset, data, length);
    write_data->offset += length;
    write_data->size = write_data->offset;
}

// Flush callback (required but does nothing for memory writing)
static void png_write_memory_flush(png_structp png_ptr) {
    // Nothing to flush for memory buffer
    (void)png_ptr;
}

// Custom read callback structure for png_set_read_fn
struct png_read_memory_data {
    const uint8_t* buffer;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + length > read_data->size) {
        png_error(png_ptr, "Insufficient data in read callback");
        return;
    }
    
    memcpy(data, read_data->buffer + read_data->offset, length);
    read_data->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for unknown chunk testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ====================================================
    // STEP 1: CREATE PNG WRITE STRUCTURE
    // ====================================================
    png_struct *write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (write_png_ptr == nullptr) {
        return 0;
    }
    
    // ====================================================
    // STEP 2: CREATE PNG INFO STRUCTURE
    // ====================================================
    png_info *write_info_ptr = png_create_info_struct(write_png_ptr);
    if (write_info_ptr == nullptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // ====================================================
    // STEP 3: SETUP MEMORY-BASED WRITING
    // ====================================================
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.size = 0;
    write_data.offset = 0;
    write_data.capacity = 8192;
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_write_memory_flush);
    
    // ====================================================
    // STEP 4: CREATE PNG READ STRUCTURE FOR COMPARISON
    // ====================================================
    png_struct *read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (read_png_ptr == nullptr) {
        free(write_data.buffer);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_info *read_info_ptr = png_create_info_struct(read_png_ptr);
    if (read_info_ptr == nullptr) {
        free(write_data.buffer);
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // Set up error handling for read
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        free(write_data.buffer);
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // ====================================================
    // STEP 5: CONSUME FUZZER INPUT FOR TEST PARAMETERS
    // ====================================================
    
    // Consume test scenario
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume parameters for png_set_keep_unknown_chunks
    int keep_mode = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 matches PNG_HANDLE_CHUNK constants
    
    // Consume number of unknown chunks to create (1-5)
    int num_chunks = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    // Allocate array for unknown chunks
    png_unknown_chunk* unknown_chunks = (png_unknown_chunk*)malloc(num_chunks * sizeof(png_unknown_chunk));
    if (unknown_chunks == nullptr) {
        free(write_data.buffer);
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // Initialize unknown chunks with fuzzer data
    for (int i = 0; i < num_chunks; i++) {
        // Ensure we have enough data
        if (fdp.remaining_bytes() < 10) {
            free(unknown_chunks);
            free(write_data.buffer);
            png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
            return 0;
        }
        
        // Generate chunk name (4 bytes + null terminator)
        std::vector<uint8_t> chunk_name_bytes = fdp.ConsumeBytes<uint8_t>(4);
        memcpy(unknown_chunks[i].name, chunk_name_bytes.data(), 4);
        unknown_chunks[i].name[4] = '\0'; // Null terminate
        
        // Generate chunk data
        size_t chunk_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() < chunk_data_size) {
            chunk_data_size = fdp.remaining_bytes();
        }
        
        std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_data_size);
        unknown_chunks[i].data = (png_byte*)malloc(chunk_data_size);
        if (unknown_chunks[i].data == nullptr) {
            // Clean up previously allocated data
            for (int j = 0; j < i; j++) {
                free(unknown_chunks[j].data);
            }
            free(unknown_chunks);
            free(write_data.buffer);
            png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
            return 0;
        }
        
        memcpy(unknown_chunks[i].data, chunk_data.data(), chunk_data_size);
        unknown_chunks[i].size = chunk_data_size;
        
        // Set location based on fuzzer input
        unknown_chunks[i].location = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
    }
    
    // ====================================================
    // STEP 6: TEST PNG SET UNKNOWN CHUNKS FUNCTIONS
    // ====================================================
    
    // Test different scenarios based on fuzzer input
    switch (test_scenario) {
        case 0:
            // Basic test: set unknown chunks then set location
            png_set_unknown_chunks(write_png_ptr, write_info_ptr, unknown_chunks, num_chunks);
            
            // Set location for each chunk
            for (int i = 0; i < num_chunks; i++) {
                int location = fdp.ConsumeIntegralInRange<int>(0, 3);
                png_set_unknown_chunk_location(write_png_ptr, write_info_ptr, i, location);
            }
            break;
            
        case 1:
            // Test with png_set_keep_unknown_chunks
            png_set_keep_unknown_chunks(write_png_ptr, keep_mode, nullptr, 0);
            png_set_unknown_chunks(write_png_ptr, write_info_ptr, unknown_chunks, num_chunks);
            break;
            
        case 2:
            // Test with specific chunk list for png_set_keep_unknown_chunks
            if (num_chunks > 0) {
                png_byte chunk_list[5]; // Maximum 4 chunk names + null
                memcpy(chunk_list, unknown_chunks[0].name, 5);
                png_set_keep_unknown_chunks(write_png_ptr, keep_mode, chunk_list, 1);
            }
            png_set_unknown_chunks(write_png_ptr, write_info_ptr, unknown_chunks, num_chunks);
            break;
            
        case 3:
            // Test negative num_chunks for png_set_keep_unknown_chunks
            png_set_keep_unknown_chunks(write_png_ptr, keep_mode, nullptr, -1);
            png_set_unknown_chunks(write_png_ptr, write_info_ptr, unknown_chunks, num_chunks);
            
            // Also test setting locations
            for (int i = 0; i < num_chunks && i < 2; i++) {
                int location = fdp.ConsumeIntegralInRange<int>(0, 3);
                png_set_unknown_chunk_location(write_png_ptr, write_info_ptr, i, location);
            }
            break;
    }
    
    // ====================================================
    // STEP 7: TEST PNG GET UNKNOWN CHUNKS
    // ====================================================
    png_unknown_chunk* retrieved_chunks = nullptr;
    int num_retrieved = png_get_unknown_chunks(write_png_ptr, write_info_ptr, &retrieved_chunks);
    
    // Verify we got something back (may be 0 if no chunks were set)
    (void)num_retrieved;
    (void)retrieved_chunks;
    
    // ====================================================
    // STEP 8: SETUP BASIC PNG HEADER FOR WRITING
    // ====================================================
    // Set basic PNG parameters so we can attempt to write
    png_set_IHDR(write_png_ptr, write_info_ptr,
                32, 32, // Width, height
                8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE,
                PNG_FILTER_TYPE_BASE);
    
    // Write PNG info
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Create dummy image data
    std::vector<png_byte> row(32 * 3); // 32 pixels * 3 bytes per pixel (RGB)
    for (size_t i = 0; i < row.size(); i++) {
        row[i] = (png_byte)(i % 256);
    }
    
    // Write dummy rows
    for (int y = 0; y < 32; y++) {
        png_write_row(write_png_ptr, row.data());
    }
    
    // End write
    png_write_end(write_png_ptr, write_info_ptr);
    
    // ====================================================
    // STEP 9: ATTEMPT TO READ BACK THE WRITTEN DATA
    // ====================================================
    // Only attempt if we actually wrote data
    if (write_data.size > 0) {
        png_read_memory_data read_data;
        read_data.buffer = write_data.buffer;
        read_data.size = write_data.size;
        read_data.offset = 0;
        
        png_set_read_fn(read_png_ptr, &read_data, png_read_memory_callback);
        
        // Check for PNG signature
        if (write_data.size >= 8 && png_sig_cmp(write_data.buffer, 0, 8) == 0) {
            png_set_sig_bytes(read_png_ptr, 8);
            read_data.offset = 8;
            
            // Try to read the PNG info (may fail due to our synthetic data)
            png_read_info(read_png_ptr, read_info_ptr);
        }
    }
    
    // ====================================================
    // STEP 10: CLEANUP
    // ====================================================
    // Free unknown chunk data
    for (int i = 0; i < num_chunks; i++) {
        free(unknown_chunks[i].data);
    }
    free(unknown_chunks);
    
    // Free write buffer
    free(write_data.buffer);
    
    // Destroy PNG structures
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    return 0;
}

} // namespace harness_017
