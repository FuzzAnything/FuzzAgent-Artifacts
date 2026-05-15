// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static tjhandle init_decompress_handle(void) {
    return tjInitDecompress();
}

static tjhandle init_decompress_handle3(void) {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_handle(tjhandle handle) {
    if (handle) tjDestroy(handle);
}

static void cleanup_handle3(tjhandle handle) {
    if (handle) tj3Destroy(handle);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Skip if input is too small
    if (Size < 2) return 0;
    
    // Initialize handles
    tjhandle handle_old = init_decompress_handle();
    tjhandle handle_new = init_decompress_handle3();
    
    if (!handle_old || !handle_new) {
        cleanup_handle(handle_old);
        cleanup_handle3(handle_new);
        return 0;
    }
    
    // Try to get header information first
    int width = 0, height = 0, jpegSubsamp = 0;
    int header_result = tjDecompressHeader2(handle_old, (unsigned char*)Data, 
                                           (unsigned long)Size, &width, &height, &jpegSubsamp);
    
    // Limit dimensions to prevent excessive memory allocation
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    
    // Test tjDecompressToYUV
    if (header_result == 0) {
        unsigned long yuvSize = tjBufSizeYUV(width, height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char* dstBuf = (unsigned char*)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV(handle_old, (unsigned char*)Data, (unsigned long)Size,
                                 dstBuf, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tj3DecompressToYUVPlanes8
    {
        // Use reasonable dimensions for allocation
        int test_width = width;
        int test_height = height;
        
        // Calculate plane sizes
        int y_plane_size = test_width * test_height;
        int uv_plane_size = 0;
        
        if (jpegSubsamp == TJSAMP_420) {
            uv_plane_size = ((test_width + 1) / 2) * ((test_height + 1) / 2);
        } else if (jpegSubsamp == TJSAMP_422) {
            uv_plane_size = ((test_width + 1) / 2) * test_height;
        } else if (jpegSubsamp == TJSAMP_444) {
            uv_plane_size = test_width * test_height;
        } else if (jpegSubsamp == TJSAMP_GRAY) {
            uv_plane_size = 0;
        } else {
            uv_plane_size = test_width * test_height; // Conservative fallback
        }
        
        // Allocate planes
        unsigned char* y_plane = NULL;
        unsigned char* u_plane = NULL;
        unsigned char* v_plane = NULL;
        unsigned char* planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        if (y_plane_size > 0 && y_plane_size < 50 * 1024 * 1024) {
            y_plane = (unsigned char*)malloc(y_plane_size);
            if (y_plane) {
                planes[0] = y_plane;
                strides[0] = test_width;
                
                if (uv_plane_size > 0 && uv_plane_size < 25 * 1024 * 1024) {
                    u_plane = (unsigned char*)malloc(uv_plane_size);
                    v_plane = (unsigned char*)malloc(uv_plane_size);
                    if (u_plane && v_plane) {
                        planes[1] = u_plane;
                        planes[2] = v_plane;
                        strides[1] = (test_width + 1) / 2;
                        strides[2] = (test_width + 1) / 2;
                    }
                }
                
                tj3DecompressToYUVPlanes8(handle_new, Data, Size, planes, strides);
                
                free(y_plane);
                free(u_plane);
                free(v_plane);
            }
        }
    }
    
    // Test tjDecompressToYUVPlanes
    if (header_result == 0) {
        int test_width = width;
        int test_height = height;
        
        int y_plane_size = test_width * test_height;
        int uv_plane_size = 0;
        
        if (jpegSubsamp == TJSAMP_420) {
            uv_plane_size = ((test_width + 1) / 2) * ((test_height + 1) / 2);
        } else if (jpegSubsamp == TJSAMP_422) {
            uv_plane_size = ((test_width + 1) / 2) * test_height;
        } else if (jpegSubsamp == TJSAMP_444) {
            uv_plane_size = test_width * test_height;
        } else if (jpegSubsamp == TJSAMP_GRAY) {
            uv_plane_size = 0;
        }
        
        unsigned char* y_plane = NULL;
        unsigned char* u_plane = NULL;
        unsigned char* v_plane = NULL;
        unsigned char* planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        if (y_plane_size > 0 && y_plane_size < 50 * 1024 * 1024) {
            y_plane = (unsigned char*)malloc(y_plane_size);
            if (y_plane) {
                planes[0] = y_plane;
                strides[0] = test_width;
                
                if (uv_plane_size > 0 && uv_plane_size < 25 * 1024 * 1024) {
                    u_plane = (unsigned char*)malloc(uv_plane_size);
                    v_plane = (unsigned char*)malloc(uv_plane_size);
                    if (u_plane && v_plane) {
                        planes[1] = u_plane;
                        planes[2] = v_plane;
                        strides[1] = (test_width + 1) / 2;
                        strides[2] = (test_width + 1) / 2;
                    }
                }
                
                tjDecompressToYUVPlanes(handle_old, Data, (unsigned long)Size,
                                       planes, test_width, strides, test_height, 0);
                
                free(y_plane);
                free(u_plane);
                free(v_plane);
            }
        }
    }
    
    // Test tjDecompress2
    if (header_result == 0) {
        int test_width = width;
        int test_height = height;
        int pixel_format = TJPF_RGB;
        int pitch = test_width * tjPixelSize[pixel_format];
        
        unsigned long rgb_size = pitch * test_height;
        if (rgb_size > 0 && rgb_size < 100 * 1024 * 1024) {
            unsigned char* dstBuf = (unsigned char*)malloc(rgb_size);
            if (dstBuf) {
                tjDecompress2(handle_old, Data, (unsigned long)Size, dstBuf,
                             test_width, pitch, test_height, pixel_format, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tjDecompressToYUV2
    if (header_result == 0) {
        int test_width = width;
        int test_height = height;
        int align = 1;
        
        unsigned long yuvSize = tjBufSizeYUV2(test_width, align, test_height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) {
            unsigned char* dstBuf = (unsigned char*)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV2(handle_old, Data, (unsigned long)Size, dstBuf,
                                  test_width, align, test_height, 0);
                free(dstBuf);
            }
        }
    }
    
    // Clean up
    cleanup_handle(handle_old);
    cleanup_handle3(handle_new);
    
    return 0;
}