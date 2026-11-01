// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static int fuzz_image_allocation(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    // Extract parameters from fuzz input
    unsigned int width = data[0] | (data[1] << 8);
    unsigned int height = data[2] | (data[3] << 8);
    unsigned int align = size > 4 ? data[4] : 16;

    // Enforce constraints: dimensions <= 2^27, alignment <= 65536
    if (width > 0x08000000 || height > 0x08000000) return 0;
    if (align > 65536) align = 65536;
    if (align == 0) align = 16;
    if (width == 0 || height == 0) return 0;

    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (size > 5 && data[5] > 0 && data[5] <= 9) {
        fmt = (vpx_img_fmt_t)data[5];
    }

    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return 0;

    // Optionally write some data to the image planes if we have enough input
    if (size > 8) {
        size_t plane_size = img->planes[0] ? (size_t)img->stride[0] * img->h : 0;
        if (plane_size > 0 && size > 8) {
            size_t copy_size = size - 8;
            if (copy_size > plane_size) copy_size = plane_size;
            memcpy(img->planes[0], data + 8, copy_size);
        }
    }

    vpx_img_free(img);
    return 0;
}

static int fuzz_vp9_encoder_interface(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    // We cannot fully initialize the encoder without more complex setup,
    // but we can verify the interface exists.
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz image allocation
    fuzz_image_allocation(Data, Size);

    // Fuzz VP9 encoder interface retrieval
    fuzz_vp9_encoder_interface(Data, Size);

    return 0;
}