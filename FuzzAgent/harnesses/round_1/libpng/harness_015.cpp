#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_015 {
/*
 * Harness for testing libpng physical dimension metadata getter functions (pHYs and oFFs chunks).
 * Targets 17+ getter functions in pngget.c with 150+ points of undiscovered complexity:
 * - png_get_oFFs (12 undiscovered branches)
 * - png_get_pixels_per_meter (10)
 * - png_get_pixels_per_inch (10)
 * - png_get_x_offset_inches_fixed (9)
 * - png_get_y_offset_inches_fixed (9)
 * - png_get_x_offset_inches (8)
 * - png_get_y_offset_inches (8)
 * - png_get_x_offset_microns (8)
 * - png_get_y_offset_microns (8)
 * - png_get_x_offset_pixels (8)
 * - png_get_y_offset_pixels (8)
 * - png_get_x_pixels_per_meter (8)
 * - png_get_y_pixels_per_meter (8)
 * - png_get_x_pixels_per_inch (8)
 * - png_get_y_pixels_per_inch (8)
 * - png_get_pixel_aspect_ratio_fixed (16)
 * - png_get_pixel_aspect_ratio (8)
 *
 * This harness implements the complete lifecycle for pHYs and oFFs chunks,
 * addressing physical dimension metadata retrieval. The pngget.c module has only 9.37%
 * coverage, and getter functions for physical dimensions are largely untouched.
 *
 * Semantically different from previous harnesses:
 * - harness_014.cpp: CICP colorimetric information and unknown chunk handling
 * - This harness: Physical dimension metadata (pHYs and oFFs chunks)
 */


// ==================== MEMORY BUFFER FOR WRITE/READ ====================

struct mem_buffer {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t offset;  // For reading
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->offset + length) {
            new_capacity = buffer->offset + length;
        }
        
        uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_capacity);
        if (new_data == NULL) {
            png_error(png_ptr, "Memory allocation failed");
            return;
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    // Copy data to buffer
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
    buffer->size = buffer->offset;
}

