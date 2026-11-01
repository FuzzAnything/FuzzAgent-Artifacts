// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void safe_cleanup(tjhandle handle) {
    if (handle != NULL) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    tjhandle handle = NULL;
    int ret = 0;

    // Parse input data for parameters
    int width = (int)(Data[0] | (Data[1] << 8));
    int height = (int)(Data[2] | (Data[3] << 8));
    int subsamp = (int)Data[4] & 0x03; // Limit to valid subsampling values (0-3)
    int pixelFormat = (int)Data[5] & 0x07; // Limit to valid pixel formats (0-7)
    int flags = (int)(Data[6] | (Data[7] << 8));
    int align = (int)Data[8];
    if (align == 0) align = 1;
    if (align > 16) align = 16; // Cap alignment to prevent huge allocations

    // Ensure dimensions are reasonable to avoid OOM or crashes
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        return 0;
    }

    // Calculate buffer sizes
    int srcSize = width * height * 4; // Assume max 4 bytes per pixel (RGBA)
    int yuvBufSize = tj3YUVBufSize(width, height, subsamp, align);
    int yPlaneSize = tj3YUVPlaneSize(width, height, subsamp, 0, align);
    int uPlaneSize = tj3YUVPlaneSize(width, height, subsamp, 1, align);
    int vPlaneSize = tj3YUVPlaneSize(width, height, subsamp, 2, align);

    if (yuvBufSize <= 0 || yPlaneSize <= 0 || uPlaneSize <= 0 || vPlaneSize <= 0) {
        return 0;
    }

    // Allocate buffers
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    unsigned char *dstBuf = (unsigned char *)malloc(yuvBufSize);
    unsigned char *yPlane = (unsigned char *)malloc(yPlaneSize);
    unsigned char *uPlane = (unsigned char *)malloc(uPlaneSize);
    unsigned char *vPlane = (unsigned char *)malloc(vPlaneSize);

    if (!srcBuf || !dstBuf || !yPlane || !uPlane || !vPlane) {
        free(srcBuf);
        free(dstBuf);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        return 0;
    }

    // Initialize source buffer with fuzz data or pattern
    memset(srcBuf, 0, srcSize);
    if (Size > srcSize) {
        memcpy(srcBuf, Data + 9, srcSize);
    } else {
        for (int i = 0; i < srcSize; i++) {
            srcBuf[i] = Data[(9 + i) % Size];
        }
    }

    // Initialize handle
    handle = tjInitCompress();
    if (!handle) {
        safe_cleanup(handle);
        free(srcBuf);
        free(dstBuf);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        return 0;
    }

    // Set subsampling if needed for some functions
    // Fixed signature: tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat, jpegBuf, jpegSize, jpegSubsamp, jpegQual, flags)
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) pixelSize = 3; // Default fallback
    int pitch = width * pixelSize;
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    // We pass NULL for jpegBuf to just set parameters or test error handling if needed
    // However, tjCompress2 usually requires a valid buffer to write to if successful.
    // Since we just want to configure the handle or test error paths, we can try with a dummy buffer or just skip this if not critical.
    // To be safe and avoid allocation issues here, we skip the actual compression call that requires output buffer,
    // as the target functions (tjEncodeYUV*) handle the conversion directly.
    // The original code tried to call tjCompress2 to set subsamp, but tjEncodeYUV2 takes subsamp as arg.
    // So we remove the tjCompress2 call which was causing the error and is not strictly needed for YUV encoding APIs.

    // Test 1: tjEncodeYUV2
    ret = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);
    if (ret < 0) {
        int errCode = tjGetErrorCode(handle);
        (void)errCode; // Use error code to explore state
    }

    // Test 2: tj3EncodeYUV8
    ret = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);
    if (ret < 0) {
        int errCode = tj3GetErrorCode(handle);
        (void)errCode;
    }

    // Test 3: tj3EncodeYUVPlanes8
    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {0, 0, 0}; // 0 means auto-calculate
    
    ret = tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides);
    if (ret < 0) {
        int errCode = tj3GetErrorCode(handle);
        (void)errCode;
    }

    // Test 4: tjDecodeYUVPlanes
    // Prepare source planes for decode (reusing the encoded planes if they have data)
    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int decodeStrides[3] = {0, 0, 0};
    
    // Output buffer for decode
    unsigned char *decodeDstBuf = (unsigned char *)malloc(srcSize);
    if (decodeDstBuf) {
        ret = tjDecodeYUVPlanes(handle, srcPlanes, decodeStrides, subsamp, 
                                decodeDstBuf, width, pitch, height, pixelFormat, flags);
        if (ret < 0) {
            int errCode = tj3GetErrorCode(handle);
            (void)errCode;
        }
        free(decodeDstBuf);
    }

    // Cleanup
    safe_cleanup(handle);
    free(srcBuf);
    free(dstBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);

    return 0;
}