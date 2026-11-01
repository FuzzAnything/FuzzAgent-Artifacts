// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzzer_init(tjhandle *handle) {
    if (!handle) return -1;
    *handle = tjInitCompress();
    if (!*handle) return -1;
    return 0;
}

static void fuzzer_cleanup(tjhandle *handle) {
    if (handle && *handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

static int extract_int(const uint8_t *data, size_t size, size_t *offset, int default_val) {
    if (*offset + 4 > size) {
        *offset = 0;
        return default_val;
    }
    int val = *(int*)(data + *offset);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned char *yuvBuf = NULL;
    const unsigned char *srcPlanes[3];
    int strides[3];
    int ret = 0;

    size_t offset = 0;
    int width = extract_int(Data, Size, &offset, 64);
    int height = extract_int(Data, Size, &offset, 64);
    int subsamp = extract_int(Data, Size, &offset, TJSAMP_420);
    int quality = extract_int(Data, Size, &offset, 75);
    int flags = extract_int(Data, Size, &offset, 0);
    int align = extract_int(Data, Size, &offset, 1);
    int componentID = extract_int(Data, Size, &offset, 0);
    int stride = extract_int(Data, Size, &offset, 0);

    // Clamp values to reasonable ranges to avoid immediate crashes from huge allocations
    if (width < 0) width = 64;
    if (height < 0) height = 64;
    if (width > 8192) width = 8192;
    if (height > 8192) height = 8192;
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    if (align < 1) align = 1;
    if (align > 16) align = 16;

    // Test 1: TJBUFSIZE
    unsigned long bufsize = TJBUFSIZE(width, height);
    if (bufsize == (unsigned long)-1) {
        // Error case, valid
    }

    // Test 2: tj3YUVPlaneSize
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    if (planeSize == 0) {
        // Error case, valid
    }

    // Test 3: tjBufSizeYUV2
    unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuvBufSize == (unsigned long)-1) {
        // Error case, valid
    } else {
        yuvBuf = (unsigned char *)malloc(yuvBufSize);
        if (yuvBuf) {
            memset(yuvBuf, 0, yuvBufSize);
            // Fill with some data from input if available
            size_t fillSize = (Size > yuvBufSize) ? yuvBufSize : Size;
            if (fillSize > 0) {
                memcpy(yuvBuf, Data, fillSize);
            }
        }
    }

    // Test 4: tjBufSize
    unsigned long jpegBufSize = tjBufSize(width, height, subsamp);
    if (jpegBufSize == (unsigned long)-1) {
        // Error case, valid
    } else {
        jpegBuf = (unsigned char *)malloc(jpegBufSize);
        if (jpegBuf) {
            memset(jpegBuf, 0, jpegBufSize);
            jpegSize = jpegBufSize;
        }
    }

    // Test 5: tjCompress (RGB mode)
    if (fuzzer_init(&handle) == 0) {
        int pitch = width * 3; // RGB
        int pixelSize = 3;
        
        // Calculate RGB buffer size safely
        size_t rgbBufferSize = (size_t)width * (size_t)height * 3;
        // Prevent overflow or excessive allocation
        if (rgbBufferSize > 100 * 1024 * 1024) {
            rgbBufferSize = 0; 
        }
        
        unsigned char *srcRGB = NULL;
        if (rgbBufferSize > 0) {
            srcRGB = (unsigned char *)malloc(rgbBufferSize);
        }
        
        if (srcRGB) {
            memset(srcRGB, 0, rgbBufferSize);
            // Copy data safely: ensure we don't read past 'Data' or write past 'srcRGB'
            size_t copySize = (Size < rgbBufferSize) ? Size : rgbBufferSize;
            if (copySize > 0) {
                memcpy(srcRGB, Data, copySize);
            }

            unsigned char *dstBuf = (unsigned char *)malloc(jpegBufSize);
            unsigned long compressedSize = 0;
            if (dstBuf) {
                ret = tjCompress(handle, srcRGB, width, pitch, height, pixelSize,
                                 dstBuf, &compressedSize, subsamp, quality, flags);
                free(dstBuf);
            }
            free(srcRGB);
        }
        fuzzer_cleanup(&handle);
    }

    // Test 6: tj3CompressFromYUVPlanes8
    if (fuzzer_init(&handle) == 0) {
        if (yuvBuf) {
            // Setup planes for YUV
            size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
            size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
            size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
            
            if (ySize > 0 && uSize > 0 && vSize > 0) {
                // Check if the allocated buffer is large enough to hold all planes
                // tjBufSizeYUV2 returns the size for a contiguous buffer containing all planes
                if (yuvBufSize >= (ySize + uSize + vSize)) {
                    srcPlanes[0] = yuvBuf;
                    srcPlanes[1] = yuvBuf + ySize;
                    srcPlanes[2] = yuvBuf + ySize + uSize;
                    
                    strides[0] = 0; // Auto calculate
                    strides[1] = 0;
                    strides[2] = 0;

                    unsigned char *jpegOut = NULL;
                    size_t jpegOutSize = 0;
                    
                    ret = tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height,
                                                    &jpegOut, &jpegOutSize);
                    
                    if (jpegOut && jpegOut != jpegBuf) {
                        free(jpegOut);
                    }
                }
            }
        }
        fuzzer_cleanup(&handle);
    }

    // Cleanup
    if (yuvBuf) free(yuvBuf);
    if (jpegBuf) free(jpegBuf);

    return 0;
}