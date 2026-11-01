// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to vary configuration slightly
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = ((Data[0] >> 1) % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[0] % 100) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Prepare a dummy frame
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(frame.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(frame.planes[1], 128, cfg.g_w * cfg.g_h / 4);
    memset(frame.planes[2], 128, cfg.g_w * cfg.g_h / 4);

    // Function 1: AV1E_SET_ENABLE_FLIP_IDTX
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val1);

    // Function 2: AV1E_SET_ENABLE_GLOBAL_MOTION
    int val2 = (Size > 1) ? (Data[1] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val2);

    // Function 3: AV1E_SET_DISABLE_TRELLIS_QUANT
    int val3 = (Size > 2) ? (Data[2] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);

    // Function 4: AV1E_SET_MODE_COST_UPD_FREQ
    int val4 = (Size > 3) ? (Data[3] % 10) + 1 : 1;
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val4);

    // Function 5: AOME_SET_ARNR_MAXFRAMES
    int val5 = (Size > 4) ? (Data[4] % 16) + 1 : 1;
    aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, val5);

    // Function 6: AOME_SET_ARNR_STRENGTH
    int val6 = (Size > 5) ? (Data[5] % 8) : 0;
    aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, val6);

    // Encode a frame to exercise the state changes
    // Signature: aom_codec_encode(ctx, img, pts, duration, flags)
    aom_codec_encode(ctx, &frame, 0, 1, 0);

    // Cleanup
    aom_img_free(&frame);
    cleanup_codec(ctx);

    return 0;
}