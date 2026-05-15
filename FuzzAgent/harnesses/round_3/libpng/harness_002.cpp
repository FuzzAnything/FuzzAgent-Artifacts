#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_002 {

// Progressive callback structure to hold user data
struct progressive_data {
    FuzzedDataProvider* fdp;
    bool info_called;
    bool row_called;
    bool end_called;
};

// Progressive info callback - called when PNG info is available
static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    progressive_data* prog_data = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (prog_data) {
        prog_data->info_called = true;
        
        // Get basic image info from PNG
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        
        // Use fuzzer data to decide transformations
        if (prog_data->fdp && prog_data->fdp->remaining_bytes() > 0) {
            int transforms = prog_data->fdp->ConsumeIntegralInRange<int>(0, 0xFFFF);
            png_set_expand(png_ptr);
            png_set_strip_16(png_ptr);
            png_set_gray_to_rgb(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
        }
    }
}

// Progressive row callback - called for each row of image data
static void row_callback(png_structp png_ptr, png_bytep new_row, 
                         png_uint_32 row_number, int pass) {
    progressive_data* prog_data = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (prog_data) {
        prog_data->row_called = true;
        
        // Allocate old_row for combine operation
        png_bytep old_row = NULL;
        png_uint_32 width = png_get_image_width(png_ptr, NULL);
        png_byte channels = png_get_channels(png_ptr, NULL);
        
        if (width > 0 && channels > 0) {
            size_t row_size = width * channels;
            old_row = (png_bytep)png_malloc(png_ptr, row_size);
            
            // Fill old_row with some data for combine operation
            if (prog_data->fdp && prog_data->fdp->remaining_bytes() >= row_size) {
                std::vector<uint8_t> row_data = prog_data->fdp->ConsumeBytes<uint8_t>(row_size);
                memcpy(old_row, row_data.data(), row_size);
            } else {
                // Fill with pattern if not enough fuzzer data
                for (size_t i = 0; i < row_size; i++) {
                    old_row[i] = (png_byte)((i + row_number) % 256);
                }
            }
            
            // Call png_progressive_combine_row - one of our target APIs
            png_progressive_combine_row(png_ptr, old_row, new_row);
            
            // Free allocated memory
            png_free(png_ptr, old_row);
        }
    }
}

// Progressive end callback - called when PNG reading is complete
static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    progressive_data* prog_data = (progressive_data*)png_get_progressive_ptr(png_ptr);
    if (prog_data) {
        prog_data->end_called = true;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for progressive reading
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create PNG read structure - required setup
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create PNG info structure - required setup
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up progressive callback data
    progressive_data prog_data;
    prog_data.fdp = &fdp;
    prog_data.info_called = false;
    prog_data.row_called = false;
    prog_data.end_called = false;

    // Set up progressive read functions - core setup API
    png_set_progressive_read_fn(png_ptr, &prog_data, 
                                info_callback, row_callback, end_callback);

    // Consume fuzzer data to determine processing strategy
    uint8_t processing_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_pause = (processing_mode & 0x1) != 0;
    bool use_skip = (processing_mode & 0x2) != 0;
    
    // Process PNG data in chunks to simulate streaming
    size_t total_processed = 0;
    size_t chunk_size_base = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    while (total_processed < size && fdp.remaining_bytes() > 0) {
        // Determine chunk size for this iteration
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, chunk_size_base);
        if (chunk_size > fdp.remaining_bytes()) {
            chunk_size = fdp.remaining_bytes();
        }
        
        if (chunk_size == 0) break;
        
        // Get chunk data
        std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(chunk_size);
        
        // Core target API: png_process_data
        png_process_data(png_ptr, info_ptr, chunk.data(), chunk.size());
        total_processed += chunk.size();
        
        // Optional pause operation - target API
        if (use_pause && total_processed > size / 2) {
            int save_pending = fdp.ConsumeBool() ? 1 : 0;
            size_t remaining = png_process_data_pause(png_ptr, save_pending);
            // If we got remaining bytes, we would need to re-supply them
            // For fuzzing simplicity, we just note the call was made
            (void)remaining;
            use_pause = false;  // Only pause once
        }
        
        // Optional skip operation - target API  
        if (use_skip && total_processed > size / 3) {
            png_uint_32 skip_bytes = png_process_data_skip(png_ptr);
            if (skip_bytes > 0 && skip_bytes < fdp.remaining_bytes()) {
                // Actually skip the bytes as required
                std::vector<uint8_t> skipped = fdp.ConsumeBytes<uint8_t>(skip_bytes);
                (void)skipped;  // Skip these bytes
            }
            use_skip = false;  // Only skip once
        }
        
        // Check if we should break early based on fuzzer input
        if (fdp.ConsumeProbability<double>() < 0.1) {
            break;
        }
    }

    // Clean up - target API
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}

} // namespace harness_002
