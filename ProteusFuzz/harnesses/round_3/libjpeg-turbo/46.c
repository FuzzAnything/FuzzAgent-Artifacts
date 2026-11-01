// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_jpeg_buffer(unsigned char **jpegBuf) {
    if (jpegBuf && *jpegBuf) {
        tjFree(*jpegBuf);
        *jpegBuf = NULL;
    }
}

static int fuzz_tjCompressFromYUVPlanes(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 16) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int subsamp = Data[4] % 4;
    int quality = Data[5] % 101;
    int flags = Data[6] | (Data[7] << 8);
    
    // Calculate strides based on subsampling to ensure plane sizes are correct
    int strides[3];
    int planeWidths[3];
    
    planeWidths[0] = width;
    planeWidths[1] = width / 2;
    planeWidths[2] = width / 2;
    
    // Ensure strides are at least the plane width to avoid under-allocated buffers
    strides[0] = (Data[8] | (Data[9] << 8));
    if (strides[0] <= 0) strides[0] = planeWidths[0];
    else if (strides[0] < planeWidths[0]) strides[0] = planeWidths[0];
    
    strides[1] = (Data[10] | (Data[11] << 8));
    if (strides[1] <= 0) strides[1] = planeWidths[1];
    else if (strides[1] < planeWidths[1]) strides[1] = planeWidths[1];
    
    strides[2] = (Data[12] | (Data[13] << 8));
    if (strides[2] <= 0) strides[2] = planeWidths[2];
    else if (strides[2] < planeWidths[2]) strides[2] = planeWidths[2];
    
    // Allocate planes with size = stride * height
    size_t yPlaneSize = (size_t)strides[0] * height;
    size_t uPlaneSize = (size_t)strides[1] * height;
    size_t vPlaneSize = (size_t)strides[2] * height;
    
    unsigned char *yPlane = (unsigned char *)tjAlloc(yPlaneSize);
    unsigned char *uPlane = (unsigned char *)tjAlloc(uPlaneSize);
    unsigned char *vPlane = (unsigned char *)tjAlloc(vPlaneSize);
    
    if (!yPlane || !uPlane || !vPlane) {
        tjFree(yPlane); tjFree(uPlane); tjFree(vPlane);
        return 0;
    }
    
    // Initialize with valid YUV values
    memset(yPlane, 0x80, yPlaneSize);
    memset(uPlane, 0x80, uPlaneSize);
    memset(vPlane, 0x80, vPlaneSize);
    
    // Copy fuzz data safely
    if (Size > 16) {
        size_t totalPlaneSize = yPlaneSize + uPlaneSize + vPlaneSize;
        size_t copyLen = (Size - 16) < totalPlaneSize ? (Size - 16) : totalPlaneSize;
        memcpy(yPlane, Data + 16, copyLen);
        // If data is large enough, also copy to U and V planes
        if (copyLen > yPlaneSize) {
            size_t remaining = copyLen - yPlaneSize;
            size_t uvCopy = remaining < (uPlaneSize + vPlaneSize) ? remaining : (uPlaneSize + vPlaneSize);
            memcpy(uPlane, Data + 16 + yPlaneSize, uvCopy);
        }
    }
    
    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int ret = tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp,
                                      &jpegBuf, &jpegSize, quality, flags);
    
    cleanup_jpeg_buffer(&jpegBuf);
    tjFree(yPlane);
    tjFree(uPlane);
    tjFree(vPlane);
    
    return ret;
}

static int fuzz_tj3CompressFromYUVPlanes8(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 12) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int strides[3];
    int planeWidths[3];
    
    planeWidths[0] = width;
    planeWidths[1] = width / 2;
    planeWidths[2] = width / 2;
    
    strides[0] = (Data[4] | (Data[5] << 8));
    if (strides[0] <= 0) strides[0] = planeWidths[0];
    else if (strides[0] < planeWidths[0]) strides[0] = planeWidths[0];
    
    strides[1] = (Data[6] | (Data[7] << 8));
    if (strides[1] <= 0) strides[1] = planeWidths[1];
    else if (strides[1] < planeWidths[1]) strides[1] = planeWidths[1];
    
    strides[2] = (Data[8] | (Data[9] << 8));
    if (strides[2] <= 0) strides[2] = planeWidths[2];
    else if (strides[2] < planeWidths[2]) strides[2] = planeWidths[2];
    
    size_t yPlaneSize = (size_t)strides[0] * height;
    size_t uPlaneSize = (size_t)strides[1] * height;
    size_t vPlaneSize = (size_t)strides[2] * height;
    
    unsigned char *yPlane = (unsigned char *)tjAlloc(yPlaneSize);
    unsigned char *uPlane = (unsigned char *)tjAlloc(uPlaneSize);
    unsigned char *vPlane = (unsigned char *)tjAlloc(vPlaneSize);
    
    if (!yPlane || !uPlane || !vPlane) {
        tjFree(yPlane); tjFree(uPlane); tjFree(vPlane);
        return 0;
    }
    
    memset(yPlane, 0x80, yPlaneSize);
    memset(uPlane, 0x80, uPlaneSize);
    memset(vPlane, 0x80, vPlaneSize);
    
    if (Size > 12) {
        size_t totalPlaneSize = yPlaneSize + uPlaneSize + vPlaneSize;
        size_t copyLen = (Size - 12) < totalPlaneSize ? (Size - 12) : totalPlaneSize;
        memcpy(yPlane, Data + 12, copyLen);
        if (copyLen > yPlaneSize) {
            size_t remaining = copyLen - yPlaneSize;
            size_t uvCopy = remaining < (uPlaneSize + vPlaneSize) ? remaining : (uPlaneSize + vPlaneSize);
            memcpy(uPlane, Data + 12 + yPlaneSize, uvCopy);
        }
    }
    
    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    int ret = tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height,
                                        &jpegBuf, &jpegSize);
    
    cleanup_jpeg_buffer(&jpegBuf);
    tjFree(yPlane);
    tjFree(uPlane);
    tjFree(vPlane);
    
    return ret;
}

