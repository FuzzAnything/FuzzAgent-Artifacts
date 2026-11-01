// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static void *allocate_buffer(size_t size) {
    if (size == 0) return NULL;
    void *buf = malloc(size);
    if (buf) {
        memset(buf, 0, size);
    }
    return buf;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    // Extract parameters from fuzz input to vary test cases
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = Data[2] % 4; // 0: 4:4:4, 1: 4:2:2, 2: 4:2:0, 3: 4:1:1 (approximate)
    int pixelFormat = Data[3] % 6; // 0-5 valid formats usually
    int align = 1 << (Data[4] % 5); // 1, 2, 4, 8, 16

    // Limit dimensions to prevent excessive allocation
    if (width > 4096 || height > 4096) return 0;
    if (width == 0 || height == 0) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    // Calculate buffer sizes
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    // Calculate RGB buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize < 1 || pixelSize > 4) {
        tjDestroy(handle);
        return 0;
    }
    size_t rgbSize = (size_t)width * pixelSize * height;

    unsigned char *yuvBuf = (unsigned char *)allocate_buffer(yuvSize);
    unsigned char *rgbBuf = (unsigned char *)allocate_buffer(rgbSize);
    
    if (!yuvBuf || !rgbBuf) {
        free(yuvBuf);
        free(rgbBuf);
        tjDestroy(handle);
        return 0;
    }

    // Fill buffers with fuzz data
    size_t fillSize = Size > yuvSize ? yuvSize : Size;
    memcpy(yuvBuf, Data, fillSize);
    
    // Prepare strides for tjDecodeYUVPlanes
    int strides[3] = {0, 0, 0};
    const unsigned char *srcPlanes[3];
    
    // Calculate plane offsets based on subsampling and alignment
    // This is a simplified calculation; real usage depends on exact subsampling
    int planeWidth = width;
    int planeHeight = height;
    int uvWidth = (width + 1) / 2;
    int uvHeight = (height + 1) / 2;
    
    // Y plane
    srcPlanes[0] = yuvBuf;
    strides[0] = (width + align - 1) & ~(align - 1);
    
    // U plane
    size_t uOffset = (size_t)strides[0] * height;
    srcPlanes[1] = yuvBuf + uOffset;
    int uStride = (uvWidth + align - 1) & ~(align - 1);
    strides[1] = uStride;
    
    // V plane
    size_t vOffset = uOffset + (size_t)uStride * uvHeight;
    srcPlanes[2] = yuvBuf + vOffset;
    strides[2] = uStride;

    // Test 1: tjDecodeYUVPlanes
    // Note: tjDecodeYUVPlanes is a legacy wrapper, ensure flags are valid
    int flags = 0;
    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, rgbBuf, width, width * pixelSize, height, pixelFormat, flags);

    // Test 2: tj3DecodeYUVPlanes8
    tj3DecodeYUVPlanes8(handle, srcPlanes, strides, rgbBuf, width, width * pixelSize, height, pixelFormat);

    // Test 3: tj3DecodeYUV8
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, width * pixelSize, height, pixelFormat);

    // Test 4: tjDecodeYUV
    tjDecodeYUV(handle, yuvBuf, align, subsamp, rgbBuf, width, width * pixelSize, height, pixelFormat, flags);

    // Test 5: tj3DecompressToYUV8 (requires valid JPEG data, but we can try with fuzz data)
    // This will likely fail with invalid JPEG, which is good for fuzzing error paths
    // We reuse the fuzz data as a potential JPEG stream
    tj3DecompressToYUV8(handle, Data, Size, yuvBuf, align);

    // Cleanup
    free(yuvBuf);
    free(rgbBuf);
    tjDestroy(handle);

    return 0;
}