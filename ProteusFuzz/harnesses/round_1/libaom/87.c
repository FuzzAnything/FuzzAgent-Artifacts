// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, arg);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid buffer overflows
    // We need at least 20 bytes for basic config, plus enough for SVC params arrays
    // AOM_MAX_LAYERS is typically 9, AOM_MAX_SS_LAYERS is 3, AOM_MAX_TS_LAYERS is 3
    // We'll use a safe minimum of 64 bytes to cover all array accesses safely
    if (Size < 64) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where safe
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 4) + 1;

    aom_codec_err_t init_res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    int skip_postproc = Data[6] % 2;
    fuzz_encoder_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_postproc);

    // Fuzz AV1E_SET_SVC_PARAMS
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = (Data[7] % 3) + 1;
    svc_params.number_temporal_layers = (Data[8] % 3) + 1;
    
    // Safe access to arrays with bounds checking
    for (int i = 0; i < AOM_MAX_LAYERS && (11 + i) < Size; i++) {
        svc_params.max_quantizers[i] = (Data[11 + i] % 64) + 1;
        svc_params.min_quantizers[i] = (Data[12 + i] % 64) + 1;
        svc_params.layer_target_bitrate[i] = (Data[13 + i] % 1000) + 100;
    }
    
    for (int i = 0; i < AOM_MAX_SS_LAYERS && (14 + i) < Size; i++) {
        svc_params.scaling_factor_num[i] = (Data[14 + i] % 256) + 1;
        svc_params.scaling_factor_den[i] = (Data[15 + i] % 256) + 1;
    }
    
    for (int i = 0; i < AOM_MAX_TS_LAYERS && (16 + i) < Size; i++) {
        svc_params.framerate_factor[i] = (Data[16 + i] % 256) + 1;
    }
    
    fuzz_encoder_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);

    // Fuzz AV1_SET_SKIP_LOOP_FILTER
    int skip_loop_filter = Data[17] % 2;
    fuzz_encoder_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, &skip_loop_filter);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    int enable_intrabc = Data[18] % 2;
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    int loopfilter_level = Data[19] % 64;
    fuzz_encoder_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, &loopfilter_level);

    // Fuzz AOME_GET_LOOPFILTER_LEVEL
    int get_loopfilter_level;
    fuzz_encoder_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, &get_loopfilter_level);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}