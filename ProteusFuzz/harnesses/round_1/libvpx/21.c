// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_encoder.h"

static int fuzz_vpx(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Extract parameters from fuzzing data
    unsigned int width = Data[0] | (Data[1] << 8);
    unsigned int height = Data[2] | (Data[3] << 8);
    unsigned int align = Data[4] | (Data[5] << 8);
    int cpu_used = Data[6] & 0x0F; // Limit to 0-15, valid range is 0-8
    int tile_cols = Data[7] & 0x0F; // Limit to 0-15, valid range is 1-8 usually

    // Constrain dimensions to valid ranges as per API docs
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align > 65536) align = 65536;
    if (align == 0) align = 1;

    // Ensure minimum dimensions for valid allocation
    if (width == 0) width = 16;
    if (height == 0) height = 16;

    // Select image format based on data
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 8) {
        switch (Data[8] % 4) {
            case 0: fmt = VPX_IMG_FMT_I420; break;
            case 1: fmt = VPX_IMG_FMT_YV12; break;
            case 2: fmt = VPX_IMG_FMT_I422; break;
            case 3: fmt = VPX_IMG_FMT_I444; break;
            default: fmt = VPX_IMG_FMT_I420; break;
        }
    }

    // 1. vpx_img_alloc
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return 0;
    }

    // 2. vpx_codec_vp9_cx
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        vpx_img_free(img);
        return 0;
    }

    // Initialize encoder context
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize config structure manually since vpx_codec_enc_config_init might be missing
    // or requires specific headers not guaranteed in all builds.
    // We rely on vpx_codec_enc_init to validate and set defaults if needed, 
    // but we must set required fields before init.
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_profile = 0; // Default profile
    cfg.g_pass = 0; // Single pass

    if (vpx_codec_enc_init(&enc, iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // 3. VP8E_SET_CPUUSED__value
    // Control ID for CPU usage
    int cpu_val = cpu_used;
    if (cpu_val > 8) cpu_val = 8;
    vpx_codec_control(&enc, VP8E_SET_CPUUSED, cpu_val);

    // 4. VP9E_SET_TILE_COLUMNS__value
    // Control ID for tile columns
    int tile_val = tile_cols;
    if (tile_val < 1) tile_val = 1;
    if (tile_val > 8) tile_val = 8;
    vpx_codec_control(&enc, VP9E_SET_TILE_COLUMNS, tile_val);

    // Encode a single frame to exercise the state
    vpx_codec_encode(&enc, img, 0, 1, 0, 0);

    // Cleanup
    vpx_codec_destroy(&enc);
    vpx_img_free(img);

    return 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    return fuzz_vpx(Data, Size);
}