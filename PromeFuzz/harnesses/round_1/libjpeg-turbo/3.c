// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        *size = 0;
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    const uint8_t *input = Data;
    size_t remaining = Size;

    // Initialize TurboJPEG instance
    int init_type = read_int(&input, &remaining);
    tjhandle handle = tj3Init(init_type);
    if (!handle) {
        return 0;
    }

    // Set parameters (8 calls)
    for (int i = 0; i < 8; i++) {
        int param = read_int(&input, &remaining);
        int value = read_int(&input, &remaining);
        tj3Set(handle, param, value);
    }

    // Get JPEG buffer size
    int width = read_int(&input, &remaining);
    int height = read_int(&input, &remaining);
    int jpeg_subsamp = read_int(&input, &remaining);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, jpeg_subsamp);

    // Allocate JPEG buffer
    unsigned char *jpeg_buf = NULL;
    if (jpeg_buf_size > 0) {
        jpeg_buf = (unsigned char *)tj3Alloc(jpeg_buf_size);
    }

    // Get YUV buffer size
    int yuv_width = read_int(&input, &remaining);
    int yuv_align = read_int(&input, &remaining);
    int yuv_height = read_int(&input, &remaining);
    int yuv_subsamp = read_int(&input, &remaining);
    size_t yuv_buf_size = tj3YUVBufSize(yuv_width, yuv_align, yuv_height, yuv_subsamp);

    // Allocate YUV buffer
    unsigned char *yuv_buf = NULL;
    if (yuv_buf_size > 0) {
        yuv_buf = (unsigned char *)tj3Alloc(yuv_buf_size);
    }

    // Set more parameters (2 calls)
    for (int i = 0; i < 2; i++) {
        int param = read_int(&input, &remaining);
        int value = read_int(&input, &remaining);
        tj3Set(handle, param, value);
    }

    // Prepare source image buffer from remaining fuzzer data
    int src_width = read_int(&input, &remaining);
    int src_pitch = read_int(&input, &remaining);
    int src_height = read_int(&input, &remaining);
    int pixel_format = read_int(&input, &remaining);
    int src_align = read_int(&input, &remaining);

    // Use remaining data as source image
    size_t src_size = remaining;
    const unsigned char *src_buf = input;

    // Encode YUV if we have valid buffers
    if (yuv_buf && src_buf && src_size > 0) {
        tj3EncodeYUV8(handle, src_buf, src_width, src_pitch, src_height,
                      pixel_format, yuv_buf, src_align);
    }

    // Compress from YUV if we have valid buffers
    if (jpeg_buf && yuv_buf && yuv_buf_size > 0) {
        size_t jpeg_size = jpeg_buf_size;
        unsigned char *jpeg_ptr = jpeg_buf;
        tj3CompressFromYUV8(handle, yuv_buf, yuv_width, yuv_align, yuv_height,
                            &jpeg_ptr, &jpeg_size);
    }

    // Cleanup
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    if (yuv_buf) {
        tj3Free(yuv_buf);
    }
    tj3Destroy(handle);

    return 0;
}