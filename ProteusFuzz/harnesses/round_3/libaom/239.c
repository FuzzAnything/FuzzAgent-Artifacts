// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

// Helper to safely initialize encoder
static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    struct aom_codec_enc_cfg cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    // Use fuzzing data to configure width/height safely
    int w = 16, h = 16;
    if (size >= 2) {
        w = (data[0] % 1920) + 16;
        h = (data[1] % 1080) + 16;
    }
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int disable_trellis = (Size > 0) ? (Data[0] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = (Size > 1) ? (Data[1] % 4) : 0;
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    int gf_min_pyramid = (Size > 2) ? (Data[2] % 4) : 0;
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyramid);

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    int arnr_maxframes = (Size > 3) ? (Data[3] % 16) : 0;
    aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);

    // Fuzz AOME_SET_ARNR_STRENGTH
    int arnr_strength = (Size > 4) ? (Data[4] % 64) : 0;
    aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength = (Size > 5) ? (Data[5] % 64) : 0;
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}