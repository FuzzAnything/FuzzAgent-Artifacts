// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t val;

    // 1. AV1E_SET_MAX_REFERENCE_FRAMES
    if (size >= sizeof(int32_t)) {
        memcpy(&val, data, sizeof(int32_t));
        aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val);
    }

    // 2. AV1E_SET_NOISE_SENSITIVITY
    if (size >= sizeof(int32_t) * 2) {
        memcpy(&val, data + sizeof(int32_t), sizeof(int32_t));
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    }

    // 3. AV1E_SET_QM_MAX
    if (size >= sizeof(int32_t) * 3) {
        memcpy(&val, data + sizeof(int32_t) * 2, sizeof(int32_t));
        aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
    }

    // 4. AV1E_SET_SVC_PARAMS
    if (size >= sizeof(int32_t) * 3 + sizeof(aom_svc_params_t)) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        
        memcpy(&svc_params, data + sizeof(int32_t) * 3, sizeof(svc_params));
        
        if (svc_params.number_spatial_layers < 1) svc_params.number_spatial_layers = 1;
        if (svc_params.number_temporal_layers < 1) svc_params.number_temporal_layers = 1;
        
        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // 5. AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (size >= sizeof(int32_t) * 4) {
        memcpy(&val, data + sizeof(int32_t) * 3, sizeof(int32_t));
        aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val);
    }

    // 6. AOME_SET_ENABLEAUTOALTREF
    if (size >= sizeof(int32_t) * 5) {
        memcpy(&val, data + sizeof(int32_t) * 4, sizeof(int32_t));
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.g_threads = (Data[3] % 16) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_control(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}