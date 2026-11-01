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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    // Attempt to set the control with the provided value
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    // Extract parameters from input data safely
    int width = (Data[0] % 256) + 16;
    int height = (Data[1] % 256) + 16;
    int bit_depth = (Data[2] % 8) + 8; // 8 to 15
    int fps_num = (Data[3] % 256) + 1;
    int fps_den = 1;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Extract control ID and value from input data safely
    int control_id = 0;
    int value = 0;

    if (Size >= 8) {
        // Read 4 bytes for control_id
        uint32_t control_id_val = 0;
        memcpy(&control_id_val, Data + 4, sizeof(uint32_t));
        control_id = (int)control_id_val;

        // Read 4 bytes for value
        uint32_t value_val = 0;
        memcpy(&value_val, Data + 8, sizeof(uint32_t));
        value = (int)value_val;
    }

    // Map control_id to specific AV1 encoder controls
    switch (control_id % 6) {
        case 0:
            fuzz_encoder_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, value);
            break;
        case 1:
            fuzz_encoder_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, value);
            break;
        case 2:
            fuzz_encoder_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, value);
            break;
        case 3:
            fuzz_encoder_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, value);
            break;
        case 4:
            fuzz_encoder_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, value);
            break;
        case 5:
            fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, value);
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}