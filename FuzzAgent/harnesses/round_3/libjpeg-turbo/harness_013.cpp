#include <fuzzer/FuzzedDataProvider.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_013 {
/*
 * Fuzzing harness for libjpeg-turbo JPEG scanline manipulation operations (skip/crop)
 * This harness targets completely uncovered scanline manipulation operations:
 * - jpeg_skip_scanlines: 8-bit scanline skipping (74 undiscovered branches, 0% coverage)
 * - jpeg12_skip_scanlines: 12-bit scanline skipping (71 undiscovered branches, 0% coverage)
 * - jpeg_crop_scanline: 8-bit scanline cropping (40 undiscovered branches, 0% coverage)
 * - jpeg12_crop_scanline: 12-bit scanline cropping (30 undiscovered branches, 0% coverage)
 * - jpeg_finish_output: Progressive JPEG output completion (14 undiscovered branches, 0% coverage)
 * 
 * Differentiates from existing harnesses (000-012):
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs
 * - harness_001.cpp: Traditional tj* TurboJPEG APIs
 * - harness_002.cpp: Modern tj3* YUV APIs
 * - harness_003.cpp: TurboJPEG 12/16-bit precision APIs
 * - harness_004.cpp: Traditional tj* YUV operations
 * - harness_005.cpp: High-bit-depth image I/O
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg I/O APIs (8-bit)
 * - harness_008.cpp: Traditional libjpeg 12-bit APIs (non-lossless)
 * - harness_009.cpp: Old turbojpeg YUV decompression APIs
 * - harness_010.cpp: Traditional file-based I/O APIs
 * - harness_011.cpp: JPEG error recovery and marker handling APIs
 * - harness_012.cpp: 12-bit lossless JPEG operations
 * 
 * Strategy: Implement targeted testing of scanline skip and crop operations during
 * decompression, focusing on edge cases, proper state management (DSTATE_SCANNING),
 * and both 8-bit/12-bit variants. Test with progressive/multi-scan JPEGs to exercise
 * jpeg_finish_output.
 * 
 * Expected Coverage Impact:
 * - Cover 5+ currently uncovered public APIs (215+ undiscovered branches total)
 * - Exercise scanline manipulation logic with complex boundary conditions
 * - Test iMCU row boundary handling in skip/crop operations
 * - Provide semantic diversity focusing specifically on scanline manipulation
 */


// Include traditional libjpeg headers
extern "C" {
}

// Helper to allocate scanline buffers
static JSAMPARRAY allocate_scanlines(int width, int height, int components) {
    if (width <= 0 || height <= 0 || components <= 0) return nullptr;
    
    JSAMPARRAY scanlines = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);
    if (!scanlines) return nullptr;
    
    for (int row = 0; row < height; row++) {
        scanlines[row] = (JSAMPROW)malloc(sizeof(JSAMPLE) * width * components);
        if (!scanlines[row]) {
            // Cleanup on failure
            for (int i = 0; i < row; i++) {
                free(scanlines[i]);
            }
            free(scanlines);
            return nullptr;
        }
    }
    return scanlines;
}

// Helper to free scanline buffers
static void free_scanlines(JSAMPARRAY scanlines, int height) {
    if (!scanlines) return;
    
    for (int row = 0; row < height; row++) {
        free(scanlines[row]);
    }
    free(scanlines);
}

// Helper to allocate 12-bit scanline buffers
static J12SAMPARRAY allocate_12bit_scanlines(int width, int height, int components) {
    if (width <= 0 || height <= 0 || components <= 0) return nullptr;
    
    J12SAMPARRAY scanlines = (J12SAMPARRAY)malloc(sizeof(J12SAMPROW) * height);
    if (!scanlines) return nullptr;
    
    for (int row = 0; row < height; row++) {
        scanlines[row] = (J12SAMPROW)malloc(sizeof(J12SAMPLE) * width * components);
        if (!scanlines[row]) {
            // Cleanup on failure
            for (int i = 0; i < row; i++) {
                free(scanlines[i]);
            }
            free(scanlines);
            return nullptr;
        }
    }
    return scanlines;
}

