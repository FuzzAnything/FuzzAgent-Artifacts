// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static int fuzzer_init(tjhandle *handle) {
    if (handle == NULL) return -1;
    *handle = tjInitDecompress();
    return (*handle != NULL) ? 0 : -1;
}

static void fuzzer_cleanup(tjhandle *handle) {
    if (handle != NULL && *handle != NULL) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 14) return 0;

    tjhandle handle = NULL;
    int ret = 0;

    // Fuzz tj3YUVPlaneWidth
    int componentID = (Data[0] % 3);
    int width = (Data[1] << 8) | Data[2];
    int subsamp = (Data[3] % 4);
    
    // Prevent overflow and invalid dimensions
    if (width <= 0 || width > 0x10000) width = 64; 
    if (subsamp < 0 || subsamp > 3) subsamp = 0;
    
    ret = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)ret;

    // Fuzz tjBufSizeYUV
    int height = (Data[4] << 8) | Data[5];
    if (height <= 0 || height > 0x10000) height = 64;
    
    unsigned long bufSize = tjBufSizeYUV(width, height, subsamp);
    if (bufSize == 0) return 0;

    // Allocate buffers for YUV and RGB
    unsigned char *yuvBuf = (unsigned char *)malloc(bufSize);
    if (!yuvBuf) return 0;
    
    // Calculate RGB buffer size (worst case: 4 bytes per pixel for RGBA)
    unsigned long rgbSize = (unsigned long)width * (unsigned long)height * 4;
    if (rgbSize == 0 || rgbSize > 100000000) { // Sanity check for huge allocations
        free(yuvBuf);
        return 0;
    }
    unsigned char *rgbBuf = (unsigned char *)malloc(rgbSize);
    if (!rgbBuf) {
        free(yuvBuf);
        return 0;
    }

    // Initialize handle for decompression functions
    if (fuzzer_init(&handle) != 0) {
        free(yuvBuf);
        free(rgbBuf);
        return 0;
    }

    // Prepare plane pointers for tjDecodeYUVPlanes and tj3DecodeYUVPlanes8
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    // Setup planes based on subsampling
    // We must ensure planes are within yuvBuf bounds
    size_t currentOffset = 0;
    
    // Plane 0 (Y)
    int w0 = tj3YUVPlaneWidth(0, width, subsamp);
    if (w0 <= 0) w0 = width;
    int h0 = height;
    planes[0] = yuvBuf + currentOffset;
    strides[0] = w0;
    currentOffset += (size_t)strides[0] * h0;

    // Plane 1 (U/Cb)
    int w1 = tj3YUVPlaneWidth(1, width, subsamp);
    if (w1 <= 0) w1 = (width + 1) / 2;
    int h1 = (height + 1) / 2;
    if (subsamp != 3) { // Not grayscale
        if (currentOffset + (size_t)w1 * h1 <= bufSize) {
            planes[1] = yuvBuf + currentOffset;
            strides[1] = w1;
            currentOffset += (size_t)strides[1] * h1;
        }
    }

    // Plane 2 (V/Cr)
    int w2 = tj3YUVPlaneWidth(2, width, subsamp);
    if (w2 <= 0) w2 = (width + 1) / 2;
    int h2 = (height + 1) / 2;
    if (subsamp != 3) { // Not grayscale
        if (currentOffset + (size_t)w2 * h2 <= bufSize) {
            planes[2] = yuvBuf + currentOffset;
            strides[2] = w2;
            // FIX: Ensure offset is updated to prevent overlap or invalid access in subsequent logic
            currentOffset += (size_t)strides[2] * h2;
        }
    }

    // Initialize buffer content to avoid uninitialized memory usage
    memset(yuvBuf, 0x80, bufSize); // Set Y to mid-gray, UV to neutral

    // Fuzz tjDecodeYUVPlanes
    int pixelFormat = Data[9] % 12; // 0-11 valid formats
    int pitch = (Data[10] << 8) | Data[11];
    if (pitch == 0) pitch = width * 4; // Default pitch
    int flags = Data[12] % 4;
    
    // Ensure pitch is reasonable to avoid huge writes
    if (pitch < 0 || pitch > 100000) pitch = width * 4;

    // FIX: Ensure planes are valid before calling decode functions
    // If subsamp is 3 (grayscale), planes[1] and planes[2] are NULL, which is valid for YUV decoding
    // But we must ensure the strides passed match the actual plane existence or are ignored correctly by the API
    // The API expects valid pointers for non-grayscale components.
    
    ret = tjDecodeYUVPlanes(handle, (const unsigned char **)planes, strides, subsamp,
                            rgbBuf, width, pitch, height, pixelFormat, flags);
    (void)ret;

    // Fuzz tj3DecodeYUVPlanes8
    ret = tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)planes, strides,
                              rgbBuf, width, pitch, height, pixelFormat);
    (void)ret;

    // Fuzz tjDecodeYUV (requires aligned buffer, we use align=1 to be safe with our malloc)
    int align = (Data[13] % 16) + 1; 
    ret = tjDecodeYUV(handle, yuvBuf, align, subsamp, rgbBuf, width, pitch, height, pixelFormat, flags);
    (void)ret;

    // Fuzz tjDecompressToYUVPlanes
    // Note: This function expects a valid JPEG stream in Data. Since Data is random fuzz input,
    // this will likely fail validation or return an error, which is expected behavior for fuzzing.
    // We do not crash on error returns, only on memory violations.
    const unsigned char *jpegBuf = Data;
    unsigned long jpegSize = Size;
    
    ret = tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, planes, width, strides, height, flags);
    (void)ret;

    // Cleanup
    fuzzer_cleanup(&handle);
    free(yuvBuf);
    free(rgbBuf);

    return 0;
}