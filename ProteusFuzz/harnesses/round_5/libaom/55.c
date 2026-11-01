// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int width = 64;
    int height = 64;
    int fps_num = 30;
    int fps_den = 1;

    if (size > 0) {
        if (size >= 2) {
            width = (data[0] % 1024) + 16;
            height = (data[1] % 1024) + 16;
        }
        if (size >= 4) {
            fps_num = (data[2] % 120) + 1;
            fps_den = (data[3] % 120) + 1;
        }
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int enable_val;

    if (!Data || Size == 0) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Derive values from input data to control parameters
    if (Size >= 1) {
        enable_val = Data[0] % 2;
    } else {
        enable_val = 0;
    }

    // 1. AV1E_SET_ENABLE_PALETTE
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
    (void)ret;

    // 2. AV1E_SET_CHROMA_SAMPLE_POSITION
    if (Size >= 2) {
        int pos = Data[1] % 3; // 0, 1, or 2
        ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, pos);
        (void)ret;
    }

    // 3. AV1E_SET_LOOPFILTER_CONTROL
    if (Size >= 3) {
        int lf_val = Data[2] % 64;
        ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_val);
        (void)ret;
    }

    // 4. AV1E_SET_ENABLE_CFL_INTRA
    if (Size >= 4) {
        int cfl_val = Data[3] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, cfl_val);
        (void)ret;
    }

    // 5. AV1E_SET_ENABLE_RECT_PARTITIONS
    if (Size >= 5) {
        int rect_val = Data[4] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, rect_val);
        (void)ret;
    }

    // 6. AV1E_SET_ENABLE_WARPED_MOTION
    if (Size >= 6) {
        int warp_val = Data[5] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, warp_val);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}