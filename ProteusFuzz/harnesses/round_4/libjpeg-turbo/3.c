// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *yuvBuf = NULL;
    unsigned char *rgbBuf = NULL;
    int width, height, align, subsamp, pixelFormat;
    size_t jpegSize;
    int ret;

    // Parse input data to derive parameters
    width = (Data[0] % 64) + 8;
    height = (Data[1] % 64) + 8;
    jpegSize = (Data[2] * 256 + Data[3]) + 1;
    if (jpegSize > Size - 4) jpegSize = Size - 4;
    
    align = 1 << (Data[4] % 5); // 1, 2, 4, 8, 16
    subsamp = Data[5] % 4; // TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY
    pixelFormat = Data[6] % 4; // TJPF_RGB, TJPF_BGR, TJPF_GRAY, etc.

    // Create a dummy JPEG buffer from input data
    // We copy the input data to simulate a JPEG stream, though it might be invalid
    // This is intentional for fuzzing to trigger error paths
    jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) return 0;
    memcpy(jpegBuf, Data + 4, jpegSize);

    // Initialize handle for decompression
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        tj3Free(jpegBuf);
        return 0;
    }

    // 1. tj3SetScalingFactor
    tjscalingfactor scalingFactor = {1, 1}; // Default 1/1
    if (Data[7] & 1) {
        scalingFactor.num = 1;
        scalingFactor.denom = 1 << (Data[8] % 4); // 1, 2, 4, 8
    }
    ret = tj3SetScalingFactor(handle, scalingFactor);
    if (ret < 0) {
        // Error is acceptable in fuzzing, continue to cleanup
    }

    // 2. tj3Alloc (for YUV buffer)
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        // Invalid dimensions, try to proceed with minimal allocation or skip
        yuvSize = 1024; 
    }
    yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Destroy(handle);
        tj3Free(jpegBuf);
        return 0;
    }

    // 3. tj3Alloc (for RGB buffer)
    int pitch = width * tjPixelSize[pixelFormat];
    size_t rgbSize = (size_t)pitch * height;
    if (rgbSize == 0) rgbSize = 1024;
    rgbBuf = (unsigned char *)tj3Alloc(rgbSize);
    if (!rgbBuf) {
        tj3Free(yuvBuf);
        tj3Destroy(handle);
        tj3Free(jpegBuf);
        return 0;
    }

    // 4. tj3YUVBufSize (re-calculate to ensure state exploration)
    size_t calcSize = tj3YUVBufSize(width, align, height, subsamp);
    (void)calcSize; // Use the value to ensure the function is called

    // 5. tj3DecompressToYUV8
    ret = tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, align);
    // Ignore return value for fuzzing flow, but it indicates success/failure

    // 6. tj3DecodeYUV8
    // Note: This requires valid YUV data. Since our input is random, this will likely fail,
    // which is a valid state to explore.
    ret = tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);
    // Ignore return value

    // Cleanup
    tj3Destroy(handle);
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    tj3Free(rgbBuf);

    return 0;
}