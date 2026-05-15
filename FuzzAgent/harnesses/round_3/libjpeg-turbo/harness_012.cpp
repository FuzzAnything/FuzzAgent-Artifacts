#include <fuzzer/FuzzedDataProvider.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_012 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit lossless JPEG operations
 * This harness targets completely uncovered 12-bit lossless JPEG processing:
 * - jpeg12_write_raw_data: 12-bit raw data writing (0% coverage, 18 undiscovered branches)
 * - jpeg12_read_raw_data: 12-bit raw data reading (0% coverage, 16 undiscovered branches)
 * - jpeg12_skip_scanlines: 12-bit scanline skipping (0% coverage, 71 undiscovered branches)
 * - jpeg12_crop_scanline: 12-bit scanline cropping (0% coverage, 40 undiscovered branches)
 * - jpeg_new_colormap: Color mapping for 12-bit images (0% coverage, 8 undiscovered branches)
 * - jpeg_enable_lossless: Lossless compression enabling (required helper)
 * 
 * Differentiates from existing harnesses (000-011):
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs
 * - harness_001.cpp: Traditional tj* TurboJPEG APIs  
 * - harness_002.cpp: Modern tj3* YUV APIs
 * - harness_003.cpp: TurboJPEG 12/16-bit precision APIs (tj3Compress12/tj3Decompress12)
 * - harness_004.cpp: Traditional tj* YUV operations
 * - harness_005.cpp: High-bit-depth image I/O (not 12-bit specific)
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg I/O APIs (8-bit)
 * - harness_008.cpp: Traditional libjpeg 12-bit APIs (non-lossless)
 * - harness_009.cpp: Old turbojpeg YUV decompression APIs
 * - harness_010.cpp: Traditional file-based I/O APIs
 * - harness_011.cpp: JPEG error recovery and marker handling APIs
 * 
 * Strategy: Implement full traditional libjpeg 12-bit lossless compression/decompression
 * with jpeg_enable_lossless, raw data operations, scanline manipulation, and color mapping
 * to exercise 12-bit lossless modules currently at 0% coverage (153+ undiscovered branches).
 * 
 * Expected Coverage Impact:
 * - Cover 5+ currently uncovered public APIs with lossless-specific paths
 * - Exercise 150+ undiscovered code branches in lossless 12-bit processing
 * - Test 12-bit precision with lossless predictor algorithms
 * - Provide semantic diversity from existing 8-bit and non-lossless 12-bit harnesses
 */


// Include traditional libjpeg headers for 12-bit operations
extern "C" {
}

// Helper to allocate 12-bit sample arrays for lossless operations
static J12SAMPARRAY allocate_12bit_samples_lossless(int width, int height, int components) {
    if (width <= 0 || height <= 0 || components <= 0) return nullptr;
    
    J12SAMPARRAY image = (J12SAMPARRAY)malloc(sizeof(J12SAMPROW) * height);
    if (!image) return nullptr;
    
    for (int row = 0; row < height; row++) {
        image[row] = (J12SAMPROW)malloc(sizeof(J12SAMPLE) * width * components);
        if (!image[row]) {
            // Cleanup on failure
            for (int i = 0; i < row; i++) {
                free(image[i]);
            }
            free(image);
            return nullptr;
        }
    }
    return image;
}

// Helper to free 12-bit sample arrays
static void free_12bit_samples_lossless(J12SAMPARRAY image, int height) {
    if (!image) return;
    
    for (int row = 0; row < height; row++) {
        free(image[row]);
    }
    free(image);
}

// Helper to allocate 12-bit sample images (3D arrays for raw data in lossless mode)
static J12SAMPIMAGE allocate_12bit_image_lossless(int width, int height, int components) {
    if (width <= 0 || height <= 0 || components <= 0) return nullptr;
    
    J12SAMPIMAGE image = (J12SAMPIMAGE)malloc(sizeof(J12SAMPARRAY) * components);
    if (!image) return nullptr;
    
    for (int comp = 0; comp < components; comp++) {
        image[comp] = allocate_12bit_samples_lossless(width, height, 1); // 1 sample per component
        if (!image[comp]) {
            // Cleanup on failure
            for (int i = 0; i < comp; i++) {
                free_12bit_samples_lossless(image[i], height);
            }
            free(image);
            return nullptr;
        }
    }
    return image;
}