// Custom flush function (does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
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
    // Need minimum input for various parameters:
    // We need: image parameters, pHYs parameters, oFFs parameters
    if (size < 200) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== CREATE WRITE STRUCTURES ====================
    
    // Create write structure
    png_structp png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        png_error_callback,
        png_warning_callback);
    
    if (png_write_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (info_write_ptr == NULL) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for write path
    if (setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // ==================== SET UP MEMORY BUFFER FOR WRITING ====================
    
    mem_buffer write_buffer;
    write_buffer.capacity = 8192;  // Initial capacity
    write_buffer.data = (uint8_t*)malloc(write_buffer.capacity);
    if (write_buffer.data == NULL) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    write_buffer.size = 0;
    write_buffer.offset = 0;
    
    // Set write function to memory buffer
    png_set_write_fn(png_write_ptr, &write_buffer, png_write_to_memory, png_flush_memory);
    
    // ==================== CONFIGURE IMAGE PARAMETERS ====================
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA,
        PNG_COLOR_TYPE_GRAY_ALPHA
    });
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Set image header (IHDR) - required before setting chunks
    png_set_IHDR(png_write_ptr, info_write_ptr, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // ==================== SET pHYs CHUNK DATA ====================
    
    // Consume pHYs parameters from fuzzer input
    png_uint_32 res_x = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 res_y = fdp.ConsumeIntegral<png_uint_32>();
    int unit_type_phys = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=unknown, 1=meter, 2=inch
    
    // Set pHYs data (TARGET API: png_set_pHYs)
    png_set_pHYs(png_write_ptr, info_write_ptr, res_x, res_y, unit_type_phys);
    
    // ==================== SET oFFs CHUNK DATA ====================
    
    // Consume oFFs parameters from fuzzer input
    png_int_32 offset_x = fdp.ConsumeIntegral<png_int_32>();
    png_int_32 offset_y = fdp.ConsumeIntegral<png_int_32>();
    int unit_type_offs = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=unknown, 1=meter, 2=inch
    
    // Set oFFs data (TARGET API: png_set_oFFs)
    png_set_oFFs(png_write_ptr, info_write_ptr, offset_x, offset_y, unit_type_offs);
    
    // ==================== WRITE PNG DATA TO MEMORY BUFFER ====================
    
    // Write PNG info (before PLTE if needed)
    png_write_info(png_write_ptr, info_write_ptr);
    
    // Write some dummy image data if dimensions are reasonable
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        png_size_t row_bytes = png_get_rowbytes(png_write_ptr, info_write_ptr);
        if (row_bytes > 0 && row_bytes < 100000) {
            // Allocate a row buffer
            png_bytep row = (png_bytep)malloc(row_bytes);
            if (row != NULL) {
                // Fill row with dummy data
                memset(row, 0, row_bytes);
                
                // Write all rows
                for (png_uint_32 y = 0; y < height; y++) {
                    png_write_row(png_write_ptr, row);
                }
                
                free(row);
            }
        }
    }
    
    // Write end of PNG
    png_write_end(png_write_ptr, info_write_ptr);
    
    // ==================== READ BACK PNG DATA ====================
    
    // Reset buffer offset for reading
    write_buffer.offset = 0;
    
    // Create read structure
    png_structp png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        png_error_callback,
        png_warning_callback);
    
    if (png_read_ptr == NULL) {
        // Clean up write structures and buffer
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Create read info structure
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (info_read_ptr == NULL) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Set up error handling for read path
    if (setjmp(png_jmpbuf(png_read_ptr))) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        free(write_buffer.data);
        return 0;
    }
    
    // Set read function to memory buffer
    png_set_read_fn(png_read_ptr, &write_buffer, png_read_from_memory);
    
    // Read PNG info (including pHYs and oFFs chunks)
    png_read_info(png_read_ptr, info_read_ptr);
    
    // ==================== TARGET GETTER FUNCTIONS ====================
    
    // 1. png_get_oFFs (12 undiscovered branches)
    {
        png_int_32 retrieved_offset_x, retrieved_offset_y;
        int retrieved_unit_type;
        png_uint_32 valid = png_get_oFFs(png_read_ptr, info_read_ptr,
                                         &retrieved_offset_x, &retrieved_offset_y,
                                         &retrieved_unit_type);
        (void)valid;  // Use result to avoid unused variable warning
    }
    
    // 2. png_get_pixels_per_meter (10)
    {
        png_uint_32 pixels_per_meter_x = png_get_pixels_per_meter(png_read_ptr, info_read_ptr);
        png_uint_32 pixels_per_meter_y = png_get_y_pixels_per_meter(png_read_ptr, info_read_ptr);
        (void)pixels_per_meter_x;
        (void)pixels_per_meter_y;
    }
    
    // 3. png_get_pixels_per_inch (10)
    {
        png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_read_ptr, info_read_ptr);
        (void)pixels_per_inch;
    }
    
    // 4. png_get_x_offset_inches_fixed (9) and png_get_y_offset_inches_fixed (9)
    {
        png_fixed_point offset_x_inches_fixed = png_get_x_offset_inches_fixed(png_read_ptr, info_read_ptr);
        png_fixed_point offset_y_inches_fixed = png_get_y_offset_inches_fixed(png_read_ptr, info_read_ptr);
        (void)offset_x_inches_fixed;
        (void)offset_y_inches_fixed;
    }
    
    // 5. png_get_x_offset_inches (8) and png_get_y_offset_inches (8)
    {
        float offset_x_inches = png_get_x_offset_inches(png_read_ptr, info_read_ptr);
        float offset_y_inches = png_get_y_offset_inches(png_read_ptr, info_read_ptr);
        (void)offset_x_inches;
        (void)offset_y_inches;
    }
    
    // 6. png_get_x_offset_microns (8) and png_get_y_offset_microns (8)
    {
        png_int_32 offset_x_microns = png_get_x_offset_microns(png_read_ptr, info_read_ptr);
        png_int_32 offset_y_microns = png_get_y_offset_microns(png_read_ptr, info_read_ptr);
        (void)offset_x_microns;
        (void)offset_y_microns;
    }
    
    // 7. png_get_x_offset_pixels (8) and png_get_y_offset_pixels (8)
    {
        png_int_32 offset_x_pixels = png_get_x_offset_pixels(png_read_ptr, info_read_ptr);
        png_int_32 offset_y_pixels = png_get_y_offset_pixels(png_read_ptr, info_read_ptr);
        (void)offset_x_pixels;
        (void)offset_y_pixels;
    }
    
    // 8. png_get_x_pixels_per_meter (8) and png_get_y_pixels_per_meter (8)
    {
        png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_read_ptr, info_read_ptr);
        png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_read_ptr, info_read_ptr);
        (void)x_ppm;
        (void)y_ppm;
    }
    
    // 9. png_get_x_pixels_per_inch (8) and png_get_y_pixels_per_inch (8)
    {
        png_uint_32 x_ppi = png_get_x_pixels_per_inch(png_read_ptr, info_read_ptr);
        png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_read_ptr, info_read_ptr);
        (void)x_ppi;
        (void)y_ppi;
    }
    
    // 10. png_get_pixel_aspect_ratio_fixed (16)
    {
        png_fixed_point aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_read_ptr, info_read_ptr);
        (void)aspect_ratio_fixed;
    }
    
    // 11. png_get_pixel_aspect_ratio (8)
    {
        float aspect_ratio = png_get_pixel_aspect_ratio(png_read_ptr, info_read_ptr);
        (void)aspect_ratio;
    }
    
    // ==================== CLEANUP ====================
    
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    free(write_buffer.data);
    
    return 0;
}

} // namespace harness_015
