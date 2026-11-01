// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    // Try AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    int val = data[0];
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, &val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Try AV1E_SET_SVC_PARAMS
    if (size >= sizeof(aom_svc_params_t)) {
        aom_svc_params_t params;
        memcpy(&params, data, sizeof(params));
        // Ensure valid ranges to avoid immediate crashes, but fuzz values
        if (params.number_spatial_layers < 1) params.number_spatial_layers = 1;
        if (params.number_spatial_layers > 3) params.number_spatial_layers = 3;
        if (params.number_temporal_layers < 1) params.number_temporal_layers = 1;
        if (params.number_temporal_layers > 3) params.number_temporal_layers = 3;
        
        if (aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &params) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    // Try AOME_SET_SPATIAL_LAYER_ID
    int layer_id = data[1];
    if (aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, &layer_id) != AOM_CODEC_OK) {
        // Ignore error
    }

    // Try AOME_SET_NUMBER_SPATIAL_LAYERS
    int num_layers = data[1] % 4; // 0 to 3
    if (aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, &num_layers) != AOM_CODEC_OK) {
        // Ignore error
    }

    // Try AV1E_SET_SVC_FRAME_DROP_MODE
    int drop_mode = data[0] % 3; // Assume 0-2 are valid modes
    if (aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &drop_mode) != AOM_CODEC_OK) {
        // Ignore error
    }

    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    // Try AV1D_SET_OPERATING_POINT
    int op = data[0];
    if (aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, &op) != AOM_CODEC_OK) {
        // Ignore error
    }

    return 1;
}

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_enc_cfg_t enc_cfg;
    int is_encoder = Data[0] % 2;

    memset(&ctx, 0, sizeof(ctx));
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    memset(&enc_cfg, 0, sizeof(enc_cfg));

    if (is_encoder) {
        // Initialize Encoder
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ret = aom_codec_enc_config_default(&enc_cfg, iface, 0);
        if (ret != AOM_CODEC_OK) return 0;

        ret = aom_codec_enc_init(&ctx, iface, &enc_cfg, 0);
        if (ret != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        fuzz_encoder_control(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    } else {
        // Initialize Decoder
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        // Initialize decoder config manually with valid members
        dec_cfg.threads = 1;
        dec_cfg.allow_lowbitdepth = 1;
        // Removed invalid decoder_mode field and constant

        ret = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (ret != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        fuzz_decoder_control(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    }

    return 0;
}