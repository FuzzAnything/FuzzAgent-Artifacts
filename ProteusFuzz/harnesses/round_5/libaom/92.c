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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL && ctx->priv != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to drive control parameters
    int val1 = Data[0];
    int val2 = Data[1];
    int val3 = (Size > 2) ? Data[2] : 0;
    int val4 = (Size > 3) ? Data[3] : 0;
    int val5 = (Size > 4) ? Data[4] : 0;
    int val6 = (Size > 5) ? Data[5] : 0;

    // 1. AV1E_SET_INTER_DCT_ONLY
    if (aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val1) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 2. AV1E_SET_REDUCED_TX_TYPE_SET
    if (aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val2) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 3. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val3) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 4. AV1E_SET_SVC_LAYER_ID
    if (Size > 6) {
        aom_svc_layer_id_t svc_id;
        svc_id.spatial_layer_id = Data[6];
        svc_id.temporal_layer_id = Data[7];
        if (aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &svc_id) != AOM_CODEC_OK) {
            // Ignore errors
        }
    } else {
        aom_svc_layer_id_t svc_id = {0, 0};
        if (aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &svc_id) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    // 5. AV1E_SET_ENABLE_AB_PARTITIONS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val5) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 6. AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val6) != AOM_CODEC_OK) {
        // Ignore errors
    }

    cleanup_codec(&ctx);
    return 0;
}