// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required Header Files */
#include "vp8dx.h"
#include "vp8cx.h"
#include "vpx_codec.h"
#include "vpx_decoder.h"
#include "vpx_encoder.h"

static vpx_codec_ctx_t *create_decoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_iface_t *decoder = vpx_codec_vp9_dx();
    if (!decoder) {
        free(ctx);
        return NULL;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    vpx_codec_err_t err = vpx_codec_dec_init(ctx, decoder, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
    if (!encoder) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    vpx_codec_enc_config_default(encoder, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    vpx_codec_err_t err = vpx_codec_enc_init(ctx, encoder, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the first byte to decide between decoder and encoder
    int mode = Data[0] % 2;
    vpx_codec_ctx_t *ctx = NULL;

    if (mode == 0) {
        ctx = create_decoder_context(Data, Size);
    } else {
        ctx = create_encoder_context(Data, Size);
    }

    if (!ctx) return 0;

    // Fuzz vpx_codec_control_ with various control IDs derived from input
    int ctrl_id = 0;
    if (Size >= 3) {
        ctrl_id = (int)(Data[1] | (Data[2] << 8));
    }

    // Attempt to call vpx_codec_control_ with the derived ID
    // Passing a pointer to the data as the first variadic argument
    vpx_codec_err_t ret = vpx_codec_control_(ctx, ctrl_id, (void *)Data);
    
    // Try with a simple integer argument derived from data
    if (Size >= 7) {
        int int_arg = (int)(Data[3] | (Data[4] << 8) | (Data[5] << 16) | (Data[6] << 24));
        vpx_codec_control_(ctx, ctrl_id, int_arg);
    }

    // Cleanup
    cleanup_context(ctx);

    return 0;
}