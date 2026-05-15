#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_000 {
/*
 * First fuzzing harness for libjpeg-turbo targeting core JPEG decompression
 * and compression APIs. This harness exercises the main libjpeg API entry points
 * with input-driven testing using FuzzedDataProvider.
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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    FuzzedDataProvider fdp(data, size);
    
    // Minimum size check - need enough data for basic JPEG header
    if (size < 100) {
        return 0;
    }
    
    /**************************************************************************
     * PART 1: DECOMPRESSION - Test core decompression APIs
     **************************************************************************/
    {
        struct jpeg_decompress_struct cinfo;
        struct fuzzer_error_mgr jerr;
        JSAMPARRAY buffer = NULL;
        int row_stride;
        
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = fuzzer_error_exit;
        jerr.pub.emit_message = fuzzer_emit_message;
        
        jpeg_create_decompress(&cinfo);
        
        if (setjmp(jerr.setjmp_buffer)) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            goto compression_test;
        }
        
        // Use memory source for JPEG data
        jpeg_mem_src(&cinfo, data, (unsigned long)size);
        
        // Read JPEG header
        int header_status = jpeg_read_header(&cinfo, TRUE);
        if (header_status != JPEG_HEADER_OK) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            goto compression_test;
        }
        
        // Sanity check dimensions to avoid memory exhaustion
        if (cinfo.image_width < 1 || cinfo.image_height < 1 ||
            (uint64_t)cinfo.image_width * cinfo.image_height > 1048576) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            goto compression_test;
        }
        
        // Consume parameters from fuzzer input
        int dct_method_val = fdp.ConsumeIntegralInRange<int>(0, 2); // JDCT_ISLOW, JDCT_IFAST, JDCT_FLOAT
        cinfo.dct_method = static_cast<J_DCT_METHOD>(dct_method_val);
        int color_space_val = fdp.ConsumeIntegralInRange<int>(0, 15); // Various color spaces
        cinfo.out_color_space = static_cast<J_COLOR_SPACE>(color_space_val);
        // Start decompression
        if (!jpeg_start_decompress(&cinfo)) {
            jpeg_abort_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            goto compression_test;
        }
        
        row_stride = cinfo.output_width * cinfo.output_components;
        buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
        
        // Read scanlines
        while (cinfo.output_scanline < cinfo.output_height) {
            // Consume whether to skip scanlines from fuzzer input
            bool skip_lines = fdp.ConsumeBool();
            if (skip_lines && cinfo.output_scanline + 8 < cinfo.output_height) {
                jpeg_skip_scanlines(&cinfo, 8);
            } else {
                if (jpeg_read_scanlines(&cinfo, buffer, 1) != 1) {
                    break;
                }
            }
        }
        
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
    }
    
compression_test:
    /**************************************************************************
     * PART 2: COMPRESSION - Test core compression APIs
     **************************************************************************/
    // Use remaining input for compression test
    if (fdp.remaining_bytes() < 50) {
        return 0;
    }
    
    {
        struct jpeg_compress_struct cinfo;
        struct fuzzer_error_mgr jerr;
        JSAMPROW row_pointer[1];
        
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = fuzzer_error_exit;
        jerr.pub.emit_message = fuzzer_emit_message;
        
        jpeg_create_compress(&cinfo);
        
        if (setjmp(jerr.setjmp_buffer)) {
            jpeg_abort_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
            return 0;
        }
        
        // Create a simple in-memory destination
        unsigned char *outbuffer = NULL;
        unsigned long outsize = 0;
        jpeg_mem_dest(&cinfo, &outbuffer, &outsize);
        
        // Set image parameters from fuzzer input
        cinfo.image_width = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 256);
        cinfo.image_height = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 256);
        cinfo.input_components = fdp.ConsumeIntegralInRange<int>(1, 4);
        int in_color_space_val = fdp.ConsumeIntegralInRange<int>(0, 15);
        cinfo.in_color_space = static_cast<J_COLOR_SPACE>(in_color_space_val);
        
        jpeg_set_defaults(&cinfo);
        
        // Set quality from fuzzer input
        int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        jpeg_set_quality(&cinfo, quality, TRUE);
        
        // Set other parameters from fuzzer input
        int compress_dct_method_val = fdp.ConsumeIntegralInRange<int>(0, 2);
        cinfo.dct_method = static_cast<J_DCT_METHOD>(compress_dct_method_val);
        cinfo.optimize_coding = fdp.ConsumeBool();
        cinfo.progressive_mode = fdp.ConsumeBool();
        
        // Start compression
        jpeg_start_compress(&cinfo, TRUE);
        
        // Allocate and fill a simple image buffer
        int row_stride = cinfo.image_width * cinfo.input_components;
        JSAMPLE *image_buffer = (JSAMPLE *)malloc(row_stride * cinfo.image_height * sizeof(JSAMPLE));
        if (!image_buffer) {
            jpeg_abort_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
            if (outbuffer) free(outbuffer);
            return 0;
        }

        
        // Fill buffer with data from fuzzer input
        size_t bytes_needed = row_stride * cinfo.image_height;
        if (fdp.remaining_bytes() < bytes_needed) {
            // Not enough data, fill with pattern
            for (JDIMENSION i = 0; i < row_stride * cinfo.image_height; i++) {
                image_buffer[i] = (JSAMPLE)(i % 256);
            }
        } else {
            // Use fuzzer data
            auto bytes = fdp.ConsumeBytes<JSAMPLE>(bytes_needed);
            memcpy(image_buffer, bytes.data(), bytes.size());
        }
        
        // Write scanlines
        while (cinfo.next_scanline < cinfo.image_height) {
            row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        
        jpeg_finish_compress(&cinfo);
        
        // Cleanup
        free(image_buffer);
        jpeg_destroy_compress(&cinfo);
        if (outbuffer) free(outbuffer);
    }
    
    return 0;
}

} // namespace harness_000
