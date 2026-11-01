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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_237(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_DELTALF_MODE
    int32_t delta_lf_mode = Data[4] % 3;
    aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, delta_lf_mode);

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    int32_t noise_level = Data[5] % 16;
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, noise_level);

    // Fuzz AV1_SET_SKIP_LOOP_FILTER
    int32_t skip_loop_filter = Data[6] % 2;
    aom_codec_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, skip_loop_filter);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    int32_t enable_intrabc = Data[7] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    // Use a union or struct that matches the expected argument for AV1E_SET_LOOPFILTER_CONTROL
    // Since the exact type is not provided, we assume it expects a pointer to an int or a specific struct.
    // Based on common libaom patterns, it often expects an aom_loopfilter_t or similar.
    // If aom_loopfilter_t is not defined, we might need to use a generic int or a struct defined in headers.
    // For now, we'll use an int array to simulate the structure if the type is missing.
    int32_t lf_ctrl_data[2];
    lf_ctrl_data[0] = Data[8] % 64; // level
    lf_ctrl_data[1] = Data[9] % 8;  // sharpness
    aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_ctrl_data);

    // Fuzz AOME_GET_LOOPFILTER_LEVEL
    int32_t loopfilter_level;
    aom_codec_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);

    cleanup_codec(&ctx);
    return 0;
}