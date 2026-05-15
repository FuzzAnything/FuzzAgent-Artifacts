#include <cstddef>      // for size_t
#include <cstdio>       // for FILE
#include <csetjmp>      // for setjmp/longjmp
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "jpeglib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_015 {
/*
 * Fuzzing harness for libjpeg-turbo targeting JPEG marker processing operations
 * This harness specifically targets jpeg_set_marker_processor and jpeg_write_marker APIs which currently have 0% coverage:
 * - jpeg_set_marker_processor: 0% covered, 6 undiscovered branches
 * - jpeg_write_marker: 0% covered, 10 undiscovered branches
 * 
 * Target APIs from coverage guidance:
 * Primary: jpeg_set_marker_processor, jpeg_write_marker
 * Required Helpers: jpeg_write_m_header, jpeg_write_m_byte, jpeg_save_markers
 * Required Setup/Cleanup APIs: jpeg_CreateCompress, jpeg_CreateDecompress, jpeg_mem_dest, jpeg_mem_src,
 *                              jpeg_set_defaults, jpeg_start_compress, jpeg_start_decompress,
 *                              jpeg_finish_compress, jpeg_finish_decompress,
 *                              jpeg_destroy_compress, jpeg_destroy_decompress
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Create compression object with jpeg_CreateCompress
 * 2. Set up memory destination with jpeg_mem_dest
 * 3. Configure compression parameters with jpeg_set_defaults
 * 4. Start compression with jpeg_start_compress
 * 5. Write custom marker using jpeg_write_marker (with jpeg_write_m_header and jpeg_write_m_byte helpers)
 * 6. Write JPEG image data
 * 7. Finish compression with jpeg_finish_compress
 * 8. Create decompression object with jpeg_CreateDecompress
 * 9. Set up memory source with jpeg_mem_src using the compressed data
 * 10. Set custom marker processor with jpeg_set_marker_processor for the same marker code
 * 11. Configure decompression parameters
 * 12. Start decompression with jpeg_start_decompress
 * 13. Read and process markers (custom processor will be invoked)
 * 14. Finish decompression with jpeg_finish_decompress
 * 15. Clean up with jpeg_destroy_compress and jpeg_destroy_decompress
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: TurboJPEG compression/decompression
 * - harness_001.cpp: 12-bit JPEG handling
 * - harness_002.cpp: JPEG compression and transformations
 * - harness_003.cpp: ICC profile handling
 * - harness_004.cpp: Memory management APIs
 * - harness_005.cpp: Error handling functions
 * - harness_006.cpp: DCT coefficient manipulation
 * - harness_007.cpp: Color space conversions
 * - harness_008.cpp: Sampling and scaling
 * - harness_009.cpp: Progressive JPEG
 * - harness_010.cpp: Lossless JPEG
 * - harness_011.cpp: TurboJPEG scaling
 * - harness_012.cpp: JPEG buffered image mode
 * - harness_013.cpp: Arithmetic coding
 * - harness_014.cpp: TurboJPEG transformation operations
 * - This harness: Focuses exclusively on custom marker processing (write during compression, process during decompression)
 * 
 * Coverage strategy:
 * 1. Test various marker codes (APP0-APP15, COM)
 * 2. Test marker data of different lengths and contents
 * 3. Test both jpeg_write_marker single call and piecemeal jpeg_write_m_header/jpeg_write_m_byte
 * 4. Test custom marker processor callback functionality
 * 5. Test error handling for invalid marker codes and data
 * 
 * Expected Coverage Gain:
 * - Direct coverage of 2 critical marker processing APIs with 16+ undiscovered branches
 * - Exploration of marker-specific code paths in both compression and decompression
 * - Custom callback execution paths in decompression
 * - Round-trip testing of marker writing and reading
 */

// Include standard headers first as required by jpeglib.h
// Now include jpeglib.h (which depends on size_t and FILE being defined)



// Custom marker processor callback for jpeg_set_marker_processor
static boolean customMarkerProcessor(j_decompress_ptr cinfo) {
    // This callback is invoked when the specified marker is encountered during decompression
    // We can examine the marker data through cinfo->marker->cur_marker and cinfo->marker->cur_marker_length
    
    // Just return TRUE to indicate we processed the marker successfully
    // This exercises the custom callback path without complex logic
    return TRUE;
}

