// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void safe_free(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Test 1: vpx_codec_error with NULL context
    const char *err_msg = vpx_codec_error(NULL);
    if (err_msg) {
        (void)strlen(err_msg);
    }

    // Test 2: vpx_codec_decode with uninitialized/invalid context
    vpx_codec_err_t decode_res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)decode_res;
    err_msg = vpx_codec_error(&ctx);
    if (err_msg) {
        (void)strlen(err_msg);
    }

    // Test 3: vpx_codec_get_frame with invalid context
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
    if (frame) {
        (void)frame;
    }

    // Test 4: vpx_codec_get_preview_frame with invalid context
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
    if (preview) {
        (void)preview;
    }

    // Test 5: vpx_codec_encode with invalid context
    vpx_image_t dummy_img;
    memset(&dummy_img, 0, sizeof(dummy_img));
    vpx_codec_err_t encode_res = vpx_codec_encode(&ctx, &dummy_img, 0, 1, 0, 0);
    (void)encode_res;
    err_msg = vpx_codec_error(&ctx);
    if (err_msg) {
        (void)strlen(err_msg);
    }

    // Test 6: vpx_codec_get_cx_data with invalid context
    vpx_codec_iter_t enc_iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = vpx_codec_get_cx_data(&ctx, &enc_iter);
    if (pkt) {
        (void)pkt;
    }

    // Test 7: Initialize a real decoder to test valid paths
    const vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    if (decoder_iface) {
        vpx_codec_ctx_t dec_ctx;
        memset(&dec_ctx, 0, sizeof(dec_ctx));
        vpx_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.w = 128;
        cfg.h = 128;

        vpx_codec_err_t init_res = vpx_codec_dec_init(&dec_ctx, decoder_iface, &cfg, 0);
        if (init_res == VPX_CODEC_OK) {
            // Decode some data
            decode_res = vpx_codec_decode(&dec_ctx, Data, Size, NULL, 0);
            (void)decode_res;
            
            // Iterate frames
            iter = NULL;
            while ((frame = vpx_codec_get_frame(&dec_ctx, &iter)) != NULL) {
                (void)frame;
            }

            // Flush decoder
            decode_res = vpx_codec_decode(&dec_ctx, NULL, 0, NULL, 0);
            (void)decode_res;

            // Cleanup
            vpx_codec_destroy(&dec_ctx);
        }
    }

    // Test 8: Initialize a real encoder to test valid paths
    const vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    if (encoder_iface) {
        vpx_codec_ctx_t enc_ctx;
        memset(&enc_ctx, 0, sizeof(enc_ctx));
        
        vpx_codec_enc_cfg_t enc_cfg;
        memset(&enc_cfg, 0, sizeof(enc_cfg));
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 128;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 100;

        vpx_codec_err_t enc_init_res = vpx_codec_enc_init(&enc_ctx, encoder_iface, &enc_cfg, 0);
        if (enc_init_res == VPX_CODEC_OK) {
            // Create a dummy image
            vpx_image_t img;
            memset(&img, 0, sizeof(img));
            img.w = 128;
            img.h = 128;
            img.d_w = 128;
            img.d_h = 128;
            img.fmt = VPX_IMG_FMT_I420;
            img.bit_depth = 8;
            img.cs = VPX_CS_BT_601;
            img.range = VPX_CR_STUDIO_RANGE;
            img.x_chroma_shift = 1;
            img.y_chroma_shift = 1;
            
            // Manually calculate size for I420 128x128
            // Y: 128*128, U: 64*64, V: 64*64
            size_t img_size = (128 * 128) + (64 * 64) + (64 * 64);
            
            uint8_t *img_data = safe_malloc(img_size);
            if (img_data) {
                size_t copy_len = (Size < img_size) ? Size : img_size;
                memcpy(img_data, Data, copy_len);
                
                img.planes[VPX_PLANE_Y] = img_data;
                img.stride[VPX_PLANE_Y] = 128;
                img.planes[VPX_PLANE_U] = img_data + (128 * 128);
                img.stride[VPX_PLANE_U] = 64;
                img.planes[VPX_PLANE_V] = img_data + (128 * 128) + (64 * 64);
                img.stride[VPX_PLANE_V] = 64;
                img.img_data = img_data;
                img.img_data_owner = 1;

                // Encode frame
                encode_res = vpx_codec_encode(&enc_ctx, &img, 0, 1, 0, 0);
                (void)encode_res;

                // Get encoded data
                enc_iter = NULL;
                while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &enc_iter)) != NULL) {
                    (void)pkt;
                }

                // Flush encoder
                encode_res = vpx_codec_encode(&enc_ctx, NULL, 0, 1, 0, 0);
                (void)encode_res;
                enc_iter = NULL;
                while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &enc_iter)) != NULL) {
                    (void)pkt;
                }

                safe_free(img_data);
            }

            vpx_codec_destroy(&enc_ctx);
        }
    }

    return 0;
}