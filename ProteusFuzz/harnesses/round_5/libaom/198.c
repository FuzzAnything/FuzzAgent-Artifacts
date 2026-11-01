// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some configuration parameters
    if (Size >= 4) {
        cfg.g_w = *(uint16_t *)(Data);
        cfg.g_h = *(uint16_t *)(Data + 2);
        if (cfg.g_w == 0 || cfg.g_h == 0) {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
        cfg.g_w = (cfg.g_w > 0 && cfg.g_w < 4096) ? cfg.g_w : 64;
        cfg.g_h = (cfg.g_h > 0 && cfg.g_h < 4096) ? cfg.g_h : 64;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    int64_t rtc_external_rc_val = 0;
    if (Size >= 8) {
        rtc_external_rc_val = *(int64_t *)(Data);
    }
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc_val);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    int max_ref_frames_val = 0;
    if (Size >= 12) {
        max_ref_frames_val = *(int *)(Data + 8);
    }
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames_val);

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    int color_primaries_val = 0;
    if (Size >= 16) {
        color_primaries_val = *(int *)(Data + 12);
    }
    aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries_val);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    int enable_ref_frame_mvs_val = 0;
    if (Size >= 20) {
        enable_ref_frame_mvs_val = *(int *)(Data + 16);
    }
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs_val);

    // Fuzz AV1E_SET_S_FRAME_MODE
    int s_frame_mode_val = 0;
    if (Size >= 24) {
        s_frame_mode_val = *(int *)(Data + 20);
    }
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode_val);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    int enable_interintra_wedge_val = 0;
    if (Size >= 28) {
        enable_interintra_wedge_val = *(int *)(Data + 24);
    }
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge_val);

    cleanup_codec(&ctx);
    return 0;
}