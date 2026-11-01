// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
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
#include <stdio.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_vpx(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from input data
    unsigned int width = (Data[0] << 8) | Data[1];
    unsigned int height = (Data[2] << 8) | Data[3];
    unsigned int align = (Size > 4) ? Data[4] : 16;

    // Clamp dimensions to valid ranges (max 2^27)
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align == 0) align = 16;
    if (align > 65536) align = 65536;

    // Select image format based on input
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 5) {
        switch (Data[5] % 10) {
            case 0: fmt = VPX_IMG_FMT_NONE; break;
            case 1: fmt = VPX_IMG_FMT_YV12; break;
            case 2: fmt = VPX_IMG_FMT_I420; break;
            case 3: fmt = VPX_IMG_FMT_I422; break;
            case 4: fmt = VPX_IMG_FMT_I444; break;
            case 5: fmt = VPX_IMG_FMT_I440; break;
            case 6: fmt = VPX_IMG_FMT_NV12; break;
            case 7: fmt = VPX_IMG_FMT_I42016; break;
            case 8: fmt = VPX_IMG_FMT_I42216; break;
            case 9: fmt = VPX_IMG_FMT_I44416; break;
        }
    }

    // 1. Allocate image
    vpx_image_t img;
    vpx_image_t *img_ptr = vpx_img_alloc(&img, fmt, width, height, align);
    if (!img_ptr) {
        return 0;
    }

    // 2. Get VP9 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        vpx_img_free(img_ptr);
        return 0;
    }

    // Initialize encoder context
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize config with defaults manually since vpx_codec_enc_config_init is not available
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_init(&enc, iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img_ptr);
        return 0;
    }

    // 3. Set CPU usage (VP8E_SET_CPUUSED)
    // Value range 0-8
    int cpu_used = (Size > 6) ? (Data[6] % 9) : 0;
    vpx_codec_control(&enc, VP8E_SET_CPUUSED, cpu_used);

    // 4. Set Tile Columns (VP9E_SET_TILE_COLUMNS)
    // Value range 1-64 (typical max), clamped to reasonable fuzz range
    int tile_cols = (Size > 7) ? (Data[7] % 64) : 1;
    if (tile_cols == 0) tile_cols = 1;
    vpx_codec_control(&enc, VP9E_SET_TILE_COLUMNS, tile_cols);

    // Cleanup
    vpx_codec_destroy(&enc);
    vpx_img_free(img_ptr);

    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    return fuzz_vpx(Data, Size);
}