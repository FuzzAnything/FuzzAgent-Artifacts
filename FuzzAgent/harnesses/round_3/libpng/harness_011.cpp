#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_011 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // PNG signature is 8 bytes, need minimum data for testing
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // PHASE 1: INITIALIZATION
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }
    
    // Create PNG info structure
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
    
    // Set up memory-based read data structure
    png_read_memory_data read_data;
    read_data.data = data;
    read_data.size = size;
    read_data.offset = 0;
    
    // Set up the read function with our memory-based callback
    png_set_read_fn(png_ptr, &read_data, png_read_memory_callback);
    
    // Check if the data starts with PNG signature
    bool has_png_signature = false;
    if (size >= 8) {
        has_png_signature = png_sig_cmp((png_bytep)data, 0, 8) == 0;
    }
    
    // Set signature bytes if PNG signature is present
    if (has_png_signature) {
        png_set_sig_bytes(png_ptr, 8);
        read_data.offset = 8; // Skip the signature bytes
    }
    
    // Read PNG info to process chunks
    png_read_info(png_ptr, info_ptr);
    
    // PHASE 2: CORE TESTING LOOP - PNG CHUNK RETRIEVAL FUNCTIONS
    
    // Get the valid chunks mask to check which chunks are present
    // Create a mask of all PNG_INFO_* values we want to check
    png_uint_32 valid_mask = png_get_valid(png_ptr, info_ptr, 
        PNG_INFO_tRNS | PNG_INFO_pHYs | PNG_INFO_iCCP | PNG_INFO_sCAL | 
        PNG_INFO_tIME | PNG_INFO_bKGD | PNG_INFO_sBIT | PNG_INFO_hIST | 
        PNG_INFO_pCAL | PNG_INFO_sPLT);
    
    // 1. png_get_tRNS - transparency data
    if (valid_mask & PNG_INFO_tRNS) {
        png_byte* trans_alpha = nullptr;
        int num_trans = 0;
        png_color_16* trans_color = nullptr;
        png_uint_32 tRNS_result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
        // Result can be used but not required for coverage
        (void)tRNS_result;
    }
    
    // 2. png_get_pHYs - physical dimensions
    if (valid_mask & PNG_INFO_pHYs) {
        png_uint_32 res_x = 0, res_y = 0;
        int unit_type = 0;
        png_uint_32 pHYs_result = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
        (void)pHYs_result;
    }
    
    // 3. png_get_iCCP - ICC profile
    if (valid_mask & PNG_INFO_iCCP) {
        png_charp name = nullptr;
        int compression_type = 0;
        png_bytep profile = nullptr;
        png_uint_32 profile_len = 0;
        png_uint_32 iCCP_result = png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, 
                                               &profile, &profile_len);
        (void)iCCP_result;
    }
    
    // 4. png_get_sCAL - physical scale (using string version)
    if (valid_mask & PNG_INFO_sCAL) {
        int unit = 0;
        char* width_str = nullptr;
        char* height_str = nullptr;
        png_uint_32 sCAL_result = png_get_sCAL_s(png_ptr, info_ptr, &unit, &width_str, &height_str);
        (void)sCAL_result;
    }
    
    // 5. png_get_tIME - timestamp
    if (valid_mask & PNG_INFO_tIME) {
        png_timep mod_time = nullptr;
        png_uint_32 tIME_result = png_get_tIME(png_ptr, info_ptr, &mod_time);
        (void)tIME_result;
    }
    
    // 6. png_get_bKGD - background color
    if (valid_mask & PNG_INFO_bKGD) {
        png_color_16p background = nullptr;
        png_uint_32 bKGD_result = png_get_bKGD(png_ptr, info_ptr, &background);
        (void)bKGD_result;
    }
    
    // 7. png_get_sBIT - significant bits
    if (valid_mask & PNG_INFO_sBIT) {
        png_color_8p sig_bit = nullptr;
        png_uint_32 sBIT_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit);
        (void)sBIT_result;
    }
    
    // 8. png_get_hIST - histogram
    if (valid_mask & PNG_INFO_hIST) {
        png_uint_16p hist = nullptr;
        png_uint_32 hIST_result = png_get_hIST(png_ptr, info_ptr, &hist);
        (void)hIST_result;
    }
    
    // 9. png_get_pCAL - calibration
    if (valid_mask & PNG_INFO_pCAL) {
        png_charp purpose = nullptr;
        png_int_32 X0 = 0, X1 = 0;
        int type = 0, nparams = 0;
        png_charp units = nullptr;
        png_charpp params = nullptr;
        png_uint_32 pCAL_result = png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, 
                                               &type, &nparams, &units, &params);
        (void)pCAL_result;
    }
    
    // 10. png_get_sPLT - suggested palette
    if (valid_mask & PNG_INFO_sPLT) {
        png_sPLT_t *entries = nullptr;
        int num_entries = png_get_sPLT(png_ptr, info_ptr, &entries);
        (void)num_entries;
    }
    
    // 11. png_get_unknown_chunks - unknown chunks
#ifdef PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
    png_unknown_chunkp unknowns = nullptr;
    int num_unknowns = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);
    (void)num_unknowns;
#endif
    
    // Also test some other getter functions that might be related
    // Get basic image info
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    
    (void)width; (void)height; (void)color_type; (void)bit_depth;
    (void)filter_type; (void)interlace_type; (void)compression_type;
    
    // Get rowbytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    (void)rowbytes;
    
    // Get valid chunk info for various chunk types
    png_uint_32 valid_tRNS = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_uint_32 valid_pHYs = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    png_uint_32 valid_iCCP = png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP);
    png_uint_32 valid_sCAL = png_get_valid(png_ptr, info_ptr, PNG_INFO_sCAL);
    png_uint_32 valid_tIME = png_get_valid(png_ptr, info_ptr, PNG_INFO_tIME);
    png_uint_32 valid_bKGD = png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD);
    png_uint_32 valid_sBIT = png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT);
    png_uint_32 valid_hIST = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    png_uint_32 valid_pCAL = png_get_valid(png_ptr, info_ptr, PNG_INFO_pCAL);
    png_uint_32 valid_sPLT = png_get_valid(png_ptr, info_ptr, PNG_INFO_sPLT);
    
    (void)valid_tRNS; (void)valid_pHYs; (void)valid_iCCP; (void)valid_sCAL;
    (void)valid_tIME; (void)valid_bKGD; (void)valid_sBIT; (void)valid_hIST;
    (void)valid_pCAL; (void)valid_sPLT;
    
    // PHASE 3: CLEANUP
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_011
