// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_GF_CBR_BOOST_PCT at aomcx.h:1933:1 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_QM at aomcx.h:2032:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_QM_MAX at aomcx.h:2042:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY at aomcx.h:1969:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE at aomcx.h:2186:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
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

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    
    if (size == 0) return NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    int val;
    unsigned int uval;
    
    /* Extract values from input data */
    if (Size >= 4) {
        val = *(const int32_t *)(Data);
        uval = *(const uint32_t *)(Data);
    } else {
        val = (int)Data[0];
        uval = (unsigned int)Data[0];
    }

    /* 
     * The functions require 3 arguments: (ctx, ctrl_id, value).
     * The ctrl_id is the macro constant (e.g., AV1E_SET_ENABLE_QM).
     */

    /* Function 1: AV1E_SET_ENABLE_QM */
    aom_codec_control_typechecked_AV1E_SET_ENABLE_QM(ctx, AV1E_SET_ENABLE_QM, uval);

    /* Function 2: AV1E_SET_QM_MAX */
    aom_codec_control_typechecked_AV1E_SET_QM_MAX(ctx, AV1E_SET_QM_MAX, uval);

    /* Function 3: AV1E_SET_NOISE_SENSITIVITY */
    aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY(ctx, AV1E_SET_NOISE_SENSITIVITY, uval);

    /* Function 4: AV1E_SET_DENOISE_BLOCK_SIZE */
    aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, uval);

    /* Function 5: AV1E_SET_ENABLE_INTRABC (takes int) */
    aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(ctx, AV1E_SET_ENABLE_INTRABC, val);

    /* Function 6: AV1E_SET_GF_CBR_BOOST_PCT */
    aom_codec_control_typechecked_AV1E_SET_GF_CBR_BOOST_PCT(ctx, AV1E_SET_GF_CBR_BOOST_PCT, uval);

    /* Cleanup */
    cleanup_encoder(ctx);

    return 0;
}