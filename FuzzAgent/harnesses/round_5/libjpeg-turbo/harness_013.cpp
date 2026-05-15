#include <stdio.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "jerror.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>

namespace harness_013 {
/*
 * Fuzzing harness for libjpeg-turbo targeting ICC profile read/write operations
 * This harness specifically targets classic libjpeg ICC Profile APIs with high coverage impact:
 * - jpeg_write_icc_profile: 0% coverage, 22 undiscovered branches (completely uncovered)
 * - jpeg_read_icc_profile: 18% coverage, 31 undiscovered branches
 * 
 * Target ICC Profile APIs:
 * 1. jpeg_write_icc_profile - Write ICC color profile to JPEG image during compression
 * 2. jpeg_read_icc_profile - Read ICC color profile from JPEG image during decompression
 * 
 * Required Helper APIs for lifecycle:
 * - jpeg_CreateCompress: Create compression object
 * - jpeg_mem_dest: Set up memory destination for compressed data
 * - jpeg_set_defaults: Set default compression parameters
 * - jpeg_start_compress: Begin compression process
 * - jpeg_save_markers: Enable APP2 marker saving for ICC profiles
 * - jpeg_write_m_header: Write marker header (0% coverage)
 * - jpeg_write_m_byte: Write marker data byte by byte
 * - jpeg_finish_compress: Complete compression
 * - jpeg_destroy_compress: Cleanup compression object
 * - jpeg_CreateDecompress: Create decompression object
 * - jpeg_mem_src: Set up memory source with compressed data
 * - jpeg_read_header: Read JPEG header
 * - jpeg_start_decompress: Begin decompression
 * - jpeg_finish_decompress: Complete decompression
 * - jpeg_destroy_decompress: Cleanup decompression object
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Compression path with ICC profile embedding
 * 2. Decompression path with ICC profile reading
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: TurboJPEG RGB compression/decompression with legacy APIs
 * - harness_001.cpp: TurboJPEG YUV decompression APIs
 * - harness_002.cpp: TurboJPEG transformation operations
 * - harness_003.cpp: TurboJPEG 12/16-bit precision compression/decompression
 * - harness_004.cpp: TurboJPEG image file I/O operations
 * - harness_011.cpp: 12-bit JPEG operations with classic libjpeg API
 * - harness_012.cpp: TurboJPEG ICC profile APIs (tj3SetICCProfile, tj3GetICCProfile)
 * - This harness: Focuses EXCLUSIVELY on classic libjpeg ICC Profile APIs (jpeg_write_icc_profile, jpeg_read_icc_profile)
 * 
 * Coverage strategy:
 * 1. Complete ICC profile write/read lifecycle
 * 2. Multiple marker handling for large ICC profiles (>64K)
 * 3. Error handling paths for invalid ICC data
 * 4. Marker processing paths (jpeg_write_m_header, jpeg_write_m_byte)
 * 5. Memory management with jpeg_mem_dest/jpeg_mem_src
 * 
 * Expected Coverage Gain:
 * - Direct coverage of 2 critical ICC profile APIs
 * - 53+ undiscovered branches (22 from write + 31 from read)
 * - Exploration of JPEG marker processing path distinct from pixel data
 */


/* Error handling structure for libjpeg */
struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

METHODDEF(void) my_error_exit(j_common_ptr cinfo) {
    my_error_mgr* myerr = (my_error_mgr*)cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic parameters + ICC profile data
    // Need at least: width (4) + height (4) + ICC profile size (4) + some ICC data (100)
    if (size < 112) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    J_COLOR_SPACE in_color_space = (J_COLOR_SPACE)fdp.ConsumeIntegralInRange<int>(JCS_UNKNOWN, JCS_YCCK);
    int num_components = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Consume ICC profile size - limited to reasonable size for fuzzing
    size_t icc_profile_size = fdp.ConsumeIntegralInRange<size_t>(1, 65535 * 2); // Up to 2 markers worth
    
    // Validate color space
    if (in_color_space < JCS_UNKNOWN || in_color_space > JCS_YCCK) {
        in_color_space = JCS_RGB;
    }
    
    // Consume ICC profile data from fuzzer input
    std::vector<uint8_t> icc_profile_data = fdp.ConsumeBytes<uint8_t>(icc_profile_size);
    if (icc_profile_data.empty()) {
        return 0; // Not enough data for ICC profile
    }
    
    // Set up error handling
    my_error_mgr jerr;
    jpeg_compress_struct cinfo_compress;
    jpeg_decompress_struct cinfo_decompress;
    
    cinfo_compress.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    
    cinfo_decompress.err = jpeg_std_error(&jerr.pub);
    
    if (setjmp(jerr.setjmp_buffer)) {
        // Error handling: clean up and return
        jpeg_destroy_compress(&cinfo_compress);
        jpeg_destroy_decompress(&cinfo_decompress);
        return 0;
    }
    
    /*** COMPRESSION PATH WITH ICC PROFILE ***/
    
    // Step 1: Create compression object
    jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(jpeg_compress_struct));
    
