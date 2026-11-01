// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int try_init_encoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface,
                            const aom_codec_enc_cfg_t *cfg, aom_codec_flags_t flags) {
    if (!ctx || !iface || !cfg) return 0;
    if (aom_codec_enc_init(ctx, iface, cfg, flags) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static uint8_t safe_read_byte(const uint8_t *Data, size_t Size, size_t index, uint8_t default_val) {
    if (index < Size) {
        return Data[index];
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder parameters fuzzily with bounds checking
    cfg.g_w = safe_read_byte(Data, Size, 0, 64);
    if (cfg.g_w == 0) cfg.g_w = 64;
    cfg.g_h = safe_read_byte(Data, Size, 1, 64);
    if (cfg.g_h == 0) cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = safe_read_byte(Data, Size, 2, 30);
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = safe_read_byte(Data, Size, 3, 1000);
    if (cfg.rc_target_bitrate == 0) cfg.rc_target_bitrate = 1000;
    cfg.g_threads = safe_read_byte(Data, Size, 4, 1);
    if (cfg.g_threads == 0) cfg.g_threads = 1;
    cfg.g_error_resilient = safe_read_byte(Data, Size, 5, 0) & 0x1;

    if (!try_init_encoder(&ctx, iface, &cfg, 0)) {
        return 0;
    }

    // Prepare SVC params for AV1E_SET_SVC_PARAMS
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    
    // Ensure we have enough data for basic SVC params
    if (Size > 10) {
        svc_params.number_spatial_layers = (safe_read_byte(Data, Size, 6, 1) % 4) + 1; // 1 to 4
        svc_params.number_temporal_layers = (safe_read_byte(Data, Size, 7, 1) % 4) + 1; // 1 to 4
        
        // Limit loops to prevent out of bounds based on Size
        size_t max_layers = (Size - 8) / 3; 
        if (max_layers > AOM_MAX_LAYERS) max_layers = AOM_MAX_LAYERS;
        
        for (size_t i = 0; i < max_layers; i++) {
            svc_params.max_quantizers[i] = safe_read_byte(Data, Size, 8 + i, 0) % 256;
            svc_params.min_quantizers[i] = safe_read_byte(Data, Size, 9 + i, 0) % 256;
            svc_params.layer_target_bitrate[i] = safe_read_byte(Data, Size, 10 + i, 0) * 100;
        }
        
        size_t max_ts = (Size - 12) / 1;
        if (max_ts > AOM_MAX_TS_LAYERS) max_ts = AOM_MAX_TS_LAYERS;
        for (size_t i = 0; i < max_ts; i++) {
            svc_params.framerate_factor[i] = safe_read_byte(Data, Size, 12 + i, 1) % 10;
        }
        
        size_t max_ss = (Size - 14) / 2;
        if (max_ss > AOM_MAX_SS_LAYERS) max_ss = AOM_MAX_SS_LAYERS;
        for (size_t i = 0; i < max_ss; i++) {
            svc_params.scaling_factor_num[i] = safe_read_byte(Data, Size, 14 + i, 1) % 100;
            int den = safe_read_byte(Data, Size, 15 + i, 1);
            svc_params.scaling_factor_den[i] = (den == 0) ? 1 : den;
        }
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    int noise_level = safe_read_byte(Data, Size, Size - 1, 0) % 256;
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, &noise_level);

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    int loopfilter_ctrl = safe_read_byte(Data, Size, Size - 2, 0) % 256;
    aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, &loopfilter_ctrl);

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    int skip_postproc = safe_read_byte(Data, Size, Size - 3, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_postproc);

    // Fuzz AV1_SET_SKIP_LOOP_FILTER
    int skip_loop_filter = safe_read_byte(Data, Size, Size - 4, 0) % 2;
    aom_codec_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, &skip_loop_filter);

    // Fuzz AOME_GET_LOOPFILTER_LEVEL
    int loopfilter_level;
    aom_codec_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}