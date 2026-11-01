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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzing data to set some configuration parameters safely
    if (Size > 0) {
        cfg.g_w = (Data[0] % 512) + 16; // Width between 16 and 527
        cfg.g_h = (Data[1 % Size] % 512) + 16; // Height between 16 and 527
    }
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions with data from the input buffer
    int32_t value;
    aom_codec_err_t res;

    // Helper macro to safely extract int32_t from data
    #define GET_INT32(idx) ((idx < Size) ? (int32_t)Data[idx] : 0)

    // 1. AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    value = GET_INT32(0);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
    (void)res; // Ignore result for fuzzing, just trigger execution

    // 2. AV1E_SET_NOISE_SENSITIVITY
    value = GET_INT32(1);
    res = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, value);
    (void)res;

    // 3. AV1E_SET_TILE_ROWS
    value = GET_INT32(2);
    res = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, value);
    (void)res;

    // 4. AV1E_SET_MAX_REFERENCE_FRAMES
    value = GET_INT32(3);
    res = aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, value);
    (void)res;

    // 5. AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    value = GET_INT32(4);
    res = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, value);
    (void)res;

    // 6. AV1E_SET_ENABLE_FLIP_IDTX
    value = GET_INT32(5);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    (void)res;

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}