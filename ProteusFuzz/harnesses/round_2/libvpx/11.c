// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_vpx(const uint8_t *data, size_t size) {
    if (size < 16) {
        return 0;
    }

    // Extract parameters from fuzz input
    uint32_t width = *(uint32_t *)(data);
    uint32_t height = *(uint32_t *)(data + 4);
    uint32_t align = *(uint32_t *)(data + 8);
    uint32_t fmt_val = *(uint32_t *)(data + 12);

    // Clamp values to safe ranges as per API requirements
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align == 0) align = 1;
    if (align > 65536) align = 65536;

    // Map fmt_val to a valid enum
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    switch (fmt_val % 10) {
        case 0: fmt = VPX_IMG_FMT_YV12; break;
        case 1: fmt = VPX_IMG_FMT_I420; break;
        case 2: fmt = VPX_IMG_FMT_I422; break;
        case 3: fmt = VPX_IMG_FMT_I444; break;
        case 4: fmt = VPX_IMG_FMT_I440; break;
        case 5: fmt = VPX_IMG_FMT_NV12; break;
        case 6: fmt = VPX_IMG_FMT_I42016; break;
        case 7: fmt = VPX_IMG_FMT_I42216; break;
        case 8: fmt = VPX_IMG_FMT_I44416; break;
        case 9: fmt = VPX_IMG_FMT_I44016; break;
    }

    // 1. vpx_img_alloc
    vpx_image_t img;
    vpx_image_t *img_ptr = vpx_img_alloc(&img, fmt, width, height, align);
    if (!img_ptr) {
        return 0;
    }

    // 2. vpx_codec_vp9_cx
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        vpx_img_free(img_ptr);
        return 0;
    }

    // Initialize encoder context to use controls
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&encoder, iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img_ptr);
        return 0;
    }

    // 3. VP8E_SET_CPUUSED__value (mapped to VP8E_SET_CPUUSED)
    // Value range 0-8
    int cpu_used = (size > 16) ? (data[16] % 9) : 0;
    if (vpx_codec_control(&encoder, VP8E_SET_CPUUSED, cpu_used) != VPX_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // 4. VP9E_SET_TILE_COLUMNS__value (mapped to VP9E_SET_TILE_COLUMNS)
    // Value range 1-64
    int tile_cols = (size > 17) ? (data[17] % 64 + 1) : 1;
    if (vpx_codec_control(&encoder, VP9E_SET_TILE_COLUMNS, tile_cols) != VPX_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_img_free(img_ptr);

    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    fuzz_vpx(Data, Size);
    return 0;
}