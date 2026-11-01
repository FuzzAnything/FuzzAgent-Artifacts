// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom.h"

/*
 * Helper function to initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (ctx == NULL) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    /* Use fuzzing data to vary configuration slightly */
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 30) + 1;
        cfg.rc_target_bitrate = (data[3] % 1000) + 100;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

/*
 * Helper function to initialize a decoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    int ret;

    if (ctx == NULL) return -1;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    /* Initialize decoder config with defaults manually */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    int ret;

    if (Size == 0) return 0;

    /* Try to initialize encoder */
    if (init_encoder(&enc_ctx, Data, Size) == 0) {
        /* Fuzz AV1E_SET_DELTALF_MODE */
        int deltal_mode = (Data[0] % 3);
        aom_codec_control(&enc_ctx, AV1E_SET_DELTALF_MODE, deltal_mode);

        /* Fuzz AV1E_SET_LOOPFILTER_CONTROL */
        int lf_level = (Data[1] % 64);
        aom_codec_control(&enc_ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_level);

        /* Fuzz AV1E_SET_ENABLE_INTRABC */
        int enable_intrabc = (Data[2] % 2);
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

        /* Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION */
        int chroma_pos = (Data[3] % 3);
        aom_codec_control(&enc_ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

        /* Fuzz AOME_GET_LOOPFILTER_LEVEL (if available as a control) */
        int get_lf;
        aom_codec_control(&enc_ctx, AOME_GET_LOOPFILTER_LEVEL, &get_lf);

        /* Cleanup encoder */
        aom_codec_destroy(&enc_ctx);
    }

    /* Try to initialize decoder */
    if (init_decoder(&dec_ctx) == 0) {
        /* Fuzz AV1_SET_SKIP_LOOP_FILTER */
        int skip_lf = (Size > 4 ? Data[4] % 2 : 0);
        aom_codec_control(&dec_ctx, AV1_SET_SKIP_LOOP_FILTER, skip_lf);

        /* Fuzz AOME_GET_LOOPFILTER_LEVEL on decoder if applicable */
        int get_lf_dec;
        aom_codec_control(&dec_ctx, AOME_GET_LOOPFILTER_LEVEL, &get_lf_dec);

        /* Cleanup decoder */
        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}