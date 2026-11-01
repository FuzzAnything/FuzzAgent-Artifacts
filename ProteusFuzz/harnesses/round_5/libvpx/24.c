// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>

/* Callback function for decoded frames */
static void put_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
    /* Do nothing, just a placeholder callback */
}

/* Helper to initialize encoder config from fuzz data */
static int init_enc_cfg_from_data(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    if (vpx_codec_enc_config_default(iface, cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Use fuzz data to set configuration parameters safely */
    if (size > 0) {
        cfg->g_w = (data[0] % 512) + 16;
    }
    if (size > 1) {
        cfg->g_h = (data[1] % 512) + 16;
    }
    if (size > 2) {
        cfg->g_threads = (data[2] % 8) + 1;
    }
    if (size > 3) {
        cfg->g_profile = data[3] % 4;
    }
    if (size > 4) {
        cfg->g_bit_depth = (data[4] % 3) == 0 ? VPX_BITS_8 : (data[4] % 3 == 1 ? VPX_BITS_10 : VPX_BITS_12);
    }
    if (size > 5) {
        cfg->g_input_bit_depth = cfg->g_bit_depth;
    }
    if (size > 7) {
        cfg->g_timebase.num = (data[6] % 100) + 1;
        cfg->g_timebase.den = (data[7] % 100) + 1;
    }
    if (size > 8) {
        cfg->rc_target_bitrate = (data[8] % 1000) + 100;
    }
    if (size > 9) {
        cfg->rc_end_usage = (data[9] % 4);
    }
    if (size > 10) {
        cfg->kf_mode = (data[10] % 3);
    }
    if (size > 11) {
        cfg->kf_min_dist = (data[11] % 100) + 1;
    }
    if (size > 12) {
        cfg->kf_max_dist = (data[12] % 100) + 1;
    }

    return 1;
}

/* Helper to initialize decoder config from fuzz data */
static int init_dec_cfg_from_data(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    cfg->threads = (size > 0 ? data[0] % 8 : 1) + 1;
    cfg->w = (size > 1 ? data[1] % 512 : 16) + 16;
    cfg->h = (size > 2 ? data[2] % 512 : 16) + 16;
    return 1;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Determine mode: 0 for encoder, 1 for decoder */
    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (mode == 0) {
        /* Encoder Fuzzing */
        vpx_codec_ctx_t ctx;
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

        if (!init_enc_cfg_from_data(&cfg, payload, payload_size)) {
            return 0;
        }

        /* Initialize encoder */
        if (vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
            return 0;
        }

        /* Try to get global headers (VP9 specific) */
        vpx_fixed_buf_t *hdrs = vpx_codec_get_global_headers(&ctx);
        (void)hdrs; /* Suppress unused warning */

        /* Try control command using a valid VP9 control ID */
        vpx_codec_control_(&ctx, VP9E_SET_TPL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        /* Encode dummy data if available */
        if (payload_size > 0) {
            vpx_image_t img;
            vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
            memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
            memset(img.planes[1], 0, img.stride[1] * cfg.g_h / 2);
            memset(img.planes[2], 0, img.stride[2] * cfg.g_h / 2);

            vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
            vpx_img_free(&img);
        }

        vpx_codec_destroy(&ctx);

    } else {
        /* Decoder Fuzzing */
        vpx_codec_ctx_t ctx;
        vpx_codec_dec_cfg_t cfg;
        vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

        if (!init_dec_cfg_from_data(&cfg, payload, payload_size)) {
            return 0;
        }

        /* Initialize decoder */
        if (vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
            return 0;
        }

        /* Register callback */
        vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);

        /* Decode dummy data */
        if (payload_size > 0) {
            vpx_codec_decode(&ctx, payload, payload_size, NULL, 0);
        }

        /* Flush decoder */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

        vpx_codec_destroy(&ctx);
    }

    return 0;
}