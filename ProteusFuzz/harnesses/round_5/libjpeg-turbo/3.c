// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
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

static void fill_buffer(unsigned char *buf, size_t size) {
    if (buf && size > 0) {
        for (size_t i = 0; i < size; i++) {
            buf[i] = (unsigned char)(i % 256);
        }
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for the parameters we need
    if (Size < 15) {
        return 0;
    }

    // Parse input to derive parameters
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4; // TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_XBGR
    int subsamp = Data[3] % 4;     // TJSAMP_444, TJSAMP_422, TJSAMP_420, TJPF_GRAY
    int align = 1 << (Data[4] % 5); // Power of 2: 1, 2, 4, 8, 16
    int quality = 1 + (Data[5] % 99);
    int jpegSubsamp = Data[6] % 4;

    // Limit dimensions to prevent OOM
    if (width > 4096 || height > 4096) {
        return 0;
    }
    if (width == 0 || height == 0) {
        return 0;
    }

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    // Set initial parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, quality);
    tj3Set(handle, TJPARAM_FASTDCT, Data[7] % 2);
    tj3Set(handle, TJPARAM_PROGRESSIVE, Data[8] % 2);
    tj3Set(handle, TJPARAM_NOREALLOC, Data[9] % 2);
    
    // Apply the second set of subsampling and quality changes
    // This updates the internal state of the handle
    tj3Set(handle, TJPARAM_SUBSAMP, Data[10] % 4);
    tj3Set(handle, TJPARAM_QUALITY, 1 + (Data[11] % 99));
    tj3Set(handle, TJPARAM_FASTDCT, Data[12] % 2);

    // Calculate buffer sizes
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // CRITICAL FIX:
    // Retrieve the actual subsampling value currently set on the handle.
    // The buffer size must be calculated based on the handle's final configuration,
    // not the initial 'subsamp' variable, because tj3EncodeYUV8 uses the handle's
    // internal state to calculate plane offsets and strides.
    int currentSubsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, currentSubsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Additional sets before encoding (2 times)
    tj3Set(handle, TJPARAM_FASTDCT, Data[13] % 2);
    tj3Set(handle, TJPARAM_PROGRESSIVE, Data[14] % 2);

    // Prepare source buffer (RGB/Gray)
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Check for overflow in size calculation
    if (width > SIZE_MAX / height || (size_t)width * height > SIZE_MAX / pixelSize) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    size_t srcSize = (size_t)width * height * pixelSize;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Fill source buffer with fuzz data
    if (Size > srcSize) {
        memcpy(srcBuf, Data, srcSize);
    } else {
        fill_buffer(srcBuf, srcSize);
    }

    // Encode to YUV
    // Ensure pitch is consistent with source buffer allocation
    int pitch = width * pixelSize;
    int ret = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align);
    (void)ret; 

    // Compress from YUV
    size_t jpegSize = jpegBufSize;
    ret = tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    (void)ret;

    // Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    free(srcBuf);
    tj3Destroy(handle);

    return 0;
}