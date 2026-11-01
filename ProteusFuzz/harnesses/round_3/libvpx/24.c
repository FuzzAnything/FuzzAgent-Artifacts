// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_GET_LAST_QUANTIZER_SVC_LAYERS__value at vp8cx.h:1103:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

/* 
 * Note: The provided API function names (e.g., VP9E_SET_ROW_MT__value) are 
 * internal macros or placeholders. This fuzzer uses the standard 
 * vpx_codec_control interface with the corresponding VP9E_* control IDs.
 */

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;
static int encoder_initialized = 0;

static int init_encoder(void) {
    const vpx_codec_iface_t *iface;
    vpx_codec_err_t err;

    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return -1;
    }

    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 500;
    cfg.g_error_resilient = 1;

    err = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    encoder_initialized = 1;
    return 0;
}

static void cleanup_encoder(void) {
    if (encoder_initialized) {
        vpx_codec_destroy(&codec);
        encoder_initialized = 0;
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    vpx_codec_err_t err;

    /* Guard against empty inputs */
    if (Size == 0) {
        return 0;
    }

    /* Initialize encoder if not already done */
    if (!encoder_initialized) {
        if (init_encoder() != 0) {
            return 0;
        }
    }

    /* 
     * Attempt to set various VP9 encoder options using vpx_codec_control.
     * Ensure bounds checking for all array accesses.
     */

    /* 1. VP9E_SET_ROW_MT (Multi-threading row level) */
    if (Size >= 1) {
        int val = Data[0] & 0xFF;
        vpx_codec_control(&codec, VP9E_SET_ROW_MT, val);
    }

    /* 2. VP9E_SET_POSTENCODE_DROP */
    if (Size >= 3) {
        int val = ((int)Data[1] << 8) | (int)Data[2];
        vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, val);
    }

    /* 3. VP9E_SET_FRAME_PERIODIC_BOOST */
    if (Size >= 5) {
        int val = ((int)Data[3] << 8) | (int)Data[4];
        vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, val);
    }

    /* 4. VP9E_SET_SVC_INTER_LAYER_PRED */
    if (Size >= 5) {
        int val = Data[4] & 0xFF;
        vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, val);
    }

    /* 5. VP9E_GET_LAST_QUANTIZER_SVC_LAYERS */
    if (Size >= 15) {
        int temp_quant[10];
        for (int i = 0; i < 10; i++) {
            temp_quant[i] = Data[5 + i];
        }
        vpx_codec_control(&codec, VP9E_GET_LAST_QUANTIZER_SVC_LAYERS, temp_quant);
    }

    /* 6. VP9E_SET_QUANTIZER_ONE_PASS */
    if (Size >= 8) {
        int val = ((int)Data[6] << 8) | (int)Data[7];
        vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, val);
    }

    /* 
     * Exercise the encoder logic further by encoding a dummy frame 
     * if the input size allows.
     * 
     * FIX: The previous implementation used vpx_img_wrap on the raw fuzzer input
     * buffer, which caused a heap-buffer-overflow because the buffer size did not
     * match the expected size for a 320x240 I420 image (which requires stride * height).
     * We now allocate a properly sized buffer and copy the input data into it safely.
     */
    if (Size >= 100) {
        vpx_image_t img;
        vpx_codec_cx_pkt_t pkt;
        int stride_y = cfg.g_w; 
        int stride_uv = cfg.g_w; // Simplified for I420
        size_t plane_size = stride_y * cfg.g_h;
        size_t total_size = plane_size + (plane_size / 2); // Y + UV
        
        /* Allocate a buffer large enough for the image */
        uint8_t *img_data = (uint8_t *)malloc(total_size);
        if (!img_data) {
            return 0;
        }

        /* Initialize image structure with allocated buffer */
        vpx_img_wrap(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data);

        /* Copy input data safely into the image buffer, avoiding overflow */
        size_t copy_len = (Size < total_size) ? Size : total_size;
        memcpy(img_data, Data, copy_len);
        
        /* Fill remaining with zeros if input was smaller than required */
        if (copy_len < total_size) {
            memset(img_data + copy_len, 0, total_size - copy_len);
        }

        /* Encode frame */
        err = vpx_codec_encode(&codec, &img, 0, 1, 0, 0);
        if (err != VPX_CODEC_OK && err != VPX_CODEC_ERROR) {
            /* Ignore errors during fuzzing */
        }

        /* Drain the encoder */
        while (vpx_codec_get_cx_data(&codec, &pkt) == VPX_CODEC_OK) {
            /* Consume packets */
            if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
                /* Free allocated data if any */
                if (pkt.data.frame.buf) {
                    /* In real code we might free, but here we just consume */
                }
            }
        }
        
        free(img_data);
    }

    return 0;
}