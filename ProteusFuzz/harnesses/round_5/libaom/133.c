// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    // AOME_SET_SPATIAL_LAYER_ID
    // Expects an int (spatial layer id)
    if (size >= sizeof(int)) {
        int layer_id = *(const int *)data;
        aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, &layer_id);
    }

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    // Expects an int (0 or 1)
    if (size >= sizeof(int)) {
        int enable = *(const int *)data;
        aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, &enable);
    }

    // AV1E_SET_SVC_FRAME_DROP_MODE
    // Expects an int (frame drop mode)
    if (size >= sizeof(int)) {
        int mode = *(const int *)data;
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &mode);
    }

    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    // AV1D_SET_OPERATING_POINT
    // Expects an int (operating point)
    if (size >= sizeof(int)) {
        int op = *(const int *)data;
        aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, &op);
    }

    // AOME_SET_NUMBER_SPATIAL_LAYERS
    // Expects an int (number of layers)
    if (size >= sizeof(int)) {
        int layers = *(const int *)data;
        aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, &layers);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize Encoder Context
    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t cfg_enc;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    
    if (!enc_iface) return 0;

    if (aom_codec_enc_config_default(enc_iface, &cfg_enc, 0) != AOM_CODEC_OK) return 0;
    
    // Set minimal config
    cfg_enc.g_w = 128;
    cfg_enc.g_h = 128;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    cfg_enc.g_threads = 1;
    cfg_enc.g_profile = 0;
    cfg_enc.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg_enc, 0) != AOM_CODEC_OK) {
        // If init fails, we can't fuzz encoder controls effectively
    } else {
        // Fuzz encoder controls
        fuzz_encoder_control(&enc_ctx, Data, Size);
        aom_codec_destroy(&enc_ctx);
    }

    // Initialize Decoder Context
    aom_codec_ctx_t dec_ctx;
    aom_codec_dec_cfg_t cfg_dec;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();

    if (!dec_iface) return 0;

    // Initialize decoder config manually since aom_codec_dec_config_default might be missing or named differently
    memset(&cfg_dec, 0, sizeof(cfg_dec));
    cfg_dec.threads = 1;

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &cfg_dec, 0) != AOM_CODEC_OK) {
        // If init fails, skip decoder fuzzing
    } else {
        // Fuzz decoder controls
        fuzz_decoder_control(&dec_ctx, Data, Size);
        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}