// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOME_SET_SHARPNESS at aomcx.h:1896:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH at aomcx.h:2285:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION at aomcx.h:2126:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS at aomcx.h:2367:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE at aomcx.h:2165:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ at aomcx.h:2222:1 in aomcx.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

/*
 * Helper function to initialize the encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                                const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    unsigned int width, height;

    if (size < 4) {
        return -1;
    }

    /* Extract dimensions from fuzz input, ensure they are valid */
    width = data[0] | (data[1] << 8);
    height = data[2] | (data[3] << 8);

    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to clean up the encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int result;

    /* Initialize context structure to zero */
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    /* Initialize encoder context */
    if (init_encoder_context(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    /*
     * Fuzzing the control functions.
     * Based on the error log, the signature is:
     * aom_codec_control_typechecked_<ID>(ctx, ID, value)
     * We must pass the control ID as the second argument.
     */

    /*
     * 1. aom_codec_control_typechecked_AOME_SET_SHARPNESS
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AOME_SET_SHARPNESS(aom_codec_ctx_t *ctx, int ctrl, unsigned int val)
     */
    if (Size >= 1) {
        unsigned int sharpness = Data[0];
        err = aom_codec_control_typechecked_AOME_SET_SHARPNESS(&ctx, AOME_SET_SHARPNESS, sharpness);
        if (err != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
    }

    /*
     * 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH(aom_codec_ctx_t *ctx, int ctrl, int val)
     */
    if (Size >= 2) {
        int enable_tx = Data[1];
        err = aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx);
        if (err != AOM_CODEC_OK) {
            // Ignore
        }
    }

    /*
     * 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION(aom_codec_ctx_t *ctx, int ctrl, int val)
     */
    if (Size >= 3) {
        int enable_gm = Data[2];
        err = aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_gm);
        if (err != AOM_CODEC_OK) {
            // Ignore
        }
    }

    /*
     * 4. aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS(aom_codec_ctx_t *ctx, int ctrl, unsigned int val)
     */
    if (Size >= 4) {
        unsigned int enable_adaptive = Data[3];
        err = aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, enable_adaptive);
        if (err != AOM_CODEC_OK) {
            // Ignore
        }
    }

    /*
     * 5. aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE(aom_codec_ctx_t *ctx, int ctrl, unsigned int val)
     */
    if (Size >= 5) {
        unsigned int delf_mode = Data[4];
        err = aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE(&ctx, AV1E_SET_DELTALF_MODE, delf_mode);
        if (err != AOM_CODEC_OK) {
            // Ignore
        }
    }

    /*
     * 6. aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ
     * Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ(aom_codec_ctx_t *ctx, int ctrl, int val)
     */
    if (Size >= 6) {
        int cost_freq = Data[5];
        err = aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, cost_freq);
        if (err != AOM_CODEC_OK) {
            // Ignore
        }
    }

    /*
     * Additional exploration: Try to encode a frame to ensure state is valid
     */
    if (Size >= 10) {
        aom_image_t img;
        aom_codec_cx_pkt_t pkt;
        aom_codec_iter_t iter = NULL;
        aom_enc_frame_flags_t enc_flags = 0;

        memset(&img, 0, sizeof(img));
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        
        // Fill image with fuzz data using correct member name 'planes'
        // planes[0] is Y, planes[1] is U, planes[2] is V
        // strides are img.stride[0], img.stride[1], img.stride[2]
        if (img.planes[0] && img.planes[1] && img.planes[2]) {
            size_t data_offset = 10;
            size_t data_len = Size - 10;
            if (data_len > 0) {
                for (int i = 0; i < img.h; i++) {
                    for (int j = 0; j < img.w; j++) {
                        size_t idx = (i * img.w + j) % data_len;
                        img.planes[0][i * img.stride[0] + j] = Data[data_offset + idx];
                        img.planes[1][i * img.stride[1] / 2 + j / 2] = Data[data_offset + (idx + 1) % data_len];
                        img.planes[2][i * img.stride[2] / 2 + j / 2] = Data[data_offset + (idx + 2) % data_len];
                    }
                }
            }
        }

        if (Size > 15) enc_flags = Data[15];

        // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
        err = aom_codec_encode(&ctx, &img, 0, 1, enc_flags);
        if (err == AOM_CODEC_OK) {
            // Fixed: aom_codec_get_cx_data takes 2 arguments (ctx, iter)
            while (aom_codec_get_cx_data(&ctx, &iter)) {
                // Consume packets
            }
        }

        aom_img_free(&img);
    }

    /* Cleanup */
    cleanup_encoder_context(&ctx);

    return 0;
}