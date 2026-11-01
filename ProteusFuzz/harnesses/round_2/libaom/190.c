// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int tune_content = (Data[4] % 3); 
    int enable_flip_idtx = (Data[5] % 2);
    int cdf_update_mode = (Data[6] % 3);
    int inter_dct_only = (Data[7] % 2);
    int intra_default_tx_only = (Data[8] % 2);
    int enable_warped_motion = (Data[9] % 2);

    fuzz_encoder_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);
    fuzz_encoder_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);
    fuzz_encoder_control(&ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, enable_warped_motion);

    aom_codec_destroy(&ctx);

    return 0;
}