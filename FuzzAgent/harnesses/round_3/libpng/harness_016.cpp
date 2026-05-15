#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_016 {

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
    // Need minimum data for pHYs testing - basic parameters for PNG creation
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Check for sufficient data before consuming parameters
    const size_t MIN_REQUIRED_BYTES = sizeof(png_uint_32) * 4 + 
                                      sizeof(int) * 4 + 
                                      sizeof(png_byte) * 2 +
                                      sizeof(png_uint_32);
    if (fdp.remaining_bytes() < MIN_REQUIRED_BYTES) {
        return 0;
    }
    
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
    
    // Set up error handling
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // ====================================================
    // STEP 3: SET UP MEMORY-BASED WRITING
    // ====================================================
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.size = 0;
    write_data.offset = 0;
    write_data.capacity = 0;
    
    // Initialize with small buffer
    write_data.buffer = (uint8_t*)malloc(4096);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    write_data.capacity = 4096;
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_write_memory_flush);
    
    // ====================================================
    // STEP 4: SET BASIC PNG METADATA FOR TESTING
    // ====================================================
    // Consume basic image parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_byte bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    png_byte color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_RGB});
    
    png_set_IHDR(write_png_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // ====================================================
    // STEP 5: SET pHYs (PHYSICAL DIMENSIONS) DATA
    // ====================================================
    // Consume pHYs parameters from fuzzer input
    png_uint_32 res_x = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 res_y = fdp.ConsumeIntegral<png_uint_32>();
    int unit_type = fdp.PickValueInArray({PNG_RESOLUTION_UNKNOWN, PNG_RESOLUTION_METER});
    
    png_set_pHYs(write_png_ptr, write_info_ptr, res_x, res_y, unit_type);
    
    // Also set oFFs (offset) chunk if we have enough data for offset calculations
    if (fdp.remaining_bytes() >= sizeof(png_int_32) * 2 + sizeof(int)) {
        png_int_32 x_offset = fdp.ConsumeIntegral<png_int_32>();
        png_int_32 y_offset = fdp.ConsumeIntegral<png_int_32>();
        int offset_unit_type = fdp.PickValueInArray({PNG_OFFSET_PIXEL, PNG_OFFSET_MICROMETER});
        png_set_oFFs(write_png_ptr, write_info_ptr, x_offset, y_offset, offset_unit_type);
    }
    
    // ====================================================
    // STEP 6: WRITE PNG DATA (MINIMAL TO CREATE VALID PNG)
    // ====================================================
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Allocate and write minimal image data
    png_uint_32 rowbytes = png_get_rowbytes(write_png_ptr, write_info_ptr);
    if (rowbytes > 0 && rowbytes < 10000) {
        png_bytep row = (png_bytep)malloc(rowbytes);
        if (row) {
            // Fill row with zeros for simplicity
            memset(row, 0, rowbytes);
            
            // Write one row (enough to create valid PNG)
            png_write_row(write_png_ptr, row);
            free(row);
        }
    }
    
    png_write_end(write_png_ptr, write_info_ptr);
    
    // ====================================================
    // STEP 7: CLEAN UP WRITER
    // ====================================================
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    // ====================================================
    // STEP 8: CREATE READ STRUCTURE TO TEST RETRIEVAL FUNCTIONS
    // ====================================================
    png_struct *read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (read_png_ptr == nullptr) {
        free(write_data.buffer);
        return 0;
    }
    
    png_info *read_info_ptr = png_create_info_struct(read_png_ptr);
    if (read_info_ptr == nullptr) {
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // Set up error handling for reader
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        free(write_data.buffer);
        return 0;
    }
    
    // ====================================================
    // STEP 9: SET UP MEMORY-BASED READING FROM BUFFER WE JUST WROTE
    // ====================================================
    png_read_memory_data read_data;
    read_data.buffer = write_data.buffer;
    read_data.size = write_data.size;
    read_data.offset = 0;
    
    png_set_read_fn(read_png_ptr, &read_data, png_read_memory_callback);
    
    // ====================================================
    // STEP 10: READ PNG INFO (INCLUDING pHYs DATA)
    // ====================================================
    png_read_info(read_png_ptr, read_info_ptr);
    
    // ====================================================
    // STEP 11: TEST ALL pHYs-RELATED RETRIEVAL FUNCTIONS
    // ====================================================
    
    // 1. Basic pHYs retrieval (already partially covered but has 6 undiscovered branches)
    png_uint_32 retrieved_res_x, retrieved_res_y;
    int retrieved_unit_type;
    png_uint_32 pHYs_valid = png_get_pHYs(read_png_ptr, read_info_ptr, 
                                         &retrieved_res_x, &retrieved_res_y, 
                                         &retrieved_unit_type);
    
    // Only proceed with DPI calculations if pHYs chunk is valid
    if (pHYs_valid & PNG_INFO_pHYs) {
        // 2. Test png_get_pHYs_dpi (18 undiscovered branches, 0% coverage)
        png_uint_32 res_x_dpi, res_y_dpi;
        int unit_type_dpi;
        png_uint_32 dpi_valid = png_get_pHYs_dpi(read_png_ptr, read_info_ptr,
                                                &res_x_dpi, &res_y_dpi, &unit_type_dpi);
        
        // 3. Test png_get_pixel_aspect_ratio_fixed (16 undiscovered branches, 0% coverage)
        #ifdef PNG_FIXED_POINT_SUPPORTED
        png_fixed_point aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(read_png_ptr, read_info_ptr);
        #endif
        
        // 4. Test png_get_pixels_per_inch (10 undiscovered branches, 0% coverage)
        #ifdef PNG_INCH_CONVERSIONS_SUPPORTED
        png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(read_png_ptr, read_info_ptr);
        png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(read_png_ptr, read_info_ptr);
        
        // Also test the consolidated function
        png_uint_32 pixels_per_inch = png_get_pixels_per_inch(read_png_ptr, read_info_ptr);
        #endif
        
        // 5. Test png_get_pixels_per_meter (10 undiscovered branches, 0% coverage)
        #ifdef PNG_INCH_CONVERSIONS_SUPPORTED
        png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(read_png_ptr, read_info_ptr);
        png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(read_png_ptr, read_info_ptr);
        #endif
    }
    
    // ====================================================
    // STEP 12: TEST OFFSET-RELATED FUNCTIONS (IF oFFs WAS SET)
    // ====================================================
    // Check if oFFs chunk is valid
    if (png_get_valid(read_png_ptr, read_info_ptr, PNG_INFO_oFFs)) {
        // 6. Test png_get_x_offset_inches_fixed (9 undiscovered branches, 0% coverage)
        #if defined(PNG_INCH_CONVERSIONS_SUPPORTED) && defined(PNG_FIXED_POINT_SUPPORTED)
        png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(read_png_ptr, read_info_ptr);
        #endif
        
        // 7. Test png_get_y_offset_inches_fixed (9 undiscovered branches, 0% coverage)
        #if defined(PNG_INCH_CONVERSIONS_SUPPORTED) && defined(PNG_FIXED_POINT_SUPPORTED)
        png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(read_png_ptr, read_info_ptr);
        #endif
        
        // Also test the floating point versions if supported
        #if defined(PNG_INCH_CONVERSIONS_SUPPORTED) && defined(PNG_FLOATING_POINT_SUPPORTED)
        float x_offset_inches = png_get_x_offset_inches(read_png_ptr, read_info_ptr);
        float y_offset_inches = png_get_y_offset_inches(read_png_ptr, read_info_ptr);
        #endif
    }
    
    // ====================================================
    // STEP 13: TEST ADDITIONAL DERIVED CALCULATIONS
    // ====================================================
    // Test floating point aspect ratio if supported
    #ifdef PNG_FLOATING_POINT_SUPPORTED
    float aspect_ratio_float = png_get_pixel_aspect_ratio(read_png_ptr, read_info_ptr);
    #endif
    
    // Test other unit conversions
    #ifdef PNG_pHYs_SUPPORTED
    png_int_32 x_offset_microns = png_get_x_offset_microns(read_png_ptr, read_info_ptr);
    png_int_32 y_offset_microns = png_get_y_offset_microns(read_png_ptr, read_info_ptr);
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(read_png_ptr, read_info_ptr);
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(read_png_ptr, read_info_ptr);
    #endif
    
    // ====================================================
    // STEP 14: CLEAN UP READER AND FREE MEMORY
    // ====================================================
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    
    // Free the memory buffer we allocated for writing
    free(write_data.buffer);
    
    return 0;
}

} // namespace harness_016
