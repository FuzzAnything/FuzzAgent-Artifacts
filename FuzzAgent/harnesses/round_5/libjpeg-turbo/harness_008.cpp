#include <cstddef>      // for size_t
#include <cstdio>       // for FILE, SEEK_SET, etc.
#include <csetjmp>      // for setjmp/longjmp
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>
#include <algorithm>    // for std::min
#include "jpeglib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/*
 * Fuzzing harness for libjpeg-turbo targeting progressive JPEG decompression APIs
 * This harness specifically targets progressive JPEG (multi-scan) decompression workflows:
 * - jpeg_resync_to_restart: Resynchronize decompressor after restart marker
 * - jpeg_finish_output: Finish outputting a scan during buffered-image mode
 * - jpeg_start_output: Start outputting a scan during buffered-image mode
 * - jpeg_has_multiple_scans: Check if JPEG has multiple scans (progressive/sequential multi-scan)
 * - jpeg_simple_progression: Set up simple progression script for progressive compression
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Focuses on TurboJPEG RGB compression/decompression with tjCompress2/tjDecompress2
 * - harness_001.cpp: Focuses on TurboJPEG YUV decompression APIs
 * - harness_002.cpp: Focuses on TurboJPEG transformation operations
 * - harness_003.cpp: Focuses on TurboJPEG 12/16-bit precision operations
 * - harness_004.cpp: Focuses on TurboJPEG file I/O operations
 * - harness_005.cpp: Focuses on TurboJPEG legacy APIs with pixelSize parameter
 * - harness_006.cpp: Focuses on TurboJPEG YUV plane and file I/O APIs
 * - harness_007.cpp: Focuses on low-level libjpeg memory/stdio I/O functions
 * - This harness: Focuses exclusively on progressive JPEG decompression workflows with multi-scan control
 * 
 * Coverage impact: Targets 70+ undiscovered branches in progressive JPEG decompression APIs
 * 
 * Strategy: 
 * 1. Create progressive JPEG from fuzzer-generated image data
 * 2. Test progressive decompression with buffered-image mode
 * 3. Exercise multi-scan control functions (jpeg_start_output, jpeg_finish_output)
 * 4. Test resynchronization with jpeg_resync_to_restart
 * 5. Test jpeg_has_multiple_scans detection
 */

// Include standard headers first as required by jpeglib.h

// Now include jpeglib.h (which depends on size_t being defined)

// Custom error manager to handle libjpeg errors during fuzzing
struct fuzz_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

METHODDEF(void)
fuzz_error_exit(j_common_ptr cinfo) {
    fuzz_error_mgr* err = (fuzz_error_mgr*)cinfo->err;
    longjmp(err->setjmp_buffer, 1);
}

METHODDEF(void)
fuzz_output_message(j_common_ptr cinfo) {
    // Suppress error messages during fuzzing
}

