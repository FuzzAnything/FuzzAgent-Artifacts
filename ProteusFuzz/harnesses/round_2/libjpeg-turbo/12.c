// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <turbojpeg.h>

static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    return val;
}

static int clamp_int(int val, int min_val, int max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    size_t offset = 0;
    
    // Extract parameters from fuzz data
    int componentID = get_int_from_data(Data, Size, &offset);
    int width = get_int_from_data(Data, Size, &offset);
    int stride = get_int_from_data(Data, Size, &offset);
    int height = get_int_from_data(Data, Size, &offset);
    int subsamp = get_int_from_data(Data, Size, &offset);
    int align = get_int_from_data(Data, Size, &offset);
    
    // Clamp values to reasonable ranges to avoid immediate crashes from extreme values
    // but still allow negative/zero values to test error handling
    width = clamp_int(width, -10000, 10000);
    height = clamp_int(height, -10000, 10000);
    stride = clamp_int(stride, -10000, 10000);
    align = clamp_int(align, -10000, 10000);
    componentID = clamp_int(componentID, -5, 10);
    subsamp = clamp_int(subsamp, -5, 10);

    // Test tjPlaneSizeYUV
    unsigned long planeSize1 = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    if (planeSize1 == (unsigned long)-1) {
        // Error case handled
    }

    // Test tj3YUVPlaneSize
    size_t planeSize2 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    if (planeSize2 == 0) {
        // Error case handled
    }

    // Test tjBufSizeYUV
    unsigned long bufSize1 = tjBufSizeYUV(width, height, subsamp);
    if (bufSize1 == (unsigned long)-1) {
        // Error case handled
    }

    // Test tjBufSizeYUV2
    unsigned long bufSize2 = tjBufSizeYUV2(width, align, height, subsamp);
    if (bufSize2 == (unsigned long)-1) {
        // Error case handled
    }

    // Test TJBUFSIZEYUV
    unsigned long bufSize3 = TJBUFSIZEYUV(width, height, subsamp);
    if (bufSize3 == (unsigned long)-1) {
        // Error case handled
    }

    // Test tjCompress
    // Only if dimensions are positive and reasonable
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        tjhandle handle = tjInitCompress();
        if (handle) {
            // Calculate source buffer size
            int pixelSize = 3; // RGB
            unsigned long srcSize = (unsigned long)width * height * pixelSize;
            unsigned char *srcBuf = NULL;
            
            if (srcSize > 0 && srcSize < (size_t)100000000) { // Limit allocation
                srcBuf = (unsigned char *)malloc(srcSize);
                if (srcBuf) {
                    // Fill with fuzz data if available, otherwise zeros
                    if (Size > srcSize) {
                        memcpy(srcBuf, Data, srcSize);
                    } else {
                        memset(srcBuf, 0, srcSize);
                    }
                }
            }

            // Calculate destination buffer size
            unsigned long jpegSize = 0;
            unsigned long maxJpegSize = tjBufSize(width, height, subsamp);
            if (maxJpegSize == (unsigned long)-1) {
                maxJpegSize = 1024 * 1024; // Fallback
            }
            
            unsigned char *dstBuf = NULL;
            if (maxJpegSize > 0 && maxJpegSize < (size_t)100000000) {
                dstBuf = (unsigned char *)malloc(maxJpegSize);
            }

            if (srcBuf && dstBuf) {
                int jpegSubsamp = clamp_int(subsamp, 0, 3);
                int jpegQual = clamp_int(get_int_from_data(Data, Size, &offset), 0, 100);
                int flags = 0;
                
                // Test compression
                int ret = tjCompress(handle, srcBuf, width, width * pixelSize, height, pixelSize, 
                                    dstBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
                // Return value 0 on success, -1 on error
            }

            if (srcBuf) free(srcBuf);
            if (dstBuf) free(dstBuf);
            tjDestroy(handle);
        }
    }

    return 0;
}