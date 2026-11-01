// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static void safe_cleanup(vpx_image_t *img, vpx_codec_ctx_t *codec) {
    if (img != NULL) {
        vpx_img_free(img);
    }
    if (codec != NULL) {
        vpx_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_image_t *img = NULL;
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = NULL;
    int ret = 0;

    // Extract parameters from fuzz input with bounds checking
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int align = 16;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;

    if (Size >= 4) {
        width = *(const unsigned int *)(Data);
        if (Size >= 8) {
            height = *(const unsigned int *)(Data + 4);
        }
        if (Size >= 12) {
            align = *(const unsigned int *)(Data + 8);
        }
        if (Size >= 16) {
            int fmt_idx = *(const int *)(Data + 12);
            // Map fuzzed index to valid enum values safely
            if (fmt_idx >= 0 && fmt_idx <= 9) {
                fmt = (vpx_img_fmt_t)fmt_idx;
            }
        }
    }

    // Enforce constraints mentioned in documentation
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align > 65536) align = 65536;
    if (align == 0) align = 16;

    // 1. Invoke vpx_img_alloc
    img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (img == NULL) {
        return 0;
    }

    // 2. Invoke vpx_codec_vp9_cx
    iface = vpx_codec_vp9_cx();
    if (iface == NULL) {
        safe_cleanup(img, NULL);
        return 0;
    }

    // Initialize codec context
    if (vpx_codec_enc_init(&codec, iface, NULL, 0) != VPX_CODEC_OK) {
        safe_cleanup(img, &codec);
        return 0;
    }

    // 3. Invoke VP8E_SET_CPUUSED__value (using vpx_codec_control)
    // Value range 0-8, fuzzed from input if available
    int cpu_used = 0;
    if (Size >= 20) {
        cpu_used = *(const int *)(Data + 16) % 9; // Ensure 0-8
    }
    ret = vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    // Ignore return value for fuzzing stability, but check for crash

    // 4. Invoke VP9E_SET_TILE_COLUMNS__value (using vpx_codec_control)
    // Value range typically 1-8, fuzzed from input
    int tile_cols = 1;
    if (Size >= 24) {
        tile_cols = *(const int *)(Data + 20) % 8;
        if (tile_cols < 1) tile_cols = 1;
    }
    ret = vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_cols);
    // Ignore return value for fuzzing stability

    // Cleanup
    safe_cleanup(img, &codec);

    return 0;
}