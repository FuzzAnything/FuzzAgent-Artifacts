#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

namespace harness_022 {
/*
 * Harness for testing completely uncovered time-related PNG functions with high complexity
 * identified by coverage analysis. Specifically targets:
 * 
 * 1. png_convert_to_rfc1123_buffer function with 28 undiscovered branches
 * 2. PNG time chunk handling (tIME chunk) - both setting and retrieval
 * 3. RFC1123 time formatting functions and time conversion utilities
 * 4. Complete tIME chunk lifecycle: creation, writing, reading, and conversion
 * 
 * Target APIs:
 * - png_convert_to_rfc1123_buffer: High complexity with 28 undiscovered branches
 * - png_set_tIME: Set modification time in PNG info structure
 * - png_get_tIME: Retrieve modification time from PNG info structure  
 * - png_convert_from_struct_tm: Convert from struct tm to png_time (if supported)
 * - png_convert_from_time_t: Convert from time_t to png_time (if supported)
 * 
 * Required helper functions for complete lifecycle:
 * - png_create_write_struct, png_create_info_struct
 * - png_create_read_struct
 * - png_set_IHDR for basic image setup
 * - png_write_info, png_write_image, png_write_end
 * - png_read_info
 * - png_destroy_write_struct, png_destroy_read_struct
 * 
 * Invocation sequence for complete time chunk lifecycle:
 * 1. Create write structures and set up memory I/O
 * 2. Create png_time structure from fuzzer input
 * 3. Test png_convert_to_rfc1123_buffer with the time data
 * 4. Set tIME chunk with png_set_tIME
 * 5. Write PNG image with time metadata
 * 6. Read back PNG and retrieve tIME chunk with png_get_tIME
 * 7. Test RFC1123 conversion on retrieved time
 * 8. Test time conversion functions if available
 * 
 * This harness provides semantic diversity from existing harnesses:
 * - harness_004.cpp: Only tests png_get_tIME retrieval (no setting or conversion)
 * - harness_010.cpp: Only tests png_get_tIME retrieval (no setting or conversion)
 * - This harness: Complete tIME chunk lifecycle with RFC1123 conversion,
 *   time setting, writing, reading, and format conversion testing
 * 
 * The time-related functions have significant coverage gaps:
 * - png_convert_to_rfc1123_buffer: 28 undiscovered branches in RFC1123 formatting
 * - tIME chunk handling: Complete write/read lifecycle untested
 * - Time conversion utilities: struct tm and time_t conversion paths uncovered
 */


// ==================== MEMORY BUFFER FOR WRITING ====================

struct write_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    write_buffer* buffer = (write_buffer*)io_ptr;
    
    // Append data to buffer
    size_t new_size = buffer->data.size() + length;
    buffer->data.resize(new_size);
    memcpy(buffer->data.data() + buffer->offset, data, length);
    buffer->offset += length;
}

// Custom flush function (required but does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

// ==================== MEMORY BUFFER FOR READING ====================

