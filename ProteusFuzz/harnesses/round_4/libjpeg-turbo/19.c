// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_resources(tjhandle handle, unsigned char *jpegBuf, unsigned char *srcBuf,
                              unsigned char *yPlane, unsigned char *uPlane, unsigned char *vPlane) {
    if (handle) {
        tjDestroy(handle);
    }
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    if (srcBuf) {
        free(srcBuf);
    }
    if (yPlane) free(yPlane);
    if (uPlane) free(uPlane);
    if (vPlane) free(vPlane);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzz data
    // Ensure dimensions are at least 16x16 to satisfy MCU requirements and avoid edge cases
    // Also limit max size to prevent huge allocations
    int width = (Data[0] % 1024) + 16; 
    int height = (Data[1] % 1024) + 16;
    int subsamp = Data[2] % 3; // 0, 1, or 2
    int quality = (Data[3] % 100) + 1; // 1 to 100
    int flags = 0;
    int pitch = width * 3; // Assume RGB
    int pixelFormat = TJPF_RGB;

    // Initialize handle
    tjhandle handle = tjInitCompress();
    if (!handle) {
        return 0;
    }

    // Allocate source buffer for RGB/YUV data
    // Check for overflow
    if (width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }
    
    size_t srcSize = (size_t)width * height * 3;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }

    // Fill source buffer with fuzz data or pattern
    size_t fillSize = (Size > srcSize) ? srcSize : Size;
    memcpy(srcBuf, Data, fillSize);
    if (fillSize < srcSize) {
        memset(srcBuf + fillSize, 0, srcSize - fillSize);
    }

    // Allocate YUV planes for planar functions
    // Calculate plane sizes based on subsampling
    // For 4:2:0 (subsamp=1) and 4:2:2 (subsamp=2), UV dimensions are halved
    // For 4:4:4 (subsamp=0), UV dimensions match Y
    int uvWidth = (subsamp == 0) ? width : (width + 1) / 2;
    int uvHeight = (subsamp == 0) ? height : (height + 1) / 2;
    
    size_t ySize = (size_t)width * height;
    size_t uvSize = (size_t)uvWidth * uvHeight;

    unsigned char *yPlane = (unsigned char *)malloc(ySize);
    unsigned char *uPlane = (unsigned char *)malloc(uvSize);
    unsigned char *vPlane = (unsigned char *)malloc(uvSize);
    
    if (!yPlane || !uPlane || !vPlane) {
        cleanup_resources(handle, NULL, srcBuf, yPlane, uPlane, vPlane);
        return 0;
    }

    // Fill planes with data safely
    // Ensure we do not read beyond srcBuf bounds when filling planes
    // We generate synthetic data for planes if srcBuf is too small, or copy from srcBuf
    
    // Fill Y plane
    if (ySize <= srcSize) {
        memcpy(yPlane, srcBuf, ySize);
    } else {
        memcpy(yPlane, srcBuf, srcSize);
        memset(yPlane + srcSize, 0, ySize - srcSize);
    }

    // Fill U plane
    // We need uvSize bytes. srcBuf has srcSize bytes.
    // We can reuse srcBuf data or just fill with pattern.
    // To be safe and deterministic, let's just fill with a pattern based on index if not enough data
    if (uvSize <= srcSize) {
        memcpy(uPlane, srcBuf, uvSize);
    } else {
        memcpy(uPlane, srcBuf, srcSize);
        memset(uPlane + srcSize, 0, uvSize - srcSize);
    }

    // Fill V plane
    if (uvSize <= srcSize) {
        memcpy(vPlane, srcBuf, uvSize);
    } else {
        memcpy(vPlane, srcBuf, srcSize);
        memset(vPlane + srcSize, 0, uvSize - srcSize);
    }

    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {width, uvWidth, uvWidth};
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    size_t jpegSize64 = 0;

    // Test tjCompress2 (RGB to JPEG)
    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                &jpegBuf, &jpegSize, subsamp, quality, flags);

    // Test tjCompress (Legacy RGB to JPEG)
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    tjCompress(handle, srcBuf, width, pitch, height, 3,
               &jpegBuf2, &jpegSize2, subsamp, quality, flags);

    // Test tjCompressFromYUV (Packed YUV to JPEG)
    // Note: srcBuf is RGB here, but the API expects YUV. 
    // Passing RGB data as YUV is a valid fuzzing strategy to test robustness, 
    // but we must ensure the buffer size matches expected YUV size if we were to be strict.
    // However, tjCompressFromYUV expects a buffer of size width*height*planes.
    // srcBuf is width*height*3. This matches 4:4:4 YUV size.
    // For 4:2:0, it expects width*height*1.5. srcBuf is larger, which is usually safe for reading 
    // if the library doesn't check size strictly, but let's be careful.
    // Actually, let's create a specific YUV buffer for this call to be safe and correct.
    size_t yuvPackedSize = (size_t)width * height * ((subsamp == 0) ? 3 : (subsamp == 1) ? 2 : 2); // Approx
    // Actually, TJBUFSIZE is for JPEG output. For YUV input size:
    // 4:4:4 -> 3 bytes/pixel
    // 4:2:2 -> 2 bytes/pixel
    // 4:2:0 -> 1.5 bytes/pixel (width*height*3/2)
    // Let's just use srcBuf (which is 3 bytes/pixel) for 4:4:4. For others, we might need a smaller buffer or just pass srcBuf and hope it doesn't OOB read.
    // To be safe, let's allocate a proper YUV buffer for this specific test.
    size_t yuvSize = (size_t)width * height * ((subsamp == 0) ? 3 : 2); // Simplified, 4:2:0 is tricky with packed
    // Actually, tjCompressFromYUV expects a specific layout. 
    // Let's just use the yPlane, uPlane, vPlane logic to create a packed buffer if needed, or just skip this if complex.
    // Simpler approach: Just pass srcBuf (RGB) as if it were YUV. The library will read width*height*planes bytes.
    // If subsamp is 4:2:0, it expects 1.5 * W * H. srcBuf has 3 * W * H. This is safe (reading less than available).
    // If subsamp is 4:4:4, it expects 3 * W * H. srcBuf has 3 * W * H. Safe.
    // If subsamp is 4:2:2, it expects 2 * W * H. srcBuf has 3 * W * H. Safe.
    // So passing srcBuf is safe for all subsampling modes in terms of buffer size.
    
    unsigned char *jpegBuf3 = NULL;
    unsigned long jpegSize3 = 0;
    tjCompressFromYUV(handle, srcBuf, width, 1, height, subsamp,
                      &jpegBuf3, &jpegSize3, quality, flags);

    // Test tjCompressFromYUVPlanes (Planar YUV to JPEG)
    // We have allocated yPlane, uPlane, vPlane with correct sizes (ySize, uvSize, uvSize).
    // The strides are set to width, uvWidth, uvWidth.
    // This should be safe.
    unsigned char *jpegBuf4 = NULL;
    unsigned long jpegSize4 = 0;
    tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp,
                            &jpegBuf4, &jpegSize4, quality, flags);

    // Test tj3CompressFromYUVPlanes8 (Planar YUV to JPEG, 8-bit)
    unsigned char *jpegBuf5 = NULL;
    size_t jpegSize5 = 0;
    tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height,
                              &jpegBuf5, &jpegSize5);

    // Test TJBUFSIZE
    unsigned long bufSize = TJBUFSIZE(width, height);
    (void)bufSize; // Suppress unused warning

    // Cleanup
    cleanup_resources(handle, jpegBuf, srcBuf, yPlane, uPlane, vPlane);
    if (jpegBuf2) tjFree(jpegBuf2);
    if (jpegBuf3) tjFree(jpegBuf3);
    if (jpegBuf4) tjFree(jpegBuf4);
    if (jpegBuf5) tjFree(jpegBuf5);

    return 0;
}