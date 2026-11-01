// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

/* 
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_OBMC
 * - AV1E_SET_QM_U
 * - AV1E_SET_AQ_MODE
 * - AV1E_SET_SKIP_POSTPROC_FILTERING
 * - AV1E_SET_ROW_MT
 * - AV1E_SET_CDF_UPDATE_MODE
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, 
                        const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    /* Fuzz width/height based on input to explore different states, 
       but keep them reasonable to avoid immediate OOM or invalid state. */
    if (size >= 2) {
        cfg->g_w = (data[0] % 255) + 16;
        cfg->g_h = (data[1] % 255) + 16;
        /* Ensure alignment if necessary, though libaom usually handles it */
        if (cfg->g_w < 16) cfg->g_w = 16;
        if (cfg->g_h < 16) cfg->g_h = 16;
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }

    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static int create_dummy_frame(aom_image_t *img, const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    unsigned int w = (data[0] % 255) + 16;
    unsigned int h = (data[1] % 255) + 16;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, w, h, 1) != 0) return 0;

    /* Fill with dummy data */
    if (size > 2) {
        size_t data_len = size - 2;
        size_t plane_size = img->d_w * img->d_h;
        if (data_len < plane_size) data_len = plane_size;
        
        memcpy(img->planes[0], data + 2, data_len);
        /* Simple fill for chroma planes to avoid uninitialized memory */
        memset(img->planes[1], 128, plane_size / 2);
        memset(img->planes[2], 128, plane_size / 2);
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_227(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    int ret;
    int obmc_val, qm_u_val, aq_mode_val, skip_pp_val, row_mt_val, cdf_mode_val;

    /* Initialize encoder */
    if (!init_encoder(&codec, &cfg, Data, Size)) return 0;

    /* Create a dummy frame to ensure the encoder is in a valid state 
       before calling control functions. */
    if (!create_dummy_frame(&img, Data, Size)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    /* Derive control values from fuzz input */
    obmc_val = Data[2] % 2;
    qm_u_val = Data[3] % 256;
    aq_mode_val = Data[4] % 4; /* Assuming 0-3 are valid AQ modes */
    skip_pp_val = Data[5] % 2;
    row_mt_val = Data[6] % 2;
    cdf_mode_val = Data[7] % 3; /* Assuming 0-2 are valid CDF update modes */

    /* Invoke target control functions */
    
    /* 1. AV1E_SET_ENABLE_OBMC */
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_OBMC, obmc_val);
    (void)ret; 

    /* 2. AV1E_SET_QM_U */
    ret = aom_codec_control(&codec, AV1E_SET_QM_U, qm_u_val);
    (void)ret;

    /* 3. AV1E_SET_AQ_MODE */
    ret = aom_codec_control(&codec, AV1E_SET_AQ_MODE, aq_mode_val);
    (void)ret;

    /* 4. AV1E_SET_SKIP_POSTPROC_FILTERING */
    ret = aom_codec_control(&codec, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_pp_val);
    (void)ret;

    /* 5. AV1E_SET_ROW_MT */
    ret = aom_codec_control(&codec, AV1E_SET_ROW_MT, row_mt_val);
    (void)ret;

    /* 6. AV1E_SET_CDF_UPDATE_MODE */
    ret = aom_codec_control(&codec, AV1E_SET_CDF_UPDATE_MODE, cdf_mode_val);
    (void)ret;

    /* Cleanup */
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}