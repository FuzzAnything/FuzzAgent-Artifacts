#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_008 {
/*
 * Fuzzing harness for libjpeg-turbo targeting 12-bit JPEG processing operations.
 * This harness specifically targets 12-bit JPEG compression and decompression
 * as identified by coverage analysis. All target 12-bit APIs are currently 0% covered.
 * 
 * Target APIs (0% covered, 160+ undiscovered branches):
 * - jpeg12_write_raw_data: 12-bit raw data compression (18 undiscovered branches)
 * - jpeg12_read_raw_data: 12-bit raw data decompression (16 undiscovered branches)
 * - jpeg12_write_scanlines: 12-bit scanline compression (20 undiscovered branches)
 * - jpeg12_read_scanlines: 12-bit scanline decompression (16 undiscovered branches)
 * - jpeg12_skip_scanlines: 12-bit scanline skipping (64 undiscovered branches)
 * - jpeg12_crop_scanline: 12-bit scanline cropping (40 undiscovered branches)
 * 
 * Required helper APIs:
 * - jpeg_CreateCompress/jpeg_CreateDecompress: Initialization
 * - jpeg_set_defaults: Configuration
 * - jpeg_set_colorspace: 12-bit color space setup
 * - jpeg_start_compress/jpeg_start_decompress: Start processing
 * - jpeg_finish_compress/jpeg_finish_decompress: Complete processing
 * - jpeg_destroy_compress/jpeg_destroy_decompress: Cleanup
 * 
 * Lifecycle: 
 * Compression: Create -> Configure -> Start -> Process (write_scanlines/write_raw_data) -> Finish -> Destroy
 * Decompression: Create -> Read header -> Start -> Process (read_scanlines/read_raw_data/skip/crop) -> Finish -> Destroy
 * 
 * Semantic diversity: This harness focuses specifically on 12-bit JPEG processing,
 * complementing existing harnesses that focus on 8-bit operations, file I/O, or
 * turbojpeg APIs. The 12-bit processing represents the largest contiguous block
 * of uncovered complexity in libjpeg-turbo.
 */


extern "C" {
}

/* Error manager structure for libjpeg */
struct fuzzer_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

/* Error exit handler */
static void fuzzer_error_exit(j_common_ptr cinfo) {
    struct fuzzer_error_mgr *fuzz_err = (struct fuzzer_error_mgr *)cinfo->err;
    longjmp(fuzz_err->setjmp_buffer, 1);
}

/* Suppress warning messages */
static void fuzzer_emit_message(j_common_ptr cinfo, int msg_level) {
    // Suppress all messages during fuzzing
}

/* Maximum dimensions to prevent memory exhaustion */
const int MAX_WIDTH_12BIT = 512;
const int MAX_HEIGHT_12BIT = 512;
const int MAX_COMPONENTS_12BIT = 4;

/* Helper to allocate 12-bit sample arrays */
static J12SAMPARRAY allocate_12bit_scanlines(j_common_ptr cinfo, int width, int height, int components) {
    J12SAMPARRAY scanlines = (J12SAMPARRAY)(*cinfo->mem->alloc_sarray)
        ((j_common_ptr)cinfo, JPOOL_IMAGE, width * components, height);
    return scanlines;
}

