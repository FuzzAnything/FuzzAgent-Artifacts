// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void free_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_234(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!initialize_encoder(&ctx, &cfg)) return 0;

    // Parse input to determine behavior
    uint32_t width = (Data[0] % 32) + 16;
    uint32_t height = (Data[1] % 32) + 16;
    uint8_t bit_depth = (Data[2] % 4) + 8; // 8, 9, 10, 11
    uint8_t format_idx = Data[3] % 4; // 0: I420, 1: I42016, 2: I422, 3: I444

    aom_img_fmt_t fmt;
    switch (format_idx) {
        case 0: fmt = AOM_IMG_FMT_I420; break;
        case 1: fmt = AOM_IMG_FMT_I42016; break;
        case 2: fmt = AOM_IMG_FMT_I422; break;
        case 3: fmt = AOM_IMG_FMT_I444; break;
        default: fmt = AOM_IMG_FMT_I420;
    }

    // Adjust config for bit depth if needed
    if (bit_depth > 8) {
        cfg.g_bit_depth = bit_depth;
    }

    aom_image_t img;
    aom_img_alloc(&img, fmt, width, height, 16);
    if (!img.planes[0]) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            size_t plane_size = (size_t)img.stride[i] * height;
            if (plane_size > 0 && Size > 0) {
                memcpy(img.planes[i], Data, Size > plane_size ? plane_size : Size);
            }
        }
    }

    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    // Fuzz control functions
    if (Size > 4) {
        int val = Data[4];
        // AV1E_SET_ENABLE_INTRABC
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val) != AOM_CODEC_OK) {
            // Ignore errors for fuzzing
        }
    }
    if (Size > 5) {
        int val = Data[5];
        // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }
    if (Size > 6) {
        int val = Data[6];
        // AV1E_SET_AUTO_INTRA_TOOLS_OFF
        if (aom_codec_control(&ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, val) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }
    if (Size > 7) {
        int val = Data[7];
        // AV1E_SET_TIER_MASK
        if (aom_codec_control(&ctx, AV1E_SET_TIER_MASK, val) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }
    // AV1E_SET_RATE_DISTRIBUTION_INFO requires a struct, skip for simple fuzzing

    // Encode frame
    aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, flags);
    if (res != AOM_CODEC_OK) {
        // Continue to flush
    }

    // Flush encoder
    while (1) {
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&ctx, &iter);
        if (!pkt) break;
        // Process pkt if needed, but for fuzzing we just iterate
    }

    free_image(&img);
    cleanup_encoder(&ctx);

    return 0;
}