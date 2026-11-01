// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test aom_codec_err_to_string with various valid error codes
    aom_codec_err_t test_errs[] = {
        AOM_CODEC_OK,
        AOM_CODEC_ERROR,
        AOM_CODEC_MEM_ERROR,
        AOM_CODEC_CORRUPT_FRAME,
        AOM_CODEC_INVALID_PARAM,
        0x12345678 // Unrecognized error code
    };
    const int num_errs = sizeof(test_errs) / sizeof(test_errs[0]);

    for (int i = 0; i < num_errs; i++) {
        const char *err_str = aom_codec_err_to_string(test_errs[i]);
        if (err_str) {
            (void)strlen(err_str);
        }
    }

    // Test aom_codec_error with NULL context
    const char *null_ctx_err = aom_codec_error(NULL);
    if (null_ctx_err) {
        (void)strlen(null_ctx_err);
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_encoder(&enc_ctx);
        return 0;
    }

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&enc_ctx);
    if (global_headers) {
        if (global_headers->buf) {
            (void)global_headers->sz;
        }
        free(global_headers->buf);
        free(global_headers);
    }

    // Test aom_codec_error on encoder context
    const char *enc_err = aom_codec_error(&enc_ctx);
    if (enc_err) {
        (void)strlen(enc_err);
    }

    // Attempt to call control functions
    int32_t quantizer = 0;
    int32_t base_q_idx = 0;
    
    aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
    aom_codec_control(&enc_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    aom_codec_control(&enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, 100);

    cleanup_encoder(&enc_ctx);

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    // Create a dummy image frame to decode (using aom_img_alloc for proper initialization)
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    // Fill with fuzz data if available
    size_t data_len = Size;
    if (data_len > img->d_w * img->d_h) data_len = img->d_w * img->d_h;
    if (img->planes[0]) {
        memcpy(img->planes[0], Data, data_len);
    }
    if (img->planes[1] && img->d_h > 0) {
        size_t uv_len = (img->d_w * img->d_h) / 4;
        if (data_len > uv_len) data_len = uv_len;
        memcpy(img->planes[1], Data, data_len);
    }
    if (img->planes[2] && img->d_h > 0) {
        size_t uv_len = (img->d_w * img->d_h) / 4;
        if (data_len > uv_len) data_len = uv_len;
        memcpy(img->planes[2], Data, data_len);
    }

    // Attempt to decode the frame using the correct function signature
    // aom_codec_decode takes (ctx, data, data_sz, user_priv)
    // We pass the image as user_priv to trigger internal logic if needed, 
    // though typically we decode into a packet. Here we just test the API call.
    aom_codec_decode(&dec_ctx, NULL, 0, img);

    // Test aom_codec_error on decoder context
    const char *dec_err = aom_codec_error(&dec_ctx);
    if (dec_err) {
        (void)strlen(dec_err);
    }

    // Cleanup decoder
    aom_img_free(img);
    cleanup_decoder(&dec_ctx);

    return 0;
}