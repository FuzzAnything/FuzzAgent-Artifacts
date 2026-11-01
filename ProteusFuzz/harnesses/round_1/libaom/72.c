// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>

static aom_codec_ctx_t codec_ctx;
static aom_codec_enc_cfg_t cfg;
static int codec_initialized = 0;

static void init_codec(const uint8_t *data, size_t size) {
    if (codec_initialized) {
        return;
    }

    // Use aom_codec_av1_cx to get the AV1 codec interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }

    // Use fuzzing data to set some config parameters safely
    // Ensure we have enough data before accessing indices
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
    }
    if (size > 1) {
        cfg.g_h = (data[1] % 256) + 16;
    }
    if (size > 2) {
        cfg.g_timebase.num = (data[2] % 100) + 1;
    }
    if (size > 3) {
        cfg.g_timebase.den = (data[3] % 100) + 1;
    }
    if (size > 4) {
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    }

    if (aom_codec_enc_init(&codec_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }

    codec_initialized = 1;
}

static void cleanup_codec(void) {
    if (codec_initialized) {
        aom_codec_destroy(&codec_ctx);
        codec_initialized = 0;
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize codec if not already done
    init_codec(Data, Size);

    if (!codec_initialized) {
        return 0;
    }

    // Fuzz AV1E_SET_EXTERNAL_RATE_CONTROL
    if (Size > 1) {
        aom_rc_funcs_t rc_funcs = {0};
        rc_funcs.rc_type = (aom_rc_type_t)(Data[1] % 7);
        aom_codec_control(&codec_ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
    }

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    if (Size > 2) {
        int color_primaries = Data[2] % 10;
        aom_codec_control(&codec_ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);
    }

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    if (Size > 3) {
        int allow_warped_motion = Data[3] % 2;
        aom_codec_control(&codec_ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped_motion);
    }

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    if (Size > 4) {
        int enable_ab_partitions = Data[4] % 2;
        aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);
    }

    // Fuzz AV1E_SET_EXTERNAL_PARTITION
    if (Size > 5) {
        aom_ext_part_funcs_t ext_part_funcs = {0};
        ext_part_funcs.decision_mode = (aom_ext_part_decision_mode_t)(Data[5] % 2);
        aom_codec_control(&codec_ctx, AV1E_SET_EXTERNAL_PARTITION, &ext_part_funcs);
    }

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    if (Size > 6) {
        int enable_ref_frame_mvs = Data[6] % 2;
        aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs);
    }

    // Cleanup
    cleanup_codec();

    return 0;
}