// Alternative marker processor that might return FALSE to skip marker
static boolean alternativeMarkerProcessor(j_decompress_ptr cinfo) {
    // Return FALSE to indicate we want to skip this marker
    // This exercises a different code path in the marker processing logic
    return FALSE;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for marker code, some marker data, and minimal image
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume fixed-size parameters from fuzzer input
    // Marker code: JPEG_APP0 (0xE0) through JPEG_APP0+15 (0xEF) or JPEG_COM (0xFE)
    int marker_code = fdp.ConsumeIntegralInRange<int>(0xE0, 0xEF); // APP0-APP15 range
    // Sometimes test COM marker (0xFE)
    if (fdp.ConsumeBool()) {
        marker_code = JPEG_COM; // 0xFE
    }
    
    // Marker data length (limited for fuzzing efficiency)
    unsigned int marker_data_len = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    
    // Image dimensions (small for fuzzing efficiency)
    int width = fdp.ConsumeIntegralInRange<int>(1, 64);
    int height = fdp.ConsumeIntegralInRange<int>(1, 64);
    
    // Color space
    int color_space_idx = fdp.ConsumeIntegralInRange<int>(0, 4);
    J_COLOR_SPACE color_space;
    switch (color_space_idx) {
        case 0: color_space = JCS_GRAYSCALE; break;
        case 1: color_space = JCS_RGB; break;
        case 2: color_space = JCS_YCbCr; break;
        case 3: color_space = JCS_CMYK; break;
        default: color_space = JCS_YCCK; break;
    }
    
    // Quality factor
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Step 2: Consume marker data
    std::vector<uint8_t> marker_data = fdp.ConsumeBytes<uint8_t>(marker_data_len);
    
    // Step 3: Consume image data (remaining bytes)
    // Calculate required image buffer size: width * height * components
    int components = (color_space == JCS_GRAYSCALE) ? 1 : 
                     (color_space == JCS_RGB || color_space == JCS_YCbCr) ? 3 : 4;
    size_t image_buffer_size = width * height * components;
    
    // Ensure we have enough data for the image
    if (fdp.remaining_bytes() < image_buffer_size) {
        return 0; // Not enough data for image
    }
    
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_buffer_size);
    
    // =============================================
    // COMPRESSION PATH: Write marker during compression
    // =============================================
    
    struct jpeg_compress_struct cinfo_compress;
    struct jpeg_error_mgr jerr_compress;
    
    // Initialize compression object with standard error handler
    cinfo_compress.err = jpeg_std_error(&jerr_compress);
    jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
    
    // Set up memory destination
    unsigned char* compressed_buffer = nullptr;
    unsigned long compressed_size = 0;
    jpeg_mem_dest(&cinfo_compress, &compressed_buffer, &compressed_size);
    
    // Set compression parameters
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = components;
    cinfo_compress.in_color_space = color_space;
    
    jpeg_set_defaults(&cinfo_compress);
    jpeg_set_quality(&cinfo_compress, quality, TRUE);
    
    // Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Write custom marker using jpeg_write_marker
    // Test both single call and piecemeal approaches based on fuzzer input
    if (fdp.ConsumeBool()) {
        // Method 1: Single jpeg_write_marker call
        jpeg_write_marker(&cinfo_compress, marker_code, marker_data.data(), marker_data_len);
    } else {
        // Method 2: Piecemeal jpeg_write_m_header and jpeg_write_m_byte
        jpeg_write_m_header(&cinfo_compress, marker_code, marker_data_len);
        for (size_t i = 0; i < marker_data_len; i++) {
            jpeg_write_m_byte(&cinfo_compress, marker_data[i]);
        }
    }
    
    // Write image scanlines (simplified - write all at once)
    JSAMPROW row_pointer[1];
    size_t row_stride = width * components;
    
    for (int row = 0; row < height; row++) {
        row_pointer[0] = &image_data[row * row_stride];
        jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    // =============================================
    // DECOMPRESSION PATH: Process marker during decompression
    // =============================================
    
    struct jpeg_decompress_struct cinfo_decompress;
    struct jpeg_error_mgr jerr_decompress;
    
    // Initialize decompression object with standard error handler
    cinfo_decompress.err = jpeg_std_error(&jerr_decompress);
    jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
    
    // Set up memory source with the compressed data
    jpeg_mem_src(&cinfo_decompress, compressed_buffer, compressed_size);
    
    // Save markers so they're available to our processor
    jpeg_save_markers(&cinfo_decompress, marker_code, 65535); // Large limit to save all data
    
    // Set custom marker processor
    // Test different processor functions based on fuzzer input
    if (fdp.ConsumeBool()) {
        jpeg_set_marker_processor(&cinfo_decompress, marker_code, customMarkerProcessor);
    } else {
        jpeg_set_marker_processor(&cinfo_decompress, marker_code, alternativeMarkerProcessor);
    }
    
    // Read JPEG header
    if (jpeg_read_header(&cinfo_decompress, TRUE) != JPEG_HEADER_OK) {
        // Clean up and return if header is invalid
        if (compressed_buffer) {
            free(compressed_buffer);
        }
        jpeg_destroy_compress(&cinfo_compress);
        jpeg_destroy_decompress(&cinfo_decompress);
        return 0;
    }
    
    // Start decompression (this will invoke our marker processor when marker is encountered)
    jpeg_start_decompress(&cinfo_decompress);
    
    // Read scanlines (simplified - just to exercise decompression path)
    std::vector<uint8_t> decompressed_buffer(cinfo_decompress.output_width * 
                                             cinfo_decompress.output_height * 
                                             cinfo_decompress.output_components);
    JSAMPROW decomp_row_pointer[1];
    
    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        decomp_row_pointer[0] = &decompressed_buffer[cinfo_decompress.output_scanline * 
                                                     cinfo_decompress.output_width * 
                                                     cinfo_decompress.output_components];
        jpeg_read_scanlines(&cinfo_decompress, decomp_row_pointer, 1);
    }
    
    // Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    // =============================================
    // CLEANUP
    // =============================================
    
    // Clean up memory buffer
    if (compressed_buffer) {
        free(compressed_buffer);
    }
    
    // Destroy compression and decompression objects
    jpeg_destroy_compress(&cinfo_compress);
    jpeg_destroy_decompress(&cinfo_decompress);
    
    return 0;
}

} // namespace harness_015
