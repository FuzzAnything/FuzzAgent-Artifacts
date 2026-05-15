// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_valid_subsamp(int subsamp) {
    if (subsamp < 0 || subsamp > 5) {
        return 0;  // Default to TJSAMP_444
    }
    return subsamp;
}

static int get_valid_pixel_format(int pixel_format) {
    if (pixel_format < 0 || pixel_format > 12) {
        return 0;  // Default to TJPF_RGB
    }
    return pixel_format;
}

static int get_valid_dimension(int dim) {
    if (dim <= 0) {
        return 1;
    }
    if (dim > 4096) {
        return 4096;
    }
    return dim;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Extract parameters from the beginning of the fuzzer input
    int width = *(const int *)Data;
    int height = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    int align = *(const int *)(Data + 12);
    
    // Validate parameters
    width = get_valid_dimension(width);
    height = get_valid_dimension(height);
    subsamp = get_valid_subsamp(subsamp);
    
    if (align <= 0) {
        align = 4;
    } else if (align > 64) {
        align = 64;
    }

    // Test tjBufSizeYUV
    unsigned long buf_size1 = tjBufSizeYUV(width, height, subsamp);
    
    // Test TJBUFSIZEYUV macro
    unsigned long buf_size2 = TJBUFSIZEYUV(width, height, subsamp);
    
    // Test tjBufSizeYUV2
    unsigned long buf_size3 = tjBufSizeYUV2(width, align, height, subsamp);
    
    // Create a dummy JPEG buffer for decompression functions
    // Use the remaining fuzzer data as JPEG data
    size_t jpeg_offset = 16;
    if (jpeg_offset >= Size) {
        return 0;
    }
    
    unsigned char *jpeg_buf = (unsigned char *)Data + jpeg_offset;
    unsigned long jpeg_size = Size - jpeg_offset;
    
    // Create a decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }
    
    // Test tjDecompressHeader2
    int decomp_width, decomp_height, decomp_subsamp;
    int header_result = tjDecompressHeader2(handle, jpeg_buf, jpeg_size, 
                                           &decomp_width, &decomp_height, 
                                           &decomp_subsamp);
    
    // If header decompression succeeded, test decompression functions
    if (header_result == 0) {
        // Validate decompressed dimensions
        decomp_width = get_valid_dimension(decomp_width);
        decomp_height = get_valid_dimension(decomp_height);
        decomp_subsamp = get_valid_subsamp(decomp_subsamp);
        
        // Calculate buffer size for YUV output
        unsigned long yuv_buf_size = tjBufSizeYUV2(decomp_width, align, 
                                                  decomp_height, decomp_subsamp);
        
        if (yuv_buf_size > 0 && yuv_buf_size < 100 * 1024 * 1024) {  // Limit to 100MB
            unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
            if (yuv_buf) {
                // Test tjDecompressToYUV2
                int decomp_result = tjDecompressToYUV2(handle, jpeg_buf, jpeg_size,
                                                      yuv_buf, decomp_width, align,
                                                      decomp_height, 0);
                
                // If YUV decompression succeeded, test tjDecodeYUVPlanes
                if (decomp_result == 0) {
                    // Extract pixel format and flags from fuzzer input
                    int pixel_format = 0;
                    int flags = 0;
                    if (Size >= jpeg_offset + 8) {
                        pixel_format = *(const int *)(Data + jpeg_offset);
                        flags = *(const int *)(Data + jpeg_offset + 4);
                    }
                    
                    pixel_format = get_valid_pixel_format(pixel_format);
                    
                    // Calculate RGB buffer size
                    int rgb_buf_size = decomp_width * decomp_height * 4;  // 4 bytes per pixel for RGBX
                    if (rgb_buf_size > 0 && rgb_buf_size < 100 * 1024 * 1024) {
                        unsigned char *rgb_buf = (unsigned char *)malloc(rgb_buf_size);
                        if (rgb_buf) {
                            // Set up YUV planes
                            const unsigned char *src_planes[3] = {NULL, NULL, NULL};
                            int strides[3] = {0, 0, 0};
                            
                            // For simplicity, use the same buffer for all planes
                            // In real YUV data, planes would be separate
                            src_planes[0] = yuv_buf;
                            
                            // Test tjDecodeYUVPlanes
                            tjDecodeYUVPlanes(handle, src_planes, strides, decomp_subsamp,
                                             rgb_buf, decomp_width, decomp_width * 4,
                                             decomp_height, pixel_format, flags);
                            
                            free(rgb_buf);
                        }
                    }
                }
                
                free(yuv_buf);
            }
        }
    }
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}