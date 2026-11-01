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

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize codec context
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Initialize the codec
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to determine control parameters
    uint32_t param1 = Data[0];
    uint32_t param2 = Data[1];
    int enable_flag = (Size > 2) ? (Data[2] % 2) : 0;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    err = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, param1);
    (void)err; // Ignore error for fuzzing

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    err = aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, param2);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_flag);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_flag);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, enable_flag);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_flag);
    (void)err;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}