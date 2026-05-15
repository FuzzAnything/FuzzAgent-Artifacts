// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    if (size < 16) return 0;

    // Initialize parameters
    int width = 16;
    int height = 16;
    int pitch = width * tjPixelSize[TJPF_RGB];
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    int align = 4;
    int flags = 0;

    // Create handles
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tjDestroy(comp_handle);
        if (decomp_handle) tjDestroy(decomp_handle);
        return 0;
    }

    // Allocate source and destination buffers
    unsigned char *srcBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
    if (!srcBuf) {
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }

    // Fill source buffer with fuzzer data
    size_t srcSize = width * height * tjPixelSize[pixelFormat];
    size_t copySize = size < srcSize ? size : srcSize;
    memcpy(srcBuf, data, copySize);

    // Test tjEncodeYUVPlanes
    {
        int strides[3] = {0, 0, 0};
        unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
        
        // Calculate plane sizes
        int planeSizes[3];
        for (int i = 0; i < 3; i++) {
            planeSizes[i] = tjPlaneSizeYUV(i, width, strides[i] ? strides[i] : width,
                                          height, subsamp);
            if (planeSizes[i] > 0) {
                dstPlanes[i] = (unsigned char *)malloc(planeSizes[i]);
                if (dstPlanes[i]) {
                    // Initialize with zeros to avoid uninitialized reads
                    memset(dstPlanes[i], 0, planeSizes[i]);
                }
            }
        }
        
        tjEncodeYUVPlanes(comp_handle, srcBuf, width, pitch, height,
                         pixelFormat, dstPlanes, strides, subsamp, flags);
        
        for (int i = 0; i < 3; i++) {
            free(dstPlanes[i]);
        }
    }

    // Test tj3DecodeYUV8
    {
        size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
        unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
        
        if (yuvBuf && dstBuf) {
            // Fill YUV buffer with fuzzer data
            size_t yuvCopySize = size < yuvSize ? size : yuvSize;
            memcpy(yuvBuf, data, yuvCopySize);
            
            // Initialize remaining bytes if needed
            if (yuvCopySize < yuvSize) {
                memset(yuvBuf + yuvCopySize, 0, yuvSize - yuvCopySize);
            }
            
            // Set subsampling parameter
            tj3Set(decomp_handle, TJPARAM_SUBSAMP, subsamp);
            
            tj3DecodeYUV8(decomp_handle, yuvBuf, align, dstBuf,
                         width, pitch, height, pixelFormat);
        }
        
        free(yuvBuf);
        free(dstBuf);
    }

    // Test tjDecompressToYUVPlanes
    {
        write_dummy_file(data, size);
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            unsigned long jpegSize = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            unsigned char *jpegBuf = (unsigned char *)malloc(jpegSize);
            if (jpegBuf) {
                fread(jpegBuf, 1, jpegSize, f);
                
                int strides[3] = {0, 0, 0};
                unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
                
                // Calculate plane sizes
                for (int i = 0; i < 3; i++) {
                    int planeWidth = (i == 0) ? width : width / 2;
                    int planeHeight = (i == 0) ? height : height / 2;
                    dstPlanes[i] = (unsigned char *)malloc(planeWidth * planeHeight);
                    if (dstPlanes[i]) {
                        memset(dstPlanes[i], 0, planeWidth * planeHeight);
                    }
                }
                
                tjDecompressToYUVPlanes(decomp_handle, jpegBuf, jpegSize,
                                       dstPlanes, width, strides, height, flags);
                
                for (int i = 0; i < 3; i++) {
                    free(dstPlanes[i]);
                }
                free(jpegBuf);
            }
            fclose(f);
        }
        cleanup_dummy_file();
    }

    // Test tjDecodeYUV
    {
        unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
        unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
        
        if (yuvBuf && dstBuf) {
            size_t yuvCopySize = size < yuvSize ? size : yuvSize;
            memcpy(yuvBuf, data, yuvCopySize);
            
            // Initialize remaining bytes if needed
            if (yuvCopySize < yuvSize) {
                memset(yuvBuf + yuvCopySize, 0, yuvSize - yuvCopySize);
            }
            
            tjDecodeYUV(decomp_handle, yuvBuf, align, subsamp, dstBuf,
                       width, pitch, height, pixelFormat, flags);
        }
        
        free(yuvBuf);
        free(dstBuf);
    }

    // Test tjDecodeYUVPlanes
    {
        const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
        
        // Allocate and fill plane buffers
        for (int i = 0; i < 3; i++) {
            int planeWidth = (i == 0) ? width : width / 2;
            int planeHeight = (i == 0) ? height : height / 2;
            size_t planeSize = planeWidth * planeHeight;
            srcPlanes[i] = (unsigned char *)malloc(planeSize);
            if (srcPlanes[i]) {
                size_t copySize = size < planeSize ? size : planeSize;
                memcpy((void *)srcPlanes[i], data, copySize);
                // Initialize remaining bytes
                if (copySize < planeSize) {
                    memset((void *)srcPlanes[i] + copySize, 0, planeSize - copySize);
                }
            }
        }
        
        if (dstBuf) {
            tjDecodeYUVPlanes(decomp_handle, srcPlanes, strides, subsamp,
                             dstBuf, width, pitch, height, pixelFormat, flags);
        }
        
        for (int i = 0; i < 3; i++) {
            free((void *)srcPlanes[i]);
        }
        free(dstBuf);
    }

    // Test tj3DecodeYUVPlanes8 - FIXED: Correct argument order for tj3YUVPlaneSize
    {
        const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
        
        // Set subsampling parameter
        tj3Set(decomp_handle, TJPARAM_SUBSAMP, subsamp);
        
        // Allocate and fill plane buffers
        for (int i = 0; i < 3; i++) {
            // Correct argument order: componentID, width, stride, height, subsamp
            size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
            srcPlanes[i] = (unsigned char *)malloc(planeSize);
            if (srcPlanes[i]) {
                size_t copySize = size < planeSize ? size : planeSize;
                memcpy((void *)srcPlanes[i], data, copySize);
                // Initialize remaining bytes to avoid reading uninitialized memory
                if (copySize < planeSize) {
                    memset((void *)srcPlanes[i] + copySize, 0, planeSize - copySize);
                }
            }
        }
        
        if (dstBuf) {
            tj3DecodeYUVPlanes8(decomp_handle, srcPlanes, strides, dstBuf,
                               width, pitch, height, pixelFormat);
        }
        
        for (int i = 0; i < 3; i++) {
            free((void *)srcPlanes[i]);
        }
        free(dstBuf);
    }

    // Cleanup
    free(srcBuf);
    tjDestroy(comp_handle);
    tjDestroy(decomp_handle);
    
    return 0;
}