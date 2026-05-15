// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Initialize TurboJPEG instance for compression
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    // Set various parameters using fuzzer data
    // Use modulo operations to keep values in reasonable ranges
    if (Size > 0) {
        tj3Set(handle, TJPARAM_SUBSAMP, Data[0] % 6);  // TJSAMP values: 0-5
    }
    if (Size > 1) {
        tj3Set(handle, TJPARAM_QUALITY, Data[1] % 101);  // Quality: 0-100
    }
    if (Size > 2) {
        tj3Set(handle, TJPARAM_OPTIMIZE, Data[2] % 2);
    }
    if (Size > 3) {
        tj3Set(handle, TJPARAM_PROGRESSIVE, Data[3] % 2);
    }
    if (Size > 4) {
        tj3Set(handle, TJPARAM_ARITHMETIC, Data[4] % 2);
    }
    if (Size > 5) {
        tj3Set(handle, TJPARAM_LOSSLESS, Data[5] % 2);
    }
    if (Size > 6) {
        tj3Set(handle, TJPARAM_COLORSPACE, Data[6] % 5);  // TJCS values: 0-4
    }
    if (Size > 7) {
        tj3Set(handle, TJPARAM_FASTDCT, Data[7] % 2);
    }

    // Define image dimensions (use small values to avoid excessive memory)
    int width = 16;
    int height = 16;
    int align = 1;
    
    // Calculate JPEG buffer size
    size_t jpegBufSize = tj3JPEGBufSize(width, height, TJSAMP_444);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Calculate YUV buffer size
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, TJSAMP_444);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Set additional parameters
    if (Size > 8) {
        tj3Set(handle, TJPARAM_NOREALLOC, Data[8] % 2);
    }
    if (Size > 9) {
        tj3Set(handle, TJPARAM_RESTARTBLOCKS, Data[9] % 100);
    }
    
    // Allocate source and YUV buffers
    unsigned char *srcBuf = (unsigned char *)malloc(width * height * 3);
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
    
    if (!srcBuf || !yuvBuf) {
        free(srcBuf);
        free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Initialize source buffer with fuzzer data (or zeros if not enough)
    size_t srcSize = width * height * 3;
    size_t copySize = Size > srcSize ? srcSize : Size;
    if (copySize > 0) {
        memcpy(srcBuf, Data, copySize);
    }
    if (copySize < srcSize) {
        memset(srcBuf + copySize, 0, srcSize - copySize);
    }
    
    // Encode to YUV
    int result = tj3EncodeYUV8(handle, srcBuf, width, 0, height, TJPF_RGB, yuvBuf, align);
    
    // Compress from YUV if encoding succeeded
    if (result == 0) {
        size_t jpegSize = jpegBufSize;
        unsigned char *jpegBufPtr = jpegBuf;
        tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBufPtr, &jpegSize);
    }
    
    // Cleanup
    free(srcBuf);
    free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);
    
    return 0;
}