/* Helper to allocate 12-bit sample images (for raw data) */
static J12SAMPIMAGE allocate_12bit_image(j_common_ptr cinfo, int width, int height, int components) {
    // Allocate array of component pointers
    J12SAMPIMAGE image = (J12SAMPIMAGE)(*cinfo->mem->alloc_small)
        ((j_common_ptr)cinfo, JPOOL_IMAGE, components * sizeof(J12SAMPARRAY));
    
    // For each component, allocate 2D array of samples
    for (int ci = 0; ci < components; ci++) {
        image[ci] = (J12SAMPARRAY)(*cinfo->mem->alloc_sarray)
            ((j_common_ptr)cinfo, JPOOL_IMAGE, width, height);
    }
    
    return image;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    FuzzedDataProvider fdp(data, size);
    
    // Minimum size check - need enough data for basic parameters and image data
    if (size < 200) {
        return 0;
    }
    
    /**************************************************************************
     * PART 1: 12-BIT JPEG COMPRESSION - Test jpeg12_write_scanlines and jpeg12_write_raw_data
     **************************************************************************/
    {
        struct jpeg_compress_struct cinfo;
        struct fuzzer_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = fuzzer_error_exit;
        jerr.pub.emit_message = fuzzer_emit_message;
        
        jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
        
        if (setjmp(jerr.setjmp_buffer)) {
            jpeg_abort_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
            goto decompression_test;
        }
        // Consume compression parameters from fuzzer input
        int width = fdp.ConsumeIntegralInRange<int>(8, MAX_WIDTH_12BIT);
        int height = fdp.ConsumeIntegralInRange<int>(8, MAX_HEIGHT_12BIT);
        int components = fdp.ConsumeIntegralInRange<int>(1, MAX_COMPONENTS_12BIT);
        int in_color_space = fdp.ConsumeIntegralInRange<int>(0, 4); // JCS_UNKNOWN=0, JCS_GRAYSCALE=1, JCS_RGB=2, JCS_YCbCr=3, JCS_CMYK=4
        
        // Set image dimensions
        cinfo.image_width = width;
        cinfo.image_height = height;
        cinfo.input_components = components;
        cinfo.in_color_space = (J_COLOR_SPACE)in_color_space;
        cinfo.data_precision = 12;
        
        // Set defaults for 12-bit compression
        jpeg_set_defaults(&cinfo);
        
        // Set color space
        jpeg_set_colorspace(&cinfo, (J_COLOR_SPACE)in_color_space);
        
        // Consume quality and other parameters
        int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        jpeg_set_quality(&cinfo, quality, TRUE);
        
        // Choose between progressive and baseline
        if (fdp.ConsumeBool()) {
            jpeg_simple_progression(&cinfo);
        }
        
        // Use memory destination for compressed data
        unsigned char *compressed_buffer = NULL;
        unsigned long compressed_size = 0;
        jpeg_mem_dest(&cinfo, &compressed_buffer, &compressed_size);
        
        // Start compression
        jpeg_start_compress(&cinfo, TRUE);
        
        // Determine which 12-bit writing method to use based on fuzzer input
        int write_method = fdp.ConsumeIntegralInRange<int>(0, 1); // 0 = scanlines, 1 = raw data
        
        if (write_method == 0) {
            // Test jpeg12_write_scanlines
            J12SAMPARRAY scanlines = allocate_12bit_scanlines((j_common_ptr)&cinfo, width, height, components);
            
            // Fill scanlines with fuzzer data
            for (int row = 0; row < height; row++) {
                for (int col = 0; col < width * components; col++) {
                    // Consume 12-bit sample values (0-4095)
                    uint16_t sample = fdp.ConsumeIntegralInRange<uint16_t>(0, 4095);
                    scanlines[row][col] = sample;
                }
            }
            
            // Write scanlines using 12-bit API
            while (cinfo.next_scanline < cinfo.image_height) {
                J12SAMPARRAY row_pointer = &scanlines[cinfo.next_scanline];
                (void)jpeg12_write_scanlines(&cinfo, row_pointer, 1);
            }
        } else {
            // Test jpeg12_write_raw_data
            // For raw data, we need downsampled data arrays
            int max_v_samp_factor = 0;
            for (int ci = 0; ci < components; ci++) {
                if (cinfo.comp_info[ci].v_samp_factor > max_v_samp_factor) {
                    max_v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
                }
            }
            
            JDIMENSION num_lines = max_v_samp_factor * DCTSIZE;
            J12SAMPIMAGE raw_data = allocate_12bit_image((j_common_ptr)&cinfo, width, height, components);
            
            // Fill raw data with fuzzer data
            for (int ci = 0; ci < components; ci++) {
                for (int row = 0; row < height; row++) {
                    for (int col = 0; col < width; col++) {
                        uint16_t sample = fdp.ConsumeIntegralInRange<uint16_t>(0, 4095);
                        raw_data[ci][row][col] = sample;
                    }
                }
            }
            (void)jpeg12_write_raw_data(&cinfo, raw_data, num_lines);
        }
        
        // Finish compression
        jpeg_finish_compress(&cinfo);
        
        // Clean up compression buffer
        if (compressed_buffer != NULL) {
            free(compressed_buffer);
        }
        
        jpeg_destroy_compress(&cinfo);
    }

decompression_test:
    /**************************************************************************
     * PART 2: 12-BIT JPEG DECOMPRESSION - Test jpeg12_read_scanlines, jpeg12_read_raw_data,
     *          jpeg12_skip_scanlines, and jpeg12_crop_scanline
     **************************************************************************/
    {
        // First, create a simple 12-bit JPEG in memory to decompress
        // We'll reuse some of the compression code to create test data
        struct jpeg_compress_struct comp_cinfo;
        struct fuzzer_error_mgr comp_jerr;
        unsigned char *test_jpeg_data = NULL;
        unsigned long test_jpeg_size = 0;
        
        comp_cinfo.err = jpeg_std_error(&comp_jerr.pub);
        comp_jerr.pub.error_exit = fuzzer_error_exit;
        comp_jerr.pub.emit_message = fuzzer_emit_message;
        
        jpeg_CreateCompress(&comp_cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
        
        if (setjmp(comp_jerr.setjmp_buffer)) {
            jpeg_abort_compress(&comp_cinfo);
            jpeg_destroy_compress(&comp_cinfo);
            return 0;
        }
        
        // Create a simple test JPEG
        comp_cinfo.image_width = 64;
        comp_cinfo.image_height = 64;
        comp_cinfo.input_components = 3;
        comp_cinfo.in_color_space = JCS_RGB;
        comp_cinfo.data_precision = 12;
        
        jpeg_set_defaults(&comp_cinfo);
        jpeg_set_colorspace(&comp_cinfo, JCS_RGB);
        jpeg_set_quality(&comp_cinfo, 75, TRUE);
        
        jpeg_mem_dest(&comp_cinfo, &test_jpeg_data, &test_jpeg_size);
        jpeg_start_compress(&comp_cinfo, TRUE);
        
        // Create simple test data
        J12SAMPARRAY test_scanlines = allocate_12bit_scanlines((j_common_ptr)&comp_cinfo, 64, 64, 3);
        for (int row = 0; row < 64; row++) {
            for (int col = 0; col < 64 * 3; col++) {
                test_scanlines[row][col] = (row * col) % 4096;
            }
        }
        
        while (comp_cinfo.next_scanline < comp_cinfo.image_height) {
            J12SAMPARRAY row_pointer = &test_scanlines[comp_cinfo.next_scanline];
            (void)jpeg12_write_scanlines(&comp_cinfo, row_pointer, 1);
        }
        
        jpeg_finish_compress(&comp_cinfo);
        jpeg_destroy_compress(&comp_cinfo);
        
        if (test_jpeg_data == NULL || test_jpeg_size == 0) {
            if (test_jpeg_data != NULL) free(test_jpeg_data);
            return 0;
        }
        
        // Now decompress the test JPEG using 12-bit APIs
        struct jpeg_decompress_struct cinfo;
        struct fuzzer_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = fuzzer_error_exit;
        jerr.pub.emit_message = fuzzer_emit_message;
        
        jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
        
        if (setjmp(jerr.setjmp_buffer)) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            free(test_jpeg_data);
            return 0;
        }
        
        // Use memory source with our test JPEG data
        jpeg_mem_src(&cinfo, test_jpeg_data, test_jpeg_size);
        
        // Read header
        int header_status = jpeg_read_header(&cinfo, TRUE);
        if (header_status != JPEG_HEADER_OK) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            free(test_jpeg_data);
            return 0;
        }
        
        // Verify this is 12-bit data
        if (cinfo.data_precision != 12) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            free(test_jpeg_data);
            return 0;
        }
        
        // Consume decompression operation from fuzzer input
        int decomp_op = fdp.ConsumeIntegralInRange<int>(0, 3); // 0 = read_scanlines, 1 = read_raw_data, 2 = skip_scanlines, 3 = crop_scanline
        
        // Start decompression
        if (!jpeg_start_decompress(&cinfo)) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            free(test_jpeg_data);
            return 0;
        }
        
        // Allocate output buffer for 12-bit data
        int row_stride = cinfo.output_width * cinfo.output_components;
        J12SAMPARRAY buffer12 = (J12SAMPARRAY)(*cinfo.mem->alloc_sarray)
            ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
        
        // Perform the selected 12-bit decompression operation
        switch (decomp_op) {
            case 0: // Test jpeg12_read_scanlines
                while (cinfo.output_scanline < cinfo.output_height) {
                    (void)jpeg12_read_scanlines(&cinfo, buffer12, 1);
                }
                break;
                
            case 1: // Test jpeg12_read_raw_data
                {
                    int max_v_samp_factor = 0;
                    for (int ci = 0; ci < cinfo.num_components; ci++) {
                        if (cinfo.comp_info[ci].v_samp_factor > max_v_samp_factor) {
                            max_v_samp_factor = cinfo.comp_info[ci].v_samp_factor;
                        }
                    }
                    
                    JDIMENSION num_lines = max_v_samp_factor * DCTSIZE;
                    J12SAMPIMAGE raw_buffer = allocate_12bit_image((j_common_ptr)&cinfo, 
                        cinfo.output_width, cinfo.output_height, cinfo.output_components);
                    
                    (void)jpeg12_read_raw_data(&cinfo, raw_buffer, num_lines);
                }
                break;
                
            case 2: // Test jpeg12_skip_scanlines
                {
                    // Skip some scanlines based on fuzzer input
                    JDIMENSION skip_count = fdp.ConsumeIntegralInRange<JDIMENSION>(0, cinfo.output_height / 2);
                    (void)jpeg12_skip_scanlines(&cinfo, skip_count);
                    
                    // Read remaining scanlines
                    while (cinfo.output_scanline < cinfo.output_height) {
                        (void)jpeg12_read_scanlines(&cinfo, buffer12, 1);
                    }
                }
                break;
                
            case 3: // Test jpeg12_crop_scanline
                {
                    // Crop based on fuzzer input
                    JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, cinfo.output_width / 2);
                    JDIMENSION width = fdp.ConsumeIntegralInRange<JDIMENSION>(1, cinfo.output_width - xoffset);
                    
                    jpeg12_crop_scanline(&cinfo, &xoffset, &width);
                    
                    // Read scanlines with cropping applied
                    while (cinfo.output_scanline < cinfo.output_height) {
                        (void)jpeg12_read_scanlines(&cinfo, buffer12, 1);
                    }
                }
                break;
        }
        
        // Finish decompression
        (void)jpeg_finish_decompress(&cinfo);
        
        // Cleanup
        jpeg_destroy_decompress(&cinfo);
        free(test_jpeg_data);
    }
    
    return 0;
}

} // namespace harness_008
