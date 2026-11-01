// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

/*
 * Helper function to initialize a decoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx_ptr = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ctx_ptr = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx_ptr) return NULL;

    memset(ctx_ptr, 0, sizeof(aom_codec_ctx_t));
    ret = aom_codec_dec_init(ctx_ptr, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx_ptr);
        return NULL;
    }

    /* Feed some data to the decoder to ensure it has processed at least one frame */
    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        
        ret = aom_codec_decode(ctx_ptr, data, size, NULL);
        if (ret == AOM_CODEC_OK) {
            while ((img = aom_codec_get_frame(ctx_ptr, &iter)) != NULL) {
                /* Frame is automatically unreferenced on next call or destroy */
            }
        }
    }

    return ctx_ptr;
}

/*
 * Helper function to initialize an encoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    /* Use fuzz data to set some config parameters if possible */
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        if (size > 1) {
            cfg.g_h = ((size_t)data[1] % 1024) + 16;
        } else {
            cfg.g_h = 128;
        }
        if (size > 2) {
            cfg.rc_target_bitrate = (data[2] * 1000) + 1000;
        } else {
            cfg.rc_target_bitrate = 1000;
        }
        cfg.g_timebase.num = 1;
        if (size > 3) {
            cfg.g_timebase.den = (data[3] % 60) + 1;
        } else {
            cfg.g_timebase.den = 30;
        }
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.rc_target_bitrate = 1000;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use first byte to decide between decoder and encoder paths */
    int is_decoder = (Data[0] % 2) == 0;
    
    aom_codec_ctx_t *ctx = NULL;
    
    if (is_decoder) {
        ctx = init_decoder(Data, Size);
    } else {
        ctx = init_encoder(Data, Size);
    }

    if (!ctx) return 0;

    /* 
     * Attempt to invoke the target control functions.
     * We assume the standard aom_codec_control API.
     */

    /* Decoder specific controls */
    if (is_decoder) {
        aom_codec_err_t ret;
        aom_s_frame_info s_frame_info;
        int show_frame_flag;
        int frame_flags;
        int last_ref_used;

        /* 2. AOMD_GET_S_FRAME_INFO */
        ret = aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
        (void)ret;

        /* 3. AOMD_GET_SHOW_FRAME_FLAG */
        ret = aom_codec_control(ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);
        (void)ret;

        /* 4. AOMD_GET_FRAME_FLAGS */
        ret = aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
        (void)ret;

        /* 5. AOMD_GET_LAST_REF_USED */
        ret = aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
        (void)ret;

        /* 1. AOMD_GET_FRAME_HEADER_INFO - Skipping as structure is not defined in provided context */
        // ret = aom_codec_control(ctx, AOMD_GET_FRAME_HEADER_INFO, &header_info);
    }

    /* Encoder specific controls */
    if (!is_decoder) {
        aom_codec_err_t ret;
        int enable_diff_wtd_comp = (Data[0] % 2); /* 0 or 1 */

        /* 6. AV1E_SET_ENABLE_DIFF_WTD_COMP */
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd_comp);
        (void)ret;
    }

    /* Cleanup */
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}