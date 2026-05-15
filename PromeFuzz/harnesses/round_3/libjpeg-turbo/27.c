// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_handle(tjhandle *handle) {
    if (*handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

static void cleanup_buffers(unsigned char **buffers, int count) {
    if (buffers) {
        for (int i = 0; i < count; i++) {
            free(buffers[i]);
            buffers[i] = NULL;
        }
        free(buffers);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Basic parameter extraction from fuzzer input
    if (Size < 4) return 0;
    
    // Use first 4 bytes for width/height (clamped to reasonable values)
    int width = (Data[0] % 64) + 1;  // 1-64 pixels
    int height = (Data[1] % 64) + 1; // 1-64 pixels
    
    // Use next bytes for parameters
    size_t offset = 2;
    if (offset >= Size) return 0;
    
    int pixelFormat = Data[offset] % 12;  // TJPF enum has 12 values
    offset++;
    if (offset >= Size) return 0;
    
    int subsamp = Data[offset] % 4;  // TJSAMP enum has 4 values
    offset++;
    if (offset >= Size) return 0;
    
    int flags = Data[offset];
    offset++;
    
    // Calculate source buffer size
    int pixelSize = 0;
    switch (pixelFormat) {
        case TJPF_GRAY: pixelSize = 1; break;
        case TJPF_RGB: pixelSize = 3; break;
        case TJPF_BGR: pixelSize = 3; break;
        case TJPF_RGBX: pixelSize = 4; break;
        case TJPF_BGRX: pixelSize = 4; break;
        case TJPF_XRGB: pixelSize = 4; break;
        case TJPF_XBGR: pixelSize = 4; break;
        case TJPF_RGBA: pixelSize = 4; break;
        case TJPF_BGRA: pixelSize = 4; break;
        case TJPF_ARGB: pixelSize = 4; break;
        case TJPF_ABGR: pixelSize = 4; break;
        default: pixelSize = 3; break;
    }
    
    int pitch = width * pixelSize;
    size_t srcSize = pitch * height;
    
    // Allocate and initialize source buffer
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) return 0;
    
    // Fill source buffer with fuzzer data (repeat if needed)
    for (size_t i = 0; i < srcSize; i++) {
        srcBuf[i] = Data[offset % Size];
        offset = (offset + 1) % Size;
    }
    
    // Create handles
    tjhandle compHandle = tjInitCompress();
    tjhandle decompHandle = tjInitDecompress();
    if (!compHandle || !decompHandle) {
        free(srcBuf);
        cleanup_handle(&compHandle);
        cleanup_handle(&decompHandle);
        return 0;
    }
    
    // Test tj3EncodeYUVPlanes8
    {
        // Set subsampling parameter
        tj3Set(compHandle, TJPARAM_SUBSAMP, subsamp);
        
        // Calculate plane sizes
        int planeWidth = width;
        int planeHeight = height;
        if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
            planeWidth = (width + 1) / 2;
        }
        if (subsamp == TJSAMP_420) {
            planeHeight = (height + 1) / 2;
        }
        
        // Allocate planes
        int numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char **dstPlanes = (unsigned char **)calloc(numPlanes, sizeof(unsigned char *));
        if (dstPlanes) {
            for (int i = 0; i < numPlanes; i++) {
                size_t planeSize = planeWidth * planeHeight;
                dstPlanes[i] = (unsigned char *)malloc(planeSize);
                if (dstPlanes[i]) {
                    memset(dstPlanes[i], 0, planeSize);
                }
            }
            
            // Call function
            tj3EncodeYUVPlanes8(compHandle, srcBuf, width, pitch, height,
                               pixelFormat, dstPlanes, NULL);
            
            // Cleanup
            cleanup_buffers(dstPlanes, numPlanes);
        }
    }
    
    // Test tjEncodeYUV
    {
        // Calculate destination buffer size
        size_t dstSize = tjBufSizeYUV(width, height, subsamp);
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (dstBuf) {
            tjEncodeYUV(compHandle, srcBuf, width, pitch, height,
                       pixelSize, dstBuf, subsamp, flags);
            free(dstBuf);
        }
    }
    
    // Test tjEncodeYUV3
    {
        size_t dstSize = tjBufSizeYUV2(width, 1, height, subsamp);
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (dstBuf) {
            tjEncodeYUV3(compHandle, srcBuf, width, pitch, height,
                        pixelFormat, dstBuf, 1, subsamp, flags);
            free(dstBuf);
        }
    }
    
    // Test tjDecodeYUV
    {
        // Create YUV source buffer
        size_t yuvSize = tjBufSizeYUV(width, height, subsamp);
        unsigned char *yuvSrc = (unsigned char *)malloc(yuvSize);
        if (yuvSrc) {
            // Fill with pattern
            for (size_t i = 0; i < yuvSize; i++) {
                yuvSrc[i] = (i * 17) % 256;
            }
            
            // Allocate destination buffer
            unsigned char *dstBuf = (unsigned char *)malloc(srcSize);
            if (dstBuf) {
                tjDecodeYUV(decompHandle, yuvSrc, 4, subsamp, dstBuf,
                           width, pitch, height, pixelFormat, flags);
                free(dstBuf);
            }
            free(yuvSrc);
        }
    }
    
    // Test tjEncodeYUVPlanes
    {
        int numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char **dstPlanes = (unsigned char **)calloc(numPlanes, sizeof(unsigned char *));
        if (dstPlanes) {
            for (int i = 0; i < numPlanes; i++) {
                int planeW = width;
                int planeH = height;
                
                if (i > 0) {  // U and V planes
                    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
                        planeW = (width + 1) / 2;
                    }
                    if (subsamp == TJSAMP_420) {
                        planeH = (height + 1) / 2;
                    }
                }
                
                dstPlanes[i] = (unsigned char *)malloc(planeW * planeH);
                if (dstPlanes[i]) {
                    memset(dstPlanes[i], 0, planeW * planeH);
                }
            }
            
            int strides[3] = {0, 0, 0};
            tjEncodeYUVPlanes(compHandle, srcBuf, width, pitch, height,
                             pixelFormat, dstPlanes, strides, subsamp, flags);
            
            cleanup_buffers(dstPlanes, numPlanes);
        }
    }
    
    // Test tjEncodeYUV2
    {
        size_t dstSize = tjBufSizeYUV2(width, pitch, height, subsamp);
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (dstBuf) {
            tjEncodeYUV2(compHandle, srcBuf, width, pitch, height,
                        pixelFormat, dstBuf, subsamp, flags);
            free(dstBuf);
        }
    }
    
    // Cleanup
    free(srcBuf);
    cleanup_handle(&compHandle);
    cleanup_handle(&decompHandle);
    
    return 0;
}