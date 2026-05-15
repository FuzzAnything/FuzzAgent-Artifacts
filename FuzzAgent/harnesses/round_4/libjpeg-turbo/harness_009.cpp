#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_009 {
/*
 * Fuzzing harness for libjpeg-turbo targeting progressive JPEG multi-scan output control operations.
 * This harness specifically targets progressive JPEG control functions for handling multi-scan
 * progressive JPEG images as identified by coverage analysis. All target progressive control APIs
 * are currently 0% covered (32+ undiscovered branches).
 * 
 * Target APIs (0% covered, 32+ undiscovered branches):
 * - jpeg_start_output: Start output for current scan in progressive mode (10 undiscovered branches)
 * - jpeg_finish_output: Finish output for current scan in progressive mode (14 undiscovered branches)
 * - jpeg_has_multiple_scans: Check if JPEG image has multiple scans (4 undiscovered branches)
 * - jpeg_input_complete: Check if input is complete for current scan (4 undiscovered branches)
 * - jpeg_new_colormap: Generate new colormap for quantized images (8 undiscovered branches)
 * 
 * Required helper APIs:
 * - jpeg_CreateDecompress: Create decompress object
 * - jpeg_mem_src: Set JPEG source from memory buffer
 * - jpeg_read_header: Read JPEG header
 * - jpeg_start_decompress: Start decompression
 * - jpeg_read_scanlines: Read scanlines from image
 * - jpeg_finish_decompress: Finish decompression
 * - jpeg_destroy_decompress: Cleanup decompress object
 * 
 * Lifecycle for progressive JPEG multi-scan images:
 * 1. Create decompress object and set error handler
 * 2. Set JPEG source from memory buffer
 * 3. Read JPEG header
 * 4. Start decompression
 * 5. Check if image has multiple scans (jpeg_has_multiple_scans)
 * 6. If multi-scan, implement progressive output loop:
 *    a. Start output for current scan (jpeg_start_output)
 *    b. Read scanlines for current scan (jpeg_read_scanlines)
 *    c. Check if input complete (jpeg_input_complete)
 *    d. Finish output for current scan (jpeg_finish_output)
 * 7. Optionally generate new colormap (jpeg_new_colormap)
 * 8. Finish decompression and cleanup
 * 
 * Semantic diversity: This harness focuses specifically on progressive JPEG multi-scan
 * output control, complementing existing harnesses that focus on basic decompression,
 * compression, or 12-bit operations. The progressive control functions represent
 * a critical logical lifecycle for handling multi-scan progressive JPEG images
 * (Start → Process → Finish cycles per scan).
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
const int MAX_WIDTH = 1024;
const int MAX_HEIGHT = 1024;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for JPEG header and basic operations
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    struct jpeg_decompress_struct cinfo;
    struct fuzzer_error_mgr jerr;
    
    // Initialize error handler
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = fuzzer_error_exit;
    jerr.pub.emit_message = fuzzer_emit_message;
    
    // Create decompress object
    jpeg_create_decompress(&cinfo);
    // Set up longjmp for error recovery
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_abort_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    
    // Set JPEG source from memory buffer
    jpeg_mem_src(&cinfo, data, (unsigned long)size);
    
    // Read JPEG header
    int header_status = jpeg_read_header(&cinfo, TRUE);
    if (header_status != JPEG_HEADER_OK) {
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    
    // Sanity check dimensions to avoid memory exhaustion
    if (cinfo.image_width < 1 || cinfo.image_height < 1 ||
        cinfo.image_width > MAX_WIDTH || cinfo.image_height > MAX_HEIGHT ||
        (uint64_t)cinfo.image_width * cinfo.image_height > 1048576) {
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    
    // Check if we have enough remaining fuzzer data for parameters
    if (fdp.remaining_bytes() < 20) {
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    
    // Consume decompression parameters from fuzzer input
    int dct_method_val = fdp.ConsumeIntegralInRange<int>(0, 2); // JDCT_ISLOW, JDCT_IFAST, JDCT_FLOAT
    cinfo.dct_method = static_cast<J_DCT_METHOD>(dct_method_val);
    
    // Consume color space parameter
    int color_space_val = fdp.ConsumeIntegralInRange<int>(0, 15);
    cinfo.out_color_space = static_cast<J_COLOR_SPACE>(color_space_val);
    
    // Consume quantization and dithering parameters
    cinfo.quantize_colors = fdp.ConsumeBool();
    if (cinfo.quantize_colors) {
        cinfo.two_pass_quantize = fdp.ConsumeBool();
        cinfo.dither_mode = static_cast<J_DITHER_MODE>(fdp.ConsumeIntegralInRange<int>(0, 2));
        cinfo.desired_number_of_colors = fdp.ConsumeIntegralInRange<int>(2, 256);
    }
    

    
    // Check if image has multiple scans (progressive JPEG)
    boolean has_multiple_scans = jpeg_has_multiple_scans(&cinfo);
    
    // Set buffered_image mode for progressive JPEG
    cinfo.buffered_image = has_multiple_scans;
    
    // Start decompression
    if (!jpeg_start_decompress(&cinfo)) {
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    
    // Allocate buffer for scanlines
    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
    
    /**************************************************************************
     * PROGRESSIVE JPEG MULTI-SCAN OUTPUT CONTROL LOOP
     * This is the core logic targeting the progressive control functions
     **************************************************************************/
    if (has_multiple_scans) {
        // Progressive JPEG with multiple scans
        while (!jpeg_input_complete(&cinfo) && cinfo.input_scan_number != cinfo.output_scan_number) {
            int retval;
            
            // Limit number of scans to prevent infinite loops
            if (cinfo.input_scan_number > 100) {
                break;
            }
            
            // Consume input data until we have a complete scan or reach the end of input
            do {
                retval = jpeg_consume_input(&cinfo);
            } while (retval != JPEG_SUSPENDED && retval != JPEG_REACHED_SOS && retval != JPEG_REACHED_EOI);
            
            if (retval == JPEG_REACHED_EOI) {
                break;
            }
            
            // Start output for current scan
            if (!jpeg_start_output(&cinfo, cinfo.input_scan_number)) {
                break;
            }
            
            // Read scanlines for current scan
            while (cinfo.output_scanline < cinfo.output_height) {
                // Check if we have fuzzer data before consuming
                if (fdp.remaining_bytes() == 0) break;
                
                // Consume whether to read or skip from fuzzer input
                bool read_line = fdp.ConsumeBool();
                
                if (read_line) {
                    // Read a scanline
                    if (jpeg_read_scanlines(&cinfo, buffer, 1) != 1) {
                        break;
                    }
                } else {
                    // Skip a scanline (simulate partial reading)
                    if (cinfo.output_scanline + 1 < cinfo.output_height) {
                        cinfo.output_scanline++;
                    }
                }
                
                // Check if we should break early based on fuzzer input
                if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool() && cinfo.output_scanline > cinfo.output_height / 2) {
                    break;
                }
            }
            
            // Finish output for current scan
            if (!jpeg_finish_output(&cinfo)) {
                break;
            }
        }
    } else {
        // Single-scan JPEG (non-progressive) - still read scanlines
        while (cinfo.output_scanline < cinfo.output_height) {
            if (jpeg_read_scanlines(&cinfo, buffer, 1) != 1) {
                break;
            }
            
            // Occasionally check if we should skip lines (for interstitial skipping)
            // Only if we have fuzzer data remaining
            if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool() && cinfo.output_scanline + 8 < cinfo.output_height) {
                jpeg_skip_scanlines(&cinfo, 8);
            }
        }
    }
    /**************************************************************************
     * COLOR MAP GENERATION - Test jpeg_new_colormap
     **************************************************************************/
    if (cinfo.quantize_colors && cinfo.out_color_space == JCS_RGB) {
        // Generate new colormap if quantizing colors
        if (cinfo.actual_number_of_colors > 0 && cinfo.colormap != NULL) {
            // Test jpeg_new_colormap for quantized RGB images
            // This function can be called to force generation of a new colormap
            // even if one already exists
            jpeg_new_colormap(&cinfo);
        }
    }
    // Finish decompression
    jpeg_finish_decompress(&cinfo);
    
    // Cleanup
    jpeg_destroy_decompress(&cinfo);
    
    return 0;
}

} // namespace harness_009
