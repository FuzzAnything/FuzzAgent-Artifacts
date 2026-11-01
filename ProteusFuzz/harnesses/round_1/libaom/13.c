// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int create_encoder_context(aom_codec_ctx_t **ctx) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    *ctx = NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!*ctx) return -1;
    memset(*ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    // Set minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(*ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    int ret;
    int val;
    int op;

    // Need at least 1 byte to determine operation
    if (Size < 1) return 0;

    // Initialize encoder context
    if (create_encoder_context(&ctx) != 0) return 0;

    // Use data to determine which control to test and the value
    op = Data[0] % 6;

    if (op == 5) {
        // Special handling for AV1E_SET_RENDER_SIZE which needs two integers
        if (Size >= 3) {
            int w = (Data[1] % 1024);
            int h = (Data[2] % 1024);
            if (w == 0) w = 1;
            if (h == 0) h = 1;
            int dims[2] = {w, h};
            ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, dims);
        } else {
            // Fallback if not enough data for render size
            int dims[2] = {64, 64};
            ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, dims);
        }
    } else {
        // Boolean controls
        val = (Size > 1) ? (Data[1] % 2) : 0;
        
        if (op == 0) {
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
        } else if (op == 1) {
            ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, val);
        } else if (op == 2) {
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
        } else if (op == 3) {
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
        } else if (op == 4) {
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
        }
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}