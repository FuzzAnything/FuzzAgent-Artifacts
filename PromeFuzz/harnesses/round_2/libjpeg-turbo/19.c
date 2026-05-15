// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

#define MAX_DIM 1024
#define MAX_JPEG_SIZE (1024 * 1024)

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static tjhandle init_decompress_handle(void) {
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return NULL;
    }
    return handle;
}

static tjhandle init_compress_handle(void) {
    tjhandle handle = tjInitCompress();
    if (!handle) {
        return NULL;
    }
    return handle;
}

static tjhandle init_tj3_decompress_handle(void) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return NULL;
    }
    return handle;
}

static tjhandle init_tj3_compress_handle(void) {
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return NULL;
    }
    return handle;
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void cleanup_tj3_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Parse parameters from the input data
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 4;
    int pixelFormat = Data[5] % 12;
    int flags = Data[6];
    int align = 1 << (Data[7] % 4);
    int pitch = width * tjPixelSize[pixelFormat];

    // Cap dimensions to avoid excessive memory usage
    if (width <= 0 || height <= 0 || width > MAX_DIM || height > MAX_DIM) {
        width = MAX_DIM / 2;
        height = MAX_DIM / 2;
        pitch = width * tjPixelSize[pixelFormat];
    }

    // Ensure align is a power of 2 and at least 1
    if (align < 1) align = 1;

    // Prepare JPEG buffer (skip first 8 bytes for parameters)
    size_t jpeg_offset = 8;
    size_t jpeg_size = Size - jpeg_offset;
    if (jpeg_size > MAX_JPEG_SIZE) {
        jpeg_size = MAX_JPEG_SIZE;
    }
    const unsigned char *jpegBuf = Data + jpeg_offset;
    unsigned long jpegSize = (unsigned long)jpeg_size;

    // Write dummy file for any potential file-based operations
    write_dummy_file(Data, Size);

    // Allocate destination buffers
    size_t rgb_size = pitch * height;
    unsigned char *dstBuf = NULL;
    if (rgb_size > 0) {
        dstBuf = (unsigned char *)malloc(rgb_size);
        if (!dstBuf) goto cleanup;
        memset(dstBuf, 0, rgb_size);
    }

    // Allocate YUV buffer for tj3DecodeYUV8 and tjDecodeYUV
    unsigned long yuv_size = tjBufSizeYUV(width, height, subsamp);
    unsigned char *yuvBuf = NULL;
    if (yuv_size > 0) {
        yuvBuf = (unsigned char *)malloc(yuv_size);
        if (!yuvBuf) goto cleanup;
        memset(yuvBuf, 0, yuv_size);
    }

    // Allocate source RGB buffer for encoding functions
    unsigned char *srcBuf = NULL;
    if (rgb_size > 0) {
        srcBuf = (unsigned char *)malloc(rgb_size);
        if (!srcBuf) goto cleanup;
        memset(srcBuf, 0, rgb_size);
    }

    // Allocate planes for planar YUV functions
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    int plane_sizes[3] = {0, 0, 0};

    // Calculate plane sizes and allocate
    plane_sizes[0] = tjPlaneSizeYUV(0, width, 0, height, subsamp);
    plane_sizes[1] = tjPlaneSizeYUV(1, width, 0, height, subsamp);
    plane_sizes[2] = tjPlaneSizeYUV(2, width, 0, height, subsamp);

    for (int i = 0; i < 3; i++) {
        if (plane_sizes[i] > 0) {
            planes[i] = (unsigned char *)malloc(plane_sizes[i]);
            if (!planes[i]) goto cleanup;
            memset(planes[i], 0, plane_sizes[i]);
        }
    }

    // Initialize handles
    tjhandle decompress_handle = init_decompress_handle();
    tjhandle compress_handle = init_compress_handle();
    tjhandle tj3_decompress_handle = init_tj3_decompress_handle();
    tjhandle tj3_compress_handle = init_tj3_compress_handle();

    if (!decompress_handle || !compress_handle || 
        !tj3_decompress_handle || !tj3_compress_handle) {
        goto cleanup;
    }

    // Set subsampling for tj3 handles
    tj3Set(tj3_decompress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(tj3_compress_handle, TJPARAM_SUBSAMP, subsamp);

    // 1. Test tjDecompressToYUVPlanes
    if (jpegSize > 0 && planes[0] && planes[1] && planes[2]) {
        tjDecompressToYUVPlanes(decompress_handle, jpegBuf, jpegSize,
                               planes, width, strides, height, flags);
    }

    // 2. Test tj3DecodeYUV8
    if (yuvBuf && dstBuf) {
        tj3DecodeYUV8(tj3_decompress_handle, yuvBuf, align, dstBuf,
                     width, pitch, height, pixelFormat);
    }

    // 3. Test tj3EncodeYUVPlanes8
    if (srcBuf && planes[0] && planes[1] && planes[2]) {
        tj3EncodeYUVPlanes8(tj3_compress_handle, srcBuf, width, pitch, height,
                           pixelFormat, planes, strides);
    }

    // 4. Test tjDecodeYUVPlanes
    if (planes[0] && planes[1] && planes[2] && dstBuf) {
        const unsigned char *srcPlanes[3] = {planes[0], planes[1], planes[2]};
        tjDecodeYUVPlanes(decompress_handle, srcPlanes, strides, subsamp,
                         dstBuf, width, pitch, height, pixelFormat, flags);
    }

    // 5. Test tjDecodeYUV
    if (yuvBuf && dstBuf) {
        tjDecodeYUV(decompress_handle, yuvBuf, align, subsamp, dstBuf,
                   width, pitch, height, pixelFormat, flags);
    }

    // 6. Test tjEncodeYUVPlanes
    if (srcBuf && planes[0] && planes[1] && planes[2]) {
        tjEncodeYUVPlanes(compress_handle, srcBuf, width, pitch, height,
                         pixelFormat, planes, strides, subsamp, flags);
    }

cleanup:
    // Clean up handles
    cleanup_handle(decompress_handle);
    cleanup_handle(compress_handle);
    cleanup_tj3_handle(tj3_decompress_handle);
    cleanup_tj3_handle(tj3_compress_handle);

    // Free allocated buffers
    free(dstBuf);
    free(yuvBuf);
    free(srcBuf);
    for (int i = 0; i < 3; i++) {
        free(planes[i]);
    }

    return 0;
}