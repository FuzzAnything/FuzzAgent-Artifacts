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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int val_int;
    int val_int2;
    int val_int3;
    int val_int4;
    int val_int5;
    int val_int6;

    // Initialize codec context structure explicitly to avoid garbage
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get encoder interface first to ensure it exists
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to set configuration parameters safely
    cfg.g_w = (Size > 0) ? (Data[0] % 128) + 16 : 16;
    cfg.g_h = (Size > 1) ? (Data[1] % 128) + 16 : 16;
    // Fixed: g_time_base -> g_timebase
    cfg.g_timebase.num = (Size > 2) ? (Data[2] % 255) + 1 : 1;
    cfg.g_timebase.den = (Size > 3) ? (Data[3] % 255) + 1 : 1;
    cfg.g_threads = (Size > 4) ? (Data[4] % 8) + 1 : 1;
    cfg.g_error_resilient = (Size > 5) ? (Data[5] % 2) : 0;
    cfg.rc_target_bitrate = (Size > 6) ? ((Data[6] % 100) * 1000) + 1000 : 1000;

    // Initialize codec context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzzer data for control commands
    if (Size > 0) val_int = Data[0];
    if (Size > 1) val_int2 = Data[1];
    if (Size > 2) val_int3 = Data[2];
    if (Size > 3) val_int4 = Data[3];
    if (Size > 4) val_int5 = Data[4];
    if (Size > 5) val_int6 = Data[5];

    // Fuzz aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val_int);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val_int2);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val_int3 % 2);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val_int4);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_QM_MAX
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, val_int5);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_TILE_ROWS
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val_int6);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}