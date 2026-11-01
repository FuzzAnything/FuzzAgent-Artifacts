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
#include <turbojpeg.h>

static int fuzz_libjpeg_turbo(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for the initial parameters
    if (Size < 20) {
        return 0;
    }

    // Parse input data for parameters
    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int pixelFormat = *(int32_t *)(Data + 8);
    int subsamp = *(int32_t *)(Data + 12);

    // Clamp dimensions to reasonable limits to avoid huge allocations
    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;

    // Validate pixel format
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = TJPF_RGB;

    // Validate subsampling
    if (subsamp < 0 || subsamp > 3) subsamp = TJSAMP_422;

    // Initialize handle
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    // Set various parameters using tj3Set (9 calls as requested)
    // Using only standard parameters known to exist in libjpeg-turbo
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, (Data[13] % 100) + 1);
    tj3Set(handle, TJPARAM_PROGRESSIVE, (Data[14] % 2));
    tj3Set(handle, TJPARAM_FASTDCT, (Data[15] % 2));
    tj3Set(handle, TJPARAM_JPEGWIDTH, width);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, height);
    tj3Set(handle, TJPARAM_NOREALLOC, (Data[16] % 2));
    // Reusing valid parameters to reach the count of 9
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, (Data[17] % 100) + 1);

    // Calculate JPEG buffer size
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
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
    // Ensure we have enough data for alignment calculation
    int align = 1 << (Data[18] % 4); // Power of 2 alignment: 1, 2, 4, 8
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Allocate YUV buffer
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Set additional parameters (2 calls as per sequence before Encode)
    tj3Set(handle, TJPARAM_JPEGWIDTH, width);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, height);

    // Prepare source buffer (RGB)
    size_t srcSize = width * height * tjPixelSize[pixelFormat];
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Fill source buffer with fuzz data
    size_t fuzzDataAvailable = Size - 19;
    if (fuzzDataAvailable > srcSize) fuzzDataAvailable = srcSize;
    memcpy(srcBuf, Data + 19, fuzzDataAvailable);
    // Fill remaining with zeros if needed
    if (fuzzDataAvailable < srcSize) {
        memset(srcBuf + fuzzDataAvailable, 0, srcSize - fuzzDataAvailable);
    }

    // Encode YUV8
    int pitch = width * tjPixelSize[pixelFormat];
    int ret = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align);
    (void)ret; // Ignore return for fuzzing exploration

    // Compress from YUV8
    size_t jpegSize = jpegBufSize;
    ret = tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    (void)ret; // Ignore return for fuzzing exploration

    // Cleanup
    free(srcBuf);
    tj3Free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    return fuzz_libjpeg_turbo(Data, Size);
}