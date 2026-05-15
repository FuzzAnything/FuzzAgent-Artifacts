// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle init_handle_v1(void) {
    return tjInitDecompress();
}

static tjhandle init_handle_v3(void) {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_handle_v1(tjhandle handle) {
    if (handle) tjDestroy(handle);
}

static void cleanup_handle_v3(tjhandle handle) {
    if (handle) tj3Destroy(handle);
}

static int get_uv_width(int width, int subsamp) {
    switch (subsamp) {
        case TJSAMP_420:
        case TJSAMP_422:
        case TJSAMP_440:
        case TJSAMP_411:
            return (width + 1) / 2;
        case TJSAMP_444:
            return width;
        default:
            return 0;
    }
}

static int get_uv_height(int height, int subsamp) {
    switch (subsamp) {
        case TJSAMP_420:
            return (height + 1) / 2;
        case TJSAMP_422:
        case TJSAMP_411:
            return height;
        case TJSAMP_444:
        case TJSAMP_440:
            return (subsamp == TJSAMP_440) ? (height + 1) / 2 : height;
        default:
            return 0;
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    // Initialize handles
    tjhandle handle_v1 = init_handle_v1();
    tjhandle handle_v3 = init_handle_v3();
    
    if (!handle_v1 || !handle_v3) {
        cleanup_handle_v1(handle_v1);
        cleanup_handle_v3(handle_v3);
        return 0;
    }
    
    // Test tjDecompressHeader2
    int width = 0, height = 0, jpegSubsamp = 0;
    int header_result = tjDecompressHeader2(handle_v1, (unsigned char*)Data, 
                                           (unsigned long)Size, &width, 
                                           &height, &jpegSubsamp);
    
    // Cap dimensions to avoid excessive memory allocation
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Test tjDecompressToYUV
    if (header_result == 0 && width > 0 && height > 0) {
        unsigned long yuv_size = tjBufSizeYUV(width, height, jpegSubsamp);
        if (yuv_size > 0 && yuv_size < 100 * 1024 * 1024) {
            unsigned char* dstBuf = (unsigned char*)malloc(yuv_size);
            if (dstBuf) {
                tjDecompressToYUV(handle_v1, (unsigned char*)Data, 
                                 (unsigned long)Size, dstBuf, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tjDecompressToYUV2 with different alignments
    if (header_result == 0 && width > 0 && height > 0) {
        int alignments[] = {1, 4, 8};
        for (int i = 0; i < 3; i++) {
            unsigned long yuv_size = tjBufSizeYUV2(width, alignments[i], 
                                                   height, jpegSubsamp);
            if (yuv_size > 0 && yuv_size < 100 * 1024 * 1024) {
                unsigned char* dstBuf = (unsigned char*)malloc(yuv_size);
                if (dstBuf) {
                    tjDecompressToYUV2(handle_v1, Data, (unsigned long)Size,
                                      dstBuf, width, alignments[i], 
                                      height, 0);
                    free(dstBuf);
                }
            }
        }
    }
    
    // Test tjDecompressToYUVPlanes
    if (header_result == 0 && width > 0 && height > 0) {
        int strides[3] = {0, 0, 0};
        unsigned char* planes[3] = {NULL, NULL, NULL};
        int plane_count = (jpegSubsamp == TJSAMP_GRAY) ? 1 : 3;
        
        for (int i = 0; i < plane_count; i++) {
            int plane_width = width;
            int plane_height = height;
            if (i > 0 && jpegSubsamp != TJSAMP_GRAY) {
                plane_width = get_uv_width(width, jpegSubsamp);
                plane_height = get_uv_height(height, jpegSubsamp);
            }
            strides[i] = plane_width;
            planes[i] = (unsigned char*)malloc(plane_width * plane_height);
        }
        
        if (planes[0] && (plane_count == 1 || (planes[1] && planes[2]))) {
            tjDecompressToYUVPlanes(handle_v1, Data, (unsigned long)Size,
                                   planes, width, strides, height, 0);
        }
        
        for (int i = 0; i < plane_count; i++) {
            free(planes[i]);
        }
    }
    
    // Test tj3DecompressToYUV8
    if (handle_v3) {
        int alignments[] = {1, 4, 8};
        for (int i = 0; i < 3; i++) {
            size_t yuv_size = tj3YUVBufSize(width, alignments[i], 
                                           height, jpegSubsamp);
            if (yuv_size > 0 && yuv_size < 100 * 1024 * 1024) {
                unsigned char* dstBuf = (unsigned char*)malloc(yuv_size);
                if (dstBuf) {
                    tj3DecompressToYUV8(handle_v3, Data, Size, dstBuf, 
                                       alignments[i]);
                    free(dstBuf);
                }
            }
        }
    }
    
    // Test tj3DecompressToYUVPlanes8
    if (handle_v3 && header_result == 0 && width > 0 && height > 0) {
        int strides[3] = {0, 0, 0};
        unsigned char* planes[3] = {NULL, NULL, NULL};
        int plane_count = (jpegSubsamp == TJSAMP_GRAY) ? 1 : 3;
        
        for (int i = 0; i < plane_count; i++) {
            int plane_width = width;
            int plane_height = height;
            if (i > 0 && jpegSubsamp != TJSAMP_GRAY) {
                plane_width = get_uv_width(width, jpegSubsamp);
                plane_height = get_uv_height(height, jpegSubsamp);
            }
            strides[i] = plane_width;
            planes[i] = (unsigned char*)malloc(plane_width * plane_height);
        }
        
        if (planes[0] && (plane_count == 1 || (planes[1] && planes[2]))) {
            tj3DecompressToYUVPlanes8(handle_v3, Data, Size, planes, strides);
        }
        
        for (int i = 0; i < plane_count; i++) {
            free(planes[i]);
        }
    }
    
    // Cleanup
    cleanup_handle_v1(handle_v1);
    cleanup_handle_v3(handle_v3);
    
    return 0;
}