struct read_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    read_buffer* buffer = (read_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful time function testing
    // We need enough data for: time structure fields, image setup, and test variations
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== CREATE PNG_TIME STRUCTURE FROM FUZZER INPUT ====================
    
    png_time png_time_data;
    
    // Consume time fields from fuzzer input with reasonable ranges
    // Year: 1970-2100 (valid PNG years, PNG has always been Year 2000 compliant!)
    png_time_data.year = fdp.ConsumeIntegralInRange<png_uint_16>(1970, 2100);
    
    // Month: 1-12
    png_time_data.month = fdp.ConsumeIntegralInRange<png_byte>(1, 12);
    
    // Day: 1-31 (note: some combinations may be invalid like Feb 30, but that's fine for fuzzing)
    png_time_data.day = fdp.ConsumeIntegralInRange<png_byte>(1, 31);
    
    // Hour: 0-23
    png_time_data.hour = fdp.ConsumeIntegralInRange<png_byte>(0, 23);
    
    // Minute: 0-59
    png_time_data.minute = fdp.ConsumeIntegralInRange<png_byte>(0, 59);
    
    // Second: 0-60 (60 for leap seconds)
    png_time_data.second = fdp.ConsumeIntegralInRange<png_byte>(0, 60);
    
    // ==================== TEST PNG_CONVERT_TO_RFC1123_BUFFER ====================
    // This is the primary target with 28 undiscovered branches
    
#ifdef PNG_TIME_RFC1123_SUPPORTED
    char rfc1123_buffer[29]; // 29-character buffer as per libpng docs
    int convert_result = png_convert_to_rfc1123_buffer(rfc1123_buffer, &png_time_data);
    
    // The result should be 0 on success, non-zero on failure
    // Even failure paths are valuable for coverage
    (void)convert_result;
    (void)rfc1123_buffer;
#endif
    
    // ==================== TEST TIME CONVERSION FUNCTIONS IF AVAILABLE ====================
    
#ifdef PNG_CONVERT_tIME_SUPPORTED
    // Test png_convert_from_time_t if we have enough input
    if (fdp.remaining_bytes() >= sizeof(time_t)) {
        time_t time_t_val = fdp.ConsumeIntegral<time_t>();
        png_time converted_from_time_t;
        png_convert_from_time_t(&converted_from_time_t, time_t_val);
        
        // Test RFC1123 conversion on the converted time
#ifdef PNG_TIME_RFC1123_SUPPORTED
        char rfc1123_from_time_t[29];
        png_convert_to_rfc1123_buffer(rfc1123_from_time_t, &converted_from_time_t);
        (void)rfc1123_from_time_t;
#endif
    }
    
    // Test png_convert_from_struct_tm if we have enough input
    if (fdp.remaining_bytes() >= sizeof(struct tm)) {
        // Create a minimal struct tm from fuzzer data
        struct tm tm_data;
        memset(&tm_data, 0, sizeof(struct tm));
        
        // Set fields from fuzzer input with reasonable ranges
        tm_data.tm_year = fdp.ConsumeIntegralInRange<int>(70, 200); // 1970-2100
        tm_data.tm_mon = fdp.ConsumeIntegralInRange<int>(0, 11);    // 0-11
        tm_data.tm_mday = fdp.ConsumeIntegralInRange<int>(1, 31);   // 1-31
        tm_data.tm_hour = fdp.ConsumeIntegralInRange<int>(0, 23);   // 0-23
        tm_data.tm_min = fdp.ConsumeIntegralInRange<int>(0, 59);    // 0-59
        tm_data.tm_sec = fdp.ConsumeIntegralInRange<int>(0, 60);    // 0-60 (leap seconds)
        tm_data.tm_isdst = fdp.ConsumeIntegral<int>() % 3 - 1;      // -1, 0, or 1
        
        png_time converted_from_tm;
        png_convert_from_struct_tm(&converted_from_tm, &tm_data);
        
        // Test RFC1123 conversion on the converted time
#ifdef PNG_TIME_RFC1123_SUPPORTED
        char rfc1123_from_tm[29];
        png_convert_to_rfc1123_buffer(rfc1123_from_tm, &converted_from_tm);
        (void)rfc1123_from_tm;
#endif
    }
#endif // PNG_CONVERT_tIME_SUPPORTED
    
    // ==================== CREATE AND WRITE PNG WITH TIME METADATA ====================
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (write_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (write_info_ptr == NULL) {
        png_destroy_write_struct(&write_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(write_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    // Setup memory write buffer
    write_buffer wbuffer;
    wbuffer.data.reserve(4096); // Reserve some space
    wbuffer.offset = 0;
    png_set_write_fn(write_ptr, &wbuffer, png_write_to_memory, png_flush_memory);
    
    // Set basic image parameters (minimal valid PNG)
    const png_uint_32 width = 1;
    const png_uint_32 height = 1;
    const int bit_depth = 8;
    const int color_type = PNG_COLOR_TYPE_GRAY;
    
    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // ==================== SET TIME CHUNK WITH PNG_SET_TIME ====================
    
#ifdef PNG_tIME_SUPPORTED
    // Set the time chunk using our fuzzer-generated time data
    png_set_tIME(write_ptr, write_info_ptr, &png_time_data);
#endif
    
    // Create simple image data (single gray pixel)
    std::vector<png_byte> image_data(width * height);
    if (!image_data.empty()) {
        image_data[0] = 128; // Mid-gray
        
        png_bytep row_pointers[1];
        row_pointers[0] = image_data.data();
        
        // Write PNG info and image
        png_write_info(write_ptr, write_info_ptr);
        png_write_image(write_ptr, row_pointers);
        png_write_end(write_ptr, NULL);
    }
    
    // Clean up write structures
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    
    // ==================== READ BACK PNG AND RETRIEVE TIME METADATA ====================
    
    // Only proceed if we successfully wrote some PNG data
    if (wbuffer.data.empty()) {
        return 0;
    }
    
    // Create read structure
    png_structp read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (read_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    if (read_info_ptr == NULL) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(read_ptr))) {
        // Error occurred during PNG reading
        png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
        return 0;
    }
    
    // Setup memory read buffer with the PNG data we just wrote
    read_buffer rbuffer;
    rbuffer.data = wbuffer.data.data();
    rbuffer.size = wbuffer.data.size();
    rbuffer.offset = 0;
    png_set_read_fn(read_ptr, &rbuffer, png_read_from_memory);
    
    // Read PNG info
    png_read_info(read_ptr, read_info_ptr);
    
    // ==================== RETRIEVE TIME CHUNK WITH PNG_GET_TIME ====================
    
#ifdef PNG_tIME_SUPPORTED
    png_timep retrieved_time = NULL;
    if (png_get_tIME(read_ptr, read_info_ptr, &retrieved_time)) {
        // Successfully retrieved time chunk
        
        // Test RFC1123 conversion on the retrieved time
#ifdef PNG_TIME_RFC1123_SUPPORTED
        char retrieved_rfc1123[29];
        png_convert_to_rfc1123_buffer(retrieved_rfc1123, retrieved_time);
        (void)retrieved_rfc1123;
#endif
        
        // Compare with original (optional, but good for testing)
        (void)retrieved_time;
    }
#endif
    
    // Clean up read structures
    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    
    return 0;
}

} // namespace harness_022
