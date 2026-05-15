#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <cmath>

namespace harness_020 {

// Structure to hold memory reading callback data
struct png_read_memory_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byte_count > read_data->size) {
        png_error(png_ptr, "Insufficient data in memory buffer");
        return;
    }
    
    memcpy(out_bytes, read_data->data + read_data->offset, byte_count);
    read_data->offset += byte_count;
}

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
static void png_flush_callback(png_structp png_ptr) {
    // Nothing to do for memory-based output
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive metadata testing
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ====================================================
    // PHASE 1: CREATE AND INITIALIZE PNG STRUCTURES FOR WRITING
    // ====================================================
    
    // Create PNG write structure
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                         nullptr, nullptr, nullptr);
    if (write_png_ptr == nullptr) {
        return 0;
    }
    
    // Create PNG info structure for writing
    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (write_info_ptr == nullptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for writing
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // Set up memory-based writing
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.capacity = 16384;  // Larger buffer for metadata testing
    write_data.size = 0;
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_flush_callback);
    
    // Set basic PNG header
    png_set_IHDR(write_png_ptr, write_info_ptr,
                 32, 32,  // Width, height
                 8,       // Bit depth
                 PNG_COLOR_TYPE_RGB,  // Color type
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    
    // ====================================================
    // PHASE 2: SET METADATA USING ALL TARGET SETTER FUNCTIONS
    // ====================================================
    
    // Consume test scenario from fuzzer input
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 6);
    
    // Consume parameters for different metadata types
    png_int_32 offset_x = fdp.ConsumeIntegral<png_int_32>();
    png_int_32 offset_y = fdp.ConsumeIntegral<png_int_32>();
    int unit_type = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=unknown, 1=meter, 2=radian
    
    // Consume cLLI parameters (content light level information)
    double max_content_light = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double max_frame_avg_light = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    png_uint_32 max_content_light_scaled = (png_uint_32)(max_content_light * 10000.0);
    png_uint_32 max_frame_avg_light_scaled = (png_uint_32)(max_frame_avg_light * 10000.0);
    
    // Consume gAMA parameters
    double gamma_value = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    png_fixed_point gamma_fixed = (png_fixed_point)(gamma_value * 100000.0);
    
    // Consume eXIf parameters
    png_uint_32 num_exif = fdp.ConsumeIntegralInRange<png_uint_32>(0, 1000);
    std::vector<png_byte> exif_data = fdp.ConsumeBytes<png_byte>(num_exif);
    
    // Consume PLTE parameters (palette)
    int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
    std::vector<png_color> palette(num_palette);
    for (int i = 0; i < num_palette; i++) {
        palette[i].red = fdp.ConsumeIntegral<uint8_t>();
        palette[i].green = fdp.ConsumeIntegral<uint8_t>();
        palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Consume hIST parameters (histogram)
    std::vector<png_uint_16> histogram(num_palette);
    for (int i = 0; i < num_palette; i++) {
        histogram[i] = fdp.ConsumeIntegral<png_uint_16>();
    }
    
    // Set different metadata based on test scenario
    switch (test_scenario) {
        case 0:
            // Test oFFs (image offset) metadata
            png_set_oFFs(write_png_ptr, write_info_ptr, offset_x, offset_y, unit_type);
            break;
            
        case 1:
            // Test cLLI metadata (floating point)
            png_set_cLLI(write_png_ptr, write_info_ptr, max_content_light, max_frame_avg_light);
            break;
            
        case 2:
            // Test cLLI_fixed metadata (fixed point)
            png_set_cLLI_fixed(write_png_ptr, write_info_ptr, 
                              max_content_light_scaled, max_frame_avg_light_scaled);
            break;
            
        case 3:
            // Test gAMA_fixed metadata
            png_set_gAMA_fixed(write_png_ptr, write_info_ptr, gamma_fixed);
            break;
            
        case 4:
            // Test eXIf_1 metadata
            if (exif_data.size() > 0) {
                png_set_eXIf_1(write_png_ptr, write_info_ptr, exif_data.size(), exif_data.data());
            }
            break;
            
        case 5:
            // Test PLTE and hIST metadata (requires palette color type)
            // Change to palette color type for PLTE testing
            png_set_IHDR(write_png_ptr, write_info_ptr,
                         32, 32,  // Width, height
                         8,       // Bit depth
                         PNG_COLOR_TYPE_PALETTE,  // Palette color type for PLTE
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE,
                         PNG_FILTER_TYPE_BASE);
            
            png_set_PLTE(write_png_ptr, write_info_ptr, palette.data(), num_palette);
            png_set_hIST(write_png_ptr, write_info_ptr, histogram.data());
            break;
            
        case 6:
            // Test combination of multiple metadata types
            png_set_oFFs(write_png_ptr, write_info_ptr, offset_x, offset_y, unit_type);
            png_set_gAMA_fixed(write_png_ptr, write_info_ptr, gamma_fixed);
            if (exif_data.size() > 0) {
                png_set_eXIf_1(write_png_ptr, write_info_ptr, exif_data.size(), exif_data.data());
            }
            break;
    }
    
    // Write PNG info and image data
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Create dummy row data for writing
    png_bytep row = (png_bytep)malloc(32 * 3);  // 32 pixels * 3 bytes per pixel (RGB)
    if (row != nullptr) {
        for (int y = 0; y < 32; y++) {
            // Fill with pattern based on fuzzer input
            size_t fill_size = fdp.ConsumeIntegralInRange<size_t>(0, 32 * 3);
            std::vector<uint8_t> pattern = fdp.ConsumeBytes<uint8_t>(fill_size);
            if (pattern.size() > 0) {
                memcpy(row, pattern.data(), std::min(pattern.size(), (size_t)(32 * 3)));
            }
            png_write_row(write_png_ptr, row);
        }
        free(row);
    }
    
    png_write_end(write_png_ptr, write_info_ptr);
    
    // Clean up write structures
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    // ====================================================
    // PHASE 3: CREATE READ STRUCTURE TO TEST GETTER FUNCTIONS
    // ====================================================
    
    if (write_data.size == 0) {
        free(write_data.buffer);
        return 0;  // Nothing was written
    }
    
    // Create PNG read structure
    png_structp read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (read_png_ptr == nullptr) {
        free(write_data.buffer);
        return 0;
    }
    
    // Create PNG info structure for reading
    png_infop read_info_ptr = png_create_info_struct(read_png_ptr);
    if (read_info_ptr == nullptr) {
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // Set up error handling for reading
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // Set up memory-based reading from written data
    png_read_memory_data read_data;
    read_data.data = write_data.buffer;
    read_data.size = write_data.size;
    read_data.offset = 0;
    
    png_set_read_fn(read_png_ptr, &read_data, png_read_memory_callback);
    
    // Read PNG info
    png_read_info(read_png_ptr, read_info_ptr);
    
    // ====================================================
    // PHASE 4: TEST ALL TARGET GETTER FUNCTIONS
    // ====================================================
    
    // Test png_get_oFFs
    if (fdp.ConsumeBool()) {
        png_int_32 retrieved_offset_x = 0;
        png_int_32 retrieved_offset_y = 0;
        int retrieved_unit_type = 0;
        png_uint_32 oFFs_result = png_get_oFFs(read_png_ptr, read_info_ptr,
                                              &retrieved_offset_x, &retrieved_offset_y, 
                                              &retrieved_unit_type);
        (void)oFFs_result;
        
        // Test with null pointers
        if (fdp.ConsumeBool()) {
            png_get_oFFs(read_png_ptr, read_info_ptr, nullptr, nullptr, nullptr);
        }
    }
    
    // Test png_get_cLLI (floating point)
    if (fdp.ConsumeBool()) {
        double retrieved_max_content = 0.0;
        double retrieved_max_frame_avg = 0.0;
        png_uint_32 cLLI_result = png_get_cLLI(read_png_ptr, read_info_ptr,
                                              &retrieved_max_content, &retrieved_max_frame_avg);
        (void)cLLI_result;
        
        // Test with null pointers
        if (fdp.ConsumeBool()) {
            png_get_cLLI(read_png_ptr, read_info_ptr, nullptr, nullptr);
        }
    }
    
    // Test png_get_cLLI_fixed
    if (fdp.ConsumeBool()) {
        png_uint_32 retrieved_max_content_scaled = 0;
        png_uint_32 retrieved_max_frame_avg_scaled = 0;
        png_uint_32 cLLI_fixed_result = png_get_cLLI_fixed(read_png_ptr, read_info_ptr,
                                                          &retrieved_max_content_scaled,
                                                          &retrieved_max_frame_avg_scaled);
        (void)cLLI_fixed_result;
        
        // Test with null pointers
        if (fdp.ConsumeBool()) {
            png_get_cLLI_fixed(read_png_ptr, read_info_ptr, nullptr, nullptr);
        }
    }
    
    // Test png_get_gAMA_fixed
    if (fdp.ConsumeBool()) {
        png_fixed_point retrieved_gamma_fixed = 0;
        png_uint_32 gAMA_fixed_result = png_get_gAMA_fixed(read_png_ptr, read_info_ptr,
                                                          &retrieved_gamma_fixed);
        (void)gAMA_fixed_result;
        
        // Test with null pointer
        if (fdp.ConsumeBool()) {
            png_get_gAMA_fixed(read_png_ptr, read_info_ptr, nullptr);
        }
    }
    
    // Test png_get_eXIf_1
    if (fdp.ConsumeBool()) {
        png_uint_32 retrieved_num_exif = 0;
        png_byte* retrieved_exif = nullptr;
        png_uint_32 eXIf_result = png_get_eXIf_1(read_png_ptr, read_info_ptr,
                                                &retrieved_num_exif, &retrieved_exif);
        (void)eXIf_result;
        
        // Free allocated exif data if any
        if (retrieved_exif != nullptr) {
            png_free(read_png_ptr, retrieved_exif);
        }
        
        // Test with null pointers
        if (fdp.ConsumeBool()) {
            png_get_eXIf_1(read_png_ptr, read_info_ptr, nullptr, nullptr);
        }
    }
    
    // Test png_get_PLTE
    if (fdp.ConsumeBool()) {
        png_color* retrieved_palette = nullptr;
        int retrieved_num_palette = 0;
        png_uint_32 PLTE_result = png_get_PLTE(read_png_ptr, read_info_ptr,
                                              &retrieved_palette, &retrieved_num_palette);
        (void)PLTE_result;
        
        // Test with null pointers
        if (fdp.ConsumeBool()) {
            png_get_PLTE(read_png_ptr, read_info_ptr, nullptr, nullptr);
        }
    }
    
    // Test png_get_hIST
    if (fdp.ConsumeBool()) {
        png_uint_16* retrieved_hist = nullptr;
        png_uint_32 hIST_result = png_get_hIST(read_png_ptr, read_info_ptr, &retrieved_hist);
        (void)hIST_result;
        
        // Test with null pointer
        if (fdp.ConsumeBool()) {
            png_get_hIST(read_png_ptr, read_info_ptr, nullptr);
        }
    }
    
    // Test combination of getter functions with partial null parameters
    if (fdp.ConsumeBool()) {
        // Test oFFs with only some parameters
        png_int_32 offset_x_only = 0;
        png_get_oFFs(read_png_ptr, read_info_ptr, &offset_x_only, nullptr, nullptr);
        
        // Test cLLI with only first parameter
        double max_content_only = 0.0;
        png_get_cLLI(read_png_ptr, read_info_ptr, &max_content_only, nullptr);
    }
    
    // Read the rest of the image data to complete processing
    png_read_png(read_png_ptr, read_info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Clean up read structures
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    
    // Free the written data buffer
    free(write_data.buffer);
    
    return 0;
}

} // namespace harness_020
