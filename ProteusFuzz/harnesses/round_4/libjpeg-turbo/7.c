// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

static int get_pixel_size(int pixel_format) {
    switch (pixel_format) {
        case TJPF_RGB:
        case TJPF_BGR:
        case TJPF_RGBA:
        case TJPF_BGRA:
        case TJPF_ABGR:
        case TJPF_ARGB:
            return 4;
        case TJPF_GRAY:
            return 1;
        case TJPF_CMYK:
            return 4;
        default:
            return 4;
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    // Extract parameters from input
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixel_format = Data[2] % 7; // TJPF_RGB(0) to TJPF_CMYK(6)
    int subsamp = Data[3] % 5;      // TJCSAMP_444 to TJCSAMP_GRAY
    int align = 1 << (Data[4] & 7); // Power of 2 alignment

    // Cap dimensions to prevent OOM
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Initialize handles
    tjhandle compHandle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);

    if (!compHandle || !decompHandle) return 0;

    // Allocate buffers
    size_t rgb_size = (size_t)width * height * get_pixel_size(pixel_format);
    size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpeg_size = tj3JPEGBufSize(width, height, subsamp);

    unsigned char *rgb_buf = (unsigned char *)malloc(rgb_size);
    unsigned char *yuv_buf = (unsigned char *)malloc(yuv_size);
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_buf_size = 0;

    if (!rgb_buf || !yuv_buf) {
        tj3Destroy(compHandle);
        tj3Destroy(decompHandle);
        free(rgb_buf);
        free(yuv_buf);
        return 0;
    }

    // Fill buffers with fuzz data
    memset(rgb_buf, 0, rgb_size);
    size_t data_offset = 5;
    size_t data_len = Size - data_offset;
    if (data_len > rgb_size) {
        data_len = rgb_size;
    }
    if (data_len > 0) {
        memcpy(rgb_buf, Data + data_offset, data_len);
    }
    memset(yuv_buf, 0, yuv_size);

    // Configure handles
    tj3Set(compHandle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(decompHandle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compHandle, TJPARAM_QUALITY, 50);

    // 1. tj3EncodeYUV8: RGB -> YUV
    int pitch = width * get_pixel_size(pixel_format);
    tj3EncodeYUV8(compHandle, rgb_buf, width, pitch, height, pixel_format, yuv_buf, align);

    // 2. tj3CompressFromYUV8: YUV -> JPEG
    tj3CompressFromYUV8(compHandle, yuv_buf, width, align, height, &jpeg_buf, &jpeg_buf_size);

    // 3. tj3DecompressToYUV8: JPEG -> YUV
    unsigned char *yuv_buf2 = (unsigned char *)malloc(yuv_size);
    if (yuv_buf2 && jpeg_buf && jpeg_buf_size > 0) {
        tj3DecompressToYUV8(decompHandle, jpeg_buf, jpeg_buf_size, yuv_buf2, align);
        free(yuv_buf2);
    }

    // 4. tj3DecodeYUV8: YUV -> RGB
    unsigned char *rgb_buf2 = (unsigned char *)malloc(rgb_size);
    if (rgb_buf2) {
        tj3DecodeYUV8(decompHandle, yuv_buf, align, rgb_buf2, width, pitch, height, pixel_format);
        free(rgb_buf2);
    }

    // Cleanup
    tj3Destroy(compHandle);
    tj3Destroy(decompHandle);
    free(rgb_buf);
    free(yuv_buf);
    if (jpeg_buf) tj3Free(jpeg_buf);

    return 0;
}