// Helper to free 12-bit scanline buffers
static void free_12bit_scanlines(J12SAMPARRAY scanlines, int height) {
    if (!scanlines) return;
    
    for (int row = 0; row < height; row++) {
        free(scanlines[row]);
    }
    free(scanlines);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for JPEG operations
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== PHASE 1: CREATE TEST JPEG IMAGE ==========
    
    struct jpeg_compress_struct cinfo_compress;
    struct jpeg_error_mgr jerr_compress;
    
    // Initialize compression object
    cinfo_compress.err = jpeg_std_error(&jerr_compress);
    jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
    
    // Consume compression parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 128);
    int height = fdp.ConsumeIntegralInRange<int>(16, 128);
    int components = fdp.ConsumeIntegralInRange<int>(1, 3);
    int quality = fdp.ConsumeIntegralInRange<int>(10, 100);
    boolean progressive = fdp.ConsumeBool();
    boolean use_12bit = fdp.ConsumeBool();  // Test 12-bit variant
    
    // Set image dimensions
    cinfo_compress.image_width = width;
    cinfo_compress.image_height = height;
    cinfo_compress.input_components = components;
    
    // Choose color space
    J_COLOR_SPACE color_space;
    if (components == 1) {
        color_space = JCS_GRAYSCALE;
    } else if (components == 3) {
        color_space = fdp.ConsumeBool() ? JCS_RGB : JCS_YCbCr;
    } else {
        color_space = JCS_UNKNOWN;
    }
    cinfo_compress.in_color_space = color_space;
    
    // Set default parameters
    jpeg_set_defaults(&cinfo_compress);
    
    // Set quality
    jpeg_set_quality(&cinfo_compress, quality, TRUE);
    
    // Set progressive mode if requested (to test jpeg_finish_output)
    if (progressive) {
        jpeg_simple_progression(&cinfo_compress);
    }
    
    // Set data precision for 12-bit if requested
    if (use_12bit) {
        cinfo_compress.data_precision = 12;
    } else {
        cinfo_compress.data_precision = 8;
    }
    
    // Setup memory destination
    unsigned char* jpeg_buffer = nullptr;
    unsigned long jpeg_size = 0;
    jpeg_mem_dest(&cinfo_compress, &jpeg_buffer, &jpeg_size);
    
    // Start compression
    jpeg_start_compress(&cinfo_compress, TRUE);
    
    // Allocate and fill scanlines
    JSAMPARRAY scanlines = nullptr;
    J12SAMPARRAY scanlines_12bit = nullptr;
    
    if (use_12bit) {
        scanlines_12bit = allocate_12bit_scanlines(width, height, components);
        if (!scanlines_12bit) {
            jpeg_destroy_compress(&cinfo_compress);
            return 0;
        }
        
        // Fill 12-bit samples
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width * components; col++) {
                scanlines_12bit[row][col] = fdp.ConsumeIntegralInRange<J12SAMPLE>(0, 4095);
            }
        }
        
        // Write scanlines
        JDIMENSION lines_written = 0;
        while (lines_written < height) {
            JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, height - lines_written);
            jpeg12_write_scanlines(&cinfo_compress, &scanlines_12bit[lines_written], num_lines);
            lines_written += num_lines;
        }
    } else {
        scanlines = allocate_scanlines(width, height, components);
        if (!scanlines) {
            jpeg_destroy_compress(&cinfo_compress);
            return 0;
        }
        
        // Fill 8-bit samples
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width * components; col++) {
                scanlines[row][col] = fdp.ConsumeIntegral<JSAMPLE>();
            }
        }
        
        // Write scanlines
        JDIMENSION lines_written = 0;
        while (lines_written < height) {
            JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, height - lines_written);
            jpeg_write_scanlines(&cinfo_compress, &scanlines[lines_written], num_lines);
            lines_written += num_lines;
        }
    }
    
    // Finish compression
    jpeg_finish_compress(&cinfo_compress);
    
    // Cleanup compression resources
    if (scanlines) free_scanlines(scanlines, height);
    if (scanlines_12bit) free_12bit_scanlines(scanlines_12bit, height);
    jpeg_destroy_compress(&cinfo_compress);
    
    // ========== PHASE 2: DECOMPRESSION WITH SCANLINE MANIPULATION ==========
    
    if (!jpeg_buffer || jpeg_size == 0) {
        if (jpeg_buffer) free(jpeg_buffer);
        return 0;
    }
    
    struct jpeg_decompress_struct cinfo_decompress;
    struct jpeg_error_mgr jerr_decompress;
    
    // Initialize decompression object
    cinfo_decompress.err = jpeg_std_error(&jerr_decompress);
    jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
    
    // Setup memory source
    jpeg_mem_src(&cinfo_decompress, jpeg_buffer, jpeg_size);
    
    // Read JPEG header
    if (jpeg_read_header(&cinfo_decompress, TRUE) != JPEG_HEADER_OK) {
        jpeg_destroy_decompress(&cinfo_decompress);
        free(jpeg_buffer);
        return 0;
    }
    
    // Check if image is progressive (for testing jpeg_finish_output)
    boolean is_progressive = jpeg_has_multiple_scans(&cinfo_decompress);
    
    // Start decompression
    jpeg_start_decompress(&cinfo_decompress);
    
    // Allocate output scanlines
    int out_width = cinfo_decompress.output_width;
    int out_height = cinfo_decompress.output_height;
    int out_components = cinfo_decompress.output_components;
    
    // Determine which scanline API to use based on precision
    boolean is_12bit = cinfo_decompress.data_precision == 12;
    
    // Allocate appropriate scanline buffers
    JSAMPARRAY output_scanlines = nullptr;
    J12SAMPARRAY output_scanlines_12bit = nullptr;
    
    if (is_12bit) {
        output_scanlines_12bit = allocate_12bit_scanlines(out_width, out_height, out_components);
        if (!output_scanlines_12bit) {
            jpeg_destroy_decompress(&cinfo_decompress);
            free(jpeg_buffer);
            return 0;
        }
    } else {
        output_scanlines = allocate_scanlines(out_width, out_height, out_components);
        if (!output_scanlines) {
            jpeg_destroy_decompress(&cinfo_decompress);
            free(jpeg_buffer);
            return 0;
        }
    }
    
    // ========== TARGET API 1: READ SOME SCANLINES FIRST ==========
    
    JDIMENSION lines_read = 0;
    JDIMENSION lines_to_read_initially = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (JDIMENSION)(out_height / 4));
    if (lines_to_read_initially > (JDIMENSION)out_height) {
        lines_to_read_initially = (JDIMENSION)out_height;
    }
    
    if (is_12bit) {
        // Read initial scanlines using 12-bit API
        while (lines_read < lines_to_read_initially && cinfo_decompress.output_scanline < out_height) {
            JDIMENSION max_lines = out_height - cinfo_decompress.output_scanline;
            JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (max_lines < 4 ? max_lines : 4));
            JDIMENSION read = jpeg12_read_scanlines(&cinfo_decompress, 
                &output_scanlines_12bit[lines_read], num_lines);
            lines_read += read;
            if (read == 0) break;
        }
    } else {
        // Read initial scanlines using 8-bit API
        while (lines_read < lines_to_read_initially && cinfo_decompress.output_scanline < out_height) {
            JDIMENSION max_lines = out_height - cinfo_decompress.output_scanline;
            JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (max_lines < 4 ? max_lines : 4));
            JDIMENSION read = jpeg_read_scanlines(&cinfo_decompress, 
                &output_scanlines[lines_read], num_lines);
            lines_read += read;
            if (read == 0) break;
        }
    }
    
    // ========== TARGET API 2: SKIP SCANLINES ==========
    
    if (cinfo_decompress.output_scanline < out_height) {
        JDIMENSION remaining = out_height - cinfo_decompress.output_scanline;
        
        // Test different skip scenarios
        int skip_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
        JDIMENSION skip_amount = 0;
        
        switch (skip_scenario) {
            case 0: // Skip 0 lines (edge case)
                skip_amount = 0;
                break;
            case 1: // Skip few lines
                skip_amount = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (remaining < 3 ? remaining : 3));
                break;
            case 2: // Skip many lines (but not all)
                skip_amount = fdp.ConsumeIntegralInRange<JDIMENSION>(remaining / 2, remaining);
                break;
            case 3: // Skip past end (should return 0)
                skip_amount = remaining + fdp.ConsumeIntegralInRange<JDIMENSION>(1, 10);
                break;
        }
        
        // Call appropriate skip API
        JDIMENSION skipped = 0;
        if (is_12bit) {
            skipped = jpeg12_skip_scanlines(&cinfo_decompress, skip_amount);
        } else {
            skipped = jpeg_skip_scanlines(&cinfo_decompress, skip_amount);
        }
        
        // Update lines_read based on actual skipped count
        lines_read += skipped;
    }
    
    // ========== TARGET API 3: CROP SCANLINE ==========
    
    // Only crop if we have valid dimensions and haven't consumed all scanlines
    if (out_width > 8 && cinfo_decompress.output_scanline < out_height) {
        JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, out_width - 8);
        JDIMENSION cropped_width = fdp.ConsumeIntegralInRange<JDIMENSION>(4, out_width - xoffset);
        
        // Call appropriate crop API
        if (is_12bit) {
            jpeg12_crop_scanline(&cinfo_decompress, &xoffset, &cropped_width);
        } else {
            jpeg_crop_scanline(&cinfo_decompress, &xoffset, &cropped_width);
        }
        
        // Read remaining scanlines with cropped width
        if (is_12bit) {
            while (cinfo_decompress.output_scanline < out_height) {
                JDIMENSION max_lines = out_height - cinfo_decompress.output_scanline;
                JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (max_lines < 2 ? max_lines : 2));
                JDIMENSION read = jpeg12_read_scanlines(&cinfo_decompress, 
                    &output_scanlines_12bit[lines_read], num_lines);
                lines_read += read;
                if (read == 0) break;
            }
        } else {
            while (cinfo_decompress.output_scanline < out_height) {
                JDIMENSION max_lines = out_height - cinfo_decompress.output_scanline;
                JDIMENSION num_lines = fdp.ConsumeIntegralInRange<JDIMENSION>(1, (max_lines < 2 ? max_lines : 2));
                JDIMENSION read = jpeg_read_scanlines(&cinfo_decompress, 
                    &output_scanlines[lines_read], num_lines);
                lines_read += read;
                if (read == 0) break;
            }
        }
    }
    
    // ========== TARGET API 4: PROGRESSIVE OUTPUT COMPLETION ==========
    
    // Test jpeg_finish_output for progressive/multi-scan images
    if (is_progressive) {
        // For progressive JPEGs, we need to handle multi-pass decompression
        int scan_number = 1;
        while (!jpeg_input_complete(&cinfo_decompress)) {
            // Start output for this scan
            if (jpeg_start_output(&cinfo_decompress, scan_number)) {
                // Read some data for this scan
                if (is_12bit) {
                    JDIMENSION lines_this_scan = 0;
                    while (cinfo_decompress.output_scanline < out_height && lines_this_scan < 2) {
                        JDIMENSION read = jpeg12_read_scanlines(&cinfo_decompress, 
                            &output_scanlines_12bit[lines_read], 1);
                        lines_read += read;
                        lines_this_scan += read;
                        if (read == 0) break;
                    }
                } else {
                    JDIMENSION lines_this_scan = 0;
                    while (cinfo_decompress.output_scanline < out_height && lines_this_scan < 2) {
                        JDIMENSION read = jpeg_read_scanlines(&cinfo_decompress, 
                            &output_scanlines[lines_read], 1);
                        lines_read += read;
                        lines_this_scan += read;
                        if (read == 0) break;
                    }
                }
                
                // Finish output for this scan - TARGET API
                jpeg_finish_output(&cinfo_decompress);
            }
            scan_number++;
            
            // Limit number of scans for fuzzing efficiency
            if (scan_number > 3) break;
        }
    }
    
    // ========== CLEANUP ==========
    
    // Finish decompression
    jpeg_finish_decompress(&cinfo_decompress);
    
    // Free resources
    if (output_scanlines) free_scanlines(output_scanlines, out_height);
    if (output_scanlines_12bit) free_12bit_scanlines(output_scanlines_12bit, out_height);
    
    jpeg_destroy_decompress(&cinfo_decompress);
    free(jpeg_buffer);
    
    return 0;
}

} // namespace harness_013
