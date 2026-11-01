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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int setup_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic defaults to ensure valid initialization
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int val;

    if (Size < 1) return 0;

    // Initialize context and config
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!setup_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    // Extract values from fuzz input to drive the API calls
    uint8_t *p = (uint8_t *)Data;
    int arg_val = (int)(*p & 0xFF);
    int arg_val2 = (Size > 1) ? (int)(p[1] & 0xFF) : 0;
    int arg_val3 = (Size > 2) ? (int)(p[2] & 0xFF) : 0;

    // 1. AV1E_ENABLE_EXT_TILE_DEBUG
    err = aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, arg_val);
    (void)err;

    // 2. AV1E_SET_TILE_ROWS
    err = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, arg_val2);
    (void)err;

    // 3. AV1E_SET_SINGLE_TILE_DECODING
    err = aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, arg_val3);
    (void)err;

    // 4. AV1E_SET_TILE_COLUMNS
    err = aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, arg_val2);
    (void)err;

    // 5. AV1_SET_TILE_MODE
    err = aom_codec_control(&ctx, AV1_SET_TILE_MODE, arg_val);
    (void)err;

    // 6. AV1E_SET_ENABLE_RECT_PARTITIONS
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, arg_val3);
    (void)err;

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}