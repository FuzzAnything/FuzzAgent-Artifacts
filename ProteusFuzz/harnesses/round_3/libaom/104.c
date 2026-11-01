// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target APIs:
 * - AV1E_SET_TIER_MASK
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_SET_FRAME_PERIODIC_BOOST
 * - AOME_SET_ENABLEAUTOBWDREF
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_MIN_CR
 */

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static int invoke_control(aom_codec_ctx_t *ctx, int ctrl_id, void *arg) {
    aom_codec_err_t res;
    res = aom_codec_control(ctx, ctrl_id, arg);
    /* We do not return error to allow fuzzing to continue even on invalid calls */
    return 0;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    int tier_mask;
    int coeff_cost_upd_freq;
    int frame_periodic_boost;
    int enable_autobwdref;
    int rtc_external_rc;
    int min_cr;

    if (Size < 6) {
        return 0;
    }

    /* Initialize configuration with defaults */
    if (initialize_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Extract parameters from input data */
    tier_mask = (int)Data[0];
    coeff_cost_upd_freq = (int)Data[1];
    frame_periodic_boost = (int)Data[2];
    enable_autobwdref = (int)Data[3];
    rtc_external_rc = (int)Data[4];
    min_cr = (int)Data[5];

    /* Invoke control functions with extracted values */
    invoke_control(&ctx, AV1E_SET_TIER_MASK, &tier_mask);
    invoke_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, &coeff_cost_upd_freq);
    invoke_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, &frame_periodic_boost);
    invoke_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, &enable_autobwdref);
    invoke_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, &rtc_external_rc);
    invoke_control(&ctx, AV1E_SET_MIN_CR, &min_cr);

    /* Attempt to encode a dummy frame to exercise more states */
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t enc_res;

    /* Initialize image structure manually to avoid member errors */
    memset(&img, 0, sizeof(img));
    
    img.w = 16;
    img.h = 16;
    img.d_w = 16;
    img.d_h = 16;
    img.stride[0] = 16;
    img.stride[1] = 16;
    img.stride[2] = 16;
    img.stride[3] = 16;
    
    /* Allocate planes */
    img.planes[0] = (uint8_t *)calloc(16 * 16, 1);
    img.planes[1] = (uint8_t *)calloc(16 * 16, 1);
    img.planes[2] = (uint8_t *)calloc(16 * 16, 1);
    img.planes[3] = (uint8_t *)calloc(16 * 16, 1);

    if (img.planes[0] && img.planes[1] && img.planes[2] && img.planes[3]) {
        /* Set format using the standard enum if available, or default to 0 (I420) */
        img.planes[0][0] = 1; /* Dummy data */
        
        aom_codec_frame_flags_t flags = 0;
        enc_res = aom_codec_encode(&ctx, &img, 0, 1, flags);
        if (enc_res == AOM_CODEC_OK) {
            while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
                if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                    /* Optionally process pkt.data.frame */
                }
                /* No explicit destroy for cx_pkt in standard API, data is managed by codec */
            }
        }
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        free(img.planes[3]);
    }

    cleanup_encoder(&ctx);
    return 0;
}