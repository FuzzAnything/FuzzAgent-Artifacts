// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!*ctx) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Safe access to input data
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16;
    } else {
        cfg.g_w = 128;
    }

    if (size > 1) {
        cfg.g_h = (data[1] % 512) + 16;
    } else {
        cfg.g_h = 128;
    }

    if (size > 2) {
        cfg.g_timebase.num = (data[2] % 30) + 1;
    } else {
        cfg.g_timebase.num = 1;
    }

    if (size > 3) {
        cfg.g_timebase.den = (data[3] % 30) + 1;
    } else {
        cfg.g_timebase.den = 30;
    }

    if (size > 4) {
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    } else {
        cfg.rc_target_bitrate = 100;
    }

    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(*ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return 0;
    }

    return 1;
}

static int initialize_decoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    (void)data;
    (void)size;

    *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!*ctx) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(*ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    int is_encoder = (Data[0] % 2);

    if (is_encoder) {
        if (!initialize_encoder(&ctx, Data, Size)) return 0;
    } else {
        if (!initialize_decoder(&ctx, Data, Size)) return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1_SET_REFERENCE
    if (is_encoder) {
        av1_ref_frame_t ref_frame;
        memset(&ref_frame, 0, sizeof(ref_frame));
        if (Size > 1) ref_frame.idx = Data[1] % 8;
        if (Size > 2) ref_frame.use_external_ref = Data[2] % 2;
        aom_codec_control(ctx, AV1_SET_REFERENCE, &ref_frame);
    }

    // Fuzz aom_codec_control_typechecked_AOME_SET_ENABLEAUTOALTREF
    if (is_encoder && Size > 3) {
        int enable_auto_alt_ref = Data[3] % 2;
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, &enable_auto_alt_ref);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES
    if (is_encoder && Size > 4) {
        int max_ref_frames = Data[4] % 8;
        aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_ref_frames);
    }

    // Fuzz aom_codec_control_typechecked_AOME_SET_ENABLEAUTOBWDREF
    if (is_encoder && Size > 5) {
        int enable_auto_bwd_ref = Data[5] % 2;
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, &enable_auto_bwd_ref);
    }

    // Fuzz aom_codec_control_typechecked_AV1_GET_REFERENCE
    if (is_encoder && Size > 6) {
        av1_ref_frame_t get_ref;
        memset(&get_ref, 0, sizeof(get_ref));
        get_ref.idx = Data[6] % 8;
        aom_codec_control(ctx, AV1_GET_REFERENCE, &get_ref);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_QM_MAX
    if (is_encoder && Size > 7) {
        int qm_max = Data[7] % 256;
        aom_codec_control(ctx, AV1E_SET_QM_MAX, &qm_max);
    }

    cleanup_codec(ctx);
    return 0;
}