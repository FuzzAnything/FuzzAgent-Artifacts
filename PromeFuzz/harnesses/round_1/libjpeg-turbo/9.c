// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIMENSION 4096
#define MIN_DIMENSION 1

static int get_valid_dimension(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 2 > size) {
        return MIN_DIMENSION;
    }
    int dim = (data[*offset] << 8) | data[*offset + 1];
    *offset += 2;
    if (dim < MIN_DIMENSION) dim = MIN_DIMENSION;
    if (dim > MAX_DIMENSION) dim = MAX_DIMENSION;
    return dim;
}

static int get_valid_align(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return 1;
    }
    int align = 1 << (data[*offset] & 0x7); // 1,2,4,8,16,32,64,128
    *offset += 1;
    if (align < 1) align = 1;
    if (align > 128) align = 128;
    return align;
}

static int get_valid_subsamp(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return TJSAMP_444;
    }
    int subsamp = data[*offset] % 6; // 0-5: TJSAMP_444, 422, 420, 440, 411, GRAY
    *offset += 1;
    return subsamp;
}

static int get_valid_pixel_format(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return TJPF_RGB;
    }
    int pf = data[*offset] % 13; // 0-12: TJPF_RGB, BGR, RGBX, BGRX, XBGR, XRGB, GRAY, RGBA, BGRA, ABGR, ARGB, CMYK, UNKNOWN
    *offset += 1;
    return pf;
}

static int get_valid_param(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return TJPARAM_SUBSAMP;
    }
    int param = data[*offset] % 32; // 0-31: valid TJPARAM values
    *offset += 1;
    return param;
}

static int get_valid_value(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return 0;
    }
    int value = data[*offset];
    *offset += 1;
    return value;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    size_t offset = 0;
    
    // Initialize handles
    tjhandle handleCompress = tj3Init(TJINIT_COMPRESS);
    tjhandle handleDecompress = tj3Init(TJINIT_DECOMPRESS);
    
    if (!handleCompress || !handleDecompress) {
        if (handleCompress) tj3Destroy(handleCompress);
        if (handleDecompress) tj3Destroy(handleDecompress);
        return 0;
    }

    // Get parameters from fuzzer input
    int width = get_valid_dimension(Data, Size, &offset);
    int height = get_valid_dimension(Data, Size, &offset);
    int align = get_valid_align(Data, Size, &offset);
    int subsamp = get_valid_subsamp(Data, Size, &offset);
    int pixelFormat = get_valid_pixel_format(Data, Size, &offset);
    int param = get_valid_param(Data, Size, &offset);
    int value = get_valid_value(Data, Size, &offset);

    // Set parameters on handles
    tj3Set(handleCompress, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handleDecompress, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handleCompress, param, value);
    tj3Set(handleDecompress, param, value);

    // Calculate buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    size_t rgbBufSize = width * height * tjPixelSize[pixelFormat];
    
    if (yuvBufSize == 0 || jpegBufSize == 0 || rgbBufSize == 0) {
        tj3Destroy(handleCompress);
        tj3Destroy(handleDecompress);
        return 0;
    }

    // Allocate buffers
    unsigned char *srcRGB = (unsigned char *)malloc(rgbBufSize);
    unsigned char *dstRGB = (unsigned char *)malloc(rgbBufSize);
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
    unsigned char *jpegBuf = (unsigned char *)malloc(jpegBufSize);
    size_t jpegSize = jpegBufSize;

    if (!srcRGB || !dstRGB || !yuvBuf || !jpegBuf) {
        free(srcRGB);
        free(dstRGB);
        free(yuvBuf);
        free(jpegBuf);
        tj3Destroy(handleCompress);
        tj3Destroy(handleDecompress);
        return 0;
    }

    // Fill buffers with fuzzer data
    size_t copySize = Size - offset;
    if (copySize > rgbBufSize) copySize = rgbBufSize;
    if (copySize > 0) {
        memcpy(srcRGB, Data + offset, copySize);
    }
    if (copySize < rgbBufSize) {
        memset(srcRGB + copySize, 0, rgbBufSize - copySize);
    }

    copySize = Size - offset;
    if (copySize > yuvBufSize) copySize = yuvBufSize;
    if (copySize > 0) {
        memcpy(yuvBuf, Data + offset, copySize);
    }
    if (copySize < yuvBufSize) {
        memset(yuvBuf + copySize, 0, yuvBufSize - copySize);
    }

    // Test tj3EncodeYUV8
    tj3EncodeYUV8(handleCompress, srcRGB, width, 0, height, pixelFormat, yuvBuf, align);

    // Test tj3DecodeYUV8
    tj3DecodeYUV8(handleDecompress, yuvBuf, align, dstRGB, width, 0, height, pixelFormat);

    // Test tj3CompressFromYUV8
    tj3CompressFromYUV8(handleCompress, yuvBuf, width, align, height, &jpegBuf, &jpegSize);

    // Cleanup
    free(srcRGB);
    free(dstRGB);
    free(yuvBuf);
    free(jpegBuf);
    tj3Destroy(handleCompress);
    tj3Destroy(handleDecompress);

    return 0;
}