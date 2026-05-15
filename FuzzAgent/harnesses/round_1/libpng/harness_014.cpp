#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_014 {
/*
 * Harness for testing libpng CICP (colorimetric information) and unknown chunk handling operations.
 * Targets high-complexity, currently uncovered APIs in pngget.c and pngset.c:
 * - png_get_cICP (14 undiscovered branches, 0% coverage in pngget.c)
 * - png_set_cICP (set CICP chunk data)
 * - png_set_unknown_chunk_location (17 undiscovered branches, 0% coverage)
 * - png_set_unknown_chunks (28 undiscovered branches)
 *
 * This harness implements the complete CICP chunk lifecycle and unknown chunk handling,
 * addressing 59+ points of undiscovered complexity. The pngget.c module has only 7.59%
 * coverage, and getter functions like png_get_cICP are largely untouched. This harness
 * specifically improves API coverage (currently 44.28%).
 *
 * Semantically different from previous harnesses:
 * - harness_012.cpp: pCAL chunk operations (pixel calibration)
 * - harness_013.cpp: Color transformation operations (runtime transformations)
 * - This harness: CICP colorimetric information and unknown chunk management
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
    // We need: image parameters, CICP parameters, unknown chunk data, etc.
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
    
    // ==================== SET CICP CHUNK DATA ====================
    
    // Consume CICP parameters from fuzzer input
    png_byte colour_primaries = fdp.ConsumeIntegral<png_byte>();
    png_byte transfer_function = fdp.ConsumeIntegral<png_byte>();
    png_byte matrix_coefficients = fdp.ConsumeIntegral<png_byte>();
    png_byte video_full_range_flag = fdp.ConsumeIntegral<png_byte>();
    
    // Set CICP data (TARGET API: png_set_cICP)
    png_set_cICP(png_write_ptr, info_write_ptr,
                 colour_primaries, transfer_function,
                 matrix_coefficients, video_full_range_flag);
    
    // ==================== SET UNKNOWN CHUNKS ====================
    
    // Determine number of unknown chunks to create
    int num_unknowns = fdp.ConsumeIntegralInRange<int>(0, 5);
    std::vector<png_unknown_chunk> unknown_chunks;
    
    if (num_unknowns > 0 && fdp.remaining_bytes() > num_unknowns * 20) {
        unknown_chunks.resize(num_unknowns);
        
        for (int i = 0; i < num_unknowns; i++) {
            png_unknown_chunk* chunk = &unknown_chunks[i];
            
            // Generate chunk name (must be 4 bytes + null terminator)
            if (fdp.remaining_bytes() >= 4) {
                std::vector<uint8_t> name_bytes = fdp.ConsumeBytes<uint8_t>(4);
                for (int j = 0; j < 4; j++) {
                    chunk->name[j] = name_bytes[j];
                }
                chunk->name[4] = '\0';  // Null terminator
            } else {
                // Use default name if not enough bytes
                const char* default_name = "TEST";
                memcpy(chunk->name, default_name, 4);
                chunk->name[4] = '\0';
            }
            
            // Generate chunk data
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(0, 100);
            if (chunk_size > 0 && fdp.remaining_bytes() >= chunk_size) {
                std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_size);
                chunk->data = (png_byte*)malloc(chunk_size);
                if (chunk->data != NULL) {
                    memcpy(chunk->data, chunk_data.data(), chunk_size);
                    chunk->size = chunk_size;
                } else {
                    chunk->size = 0;
                }
            } else {
                chunk->data = NULL;
                chunk->size = 0;
            }
            
            // Set location (flags for where chunk should be written)
            // Use fuzzer input to determine location
            chunk->location = 0;
            if (fdp.ConsumeBool()) chunk->location |= PNG_HAVE_IHDR;
            if (fdp.ConsumeBool()) chunk->location |= PNG_HAVE_PLTE;
            if (fdp.ConsumeBool()) chunk->location |= PNG_AFTER_IDAT;
        }
        
        // Set unknown chunks (TARGET API: png_set_unknown_chunks)
        if (!unknown_chunks.empty()) {
            png_set_unknown_chunks(png_write_ptr, info_write_ptr,
                                   unknown_chunks.data(), num_unknowns);
            
            // Set unknown chunk locations (TARGET API: png_set_unknown_chunk_location)
            for (int i = 0; i < num_unknowns; i++) {
                int location = unknown_chunks[i].location;
                // Ensure at least one location bit is set
                if (location == 0) {
                    location = PNG_HAVE_IHDR;
                }
                png_set_unknown_chunk_location(png_write_ptr, info_write_ptr, i, location);
            }
        }
    }
    
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
        for (auto& chunk : unknown_chunks) {
            if (chunk.data != NULL) {
                free(chunk.data);
            }
        }
        free(write_buffer.data);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Create info structure for reading
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (info_read_ptr == NULL) {
        for (auto& chunk : unknown_chunks) {
            if (chunk.data != NULL) {
                free(chunk.data);
            }
        }
        free(write_buffer.data);
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set up error handling for read path
    if (setjmp(png_jmpbuf(png_read_ptr))) {
        for (auto& chunk : unknown_chunks) {
            if (chunk.data != NULL) {
                free(chunk.data);
            }
        }
        free(write_buffer.data);
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set read function to memory buffer
    png_set_read_fn(png_read_ptr, &write_buffer, png_read_from_memory);
    
    // Read PNG info
    png_read_info(png_read_ptr, info_read_ptr);
    
    // ==================== GET CICP DATA ====================
    
    png_byte read_colour_primaries = 0;
    png_byte read_transfer_function = 0;
    png_byte read_matrix_coefficients = 0;
    png_byte read_video_full_range_flag = 0;
    
    // Get CICP data (TARGET API: png_get_cICP)
    png_uint_32 cicp_result = png_get_cICP(png_read_ptr, info_read_ptr,
                                          &read_colour_primaries,
                                          &read_transfer_function,
                                          &read_matrix_coefficients,
                                          &read_video_full_range_flag);
    
    // Note: We don't validate the retrieved values since the goal is to exercise
    // the getter API and explore its code paths. The fuzzer will discover
    // edge cases and validation paths within png_get_cICP.
    
    // ==================== READ IMAGE DATA ====================
    
    // Read image data if dimensions are reasonable
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        png_size_t row_bytes = png_get_rowbytes(png_read_ptr, info_read_ptr);
        if (row_bytes > 0 && row_bytes < 100000) {
            // Allocate row pointers
            png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
            if (row_pointers != NULL) {
                // Allocate individual rows
                bool allocation_failed = false;
                for (png_uint_32 y = 0; y < height; y++) {
                    row_pointers[y] = (png_bytep)malloc(row_bytes);
                    if (row_pointers[y] == NULL) {
                        allocation_failed = true;
                        // Clean up rows allocated so far
                        for (png_uint_32 yy = 0; yy < y; yy++) {
                            free(row_pointers[yy]);
                        }
                        free(row_pointers);
                        break;
                    }
                }
                
                if (!allocation_failed) {
                    // Read the image data
                    png_read_image(png_read_ptr, row_pointers);
                    
                    // Read end of PNG
                    png_read_end(png_read_ptr, nullptr);
                    
                    // Clean up allocated rows
                    for (png_uint_32 y = 0; y < height; y++) {
                        free(row_pointers[y]);
                    }
                    free(row_pointers);
                }
            }
        }
    }
    
    // ==================== CLEANUP ====================
    
    // Free unknown chunk data
    for (auto& chunk : unknown_chunks) {
        if (chunk.data != NULL) {
            free(chunk.data);
        }
    }
    
    // Free memory buffer
    free(write_buffer.data);
    
    // Destroy read structures
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    
    // Destroy write structures
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    return 0;
}

} // namespace harness_014
