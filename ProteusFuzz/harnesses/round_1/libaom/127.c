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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    // Use the AV1 encoder interface directly
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    // Use fuzzing data to set configuration parameters safely
    if (size > 0) {
        cfg.g_w = (data[0] % 256) * 16 + 16; // Ensure width is at least 16 and multiple of 16
        cfg.g_h = (data[1] % 256) * 16 + 16; // Ensure height is at least 16 and multiple of 16
        if (size > 2) cfg.g_timebase.num = data[2] + 1;
        if (size > 3) cfg.g_timebase.den = data[3] + 1;
        if (size > 4) cfg.g_threads = (data[4] % 8) + 1;
        if (size > 5) cfg.rc_target_bitrate = (data[5] % 1000) + 100;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx = {0};
    int ret;

    // Initialize encoder context
    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    int enable_dir_intra = (Data[0] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_dir_intra);
    (void)ret; // Ignore return for fuzzing purposes

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    int error_resilient_mode = (Data[1] % 3);
    ret = aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    (void)ret;

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int seq_level_idx;
    ret = aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    int enable_paeth_intra = (Data[2] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth_intra);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    int enable_diag_intra = (Data[3] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diag_intra);
    (void)ret;

    // Fuzz AV1E_SET_RENDER_SIZE
    // The API expects a pointer to an array of two integers (width, height)
    // Passing separate pointers causes the stack overflow observed in the sanitizer log.
    int render_size[2];
    render_size[0] = (Data[4] % 256) * 16 + 16;
    render_size[1] = (Data[5] % 256) * 16 + 16;
    
    ret = aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_size);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}