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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static void init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return;

    // Use input data to configure encoder settings fuzzily
    if (size > 0) {
        cfg->g_w = 16 + (data[0] % 1000);
        cfg->g_h = 16 + (data[1] % 1000);
        cfg->g_timebase.num = 1 + (data[2] % 100);
        cfg->g_timebase.den = 1 + (data[3] % 100);
        cfg->g_threads = 1 + (data[4] % 8);
        cfg->g_error_resilient = (data[5] % 2);
        cfg->g_lag_in_frames = (data[6] % 25);
        cfg->rc_target_bitrate = 1000 + (data[7] % 10000);
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder(&ctx, &cfg, Data, Size);

    if (ctx.err != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Extract parameters from input data for fuzzing
    int tune_content = Data[0] % 3; // 0: AOM_CONTENT_DEFAULT, 1: AOM_CONTENT_FILM, 2: AOM_CONTENT_PHOTO
    int enable_smooth_intra = Data[1] % 2;
    int enable_dual_filter = Data[2] % 2;
    int enable_intra_edge_filter = Data[3] % 2;
    int enable_smooth_interintra = Data[4] % 2;
    int spatial_layer_id = Data[5] % 4;
    int temporal_layer_id = Data[6] % 8;

    // Prepare SVC layer ID
    aom_svc_layer_id_t svc_id;
    svc_id.spatial_layer_id = spatial_layer_id;
    svc_id.temporal_layer_id = temporal_layer_id;

    // Attempt to call the control functions.
    // Using standard aom_codec_control with likely control codes.
    // Note: If the specific typechecked wrappers are required and have different
    // signatures, this driver might need adjustment once signatures are known.
    // Assuming standard control codes exist as AV1E_SET_...

    // 1. AV1E_SET_TUNE_CONTENT
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);

    // 2. AV1E_SET_ENABLE_SMOOTH_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_smooth_intra);

    // 3. AV1E_SET_SVC_LAYER_ID
    aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &svc_id);

    // 4. AV1E_SET_ENABLE_DUAL_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, enable_dual_filter);

    // 5. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);

    // 6. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_smooth_interintra);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}