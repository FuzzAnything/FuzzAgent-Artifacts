// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic configuration
    if (Size < 12) {
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

    // Use input data to configure encoder settings with bounds checking
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) + 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions
    // 1. AV1E_SET_COEFF_COST_UPD_FREQ
    int freq_val = (Data[5] % 100) + 1;
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq_val);

    // 2. AV1E_SET_ENABLE_REF_FRAME_MVS
    int ref_mvs = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, ref_mvs);

    // 3. AV1E_SET_ENABLE_SUPERRES
    int superres = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, superres);

    // 4. AV1E_SET_TARGET_SEQ_LEVEL_IDX
    int level_idx = (Data[8] % 10);
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);

    // 5. AV1E_SET_ENABLE_FILTER_INTRA
    int filter_intra = (Data[9] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra);

    // 6. AV1E_SET_ALLOW_WARPED_MOTION
    int warped_motion = (Data[10] % 2);
    aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, warped_motion);

    // Attempt to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill the image plane with data from the input
    if (img.planes[0] && img.stride[0] > 0 && cfg.g_h > 0) {
        size_t fill_size = (size_t)img.stride[0] * cfg.g_h;
        // Use a safe loop to fill without overflowing if input is small
        for (size_t i = 0; i < fill_size; i++) {
            img.planes[0][i] = Data[i % Size];
        }
    }

    aom_enc_frame_flags_t flags = (Size > 11) ? (Data[11] % 4) : 0;

    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Cleanup
    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}