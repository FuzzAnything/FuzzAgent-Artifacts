// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = AOM_CQ;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    int val;
    int idx = 0;

    // Helper to get int from fuzz data
    #define GET_INT() ((idx < Size) ? (int)Data[idx++] : 0)
    #define GET_BOOL() ((idx < Size) ? (Data[idx++] != 0) : 0)

    // 1. AV1E_SET_ENABLE_DIFF_WTD_COMP
    val = GET_BOOL();
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);

    // 2. AV1E_SET_DISABLE_TRELLIS_QUANT
    val = GET_BOOL();
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val);

    // 3. AV1E_SET_ERROR_RESILIENT_MODE
    val = GET_INT() % 4; // Assuming modes 0-3
    aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val);

    // 4. AV1E_SET_QM_Y
    val = GET_INT() % 64; // QM values usually 0-63
    aom_codec_control(&ctx, AV1E_SET_QM_Y, val);

    // 5. AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    val = GET_INT() % 16; // Reasonable range
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val);

    // 6. AV1E_SET_ALLOW_REF_FRAME_MVS
    val = GET_BOOL();
    aom_codec_control(&ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val);

    // Optional: Encode a dummy frame to ensure state is exercised
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    memset(img.planes[0], 128, 64*64);
    memset(img.planes[1], 128, 32*32);
    memset(img.planes[2], 128, 32*32);

    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}