static int fuzz_tjCompress(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 16) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int pitch = Data[4] | (Data[5] << 8);
    // Ensure pitch is at least width * pixelSize to avoid buffer issues
    int pixelSize = (Data[6] % 4) + 1;
    int minPitch = width * pixelSize;
    if (pitch <= 0 || pitch < minPitch) pitch = minPitch;
    
    int subsamp = Data[7] % 4;
    int quality = Data[8] % 101;
    int flags = Data[9] | (Data[10] << 8);
    
    size_t srcSize = (size_t)pitch * height;
    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    if (!srcBuf) return 0;
    
    memset(srcBuf, 0, srcSize);
    if (Size > 16) {
        size_t copyLen = (Size - 16) < srcSize ? (Size - 16) : srcSize;
        memcpy(srcBuf, Data + 16, copyLen);
    }
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                         jpegBuf, &jpegSize, subsamp, quality, flags);
    
    tjFree(srcBuf);
    cleanup_jpeg_buffer(&jpegBuf);
    
    return ret;
}

static int fuzz_tj3CompressFromYUV8(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 12) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int align = 1 << (Data[4] % 4);
    
    // Calculate buffer size based on alignment
    size_t ySize = ((size_t)width * align + align - 1) / align * height;
    size_t uvSize = ySize / 4;
    size_t totalSize = ySize + 2 * uvSize;
    
    unsigned char *srcBuf = (unsigned char *)tjAlloc(totalSize);
    if (!srcBuf) return 0;
    
    memset(srcBuf, 0x80, totalSize);
    if (Size > 12) {
        size_t copyLen = (Size - 12) < totalSize ? (Size - 12) : totalSize;
        memcpy(srcBuf, Data + 12, copyLen);
    }
    
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    int ret = tj3CompressFromYUV8(handle, srcBuf, width, align, height,
                                  &jpegBuf, &jpegSize);
    
    tjFree(srcBuf);
    cleanup_jpeg_buffer(&jpegBuf);
    
    return ret;
}

static int fuzz_tjCompressFromYUV(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 16) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int align = 1 << (Data[4] % 4);
    int subsamp = Data[5] % 4;
    int quality = Data[6] % 101;
    int flags = Data[7] | (Data[8] << 8);
    
    size_t ySize = ((size_t)width * align + align - 1) / align * height;
    size_t uvSize = ySize / 4;
    size_t totalSize = ySize + 2 * uvSize;
    
    unsigned char *srcBuf = (unsigned char *)tjAlloc(totalSize);
    if (!srcBuf) return 0;
    
    memset(srcBuf, 0x80, totalSize);
    if (Size > 16) {
        size_t copyLen = (Size - 16) < totalSize ? (Size - 16) : totalSize;
        memcpy(srcBuf, Data + 16, copyLen);
    }
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int ret = tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                                &jpegBuf, &jpegSize, quality, flags);
    
    tjFree(srcBuf);
    cleanup_jpeg_buffer(&jpegBuf);
    
    return ret;
}

static int fuzz_tjCompress2(const uint8_t *Data, size_t Size, tjhandle handle) {
    if (Size < 16) return 0;
    
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return 0;
    
    int pitch = Data[4] | (Data[5] << 8);
    int pixelFormat = Data[6] % 6;
    int subsamp = Data[7] % 4;
    int quality = Data[8] % 101;
    int flags = Data[9] | (Data[10] << 8);
    
    int pixelSize;
    switch (pixelFormat) {
        case TJPF_GRAY: pixelSize = 1; break;
        case TJPF_RGB: pixelSize = 3; break;
        case TJPF_BGR: pixelSize = 3; break;
        case TJPF_RGBX: pixelSize = 4; break;
        case TJPF_BGRX: pixelSize = 4; break;
        case TJPF_XBGR: pixelSize = 4; break;
        case TJPF_XRGB: pixelSize = 4; break;
        default: pixelSize = 3; break;
    }
    
    int minPitch = width * pixelSize;
    if (pitch <= 0 || pitch < minPitch) pitch = minPitch;
    
    size_t srcSize = (size_t)pitch * height;
    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    if (!srcBuf) return 0;
    
    memset(srcBuf, 0, srcSize);
    if (Size > 16) {
        size_t copyLen = (Size - 16) < srcSize ? (Size - 16) : srcSize;
        memcpy(srcBuf, Data + 16, copyLen);
    }
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int ret = tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                          &jpegBuf, &jpegSize, subsamp, quality, flags);
    
    tjFree(srcBuf);
    cleanup_jpeg_buffer(&jpegBuf);
    
    return ret;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    int choice = Data[0] % 6;
    
    switch (choice) {
        case 0:
            fuzz_tjCompressFromYUVPlanes(Data, Size, handle);
            break;
        case 1:
            fuzz_tj3CompressFromYUVPlanes8(Data, Size, handle);
            break;
        case 2:
            fuzz_tjCompress(Data, Size, handle);
            break;
        case 3:
            fuzz_tj3CompressFromYUV8(Data, Size, handle);
            break;
        case 4:
            fuzz_tjCompressFromYUV(Data, Size, handle);
            break;
        case 5:
            fuzz_tjCompress2(Data, Size, handle);
            break;
    }
    
    tjDestroy(handle);
    return 0;
}