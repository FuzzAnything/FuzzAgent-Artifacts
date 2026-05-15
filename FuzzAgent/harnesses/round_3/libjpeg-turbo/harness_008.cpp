#include <fuzzer/FuzzedDataProvider.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_008 {
/*
 * Fuzzing harness for libjpeg-turbo traditional libjpeg 12-bit precision APIs
 * This harness targets completely uncovered traditional libjpeg 12-bit image processing:
 * - jpeg12_write_raw_data, jpeg12_read_raw_data: 12-bit raw data I/O (0% coverage, 34 undiscovered branches)
 * - jpeg12_skip_scanlines, jpeg12_crop_scanline: 12-bit scanline manipulation (0% coverage, 111 undiscovered branches)
 * - jpeg12_write_scanlines, jpeg12_read_scanlines: 12-bit scanline I/O (partially covered, 15 undiscovered branches)
 * - jpeg_new_colormap: Color mapping for 12-bit images (0% coverage, 8 undiscovered branches)
 * 
 * Differentiates from existing harnesses (000-007):
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs
 * - harness_001.cpp: Traditional tj* TurboJPEG APIs  
 * - harness_002.cpp: Modern tj3* YUV APIs
 * - harness_003.cpp: TurboJPEG 12/16-bit precision APIs (tj3Compress12/tj3Decompress12)
 * - harness_004.cpp: Traditional tj* YUV operations
 * - harness_005.cpp: High-bit-depth image I/O (not 12-bit specific)
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg I/O APIs (8-bit)
 * 
 * Strategy: Implement full traditional libjpeg 12-bit compression/decompression lifecycle
 * with raw data operations, scanline manipulation, and color mapping to exercise
 * 12-bit precision modules currently at 0% coverage (145+ undiscovered branches).
 */


// Include traditional libjpeg headers for 12-bit operations
extern "C" {
}

