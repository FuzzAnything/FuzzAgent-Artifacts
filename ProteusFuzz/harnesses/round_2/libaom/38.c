// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int32_t read_int32_safe(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + sizeof(int32_t)) {
        return 0;
    }
    int32_t val;
    memcpy(&val, Data + offset, sizeof(int32_t));
    return val;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    int32_t val1 = read_int32_safe(Data, Size, 0);
    int32_t val2 = read_int32_safe(Data, Size, 4);
    int32_t val3 = read_int32_safe(Data, Size, 8);
    int32_t val4 = read_int32_safe(Data, Size, 12);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val1);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val2);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val3);

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val4);

    // Fuzz AOME_SET_NUMBER_SPATIAL_LAYERS
    aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val1 & 0x7);

    // Fuzz AV1E_SET_SVC_PARAMS
    if (Size >= 24) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = (Data[16] % 4) + 1;
        svc_params.number_temporal_layers = (Data[17] % 4) + 1;
        
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            if (Size >= 18 + i) svc_params.max_quantizers[i] = (Data[18 + i] % 64) + 1;
            if (Size >= 19 + i) svc_params.min_quantizers[i] = (Data[19 + i] % 32) + 1;
        }
        
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            if (Size >= 20 + i) svc_params.scaling_factor_num[i] = (Data[20 + i] % 256);
            if (Size >= 21 + i) svc_params.scaling_factor_den[i] = (Data[21 + i] % 256) + 1;
        }
        
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            if (Size >= 22 + i) svc_params.framerate_factor[i] = (Data[22 + i] % 16) + 1;
        }
        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    cleanup_encoder_context(ctx);
    return 0;
}