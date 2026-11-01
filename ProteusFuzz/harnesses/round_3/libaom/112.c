// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
    }
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure encoder settings diversely
    if (Size > 0) cfg.g_w = Data[0] ? Data[0] : 64;
    if (Size > 1) cfg.g_h = Data[1] ? Data[1] : 64;
    if (Size > 2) cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] ? Data[3] : 30;
    if (Size > 4) cfg.rc_target_bitrate = Data[4] * 1000;
    if (Size > 5) cfg.g_error_resilient = Data[5] % 2;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz various control functions using input data
    int32_t val_int;
    uint8_t val_uint;

    // 1. AV1E_SET_ENABLE_QM
    if (Size > 6) {
        val_uint = Data[6];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val_uint);
    }

    // 2. AV1E_SET_DELTAQ_MODE
    if (Size > 7) {
        val_uint = Data[7];
        aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val_uint);
    }

    // 3. AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size > 8) {
        val_int = Data[8];
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val_int);
    }

    // 4. AV1E_SET_FRAME_PERIODIC_BOOST
    if (Size > 9) {
        val_int = Data[9];
        aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val_int);
    }

    // 5. AOME_SET_ENABLEAUTOBWDREF
    if (Size > 10) {
        val_uint = Data[10];
        aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val_uint);
    }

    // 6. AV1E_SET_DELTAQ_STRENGTH
    if (Size > 11) {
        val_int = Data[11];
        aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val_int);
    }

    // Attempt to encode a dummy frame to exercise state changes
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

    // Fixed: aom_codec_encode takes 5 arguments, not 6
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}