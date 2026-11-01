// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t *init_encoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    int ret;
    int value = 0;

    if (Size < 1) return 0;

    // Use input data to determine which control to test and the value to set
    int control_idx = Data[0] % 6;
    value = (Size > 1) ? (Data[1] % 2) : 0; // 0 or 1 for boolean flags

    ctx = init_encoder();
    if (!ctx) return 0;

    switch (control_idx) {
        case 0:
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, value);
            break;
        case 1:
            ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, value);
            break;
        case 2:
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, value);
            break;
        case 3:
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, value);
            break;
        case 4:
            ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, value);
            break;
        case 5:
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
            break;
        default:
            break;
    }

    // Additional exploration: try setting with different values if Size allows
    if (Size > 2) {
        int alt_value = Data[2] % 2;
        if (control_idx == 0) ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, alt_value);
        else if (control_idx == 1) ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, alt_value);
        else if (control_idx == 2) ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, alt_value);
        else if (control_idx == 3) ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, alt_value);
        else if (control_idx == 4) ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, alt_value);
        else if (control_idx == 5) ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, alt_value);
    }

    // Try to encode a frame to ensure state is exercised
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;
    aom_codec_frame_flags_t flags = 0;

    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    memset(img.planes[0], 0, 64 * 64);
    memset(img.planes[1], 128, 32 * 32);
    memset(img.planes[2], 128, 32 * 32);

    ret = aom_codec_encode(ctx, &img, 0, 1, flags);
    if (ret == AOM_CODEC_OK) {
        while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                // No need to manually destroy frame data here as it's managed by the codec context
                // or should be handled by aom_codec_destroy when the context is cleaned up.
            }
        }
    }

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}