// Helper to setup error manager
static fuzz_error_mgr* setup_error_mgr(j_common_ptr cinfo) {
    fuzz_error_mgr* err = (fuzz_error_mgr*)cinfo->err;
    
    if (!err) {
        err = (fuzz_error_mgr*)(*cinfo->mem->alloc_small)
            ((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(fuzz_error_mgr));
        cinfo->err = &err->pub;
    }
    
    jpeg_std_error(&err->pub);
    err->pub.error_exit = fuzz_error_exit;
    err->pub.output_message = fuzz_output_message;
    
    return err;
}

// Memory source manager for progressive decompression testing
static void init_mem_source(j_decompress_ptr cinfo, const uint8_t* data, size_t size) {
    struct jpeg_source_mgr* src;
    
    if (cinfo->src == NULL) {
        cinfo->src = (struct jpeg_source_mgr*)
            (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_PERMANENT,
                                       sizeof(struct jpeg_source_mgr));
    }
    
    src = cinfo->src;
    src->init_source = [](j_decompress_ptr cinfo) -> void {
        // Nothing to do
    };
    
    src->fill_input_buffer = [](j_decompress_ptr cinfo) -> boolean {
        static const JOCTET eoi_buffer[2] = { (JOCTET)0xFF, (JOCTET)JPEG_EOI };
        // Already set up by init_mem_source
        return TRUE;
    };
    
    src->skip_input_data = [](j_decompress_ptr cinfo, long num_bytes) -> void {
        struct jpeg_source_mgr* src = cinfo->src;
        
        if (num_bytes > 0) {
            if ((size_t)num_bytes > src->bytes_in_buffer) {
                src->bytes_in_buffer = 0;
            } else {
                src->next_input_byte += num_bytes;
                src->bytes_in_buffer -= num_bytes;
            }
        }
    };
    
    src->resync_to_restart = jpeg_resync_to_restart;
    
    src->term_source = [](j_decompress_ptr cinfo) -> void {
        // Nothing to do
    };
    
    src->bytes_in_buffer = size;
    src->next_input_byte = (const JOCTET*)data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic parameters + some image data
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    int restart_interval = fdp.ConsumeIntegralInRange<int>(0, 100);
    int color_space = fdp.ConsumeIntegralInRange<int>(0, 5); // JCS_GRAYSCALE=0, JCS_RGB=1, etc.
    
    // Validate color space
    J_COLOR_SPACE jpeg_color_space;
    switch (color_space % 3) {
        case 0: jpeg_color_space = JCS_GRAYSCALE; break;
        case 1: jpeg_color_space = JCS_RGB; break;
        case 2: jpeg_color_space = JCS_YCbCr; break;
        default: jpeg_color_space = JCS_RGB; break;
    }
    
    // Step 1: Create a progressive JPEG in memory
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        JSAMPLE* image_buffer = nullptr;
        std::vector<JOCTET> jpeg_buffer;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        
        // Set up memory destination
        unsigned char* out_buffer = nullptr;
        unsigned long out_size = 0;
        jpeg_mem_dest(&cinfo, &out_buffer, &out_size);
        
        // Set basic parameters
        cinfo.image_width = width;
        cinfo.image_height = height;
        cinfo.input_components = (jpeg_color_space == JCS_GRAYSCALE) ? 1 : 3;
        cinfo.in_color_space = jpeg_color_space;
        
        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, quality, TRUE);
        
        // Set up progressive mode
        jpeg_simple_progression(&cinfo);
        
        // Set restart interval if requested
        if (restart_interval > 0) {
            cinfo.restart_interval = restart_interval;
        }
        
        // Start compression
        jpeg_start_compress(&cinfo, TRUE);
        
        // Create simple image data from fuzzer input
        size_t row_stride = width * cinfo.input_components;
        image_buffer = (JSAMPLE*)malloc(row_stride * height);
        if (!image_buffer) {
            jpeg_destroy_compress(&cinfo);
            if (out_buffer) free(out_buffer);
            return 0;
        }
        
        // Fill image with fuzzer data
        size_t remaining = fdp.remaining_bytes();
        size_t to_fill = std::min(remaining, row_stride * height);
        if (to_fill > 0) {
            std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(to_fill);
            memcpy(image_buffer, img_data.data(), to_fill);
        }
        
        // Write scanlines
        while (cinfo.next_scanline < cinfo.image_height) {
            JSAMPROW row_pointer = &image_buffer[cinfo.next_scanline * row_stride];
            jpeg_write_scanlines(&cinfo, &row_pointer, 1);
        }
        
        // Finish compression
        jpeg_finish_compress(&cinfo);
        
        // Store JPEG buffer for decompression testing
        if (out_buffer && out_size > 0) {
            jpeg_buffer.assign(out_buffer, out_buffer + out_size);
            free(out_buffer);
        }
        
        jpeg_destroy_compress(&cinfo);
        free(image_buffer);
        
        // Step 2: Test progressive decompression
        if (!jpeg_buffer.empty()) {
            struct jpeg_decompress_struct dinfo;
            fuzz_error_mgr ferr;
            
            dinfo.err = jpeg_std_error(&ferr.pub);
            ferr.pub.error_exit = fuzz_error_exit;
            ferr.pub.output_message = fuzz_output_message;
            
            if (setjmp(ferr.setjmp_buffer)) {
                jpeg_destroy_decompress(&dinfo);
                return 0;
            }
            
            jpeg_create_decompress(&dinfo);
            
            // Set up memory source
            jpeg_mem_src(&dinfo, jpeg_buffer.data(), jpeg_buffer.size());
            
            // Read header
            jpeg_read_header(&dinfo, TRUE);
            
            // Test jpeg_has_multiple_scans
            boolean has_multiple_scans = jpeg_has_multiple_scans(&dinfo);
            
            // Start decompression
            jpeg_start_decompress(&dinfo);
            
            // If it has multiple scans, test buffered-image mode
            if (has_multiple_scans && dinfo.progressive_mode) {
                // Test 1: Simple progressive decompression without buffered-image mode
                JSAMPLE* output_buffer = (JSAMPLE*)malloc(dinfo.output_width * dinfo.output_height * dinfo.output_components);
                if (output_buffer) {
                    JSAMPROW row_pointer[1];
                    
                    while (dinfo.output_scanline < dinfo.output_height) {
                        row_pointer[0] = &output_buffer[dinfo.output_scanline * dinfo.output_width * dinfo.output_components];
                        jpeg_read_scanlines(&dinfo, row_pointer, 1);
                    }
                    
                    free(output_buffer);
                }
                
                // Test 2: Buffered-image mode with jpeg_start_output/jpeg_finish_output
                // First, we need to set buffered_image mode and reinitialize
                jpeg_abort_decompress(&dinfo);
                jpeg_mem_src(&dinfo, jpeg_buffer.data(), jpeg_buffer.size());
                jpeg_read_header(&dinfo, TRUE);
                
                // Enable buffered-image mode
                dinfo.buffered_image = TRUE;
                
                // Start decompression in buffered-image mode
                jpeg_start_decompress(&dinfo);
                
                // Try to process each scan
                // We'll try up to 10 scans or until we run out of input scans
                for (int scan_num = 1; scan_num <= 10; scan_num++) {
                    // Check if we have more input before trying to start output
                    if (dinfo.input_scan_number >= scan_num) {
                        if (jpeg_start_output(&dinfo, scan_num)) {
                            // Read some scanlines from this scan
                            int lines_to_read = fdp.ConsumeIntegralInRange<int>(1, dinfo.output_height);
                            lines_to_read = std::min(lines_to_read, (int)(dinfo.output_height - dinfo.output_scanline));
                            
                            if (lines_to_read > 0) {
                                JSAMPLE* scan_buffer = (JSAMPLE*)malloc(dinfo.output_width * lines_to_read * dinfo.output_components);
                                if (scan_buffer) {
                                    JSAMPROW row_ptr[1];
                                    for (int i = 0; i < lines_to_read; i++) {
                                        row_ptr[0] = &scan_buffer[i * dinfo.output_width * dinfo.output_components];
                                        jpeg_read_scanlines(&dinfo, row_ptr, 1);
                                    }
                                    free(scan_buffer);
                                }
                            }
                            
                            jpeg_finish_output(&dinfo);
                        }
                    }
                    
                    // Test jpeg_resync_to_restart with random desired restart marker
                    if (restart_interval > 0) {
                        int desired_restart = fdp.ConsumeIntegralInRange<int>(0, 7);
                        jpeg_resync_to_restart(&dinfo, desired_restart);
                    }
                }
            } else {
                // Single-scan sequential JPEG - just read all scanlines
                JSAMPLE* output_buffer = (JSAMPLE*)malloc(dinfo.output_width * dinfo.output_height * dinfo.output_components);
                if (output_buffer) {
                    JSAMPROW row_pointer[1];
                    
                    while (dinfo.output_scanline < dinfo.output_height) {
                        row_pointer[0] = &output_buffer[dinfo.output_scanline * dinfo.output_width * dinfo.output_components];
                        jpeg_read_scanlines(&dinfo, row_pointer, 1);
                    }
                    
                    free(output_buffer);
                }
            }
            
            jpeg_destroy_decompress(&dinfo);
        }
    }
    
    return 0;
}

} // namespace harness_008
