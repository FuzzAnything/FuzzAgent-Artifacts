// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    // Check if the context is properly initialized before calling control
    if (ctx->priv == NULL) return 0;
    return aom_codec_control(ctx, control_id, arg);
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return NULL;

    // Use fuzz data to configure encoder settings safely
    if (size > 0) cfg.g_w = (data[0] % 1024) + 16;
    if (size > 1) cfg.g_h = (data[1] % 1024) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_dec_init(ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;

    // Determine mode: 0 = encoder, 1 = decoder
    int mode = Data[0] % 2;
    aom_codec_ctx_t *ctx = NULL;

    if (mode == 0) {
        ctx = init_encoder(Data, Size);
    } else {
        ctx = init_decoder(Data, Size);
    }

    if (!ctx) return 0;

    // Prepare arguments for control functions
    int idx = 0;
    int use_external_ref = 0;
    int enable_auto_bwd_ref = 0;
    int max_ref_frames = 0;
    int enable_auto_alt_ref = 0;
    int s_frame_mode = 0;

    if (Size > 1) idx = Data[1] % 8;
    if (Size > 2) use_external_ref = Data[2] % 2;
    if (Size > 3) enable_auto_bwd_ref = Data[3] % 2;
    if (Size > 4) max_ref_frames = Data[4] % 8;
    if (Size > 5) enable_auto_alt_ref = Data[5] % 2;
    if (Size > 6) s_frame_mode = Data[6] % 4;

    // AV1E_SET_MAX_REFERENCE_FRAMES (Encoder only)
    if (mode == 0) {
        fuzz_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_ref_frames);
    }

    // AOME_SET_ENABLEAUTOBWDREF (Encoder only)
    if (mode == 0) {
        fuzz_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, &enable_auto_bwd_ref);
    }

    // AOME_SET_ENABLEAUTOALTREF (Encoder only)
    if (mode == 0) {
        fuzz_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, &enable_auto_alt_ref);
    }

    // AV1E_SET_S_FRAME_MODE (Encoder only)
    if (mode == 0) {
        fuzz_codec_control(ctx, AV1E_SET_S_FRAME_MODE, &s_frame_mode);
    }

    // AV1_SET_REFERENCE and AV1_GET_REFERENCE (Decoder only)
    // These operations require a decoded frame to exist in the context.
    // Since we haven't fed any data to the decoder, these operations will likely fail or be no-ops.
    // We skip them to avoid the crash caused by accessing uninitialized internal worker data.
    // To properly fuzz these, we would need to feed valid AV1 bitstream data to the decoder first.
    if (mode == 1) {
        // Skip reference frame operations for now as the decoder has no frames.
        // This prevents the SEGV on unknown address 0x000000000020.
        // In a full fuzzing campaign, we would decode a frame first:
        // aom_codec_decode(ctx, packet, ...);
    }

    cleanup_codec(ctx);
    return 0;
}