// Helper to allocate 12-bit sample arrays
static J12SAMPARRAY allocate_12bit_samples(int width, int height, int components) {
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
static void free_12bit_samples(J12SAMPARRAY image, int height) {
    if (!image) return;
    
    for (int row = 0; row < height; row++) {
        free(image[row]);
    }
    free(image);
}

// Helper to allocate 12-bit sample images (3D arrays for raw data)
static J12SAMPIMAGE allocate_12bit_image(int width, int height, int components) {
    if (width <= 0 || height <= 0 || components <= 0) return nullptr;
    
    J12SAMPIMAGE image = (J12SAMPIMAGE)malloc(sizeof(J12SAMPARRAY) * components);
    if (!image) return nullptr;
    
    for (int comp = 0; comp < components; comp++) {
        image[comp] = allocate_12bit_samples(width, height, 1); // 1 sample per component
        if (!image[comp]) {
            // Cleanup on failure
            for (int i = 0; i < comp; i++) {
                free_12bit_samples(image[i], height);
            }
            free(image);
            return nullptr;
        }
    }
    return image;
}

// Helper to free 12-bit sample images
static void free_12bit_image(J12SAMPIMAGE image, int height, int components) {
    if (!image) return;
    
    for (int comp = 0; comp < components; comp++) {
        free_12bit_samples(image[comp], height);
    }
    free(image);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for 12-bit operations
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: 12-BIT COMPRESSION ==========
    
    struct jpeg_compress_struct cinfo_compress;
    struct jpeg_error_mgr jerr_compress;
    
    // Initialize 12-bit compression object
    cinfo_compress.err = jpeg_std_error(&jerr_compress);
    jpeg_create_compress(&cinfo_compress);
    
    // Consume compression parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    boolean progressive = fdp.ConsumeBool();
    boolean use_colormap = fdp.ConsumeBool();
    int color_space_idx = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Set image dimensions for 12-bit
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = 3; // RGB
    cinfo_compress.in_color_space = JCS_RGB;
    
    // Set default parameters
    jpeg_set_defaults(&cinfo_compress);
    
    // Set quality
    jpeg_set_quality(&cinfo_compress, quality, TRUE /* force_baseline */);
    
    // Set color space based on fuzzer input
    J_COLOR_SPACE color_spaces[] = {JCS_RGB, JCS_GRAYSCALE, JCS_YCbCr};
    cinfo_compress.in_color_space = color_spaces[color_space_idx % 3];
    
    // Adjust input components based on color space
    if (cinfo_compress.in_color_space == JCS_GRAYSCALE) {
        cinfo_compress.input_components = 1;
    }
    
    // Set progressive mode if requested
    if (progressive) {
        jpeg_simple_progression(&cinfo_compress);
    }
    
    // Setup memory destination for compressed data
    unsigned char* jpeg_buffer = nullptr;
    unsigned long jpeg_size = 0;
    jpeg_mem_dest(&cinfo_compress, &jpeg_buffer, &jpeg_size);
    
    // Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Allocate 12-bit sample arrays for compression
    J12SAMPARRAY scanlines = allocate_12bit_samples(width, height, cinfo_compress.input_components);
    if (!scanlines) {
        jpeg_destroy_compress(&cinfo_compress);
        if (jpeg_buffer) free(jpeg_buffer);
        return 0;
    }
    
    // Fill 12-bit samples with fuzzer data (12-bit values 0-4095)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width * cinfo_compress.input_components; col++) {
            // Generate 12-bit values from fuzzer input
            short sample = fdp.ConsumeIntegralInRange<short>(0, 4095); // 12-bit range
            scanlines[row][col] = sample;
        }
    }
    
    // Test jpeg12_write_scanlines - write 12-bit scanlines
    JDIMENSION lines_written = 0;
    while (lines_written < cinfo_compress.image_height) {
        JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 
            cinfo_compress.image_height - lines_written);
        JDIMENSION written = jpeg12_write_scanlines(&cinfo_compress, 
            &scanlines[lines_written], num_lines);
        lines_written += written;
        if (written == 0) break; // Error or buffer full
    }
    
    // If we have remaining fuzzer data, test raw data operations
    if (fdp.remaining_bytes() > 64 && cinfo_compress.raw_data_in) {
        // Allocate 12-bit image for raw data operations
        int components = cinfo_compress.num_components;
        J12SAMPIMAGE raw_image = allocate_12bit_image(width, height, components);
        if (raw_image) {
            // Fill raw image with fuzzer data
            for (int comp = 0; comp < components; comp++) {
                for (int row = 0; row < height; row++) {
                    for (int col = 0; col < width; col++) {
                        raw_image[comp][row][col] = fdp.ConsumeIntegralInRange<short>(0, 4095);
                    }
                }
            }
            
            // Test jpeg12_write_raw_data
            jpeg12_write_raw_data(&cinfo_compress, raw_image, height);
            
            free_12bit_image(raw_image, height, components);
        }
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    // Cleanup compression resources
    free_12bit_samples(scanlines, height);
    jpeg_destroy_compress(&cinfo_compress);
    
    // ========== PHASE 2: 12-BIT DECOMPRESSION ==========
    
    if (!jpeg_buffer || jpeg_size == 0) {
        if (jpeg_buffer) free(jpeg_buffer);
        return 0;
    }
    
    struct jpeg_decompress_struct cinfo_decompress;
    struct jpeg_error_mgr jerr_decompress;
    
    // Initialize 12-bit decompression object
    cinfo_decompress.err = jpeg_std_error(&jerr_decompress);
    jpeg_create_decompress(&cinfo_decompress);
    
    // Setup memory source with compressed data
    jpeg_mem_src(&cinfo_decompress, jpeg_buffer, jpeg_size);
    
    // Read JPEG header
    if (jpeg_read_header(&cinfo_decompress, TRUE) != JPEG_HEADER_OK) {
        jpeg_destroy_decompress(&cinfo_decompress);
        free(jpeg_buffer);
        return 0;
    }
    
    // Test jpeg_new_colormap if requested and applicable
    if (use_colormap && cinfo_decompress.out_color_space == JCS_GRAYSCALE) {
        jpeg_new_colormap(&cinfo_decompress);
    }
    
    // Start decompression
    jpeg_start_decompress(&cinfo_decompress);
    
    // Allocate 12-bit sample arrays for decompression
    int out_width = cinfo_decompress.output_width;
    int out_height = cinfo_decompress.output_height;
    int out_components = cinfo_decompress.output_components;
    
    J12SAMPARRAY output_scanlines = allocate_12bit_samples(out_width, out_height, out_components);
    if (!output_scanlines) {
        jpeg_destroy_decompress(&cinfo_decompress);
        free(jpeg_buffer);
        return 0;
    }
    
    // Test jpeg12_read_scanlines - read 12-bit scanlines
    JDIMENSION lines_read = 0;
    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        JDIMENSION max_lines = cinfo_decompress.output_height - cinfo_decompress.output_scanline;
        JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, max_lines);
        JDIMENSION read = jpeg12_read_scanlines(&cinfo_decompress, 
            &output_scanlines[lines_read], num_lines);
        lines_read += read;
        if (read == 0) break; // Error or EOF
    }
    
    // Test jpeg12_skip_scanlines if we have remaining scanlines
    if (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        JDIMENSION remaining = cinfo_decompress.output_height - cinfo_decompress.output_scanline;
        JDIMENSION skip_count = fdp.ConsumeIntegralInRange<JDIMENSION>(1, remaining);
        jpeg12_skip_scanlines(&cinfo_decompress, skip_count);
    }
    
    // Test jpeg12_crop_scanline if we have valid dimensions
    if (out_width > 4 && out_height > 0) {
        JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, out_width - 4);
        JDIMENSION cropped_width = fdp.ConsumeIntegralInRange<JDIMENSION>(4, out_width - xoffset);
        jpeg12_crop_scanline(&cinfo_decompress, &xoffset, &cropped_width);
    }
    
    // If we have raw data mode, test jpeg12_read_raw_data
    if (cinfo_decompress.raw_data_out && fdp.remaining_bytes() > 32) {
        int components = cinfo_decompress.num_components;
        J12SAMPIMAGE raw_output = allocate_12bit_image(out_width, out_height, components);
        if (raw_output) {
            JDIMENSION max_lines = cinfo_decompress.output_height;
            jpeg12_read_raw_data(&cinfo_decompress, raw_output, max_lines);
            free_12bit_image(raw_output, out_height, components);
        }
    }
    
    // Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    // Cleanup decompression resources
    free_12bit_samples(output_scanlines, out_height);
    jpeg_destroy_decompress(&cinfo_decompress);
    free(jpeg_buffer);
    
    return 0;
}

} // namespace harness_008
