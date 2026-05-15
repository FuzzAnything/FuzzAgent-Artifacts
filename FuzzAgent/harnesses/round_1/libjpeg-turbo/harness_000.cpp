#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_000 {
/*
 * libjpeg-turbo fuzzing harness targeting core JPEG encoding/decoding APIs
 * This harness tests both compression and decompression paths using the
 * traditional libjpeg API with fuzzer-generated input data.
 * Fixed memory leak issues: removed cleanup_done flags, fixed setjmp/longjmp
 * error handling to ensure all allocations (image_buffer, jpeg_buffer) are
 * properly freed in all error paths, including longjmp bypass scenarios.
 * Added robust error handling for small inputs and memory exhaustion checks.
 */


extern "C" {
}

/* Error manager structure for setjmp/longjmp error handling */
struct fuzzer_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

/* Custom error exit handler */
static void fuzzer_error_exit(j_common_ptr cinfo) {
    struct fuzzer_error_mgr *fuzz_err = (struct fuzzer_error_mgr *)cinfo->err;
    longjmp(fuzz_err->setjmp_buffer, 1);
}

/* Custom emit message handler (silent) */
static void fuzzer_emit_message(j_common_ptr cinfo, int msg_level) {
    /* Suppress warning messages during fuzzing */
}

/* Test JPEG decompression with various parameters */
static void test_decompress(FuzzedDataProvider &fdp, const uint8_t *jpeg_data, size_t jpeg_size) {
    if (jpeg_size < 2) return;  /* Need at least 2 bytes for JPEG header check */
    
    struct jpeg_decompress_struct cinfo;
    struct fuzzer_error_mgr jerr;
    JSAMPARRAY buffer = NULL;
    int row_stride;
    
    /* Initialize error manager */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = fuzzer_error_exit;
    jerr.pub.emit_message = fuzzer_emit_message;
    
    /* Set up error handling BEFORE any JPEG library calls that might fail */
    if (setjmp(jerr.setjmp_buffer)) {
        /* Error cleanup - jump here on any JPEG library error */
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Now safe to call JPEG library functions */
    jpeg_create_decompress(&cinfo);
    
    /* Set memory source - doesn't allocate, just references existing buffer */
    jpeg_mem_src(&cinfo, jpeg_data, (unsigned long)jpeg_size);
    
    /* Read header */
    jpeg_read_header(&cinfo, TRUE);
    
    /* Stricter size limits to prevent memory exhaustion */
    /* Maximum dimensions: 2048x2048, maximum pixels: 1 million to prevent 25766x504 issue */
    if (cinfo.image_width < 1 || cinfo.image_height < 1 ||
        cinfo.image_width > 2048 || cinfo.image_height > 2048 ||
        (uint64_t)cinfo.image_width * cinfo.image_height > 1048576) {
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Consume fuzzer data for DCT method choice */
    bool use_float_dct = fdp.ConsumeBool();
    
    /* Set various decompression parameters based on fuzzer input */
    cinfo.dct_method = use_float_dct ? JDCT_FLOAT : JDCT_ISLOW;
    cinfo.out_color_space = JCS_RGB;  /* Default output color space */
    
    /* Start decompression */
    if (!jpeg_start_decompress(&cinfo)) {
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Additional safety check: ensure output_components is valid */
    if (cinfo.output_components < 1 || cinfo.output_components > 4) {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Calculate row stride and validate */
    row_stride = cinfo.output_width * cinfo.output_components;
    if (row_stride <= 0 || row_stride > 65536) {  /* Reasonable limit */
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Allocate scanline buffer - library manages this memory */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
    
    /* Check if allocation succeeded */
    if (buffer == NULL) {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    
    /* Read scanlines */
    while (cinfo.output_scanline < cinfo.output_height) {
        if (jpeg_read_scanlines(&cinfo, buffer, 1) != 1) {
            break;
        }
    }
    
    /* Finish decompression */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}

/* Test JPEG compression with various parameters */
static void test_compress(FuzzedDataProvider &fdp) {
    /* Need enough data for width, height, quality, progressive, color_space */
    if (fdp.remaining_bytes() < 4) return;
    
    /* Consume all fuzzer data BEFORE setjmp to avoid C++ destructor issues */
    int width = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int height = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    bool progressive = fdp.ConsumeBool();
    J_COLOR_SPACE color_space = fdp.ConsumeBool() ? JCS_RGB : JCS_GRAYSCALE;
    
    /* Check for potential memory exhaustion before allocation */
    int max_pixels = width * height;
    if (max_pixels > 1048576) {  /* 1MP limit for compression */
        return;
    }
    
    /* Calculate image buffer size */
    int input_components = (color_space == JCS_GRAYSCALE) ? 1 : 3;
    int row_stride = width * input_components;
    size_t image_buffer_size = (size_t)row_stride * height;
    
    /* Additional safety check */
    if (image_buffer_size > 16777216) {  /* 16MB limit */
        return;
    }
    
    /* Allocate image buffer with C-style malloc - initialize to NULL */
    uint8_t *image_buffer = NULL;
    uint8_t *jpeg_buffer = NULL;
    
    /* Get image data from fuzzer using C-style allocation to avoid C++ destructor issues */
    size_t bytes_needed = image_buffer_size;
    size_t bytes_available = fdp.remaining_bytes();
    size_t bytes_to_use = bytes_available < bytes_needed ? bytes_available : bytes_needed;
    
    if (bytes_needed > 0) {
        image_buffer = (uint8_t *)malloc(bytes_needed);
        if (!image_buffer) {
            return;  /* No allocation, no leak */
        }
        
        /* Fill image with fuzzer data using simple byte-by-byte copy */
        if (bytes_to_use > 0) {
            size_t i = 0;
            while (i < bytes_to_use && fdp.remaining_bytes() > 0) {
                image_buffer[i] = fdp.ConsumeIntegral<uint8_t>();
                i++;
            }
        }
        
        /* Pad remaining bytes with zeros if needed */
        if (bytes_to_use < bytes_needed) {
            memset(image_buffer + bytes_to_use, 0, bytes_needed - bytes_to_use);
        }
    }
    
    struct jpeg_compress_struct cinfo;
    struct fuzzer_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned long jpeg_size = 0;
    
    /* Initialize error manager */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = fuzzer_error_exit;
    jerr.pub.emit_message = fuzzer_emit_message;
    
    /* Set up error handling BEFORE any JPEG library calls */
    if (setjmp(jerr.setjmp_buffer)) {
        /* Error cleanup - jump here on any JPEG library error */
        if (image_buffer) free(image_buffer);
        if (jpeg_buffer) free(jpeg_buffer);
        jpeg_destroy_compress(&cinfo);
        return;
    }
    
    /* Now safe to call JPEG library functions */
    jpeg_create_compress(&cinfo);
    
    /* Set image dimensions and color space */
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = input_components;
    cinfo.in_color_space = color_space;
    
    /* Set default compression parameters */
    jpeg_set_defaults(&cinfo);
    
    /* Set quality */
    jpeg_set_quality(&cinfo, quality, TRUE);
    
    /* Set progressive mode if requested */
    if (progressive) {
        jpeg_simple_progression(&cinfo);
    }
    
    /* Set memory destination - this allocates buffer via malloc */
    jpeg_buffer = NULL;
    jpeg_size = 0;
    jpeg_mem_dest(&cinfo, &jpeg_buffer, &jpeg_size);
    
    /* Start compression */
    jpeg_start_compress(&cinfo, TRUE);
    
    /* Write scanlines */
    if (image_buffer) {
        while (cinfo.next_scanline < cinfo.image_height) {
            row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
    }
    
    /* Finish compression - transfers buffer ownership to application */
    jpeg_finish_compress(&cinfo);
    
    /* Clean up - success path */
    if (image_buffer) free(image_buffer);
    /* jpeg_buffer is now owned by application after successful jpeg_finish_compress */
    if (jpeg_buffer) {
        free(jpeg_buffer);
    }
    jpeg_destroy_compress(&cinfo);
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 1) return 0;  /* Need at least 1 byte to decide test type */
    
    FuzzedDataProvider fdp(data, size);
    
    /* Decide whether to test compression, decompression, or both */
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (test_type) {
        case 0:  /* Test decompression only */
            {
                /* Use remaining bytes as JPEG data with C-style memory management */
                size_t remaining_size = fdp.remaining_bytes();
                if (remaining_size < 2) return 0;  /* Need at least 2 bytes for JPEG */
                
                /* Allocate buffer for JPEG data */
                uint8_t *jpeg_buffer = (uint8_t *)malloc(remaining_size);
                if (!jpeg_buffer) return 0;
                
                /* Consume data directly into buffer */
                size_t bytes_consumed = 0;
                while (bytes_consumed < remaining_size && fdp.remaining_bytes() > 0) {
                    jpeg_buffer[bytes_consumed] = fdp.ConsumeIntegral<uint8_t>();
                    bytes_consumed++;
                }
                
                /* Create a new FDP with just the JPEG data for decompression parameters */
                FuzzedDataProvider decomp_fdp(jpeg_buffer, bytes_consumed);
                /* Check if we have data for the bool parameter */
                if (decomp_fdp.remaining_bytes() < 1) {
                    free(jpeg_buffer);
                    return 0;
                }
                
                /* Call decompression with the JPEG data */
                test_decompress(decomp_fdp, jpeg_buffer, bytes_consumed);
                free(jpeg_buffer);
            }
            break;
            
        case 1:  /* Test compression only */
            test_compress(fdp);
            break;
            
        case 2:  /* Test both: compress then decompress */
            {
                /* First test compression with current FDP state */
                test_compress(fdp);
                
                /* Then try to decompress any remaining data as JPEG */
                size_t remaining_size = fdp.remaining_bytes();
                if (remaining_size >= 2) {
                    /* Allocate buffer for remaining data */
                    uint8_t *remaining_buffer = (uint8_t *)malloc(remaining_size);
                    if (remaining_buffer) {
                        /* Consume data directly into buffer */
                        size_t bytes_consumed = 0;
                        while (bytes_consumed < remaining_size && fdp.remaining_bytes() > 0) {
                            remaining_buffer[bytes_consumed] = fdp.ConsumeIntegral<uint8_t>();
                            bytes_consumed++;
                        }
                        
                        FuzzedDataProvider decomp_fdp(remaining_buffer, bytes_consumed);
                        if (decomp_fdp.remaining_bytes() >= 1) {
                            test_decompress(decomp_fdp, remaining_buffer, bytes_consumed);
                        }
                        free(remaining_buffer);
                    }
                }
            }
            break;
    }
    
    return 0;
}

} // namespace harness_000
