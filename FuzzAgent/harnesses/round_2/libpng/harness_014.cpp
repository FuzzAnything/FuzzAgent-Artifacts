#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_014 {
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

// Custom write function for libpng that writes to memory buffer
void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length writes according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to write
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    // Cast to vector pointer and append data
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(io_ptr);
    buffer->insert(buffer->end(), data, data + length);
}

// Flush function (required but can be empty for memory buffers)
void user_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

// Helper function to duplicate string to mutable buffer
char* duplicate_string(const std::string& str) {
    char* copy = (char*)malloc(str.length() + 1);
    if (copy) {
        memcpy(copy, str.c_str(), str.length() + 1);
    }
    return copy;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for metadata/chunk testing
    // Required: image parameters + all metadata values for pCAL, cLLI, text, cHRM, gamma
    // Each metadata structure needs multiple parameters and strings
    if (size < 1024) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Store allocated memory that needs to be freed
    std::vector<char*> allocated_strings;
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(8, 16);
    int color_type = PNG_COLOR_TYPE_RGB; // Use RGB for metadata testing
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  NULL, 
                                                  user_error_fn, 
                                                  user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Cleanup allocated strings before returning
        for (char* str : allocated_strings) {
            free(str);
        }
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up output buffer for writing
    std::vector<uint8_t> output_buffer;
    png_set_write_fn(png_ptr, &output_buffer, user_write_data, user_flush_data);
    
    // Set image header information (Required Setup API)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET APIs: PNG Metadata/Chunk Getter/Setter Operations
    // Targeting 200+ undiscovered branch complexity across metadata APIs
    // Following the lifecycle: Create PNG → Set metadata → Get metadata → Verify → Destroy
    ////////////////////////////////////////////////////////////////////////////
    
    // TARGET 1: pCAL (Pixel Calibration) metadata
#ifdef PNG_pCAL_SUPPORTED
    if (fdp.remaining_bytes() > 512) {
        // Generate pCAL data from fuzzer input
        std::string purpose_str = fdp.ConsumeRandomLengthString(100);
        char* purpose = duplicate_string(purpose_str);
        if (purpose) allocated_strings.push_back(purpose);
        
        png_int_32 X0 = fdp.ConsumeIntegral<png_int_32>();
        png_int_32 X1 = fdp.ConsumeIntegral<png_int_32>();
        int type = fdp.ConsumeIntegralInRange<int>(0, 3);  // Valid pCAL types: 0-3
        int nparams = fdp.ConsumeIntegralInRange<int>(0, 5);  // Up to 5 parameters
        
        std::string units_str = fdp.ConsumeRandomLengthString(50);
        char* units = duplicate_string(units_str);
        if (units) allocated_strings.push_back(units);
        
        // Create parameter strings for pCAL
        char* params[5] = {NULL};
        for (int i = 0; i < nparams && fdp.remaining_bytes() > 50; i++) {
            std::string param_str = fdp.ConsumeRandomLengthString(50);
            char* param = duplicate_string(param_str);
            if (param) {
                allocated_strings.push_back(param);
                params[i] = param;
            }
        }
        
        // Set pCAL metadata (Target: 36 branches, 41 undiscovered)
        png_set_pCAL(png_ptr, info_ptr, purpose, X0, X1, type, 
                     nparams, units, params);
        
        // Get pCAL metadata (Target: 20 undiscovered branches)
        // Call png_get_pCAL regardless of whether it was set successfully
        // The function will return 0 if the data wasn't properly set
        char* retrieved_purpose = NULL;
        png_int_32 retrieved_X0, retrieved_X1;
        int retrieved_type, retrieved_nparams;
        char* retrieved_units = NULL;
        char** retrieved_params = NULL;
        
        png_uint_32 result = png_get_pCAL(png_ptr, info_ptr, &retrieved_purpose,
                                          &retrieved_X0, &retrieved_X1, &retrieved_type,
                                          &retrieved_nparams, &retrieved_units, 
                                          &retrieved_params);
        
        // Note: retrieved pointers are owned by libpng, don't free them
    }
#endif // PNG_pCAL_SUPPORTED
    
    // TARGET 2: cLLI (Content Light Level Information) metadata
#ifdef PNG_cLLI_SUPPORTED
    if (fdp.remaining_bytes() > 16) {
        // Generate cLLI data from fuzzer input
        double max_content_light = fdp.ConsumeFloatingPoint<double>();
        double max_frame_avg_light = fdp.ConsumeFloatingPoint<double>();
        
        // Ensure positive values for light levels
        if (max_content_light < 0) max_content_light = -max_content_light;
        if (max_frame_avg_light < 0) max_frame_avg_light = -max_frame_avg_light;
        
        // Set cLLI metadata (Target: 28 branches, 18 undiscovered)
        png_set_cLLI(png_ptr, info_ptr, max_content_light, max_frame_avg_light);
        
        // Get cLLI metadata (Target: 10 undiscovered branches)
        double retrieved_max_content_light, retrieved_max_frame_avg_light;
        
        png_uint_32 result = png_get_cLLI(png_ptr, info_ptr,
                                         &retrieved_max_content_light,
                                         &retrieved_max_frame_avg_light);
        
        // Also test fixed-point version if available
#ifdef PNG_FIXED_POINT_SUPPORTED
        if (fdp.remaining_bytes() > 8) {
            png_uint_32 max_content_fixed = fdp.ConsumeIntegral<png_uint_32>();
            png_uint_32 max_frame_avg_fixed = fdp.ConsumeIntegral<png_uint_32>();
            
            png_set_cLLI_fixed(png_ptr, info_ptr, max_content_fixed, max_frame_avg_fixed);
            
            // Get the fixed-point version
            png_uint_32 retrieved_max_content_fixed, retrieved_max_frame_avg_fixed;
            png_get_cLLI_fixed(png_ptr, info_ptr,
                              &retrieved_max_content_fixed,
                              &retrieved_max_frame_avg_fixed);
        }
#endif // PNG_FIXED_POINT_SUPPORTED
    }
#endif // PNG_cLLI_SUPPORTED
    
    // TARGET 3: TEXT metadata
#ifdef PNG_TEXT_SUPPORTED
    if (fdp.remaining_bytes() > 300) {
        // Create text chunks from fuzzer input
        int num_text = fdp.ConsumeIntegralInRange<int>(1, 3);
        png_text text_chunks[3];
        int successful_text = 0;
        
        for (int i = 0; i < num_text && fdp.remaining_bytes() > 100; i++) {
            // Create mutable copies of strings
            std::string key_str = fdp.ConsumeRandomLengthString(80);
            std::string text_str = fdp.ConsumeRandomLengthString(20);
            
            char* key = duplicate_string(key_str);
            char* text = duplicate_string(text_str);
            
            if (key && text) {
                allocated_strings.push_back(key);
                allocated_strings.push_back(text);
                
                text_chunks[successful_text].compression = fdp.ConsumeIntegralInRange<int>(0, 2);
                text_chunks[successful_text].key = key;
                text_chunks[successful_text].text = text;
                text_chunks[successful_text].text_length = text_str.length();
                text_chunks[successful_text].itxt_length = 0;
                text_chunks[successful_text].lang = NULL;
                text_chunks[successful_text].lang_key = NULL;
                successful_text++;
            } else {
                // Cleanup if allocation failed
                if (key) free(key);
                if (text) free(text);
                // Skip this text chunk
            }
        }
        
        // Only set text metadata if we have successful text chunks
        if (successful_text > 0) {
            // Set text metadata (Target: 32 branches, 16 undiscovered)
            png_set_text(png_ptr, info_ptr, text_chunks, successful_text);
            
            // Get text metadata (Target: 12 undiscovered branches)
            png_text* retrieved_text = NULL;
            int retrieved_num_text = 0;
            
            int result = png_get_text(png_ptr, info_ptr, &retrieved_text, &retrieved_num_text);
            
            // Note: retrieved_text is owned by libpng, don't free it
        }
    }
#endif // PNG_TEXT_SUPPORTED
    
    // TARGET 4: cHRM (Chromaticity) metadata
#ifdef PNG_cHRM_SUPPORTED
    if (fdp.remaining_bytes() > 128) {
        // Generate chromaticity data from fuzzer input
        png_fixed_point white_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point white_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_y = fdp.ConsumeIntegral<png_fixed_point>();
        
        // Set cHRM metadata (Target: 14 branches, 7 undiscovered)
        png_set_cHRM_fixed(png_ptr, info_ptr, white_x, white_y, red_x, red_y,
                           green_x, green_y, blue_x, blue_y);
        
        // Get cHRM metadata (Target: 22 branches, 10 undiscovered)
        png_fixed_point retrieved_white_x, retrieved_white_y;
        png_fixed_point retrieved_red_x, retrieved_red_y;
        png_fixed_point retrieved_green_x, retrieved_green_y;
        png_fixed_point retrieved_blue_x, retrieved_blue_y;
        
        png_uint_32 result = png_get_cHRM_fixed(png_ptr, info_ptr,
                                                &retrieved_white_x, &retrieved_white_y,
                                                &retrieved_red_x, &retrieved_red_y,
                                                &retrieved_green_x, &retrieved_green_y,
                                                &retrieved_blue_x, &retrieved_blue_y);
    }
#endif // PNG_cHRM_SUPPORTED
    
    // TARGET 5: gAMA (Gamma) metadata
#ifdef PNG_gAMA_SUPPORTED
    if (fdp.remaining_bytes() > sizeof(png_fixed_point)) {
        // Generate gamma data from fuzzer input
        png_fixed_point gamma = fdp.ConsumeIntegral<png_fixed_point>();
        
        // Set gamma metadata (Target: 14 branches, 7 undiscovered)
        png_set_gAMA_fixed(png_ptr, info_ptr, gamma);
        
        // Get gamma metadata (Target: 8 undiscovered branches)
        png_fixed_point retrieved_gamma;
        
        png_uint_32 result = png_get_gAMA_fixed(png_ptr, info_ptr, &retrieved_gamma);
    }
#endif // PNG_gAMA_SUPPORTED
    
    // Try to write the PNG to exercise the metadata APIs
    if (info_ptr != NULL) {
        // Write info to output buffer
        png_write_info(png_ptr, info_ptr);
        
        // Create some dummy row data using vector instead of VLA
        std::vector<png_byte> row(width * 3); // RGB image
        for (png_uint_32 y = 0; y < height && y < 10; y++) { // Limit rows for performance
            for (png_uint_32 x = 0; x < width * 3; x++) {
                row[x] = (png_byte)((x + y) % 256);
            }
            png_write_row(png_ptr, row.data());
        }
        
        png_write_end(png_ptr, info_ptr);
    }
    
    // Cleanup: Free allocated strings, then destroy PNG structures
    for (char* str : allocated_strings) {
        free(str);
    }
    
    // Cleanup: Destroy info structure and PNG structure
    if (info_ptr != NULL) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr != NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
    }
    
    return 0;
}

} // namespace harness_014