    // Step 2: Set up memory destination
    unsigned char* jpeg_buffer = nullptr;
    unsigned long jpeg_size = 0;
    jpeg_mem_dest(&cinfo_compress, &jpeg_buffer, &jpeg_size);
    
    // Step 3: Set default parameters
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = num_components;
    cinfo_compress.in_color_space = in_color_space;
    
    jpeg_set_defaults(&cinfo_compress);
    jpeg_set_quality(&cinfo_compress, quality, TRUE);
    
    // Step 4: Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Step 5: Enable ICC marker saving (for decompression path)
    // Note: jpeg_save_markers is for decompression, but we call it here for completeness
    // in a real scenario, we would call it on the decompress object
    
    // Step 6-8: Write ICC profile using jpeg_write_icc_profile
    // This internally calls jpeg_write_m_header and jpeg_write_m_byte
    jpeg_write_icc_profile(&cinfo_compress, icc_profile_data.data(), icc_profile_data.size());
    
    // Step 9: Write dummy scanlines (ICC profile must be written before scanlines)
    // Create a dummy scanline buffer
    size_t row_stride = width * num_components;
    std::vector<JSAMPLE> scanline(row_stride);
    JSAMPROW row_pointer[1];
    row_pointer[0] = scanline.data();
    
    // Fill with some data from fuzzer input
    std::vector<uint8_t> pixel_data = fdp.ConsumeBytes<uint8_t>(row_stride * height);
    if (pixel_data.size() < row_stride * height) {
        // Not enough pixel data, fill with zeros
        memset(scanline.data(), 0, row_stride);
    } else {
        // Use fuzzer data for first scanline
        memcpy(scanline.data(), pixel_data.data(), row_stride);
    }
    
    // Write scanlines
    while (cinfo_compress.next_scanline < cinfo_compress.image_height) {
        if (pixel_data.size() >= row_stride * (cinfo_compress.next_scanline + 1)) {
            // Use fuzzer data for this scanline
            memcpy(scanline.data(), pixel_data.data() + row_stride * cinfo_compress.next_scanline, row_stride);
        }
        jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
    }
    
    // Step 10: Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    /*** DECOMPRESSION PATH WITH ICC PROFILE READING ***/
    
    // Step 11: Create decompression object
    jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(jpeg_decompress_struct));
    
    // Step 12: Set up memory source with compressed JPEG data
    jpeg_mem_src(&cinfo_decompress, jpeg_buffer, jpeg_size);
    
    // Step 13: Read JPEG header
    int header_result = jpeg_read_header(&cinfo_decompress, TRUE);
    if (header_result != JPEG_HEADER_OK) {
        // Clean up and return
        free(jpeg_buffer);
        jpeg_destroy_compress(&cinfo_compress);
        jpeg_destroy_decompress(&cinfo_decompress);
        return 0;
    }
    
    // Step 14: Enable ICC marker saving for reading
    jpeg_save_markers(&cinfo_decompress, JPEG_APP0 + 2, 0xFFFF); // APP2 marker for ICC profiles
    
    // Step 15: Start decompression
    jpeg_start_decompress(&cinfo_decompress);
    
    // Step 16: Read ICC profile
    JOCTET* retrieved_icc_data = nullptr;
    unsigned int retrieved_icc_size = 0;
    
    boolean icc_found = jpeg_read_icc_profile(&cinfo_decompress, &retrieved_icc_data, &retrieved_icc_size);
    
    if (icc_found && retrieved_icc_data) {
        // ICC profile was found and read successfully
        // We could compare with original, but for fuzzing we just exercise the code
        free(retrieved_icc_data);
    }
    
    // Step 17: Read dummy scanlines (just to exercise decompression path)
    std::vector<JSAMPLE> output_scanline(cinfo_decompress.output_width * cinfo_decompress.output_components);
    JSAMPROW output_row[1];
    output_row[0] = output_scanline.data();
    
    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        jpeg_read_scanlines(&cinfo_decompress, output_row, 1);
    }
    
    // Step 18: Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    /*** CLEANUP ***/
    
    // Step 19 & 20: Destroy objects
    jpeg_destroy_compress(&cinfo_compress);
    jpeg_destroy_decompress(&cinfo_decompress);
    
    // Free memory buffer
    free(jpeg_buffer);
    
    return 0;
}

} // namespace harness_013
