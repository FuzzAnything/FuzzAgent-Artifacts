// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use input data to configure width/height if possible, otherwise defaults
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && w < 16384 && h > 0 && h < 16384) {
            cfg.g_w = w;
            cfg.g_h = h;
        }
    }
    cfg.g_w = cfg.g_w ? cfg.g_w : 64;
    cfg.g_h = cfg.g_h ? cfg.g_h : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    int result = 0;
    int8_t val8;
    int32_t val32;

    // Extract values from fuzz input for parameters
    if (Size >= 1) val8 = Data[0];
    if (Size >= 5) val32 = *(const int32_t *)(Data);

    // 1. aom_codec_control_typechecked_AV1E_SET_EXTERNAL_PARTITION
    // Requires aom_ext_part_funcs_t. We construct a dummy one.
    // The correct macro is AV1E_SET_EXTERNAL_PARTITION based on the error log.
    if (Size > 10) {
        aom_ext_part_funcs_t ext_funcs;
        memset(&ext_funcs, 0, sizeof(ext_funcs));
        // Attempt to set the external partition model.
        // Note: Passing NULL function pointers might trigger assertions or crashes,
        // which is valid for fuzzing to find robustness issues.
        aom_codec_control(ctx, AV1E_SET_EXTERNAL_PARTITION, &ext_funcs);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES
    // Parameter is usually an int (number of frames).
    int max_ref = (int)val8;
    if (max_ref < 0) max_ref = 0;
    if (max_ref > 8) max_ref = 8; // AV1 max is typically 8
    aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_ref);

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS
    // Parameter is an int (0 or 1).
    int enable_mvs = (int)(val8 & 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, &enable_mvs);

    // 4. aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE
    // Parameter is an int (mode).
    int s_frame_mode = (int)val8;
    aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, &s_frame_mode);

    // 5. aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
    // Parameter is an int (enable/disable).
    int rtc_ext_rc = (int)(val8 & 1);
    aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, &rtc_ext_rc);

    // 6. aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX
    // Parameter is an int (level index).
    int level_idx = (int)val8;
    if (level_idx < 0) level_idx = 0;
    if (level_idx > 10) level_idx = 10; // Reasonable bound
    aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, &level_idx);

    // Cleanup
    cleanup_encoder_context(ctx);
    return 0;
}