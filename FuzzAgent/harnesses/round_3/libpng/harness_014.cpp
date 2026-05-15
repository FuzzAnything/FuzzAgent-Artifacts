#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_014 {
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for time testing: need at least enough for time values
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters from fuzzer input to control test flow
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    bool use_time_t = fdp.ConsumeBool();
    bool use_struct_tm = fdp.ConsumeBool();
    
    // STEP 1: Initialization with png_create_write_struct
    png_struct *png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                   nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }
    
    // STEP 2: I/O setup with png_set_write_fn
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.size = 0;
    write_data.offset = 0;
    write_data.capacity = 4096; // Initial capacity
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    png_set_write_fn(png_ptr, &write_data, png_write_memory_callback, png_write_memory_flush);
    
    // STEP 3: Info structure creation with png_create_info_struct
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        free(write_data.buffer);
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        free(write_data.buffer);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // STEP 4: System time to PNG time conversion
    png_time png_time_value;
    memset(&png_time_value, 0, sizeof(png_time_value));
    
    if (use_time_t) {
        // Convert from time_t to png_time
        time_t time_val = fdp.ConsumeIntegral<time_t>();
        png_convert_from_time_t(&png_time_value, time_val);
    }
    
    if (use_struct_tm) {
        // Convert from struct tm to png_time
        struct tm tm_val;
        memset(&tm_val, 0, sizeof(tm_val));
        
        // Consume time components from fuzzer input
        tm_val.tm_year = fdp.ConsumeIntegralInRange<int>(0, 200) - 1900; // Years since 1900
        tm_val.tm_mon = fdp.ConsumeIntegralInRange<int>(0, 11); // 0-11
        tm_val.tm_mday = fdp.ConsumeIntegralInRange<int>(1, 31); // 1-31
        tm_val.tm_hour = fdp.ConsumeIntegralInRange<int>(0, 23); // 0-23
        tm_val.tm_min = fdp.ConsumeIntegralInRange<int>(0, 59); // 0-59
        tm_val.tm_sec = fdp.ConsumeIntegralInRange<int>(0, 60); // 0-60 (including leap seconds)
        tm_val.tm_isdst = fdp.ConsumeIntegralInRange<int>(-1, 1); // -1=unknown, 0=no DST, 1=DST
        
        png_convert_from_struct_tm(&png_time_value, &tm_val);
    }
    
    // If neither time_t nor struct_tm conversion was used, generate time from fuzzer data
    if (!use_time_t && !use_struct_tm) {
        png_time_value.year = fdp.ConsumeIntegralInRange<png_uint_16>(1900, 2200);
        png_time_value.month = fdp.ConsumeIntegralInRange<png_byte>(1, 12);
        png_time_value.day = fdp.ConsumeIntegralInRange<png_byte>(1, 31);
        png_time_value.hour = fdp.ConsumeIntegralInRange<png_byte>(0, 23);
        png_time_value.minute = fdp.ConsumeIntegralInRange<png_byte>(0, 59);
        png_time_value.second = fdp.ConsumeIntegralInRange<png_byte>(0, 60);
    }
    
    // STEP 5: Set PNG time chunk with png_set_tIME
    png_set_tIME(png_ptr, info_ptr, &png_time_value);
    
    // STEP 6: Get PNG time chunk with png_get_tIME for verification
    png_time* retrieved_time = nullptr;
    png_get_tIME(png_ptr, info_ptr, &retrieved_time);
    
    // STEP 7: PNG time to RFC1123 format conversion with png_convert_to_rfc1123_buffer
    char rfc1123_buffer[29]; // Buffer for RFC1123 format (29 chars including null terminator)
    int rfc1123_result = png_convert_to_rfc1123_buffer(rfc1123_buffer, &png_time_value);
    
    // Test different scenarios based on fuzzer input
    switch (test_scenario) {
        case 0:
            // Basic test: just set and get time
            break;
        case 1:
            // Test with invalid time values (edge cases)
            png_time invalid_time;
            invalid_time.year = 0;
            invalid_time.month = 0;
            invalid_time.day = 0;
            invalid_time.hour = 25;
            invalid_time.minute = 70;
            invalid_time.second = 70;
            png_set_tIME(png_ptr, info_ptr, &invalid_time);
            break;
        case 2:
            // Test multiple time conversions
            png_time time2;
            time2.year = 2025;
            time2.month = 6;
            time2.day = 15;
            time2.hour = 12;
            time2.minute = 30;
            time2.second = 45;
            png_set_tIME(png_ptr, info_ptr, &time2);
            
            char buffer2[29];
            png_convert_to_rfc1123_buffer(buffer2, &time2);
            break;
        case 3:
            // Test with maximum values
            png_time max_time;
            max_time.year = 4095; // Max for png_uint_16
            max_time.month = 12;
            max_time.day = 31;
            max_time.hour = 23;
            max_time.minute = 59;
            max_time.second = 60; // Leap second
            png_set_tIME(png_ptr, info_ptr, &max_time);
            
            char buffer3[29];
            png_convert_to_rfc1123_buffer(buffer3, &max_time);
            break;
    }
    
    // STEP 8: Memory cleanup with png_free_data if needed
    // Note: png_free_data is typically used to free specific data, not needed for basic time chunk
    
    // STEP 9: Structure destruction
    free(write_data.buffer);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_014