// Helper to free 12-bit sample images
static void free_12bit_image_lossless(J12SAMPIMAGE image, int height, int components) {
    if (!image) return;
    
    for (int comp = 0; comp < components; comp++) {
        free_12bit_samples_lossless(image[comp], height);
    }
    free(image);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for 12-bit lossless operations
    if (size < 512) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: 12-BIT LOSSLESS COMPRESSION ==========
    
    struct jpeg_compress_struct cinfo_compress;
    struct jpeg_error_mgr jerr_compress;
    
    // Initialize 12-bit compression object
    cinfo_compress.err = jpeg_std_error(&jerr_compress);
    jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
    
    // Consume compression parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 64);
    int height = fdp.ConsumeIntegralInRange<int>(8, 64);
    int components = fdp.ConsumeIntegralInRange<int>(1, 4);
    int predictor = fdp.ConsumeIntegralInRange<int>(1, 7);  // Predictor selection (1-7)
    int point_transform = fdp.ConsumeIntegralInRange<int>(0, 11);  // Point transform for 12-bit (0-11)
    boolean use_colormap = fdp.ConsumeBool();
    
    // Set image dimensions for 12-bit lossless
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = components;
    
    // Choose color space based on components
    J_COLOR_SPACE color_space;
    if (components == 1) {
        color_space = JCS_GRAYSCALE;
    } else if (components == 3) {
        int color_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
        J_COLOR_SPACE color_spaces[] = {JCS_RGB, JCS_YCbCr, JCS_CMYK};
        color_space = color_spaces[color_choice % 3];
    } else {
        color_space = JCS_UNKNOWN;
    }
    cinfo_compress.in_color_space = color_space;
    
    // Set default parameters
    jpeg_set_defaults(&cinfo_compress);
    
    // Enable 12-bit lossless mode - CRITICAL TARGET API
    jpeg_enable_lossless(&cinfo_compress, predictor, point_transform);
    
    // Set data precision to 12-bit for lossless
    cinfo_compress.data_precision = 12;
    
    // Setup memory destination for compressed data
    unsigned char* jpeg_buffer = nullptr;
    unsigned long jpeg_size = 0;
    jpeg_mem_dest(&cinfo_compress, &jpeg_buffer, &jpeg_size);
    
    // Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Allocate 12-bit sample arrays for compression
    J12SAMPARRAY scanlines = allocate_12bit_samples_lossless(width, height, components);
    if (!scanlines) {
        jpeg_destroy_compress(&cinfo_compress);
        if (jpeg_buffer) free(jpeg_buffer);
        return 0;
    }
    
    // Fill 12-bit samples with fuzzer data (12-bit values 0-4095)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width * components; col++) {
            // Generate 12-bit values from fuzzer input
            short sample = fdp.ConsumeIntegralInRange<short>(0, 4095); // 12-bit range
            scanlines[row][col] = sample;
        }
    }
    
    // Test jpeg12_write_scanlines for lossless - write 12-bit scanlines
    JDIMENSION lines_written = 0;
    while (lines_written < cinfo_compress.image_height) {
        JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 
            cinfo_compress.image_height - lines_written);
        JDIMENSION written = jpeg12_write_scanlines(&cinfo_compress, 
            &scanlines[lines_written], num_lines);
        lines_written += written;
        if (written == 0) break; // Error or buffer full
    }
    
    // Test jpeg12_write_raw_data for lossless if we have remaining fuzzer data
    if (fdp.remaining_bytes() > 128 && cinfo_compress.raw_data_in) {
        // Allocate 12-bit image for raw data operations
        J12SAMPIMAGE raw_image = allocate_12bit_image_lossless(width, height, components);
        if (raw_image) {
            // Fill raw image with fuzzer data
            for (int comp = 0; comp < components; comp++) {
                for (int row = 0; row < height; row++) {
                    for (int col = 0; col < width; col++) {
                        raw_image[comp][row][col] = fdp.ConsumeIntegralInRange<short>(0, 4095);
                    }
                }
            }
            
            // Test jpeg12_write_raw_data - CRITICAL TARGET API
            jpeg12_write_raw_data(&cinfo_compress, raw_image, height);
            
            free_12bit_image_lossless(raw_image, height, components);
        }
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    // Cleanup compression resources
    free_12bit_samples_lossless(scanlines, height);
    jpeg_destroy_compress(&cinfo_compress);
    
    // ========== PHASE 2: 12-BIT LOSSLESS DECOMPRESSION ==========
    
    if (!jpeg_buffer || jpeg_size == 0) {
        if (jpeg_buffer) free(jpeg_buffer);
        return 0;
    }
    
    struct jpeg_decompress_struct cinfo_decompress;
    struct jpeg_error_mgr jerr_decompress;
    
    // Initialize 12-bit decompression object
    cinfo_decompress.err = jpeg_std_error(&jerr_decompress);
    jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
    
    // Setup memory source with compressed data
    jpeg_mem_src(&cinfo_decompress, jpeg_buffer, jpeg_size);
    
    // Read JPEG header
    if (jpeg_read_header(&cinfo_decompress, TRUE) != JPEG_HEADER_OK) {
        jpeg_destroy_decompress(&cinfo_decompress);
        free(jpeg_buffer);
        return 0;
    }
    
    // Test jpeg_new_colormap if requested and applicable - CRITICAL TARGET API
    if (use_colormap && cinfo_decompress.out_color_space == JCS_GRAYSCALE) {
        jpeg_new_colormap(&cinfo_decompress);
    }
    
    // Start decompression
    jpeg_start_decompress(&cinfo_decompress);
    
    // Allocate 12-bit sample arrays for decompression
    int out_width = cinfo_decompress.output_width;
    int out_height = cinfo_decompress.output_height;
    int out_components = cinfo_decompress.output_components;
    
    J12SAMPARRAY output_scanlines = allocate_12bit_samples_lossless(out_width, out_height, out_components);
    if (!output_scanlines) {
        jpeg_destroy_decompress(&cinfo_decompress);
        free(jpeg_buffer);
        return 0;
    }
    
    // Test jpeg12_read_scanlines for lossless - read 12-bit scanlines
    JDIMENSION lines_read = 0;
    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        JDIMENSION max_lines = cinfo_decompress.output_height - cinfo_decompress.output_scanline;
        JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, max_lines);
        JDIMENSION read = jpeg12_read_scanlines(&cinfo_decompress, 
            &output_scanlines[lines_read], num_lines);
        lines_read += read;
        if (read == 0) break; // Error or EOF
    }
    
    // Test jpeg12_skip_scanlines if we have remaining scanlines - CRITICAL TARGET API
    if (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        JDIMENSION remaining = cinfo_decompress.output_height - cinfo_decompress.output_scanline;
        JDIMENSION skip_count = fdp.ConsumeIntegralInRange<JDIMENSION>(1, remaining);
        jpeg12_skip_scanlines(&cinfo_decompress, skip_count);
    }
    
    // Test jpeg12_crop_scanline if we have valid dimensions - CRITICAL TARGET API
    if (out_width > 4 && out_height > 0) {
        JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, out_width - 4);
        JDIMENSION cropped_width = fdp.ConsumeIntegralInRange<JDIMENSION>(4, out_width - xoffset);
        jpeg12_crop_scanline(&cinfo_decompress, &xoffset, &cropped_width);
    }
    
    // Test jpeg12_read_raw_data for lossless if in raw data mode - CRITICAL TARGET API
    if (cinfo_decompress.raw_data_out && fdp.remaining_bytes() > 64) {
        J12SAMPIMAGE raw_output = allocate_12bit_image_lossless(out_width, out_height, out_components);
        if (raw_output) {
            JDIMENSION max_lines = cinfo_decompress.output_height;
            jpeg12_read_raw_data(&cinfo_decompress, raw_output, max_lines);
            free_12bit_image_lossless(raw_output, out_height, out_components);
        }
    }
    
    // Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    // Cleanup decompression resources
    free_12bit_samples_lossless(output_scanlines, out_height);
    jpeg_destroy_decompress(&cinfo_decompress);
    free(jpeg_buffer);
    
    return 0;
}

} // namespace harness_012
