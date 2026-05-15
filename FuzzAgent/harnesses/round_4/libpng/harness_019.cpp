#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_019 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom read function for memory buffers
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for basic PNG operations and chunk handling
    // We need:
    // - PNG signature: 8 bytes
    // - Basic parameters for chunk retrieval: ~50 bytes  
    // - PNG data for reading: ~100 bytes
    // - Unknown chunk handling parameters: ~50 bytes
    if (size < 210) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Check PNG signature
    png_byte png_sig[8];
    auto sig_bytes = fdp.ConsumeBytes<uint8_t>(8);
    memcpy(png_sig, sig_bytes.data(), 8);
    int is_png = png_sig_cmp(png_sig, 0, 8);
    (void)is_png; // Result can be ignored for fuzzing
    
    // Step 2: Create PNG read structure - TARGET HELPER: png_create_read_struct
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 3: Create info structure - TARGET HELPER: png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Step 4: Set up custom read functions - TARGET HELPER: png_set_read_fn
    // Consume remaining bytes for PNG data after parameter consumption
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    png_memory_buffer buffer;
    buffer.data = png_data.data();
    buffer.size = png_data.size();
    buffer.offset = 0;
    
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Step 5: Set PNG signature bytes if available - TARGET HELPER: png_set_sig_bytes
    // Use some bytes from the consumed PNG data as signature
    if (buffer.size >= 8) {
        png_set_sig_bytes(png_ptr, 8);
    }
    
    // Step 6: Read PNG info - TARGET HELPER: png_read_info
    // Note: This may fail if the PNG data is not valid, which is expected
    png_read_info(png_ptr, info_ptr);
    
    // Step 7: Configure unknown chunk handling - TARGET HELPER: png_set_unknown_chunks
    // Set up parameters for unknown chunk handling
    int num_unknown_chunks = fdp.ConsumeIntegralInRange<int>(0, 5);
    png_set_unknown_chunks(png_ptr, info_ptr, nullptr, num_unknown_chunks);
    
    // Step 8: Set unknown chunk storage location - TARGET HELPER: png_set_unknown_chunk_location
    // Try to set chunk location based on fuzzed input
    int location = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 valid locations
    png_set_unknown_chunk_location(png_ptr, info_ptr, 0, location);
    
    // Step 9: Retrieve various chunk data using png_get_* functions
    
    // TARGET: png_get_PLTE - Retrieve palette information
    png_color* palette = nullptr;
    int num_palette = 0;
    png_uint_32 plte_result = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    (void)plte_result;
    (void)palette;
    (void)num_palette;
    
    // TARGET: png_get_gAMA_fixed - Retrieve gamma information (fixed point)
    png_fixed_point gamma_fixed = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_fixed);
    (void)gama_result;
    (void)gamma_fixed;
    
    // TARGET: png_get_sRGB - Retrieve sRGB rendering intent
    int file_srgb_intent = 0;
    png_uint_32 srgb_result = png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);
    (void)srgb_result;
    (void)file_srgb_intent;
    
    // TARGET: png_get_sBIT - Retrieve significant bits information
    png_color_8* sig_bit = nullptr;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit);
    (void)sbit_result;
    (void)sig_bit;
    
    // TARGET: png_get_hIST - Retrieve histogram information
    png_uint_16p hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;
    (void)hist;
    
    // TARGET: png_get_bKGD - Retrieve background color information
    png_color_16p background = nullptr;
    png_uint_32 bkgd_result = png_get_bKGD(png_ptr, info_ptr, &background);
    (void)bkgd_result;
    (void)background;
    // TARGET: png_get_sPLT - Retrieve suggested palette information
    png_sPLT_t** entries = nullptr;
    int splt_result = png_get_sPLT(png_ptr, info_ptr, entries);
    (void)splt_result;
    (void)entries;
    
    int unit = 0;
    char* scal_width = nullptr;
    char* scal_height = nullptr;
    png_uint_32 scal_s_result = png_get_sCAL_s(png_ptr, info_ptr, &unit, &scal_width, &scal_height);
    (void)scal_s_result;
    (void)unit;
    (void)scal_width;
    (void)scal_height;
    
    // TARGET: png_get_sCAL - Retrieve scaling information (double version)
    int scal_unit = 0;
    double scal_width_d = 0.0;
    double scal_height_d = 0.0;
    png_uint_32 scal_result = png_get_sCAL(png_ptr, info_ptr, &scal_unit, &scal_width_d, &scal_height_d);
    (void)scal_result;
    (void)scal_unit;
    (void)scal_width_d;
    (void)scal_height_d;
    
    // TARGET: png_get_unknown_chunks - Retrieve unknown chunks information
    png_unknown_chunkp unknown_chunks = nullptr;
    int num_unknowns = 0;
    num_unknowns = png_get_unknown_chunks(png_ptr, info_ptr, &unknown_chunks);
    (void)num_unknowns;
    (void)unknown_chunks;
    
    // Step 10: Clean up info structure - TARGET HELPER: png_destroy_info_struct
    png_destroy_info_struct(png_ptr, &info_ptr);
    
    // Step 11: Clean up read structure - TARGET HELPER: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}

} // namespace harness_019
