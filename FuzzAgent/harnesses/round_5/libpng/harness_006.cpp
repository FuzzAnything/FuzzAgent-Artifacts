#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_006 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

// Harness specifically targeting PNG metadata get/set operations
// Focuses on pngget.c and pngset.c with 47+ uncovered APIs
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 50) {
        // Need enough data for PNG signature and basic configuration
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration from fuzzer input
    uint8_t api_config = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4; // 0: read only, 1: write only, 2: mixed, 3: metadata manipulation
    
    // Consume PNG data from remaining input
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (png_data.size() < 8) {
        return 0; // PNG signature is 8 bytes
    }
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        }
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function to read from our buffer
    png_read_data read_data = {png_data.data(), png_data.size(), 0};
    png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
    
    // Tell libpng we haven't read any signature bytes yet
    png_set_sig_bytes(png_ptr, 0);
    
    // Read PNG info (this reads the signature and IHDR)
    png_read_info(png_ptr, info_ptr);
    
    // ========== TARGETED PNG GET OPERATIONS ==========
    // Comprehensive metadata retrieval targeting pngget.c
    
    // 1. Basic image information (already partially covered in existing harnesses)
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    int compression_type = png_get_compression_type(png_ptr, info_ptr);
    int filter_type = png_get_filter_type(png_ptr, info_ptr);
    
    // 2. IHDR retrieval (specific target from coverage analysis)
    png_uint_32 ihdr_width, ihdr_height;
    int ihdr_bit_depth, ihdr_color_type, ihdr_compression_type, ihdr_filter_type, ihdr_interlace_type;
    if (png_get_IHDR(png_ptr, info_ptr, &ihdr_width, &ihdr_height,
                     &ihdr_bit_depth, &ihdr_color_type, &ihdr_interlace_type,
                     &ihdr_compression_type, &ihdr_filter_type)) {
        // Successfully retrieved IHDR
        (void)ihdr_width; (void)ihdr_height; (void)ihdr_bit_depth;
        (void)ihdr_color_type; (void)ihdr_interlace_type;
        (void)ihdr_compression_type; (void)ihdr_filter_type;
    }
    
    // 3. Color palette retrieval (PLTE)
    png_colorp palette;
    int num_palette;
    if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
        // PLTE chunk exists
        (void)palette; (void)num_palette;
    }
    
    // 4. Transparency retrieval (tRNS)
    png_bytep trans;
    int num_trans;
    png_color_16p trans_values;
    if (png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values)) {
        // tRNS chunk exists
        (void)trans; (void)num_trans; (void)trans_values;
    }
    
    // 5. Gamma information
    double file_gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &file_gamma)) {
        // gAMA chunk exists
        (void)file_gamma;
    }
    
    // 6. Chromaticity information (cHRM)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    if (png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y,
                     &green_x, &green_y, &blue_x, &blue_y)) {
        // cHRM chunk exists
        (void)white_x; (void)white_y; (void)red_x; (void)red_y;
        (void)green_x; (void)green_y; (void)blue_x; (void)blue_y;
    }
    
    // 7. sRGB information
    int srgb_intent;
    if (png_get_sRGB(png_ptr, info_ptr, &srgb_intent)) {
        // sRGB chunk exists
        (void)srgb_intent;
    }
    
    // 8. ICC profile information
    png_charp name;
    int compression;
    png_bytep profile;
    png_uint_32 proflen;
    if (png_get_iCCP(png_ptr, info_ptr, &name, &compression, &profile, &proflen)) {
        // iCCP chunk exists
        (void)name; (void)compression; (void)profile; (void)proflen;
    }
    
    // 9. Significant bits information
    png_color_8p sig_bit;
    if (png_get_sBIT(png_ptr, info_ptr, &sig_bit)) {
        // sBIT chunk exists
        (void)sig_bit;
    }
    
    // 10. Text information
    png_textp text_ptr;
    int num_text;
    if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text)) {
        // Text chunks exist
        (void)text_ptr; (void)num_text;
    }
    
    // 11. Time information
    png_timep mod_time;
    if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
        // tIME chunk exists
        (void)mod_time;
    }
    
    // 12. Physical dimensions
    png_uint_32 res_x, res_y;
    int unit_type;
    if (png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type)) {
        // pHYs chunk exists
        (void)res_x; (void)res_y; (void)unit_type;
    }
    
    // 13. sCAL information
    int scal_unit;
    double scal_width, scal_height;
    if (png_get_sCAL(png_ptr, info_ptr, &scal_unit, &scal_width, &scal_height)) {
        // sCAL chunk exists
        (void)scal_unit; (void)scal_width; (void)scal_height;
    }
    
    // 14. Histogram information
    png_uint_16p hist;
    if (png_get_hIST(png_ptr, info_ptr, &hist)) {
        // hIST chunk exists
        (void)hist;
    }
    
    // 15. Offset information
    png_int_32 offset_x, offset_y;
    int offset_unit_type;
    if (png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &offset_unit_type)) {
        // oFFs chunk exists
        (void)offset_x; (void)offset_y; (void)offset_unit_type;
    }
    
    // 16. pCAL information
    png_charp pcal_purpose, pcal_units;
    png_int_32 pcal_X0, pcal_X1;
    int pcal_type, pcal_nparams;
    png_charpp pcal_params;
    if (png_get_pCAL(png_ptr, info_ptr, &pcal_purpose, &pcal_X0, &pcal_X1,
                     &pcal_type, &pcal_nparams, &pcal_units, &pcal_params)) {
        // pCAL chunk exists
        (void)pcal_purpose; (void)pcal_X0; (void)pcal_X1;
        (void)pcal_type; (void)pcal_nparams; (void)pcal_units; (void)pcal_params;
    }
    // 17. sPLT information (commented out due to compilation issues)
    // png_sPLT_tp splt_ptr = NULL;
    // int num_splt = png_get_sPLT(png_ptr, info_ptr, &splt_ptr);
    // if (num_splt > 0) {
    //     // sPLT chunks exist
    //     (void)splt_ptr; (void)num_splt;
    // }
    
    // 18. Unknown chunks
    png_unknown_chunkp unknown_ptr;
    int num_unknown = png_get_unknown_chunks(png_ptr, info_ptr, &unknown_ptr);
    if (num_unknown > 0) {
        // Unknown chunks exist
        (void)unknown_ptr; (void)num_unknown;
    }
    
    // 19. Validity checking
    png_uint_32 valid = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    (void)valid;
    
    // 20. Row bytes and channels
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);
    (void)rowbytes; (void)channels;
    
    // ========== TARGETED PNG SET OPERATIONS ==========
    // Test metadata setting operations based on fuzzer input
    if (operation_mode == 1 || operation_mode == 2 || operation_mode == 3) {
        // Use bits from api_config to determine which set operations to perform
        if (api_config & 0x01) {
            // Set gamma using floating point version
            png_set_gAMA(png_ptr, info_ptr, 0.45455);
        }
        
        if (api_config & 0x02) {
            // Set chromaticity using floating point version
            png_set_cHRM(png_ptr, info_ptr,
                        0.3127, 0.3290,  // white
                        0.6400, 0.3300,  // red
                        0.3000, 0.6000,  // green
                        0.1500, 0.0600); // blue
        }
        
        if (api_config & 0x04) {
            // Set sRGB intent
            png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);
        }
        
        if (api_config & 0x08) {
            // Set physical dimensions
            png_set_pHYs(png_ptr, info_ptr, 72, 72, PNG_RESOLUTION_METER);
        }
        
        if (api_config & 0x10) {
            // Set offset
            png_set_oFFs(png_ptr, info_ptr, 0, 0, PNG_OFFSET_PIXEL);
        }
        
        if (api_config & 0x20) {
            // Set text (simple text entry)
            png_text text;
            text.compression = PNG_TEXT_COMPRESSION_NONE;
            text.key = (png_charp)"Fuzzer";
            text.text = (png_charp)"Generated by fuzzer";
            text.text_length = strlen(text.text);
            text.itxt_length = 0;
            text.lang = NULL;
            text.lang_key = NULL;
            png_set_text(png_ptr, info_ptr, &text, 1);
        }
        
        if (api_config & 0x40) {
            // Set time
            png_time mod_time_set;
            mod_time_set.year = 2024;
            mod_time_set.month = 1;
            mod_time_set.day = 1;
            mod_time_set.hour = 12;
            mod_time_set.minute = 0;
            mod_time_set.second = 0;
            png_set_tIME(png_ptr, info_ptr, &mod_time_set);
        }
        
        if (api_config & 0x80) {
            // Set significant bits
            png_color_8 sig_bit_set;
            sig_bit_set.red = 8;
            sig_bit_set.green = 8;
            sig_bit_set.blue = 8;
            sig_bit_set.gray = 8;
            sig_bit_set.alpha = 8;
            png_set_sBIT(png_ptr, info_ptr, &sig_bit_set);
        }
    }
    
    // Update info after potential set operations
    png_read_update_info(png_ptr, info_ptr);
    
    // ========== ADVANCED METADATA MANIPULATION ==========
    if (operation_mode == 3) {
        // Test more complex metadata scenarios
        
        // Check and manipulate valid flags
        png_uint_32 current_valid = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
        
        // Set invalid flag (test png_set_invalid) - commented out due to potential issues
        // if (current_valid & PNG_INFO_gAMA) {
        //     png_set_invalid(png_ptr, info_ptr, PNG_INFO_gAMA);
        // }
        
        // Test user limits APIs
        png_set_user_limits(png_ptr, 10000, 10000);
        png_uint_32 user_width_max = png_get_user_width_max(png_ptr);
        png_uint_32 user_height_max = png_get_user_height_max(png_ptr);
        (void)user_width_max; (void)user_height_max;
        
        // Test chunk cache limits
        png_set_chunk_cache_max(png_ptr, 1000);
        png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);
        (void)chunk_cache_max;
        
        // Test chunk malloc max
        png_set_chunk_malloc_max(png_ptr, 8000000);
        png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
        (void)chunk_malloc_max;
        
        // Test compression buffer size
        png_set_compression_buffer_size(png_ptr, 8192);
        size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr);
        (void)comp_buffer_size;
        
        // Test benign errors setting
        png_set_benign_errors(png_ptr, 1);
        
        // Test check for invalid index
        png_set_check_for_invalid_index(png_ptr, 1);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    return 0;
}

} // namespace harness_006
