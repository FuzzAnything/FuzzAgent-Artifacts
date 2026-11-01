// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int setup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_global_headers(aom_fixed_buf_t *hdr) {
    if (hdr) {
        if (hdr->buf) {
            free(hdr->buf);
        }
        free(hdr);
    }
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_fixed_buf_t *global_hdr = NULL;
    int initialized = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!setup_encoder(&ctx, &cfg)) {
        return 0;
    }
    initialized = 1;

    global_hdr = aom_codec_get_global_headers(&ctx);

    if (global_hdr && global_hdr->buf) {
        volatile uint8_t dummy = 0;
        if (global_hdr->sz > 0) {
            dummy = *(uint8_t *)global_hdr->buf;
        }
        (void)dummy;
    }

    cleanup_global_headers(global_hdr);
    cleanup_encoder(&ctx);

    return 0;
}