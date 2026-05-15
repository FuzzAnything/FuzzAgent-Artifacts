// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
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

static void cleanup_jpeg_buffer(unsigned char **jpegBuf, size_t *jpegSize, int use_v3) {
    if (jpegBuf && *jpegBuf) {
        if (use_v3) {
            tj3Free(*jpegBuf);
        } else {
            tjFree(*jpegBuf);
        }
        *jpegBuf = NULL;
    }
    if (jpegSize) *jpegSize = 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Parse parameters from input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int align = 1 << (Data[4] & 0x07);  // Power of 2: 1,2,4,8,16,32,64,128
    int subsamp = Data[5] % 6;          // 0-5 for TJSAMP_444,422,420,GRAY,440,411
    int jpegQual = Data[6] % 101;       // 0-100
    int flags = Data[7] & 0x3F;         // Limited flags
    int stride_scale = (Data[8] & 0x0F) + 1;
    
    // Limit dimensions to avoid excessive memory usage
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    if (align == 0 || align > 128) align = 1;

    // Create handle
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, jpegQual);

    // Calculate required buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    
    if (yuvBufSize == 0 || jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Prepare source buffer (use circular input to fill)
    unsigned char *srcBuf = (unsigned char *)malloc(yuvBufSize);
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }
    for (size_t i = 0; i < yuvBufSize; i++) {
        srcBuf[i] = Data[i % Size];
    }

    // Test tj3CompressFromYUV8
    unsigned char *jpegBuf1 = NULL;
    size_t jpegSize1 = jpegBufSize;
    if (tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf1, &jpegSize1) == 0) {
        // Compression succeeded
    }
    cleanup_jpeg_buffer(&jpegBuf1, &jpegSize1, 1);

    // Test tjCompressFromYUV (legacy API)
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = jpegBufSize;
    if (tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp, 
                         &jpegBuf2, &jpegSize2, jpegQual, flags) == 0) {
        // Compression succeeded
    }
    cleanup_jpeg_buffer(&jpegBuf2, (size_t*)&jpegSize2, 0);

    // Test planar compression functions
    const unsigned char *srcPlanes[3];
    int strides[3];
    
    // Calculate plane sizes and set up pointers
    size_t yPlaneSize = width * height;
    size_t uvWidth = width;
    size_t uvHeight = height;
    
    switch (subsamp) {
        case TJSAMP_420:
            uvWidth = (width + 1) / 2;
            uvHeight = (height + 1) / 2;
            break;
        case TJSAMP_422:
            uvWidth = (width + 1) / 2;
            break;
        case TJSAMP_440:
            uvHeight = (height + 1) / 2;
            break;
        case TJSAMP_411:
            uvWidth = (width + 3) / 4;
            break;
        case TJSAMP_GRAY:
            // For grayscale, only Y plane exists
            uvWidth = 0;
            uvHeight = 0;
            break;
    }
    
    size_t uPlaneSize = uvWidth * uvHeight;
    size_t vPlaneSize = uvWidth * uvHeight;
    
    // Ensure we don't overflow the allocated buffer
    size_t totalPlaneSize = yPlaneSize + uPlaneSize + vPlaneSize;
    if (totalPlaneSize > yuvBufSize) {
        free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Set up plane pointers within the allocated buffer
    srcPlanes[0] = srcBuf;  // Y plane
    srcPlanes[1] = srcBuf + yPlaneSize;  // U plane
    srcPlanes[2] = srcBuf + yPlaneSize + uPlaneSize;  // V plane
    
    // For grayscale, set U and V planes to NULL
    if (subsamp == TJSAMP_GRAY) {
        srcPlanes[1] = NULL;
        srcPlanes[2] = NULL;
    }
    
    // Calculate strides
    strides[0] = width * stride_scale;
    strides[1] = uvWidth * stride_scale;
    strides[2] = uvWidth * stride_scale;
    
    // Ensure strides are non-negative
    if (strides[0] < 0 || strides[1] < 0 || strides[2] < 0) {
        free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Test tjCompressFromYUVPlanes
    unsigned char *jpegBuf3 = NULL;
    unsigned long jpegSize3 = jpegBufSize;
    if (tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp,
                               &jpegBuf3, &jpegSize3, jpegQual, flags) == 0) {
        // Compression succeeded
    }
    cleanup_jpeg_buffer(&jpegBuf3, (size_t*)&jpegSize3, 0);

    // Test tj3CompressFromYUVPlanes8
    unsigned char *jpegBuf4 = NULL;
    size_t jpegSize4 = jpegBufSize;
    if (tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, 
                                 &jpegBuf4, &jpegSize4) == 0) {
        // Compression succeeded
    }
    cleanup_jpeg_buffer(&jpegBuf4, &jpegSize4, 1);

    // Test with file input
    write_dummy_file(Data, Size);
    
    // Cleanup
    free(srcBuf);
    tj3Destroy(handle);
    
    return 0;
}