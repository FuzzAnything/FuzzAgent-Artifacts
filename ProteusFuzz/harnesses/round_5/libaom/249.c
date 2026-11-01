// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int idx = 0;
    int val;
    aom_svc_ref_frame_config_t svc_config;

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    if (idx + sizeof(int) <= size) {
        val = *(int *)(data + idx);
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
    }

    // Fuzz AV1E_SET_SVC_REF_FRAME_CONFIG
    if (idx + sizeof(svc_config) <= size) {
        memcpy(&svc_config, data + idx, sizeof(svc_config));
        idx += sizeof(svc_config);
        aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_config);
    }

    // Fuzz AOME_SET_SPATIAL_LAYER_ID
    if (idx + sizeof(int) <= size) {
        val = *(int *)(data + idx);
        idx += sizeof(int);
        aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val);
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    if (idx + sizeof(int) <= size) {
        val = *(int *)(data + idx);
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);
    }

    // Fuzz AOME_SET_NUMBER_SPATIAL_LAYERS
    if (idx + sizeof(int) <= size) {
        val = *(int *)(data + idx);
        idx += sizeof(int);
        aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val);
    }

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    if (idx + sizeof(int) <= size) {
        val = *(int *)(data + idx);
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize codec context
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}