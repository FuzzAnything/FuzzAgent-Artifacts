// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
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
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int clamp(int val, int min_val, int max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (handle == NULL) return 0;

    // Extract parameters from fuzz input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int align = 1 << (Data[4] % 9); // Power of 2 between 1 and 256
    int subsamp = Data[5] % 5;      // 0-4
    int quality = 1 + (Data[6] % 99);
    int pixelFormat = Data[7] % 6;  // 0-5
    int jpegSubsamp = Data[8] % 5;  // 0-4

    // Clamp dimensions to reasonable values to avoid OOM
    width = clamp(width, 1, 4096);
    height = clamp(height, 1, 4096);

    // tj3Set calls as per task order
    // Using valid parameters: SUBSAMP, QUALITY, JPEGWIDTH, JPEGHEIGHT
    // Replacing invalid TJPARAM_ROWALIGN and TJPARAM_ACCURACY with valid ones or defaults
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, quality);
    tj3Set(handle, TJPARAM_JPEGWIDTH, width);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, height);
    // Using TJPARAM_PROGRESSIVE as a valid alternative for the 5th set
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);
    // Using TJPARAM_COLORSPACE as a valid alternative for the 6th set
    tj3Set(handle, TJPARAM_COLORSPACE, 0);
    // Using TJPARAM_ACCURACY is invalid, using TJPARAM_JPEGQUALITY (alias for quality) or just reusing valid ones
    // Let's use TJPARAM_PROGRESSIVE again or TJPARAM_COLORSPACE
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);
    tj3Set(handle, TJPARAM_NOREALLOC, 0);
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);

    // Calculate buffer sizes
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (jpegBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (yuvBuf == NULL) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Additional tj3Set calls before encoding
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, quality);

    // Prepare source buffer for tj3EncodeYUV8
    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcBufSize = (size_t)pitch * height;
    unsigned char *srcBuf = (unsigned char *)malloc(srcBufSize);
    if (srcBuf == NULL) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Fill source buffer with fuzz data
    size_t copyLen = (Size > srcBufSize) ? srcBufSize : Size;
    memcpy(srcBuf, Data, copyLen);
    if (copyLen < srcBufSize) {
        memset(srcBuf + copyLen, 0, srcBufSize - copyLen);
    }

    // Encode YUV
    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align);

    // Compress from YUV
    size_t jpegSize = jpegBufSize;
    tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);

    // Cleanup
    free(srcBuf);
    tj3Free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}