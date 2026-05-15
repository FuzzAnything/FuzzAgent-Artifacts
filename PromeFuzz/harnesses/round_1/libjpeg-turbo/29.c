// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void cleanup_jpeg_buffer(unsigned char **jpegBuf, size_t *jpegSize) {
    if (*jpegBuf) {
        free(*jpegBuf);
        *jpegBuf = NULL;
    }
    if (jpegSize) {
        *jpegSize = 0;
    }
}

static void cleanup_jpeg_buffer_old(unsigned char **jpegBuf, unsigned long *jpegSize) {
    if (*jpegBuf) {
        free(*jpegBuf);
        *jpegBuf = NULL;
    }
    if (jpegSize) {
        *jpegSize = 0;
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize handles
    tjhandle handle3 = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_old = tjInitCompress();
    
    if (!handle3 || !handle_old) {
        if (handle3) tj3Destroy(handle3);
        if (handle_old) tjDestroy(handle_old);
        return 0;
    }

    // Extract parameters from fuzzer input
    // Use first bytes for dimensions and parameters
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 5;  // 0-4 for TJSAMP_* values
    int pixelFormat = Data[5] % 12;  // 0-11 for TJPF_* values
    int jpegQual = Data[6] % 101;  // 0-100
    int flags = Data[7] & 0xFF;
    int align = 1 << (Data[8] % 4);  // 1, 2, 4, or 8
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    
    // Ensure align is valid (power of 2, at least 1)
    if (align < 1) align = 1;
    if (align > 16) align = 16;
    // Ensure align is power of 2
    if ((align & (align - 1)) != 0) align = 1;

    // Adjust dimensions for subsampling requirements
    // For 4:2:0 and 4:2:2, width must be multiple of 2
    // For 4:2:0, height must be multiple of 2
    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
        if (width % 2 != 0) width++;
    }
    if (subsamp == TJSAMP_420) {
        if (height % 2 != 0) height++;
    }
    if (subsamp == TJSAMP_440) {
        if (height % 2 != 0) height++;
    }

    // Calculate buffer sizes using old API for old functions
    unsigned long yuvBufSizeOld = 0;
    if (width > 0 && height > 0 && align > 0 && subsamp >= 0 && subsamp <= 4) {
        yuvBufSizeOld = tjBufSizeYUV2(width, align, height, subsamp);
    }
    
    size_t yuvBufSizeNew = 0;
    if (width > 0 && height > 0 && align > 0 && subsamp >= 0 && subsamp <= 4) {
        yuvBufSizeNew = tj3YUVBufSize(width, align, height, subsamp);
    }
    
    // Use the larger of the two buffer sizes to ensure we have enough memory
    size_t yuvBufSize = (yuvBufSizeNew > (size_t)yuvBufSizeOld) ? yuvBufSizeNew : (size_t)yuvBufSizeOld;
    
    // Calculate RGB buffer size
    size_t rgbBufSize = 0;
    if (width > 0 && height > 0 && pixelFormat >= 0 && pixelFormat <= 11) {
        rgbBufSize = (size_t)(width * height * tjPixelSize[pixelFormat]);
    }
    
    // Limit buffer sizes to avoid excessive memory usage
    if (yuvBufSize > 2 * 1024 * 1024) yuvBufSize = 2 * 1024 * 1024;
    if (rgbBufSize > 2 * 1024 * 1024) rgbBufSize = 2 * 1024 * 1024;
    
    // Ensure minimum buffer sizes
    if (yuvBufSize < 16) yuvBufSize = 16;
    if (rgbBufSize < 16) rgbBufSize = 16;

    // Allocate and initialize source buffers
    unsigned char *srcBuf = NULL;
    unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    if (yuvBufSize > 0) {
        srcBuf = (unsigned char *)malloc(yuvBufSize);
        if (srcBuf && Size > 16) {
            size_t copySize = Size - 16;
            if (copySize > yuvBufSize) copySize = yuvBufSize;
            // Initialize the entire buffer
            memset(srcBuf, 0, yuvBufSize);
            memcpy(srcBuf, Data + 16, copySize);
        } else if (srcBuf) {
            // Initialize with zeros if no data
            memset(srcBuf, 0, yuvBufSize);
        }
        
        // For planar YUV, we need to set up planes
        // Calculate plane sizes using the new API
        size_t ySize = 0, uSize = 0, vSize = 0;
        if (width > 0 && height > 0 && subsamp >= 0 && subsamp <= 4) {
            ySize = tj3YUVPlaneSize(width, 0, height, 0, subsamp);
            uSize = tj3YUVPlaneSize(width, 0, height, 1, subsamp);
            vSize = tj3YUVPlaneSize(width, 0, height, 2, subsamp);
        }
        
        // Ensure minimum plane sizes
        if (ySize < 1) ySize = 1;
        if (uSize < 1) uSize = 1;
        if (vSize < 1) vSize = 1;
        
        // Limit plane sizes
        if (ySize > 512 * 1024) ySize = 512 * 1024;
        if (uSize > 256 * 1024) uSize = 256 * 1024;
        if (vSize > 256 * 1024) vSize = 256 * 1024;
        
        if (ySize > 0) {
            srcPlanes[0] = (unsigned char *)malloc(ySize);
            if (srcPlanes[0]) {
                memset(srcPlanes[0], 0, ySize);
                if (Size > 16) {
                    size_t copySize = Size - 16;
                    if (copySize > ySize) copySize = ySize;
                    memcpy(srcPlanes[0], Data + 16, copySize);
                }
            }
        }
        
        if (uSize > 0) {
            srcPlanes[1] = (unsigned char *)malloc(uSize);
            if (srcPlanes[1]) {
                memset(srcPlanes[1], 0, uSize);
                if (Size > 16 + ySize) {
                    size_t copySize = Size - 16 - ySize;
                    if (copySize > uSize) copySize = uSize;
                    memcpy(srcPlanes[1], Data + 16 + ySize, copySize);
                }
            }
        }
        
        if (vSize > 0) {
            srcPlanes[2] = (unsigned char *)malloc(vSize);
            if (srcPlanes[2]) {
                memset(srcPlanes[2], 0, vSize);
                if (Size > 16 + ySize + uSize) {
                    size_t copySize = Size - 16 - ySize - uSize;
                    if (copySize > vSize) copySize = vSize;
                    memcpy(srcPlanes[2], Data + 16 + ySize + uSize, copySize);
                }
            }
        }
        
        // Set strides - ensure they're reasonable
        strides[0] = width;
        strides[1] = (width + 1) / 2;  // Handle odd widths
        strides[2] = (width + 1) / 2;
    }

    // Allocate RGB buffer for tjCompress2
    unsigned char *rgbBuf = NULL;
    if (rgbBufSize > 0) {
        rgbBuf = (unsigned char *)malloc(rgbBufSize);
        if (rgbBuf) {
            memset(rgbBuf, 0, rgbBufSize);
            if (Size > 16) {
                size_t copySize = Size - 16;
                if (copySize > rgbBufSize) copySize = rgbBufSize;
                memcpy(rgbBuf, Data + 16, copySize);
            }
        }
    }

    // Test tj3CompressFromYUVPlanes8
    if (srcPlanes[0] && width > 0 && height > 0) {
        unsigned char *jpegBuf1 = NULL;
        size_t jpegSize1 = 0;
        
        tj3Set(handle3, TJPARAM_SUBSAMP, subsamp);
        // Ensure width and height are valid for the subsampling
        if (width >= 1 && height >= 1) {
            int result1 = tj3CompressFromYUVPlanes8(handle3, srcPlanes, width, strides, 
                                                  height, &jpegBuf1, &jpegSize1);
            
            // Check error code
            int errCode1 = tj3GetErrorCode(handle3);
            (void)errCode1;  // Use variable to avoid unused warning
        }
        
        cleanup_jpeg_buffer(&jpegBuf1, &jpegSize1);
    }

    // Test tj3CompressFromYUV8
    if (srcBuf && width > 0 && height > 0 && align > 0) {
        unsigned char *jpegBuf2 = NULL;
        size_t jpegSize2 = 0;
        
        tj3Set(handle3, TJPARAM_SUBSAMP, subsamp);
        // Ensure width and height are valid
        if (width >= 1 && height >= 1 && align >= 1) {
            int result2 = tj3CompressFromYUV8(handle3, srcBuf, width, align, height,
                                             &jpegBuf2, &jpegSize2);
            
            // Check error code
            int errCode2 = tj3GetErrorCode(handle3);
            (void)errCode2;
        }
        
        cleanup_jpeg_buffer(&jpegBuf2, &jpegSize2);
    }

    // Test tjCompressFromYUV - use old buffer size calculation
    if (srcBuf && yuvBufSizeOld > 0 && width > 0 && height > 0 && align > 0) {
        unsigned char *jpegBuf3 = NULL;
        unsigned long jpegSize3 = 0;
        
        // Ensure parameters are valid for old API
        if (width >= 1 && height >= 1 && align >= 1 && subsamp >= 0 && subsamp <= 4) {
            // Create a properly sized buffer for the old API
            unsigned char *srcBufOld = (unsigned char *)malloc(yuvBufSizeOld);
            if (srcBufOld) {
                memset(srcBufOld, 0, yuvBufSizeOld);
                size_t copySize = yuvBufSizeOld;
                if (copySize > yuvBufSize) copySize = yuvBufSize;
                if (srcBuf && copySize > 0) {
                    memcpy(srcBufOld, srcBuf, copySize);
                }
                
                int result3 = tjCompressFromYUV(handle_old, srcBufOld, width, align, height,
                                               subsamp, &jpegBuf3, &jpegSize3, jpegQual, flags);
                
                // Check error code
                int errCode3 = tjGetErrorCode(handle_old);
                (void)errCode3;
                
                free(srcBufOld);
            }
        }
        
        cleanup_jpeg_buffer_old(&jpegBuf3, &jpegSize3);
    }

    // Test tjCompress2
    if (rgbBuf && rgbBufSize > 0 && width > 0 && height > 0) {
        unsigned char *jpegBuf4 = NULL;
        unsigned long jpegSize4 = 0;
        
        // Ensure valid parameters
        if (width >= 1 && height >= 1 && pixelFormat >= 0 && pixelFormat <= 11) {
            int pitch = width * tjPixelSize[pixelFormat];
            if (pitch > 0) {
                int result4 = tjCompress2(handle_old, rgbBuf, width, pitch, height,
                                         pixelFormat, &jpegBuf4, &jpegSize4,
                                         subsamp, jpegQual, flags);
                
                // Check error code
                int errCode4 = tjGetErrorCode(handle_old);
                (void)errCode4;
            }
        }
        
        cleanup_jpeg_buffer_old(&jpegBuf4, &jpegSize4);
    }

    // Cleanup
    if (srcBuf) free(srcBuf);
    for (int i = 0; i < 3; i++) {
        if (srcPlanes[i]) free(srcPlanes[i]);
    }
    if (rgbBuf) free(rgbBuf);
    
    tj3Destroy(handle3);
    tjDestroy(handle_old);
    
    return 0;
}