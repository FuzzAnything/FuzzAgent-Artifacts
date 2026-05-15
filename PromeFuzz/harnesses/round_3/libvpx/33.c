// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1 + (Data[0] % 4);
    
    const vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    if (vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Decode with fuzz data
    size_t decode_size = Size > 1024 ? 1024 : Size;
    vpx_codec_decode(&decoder, Data, (unsigned int)decode_size, NULL, 0);
    
    // Get error details if any
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail;

    // Try to get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Do nothing, just consume frames
    }

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    const vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    enc_cfg.g_w = 64 + (Data[0] % 192);
    enc_cfg.g_h = 64 + (Data[1] % 192);
    enc_cfg.g_threads = 1 + (Data[2] % 4);
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 100 + (Data[3] % 900);
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = Data[4] % 16;

    if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Create test image for encoding
    vpx_image_t image;
    vpx_img_alloc(&image, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16);
    
    if (image.planes[0] != NULL) {
        // Fill image planes with fuzz data
        size_t y_size = (size_t)enc_cfg.g_w * enc_cfg.g_h;
        size_t uv_size = y_size / 4;
        
        if (Size >= y_size + 2 * uv_size) {
            memcpy(image.planes[0], Data, y_size);
            memcpy(image.planes[1], Data + y_size, uv_size);
            memcpy(image.planes[2], Data + y_size + uv_size, uv_size);
        } else {
            // Fill with pattern if not enough data
            for (size_t i = 0; i < y_size; i++) {
                image.planes[0][i] = (uint8_t)(i ^ Data[i % Size]);
            }
            for (size_t i = 0; i < uv_size; i++) {
                image.planes[1][i] = (uint8_t)(i ^ Data[(i + 1) % Size]);
                image.planes[2][i] = (uint8_t)(i ^ Data[(i + 2) % Size]);
            }
        }

        // Encode the image
        vpx_codec_encode(&encoder, &image, 0, 1, 0, VPX_DL_REALTIME);

        // Set output buffer
        uint8_t output_buffer[4096];
        vpx_fixed_buf_t buf;
        buf.buf = output_buffer;
        buf.sz = sizeof(output_buffer);
        vpx_codec_set_cx_data_buf(&encoder, &buf, Data[5] % 16, Data[6] % 16);

        // Get encoded data
        vpx_codec_iter_t enc_iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            // Consume packets
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Try to decode the encoded frame
                vpx_codec_decode(&decoder, (const uint8_t*)pkt->data.frame.buf,
                                (unsigned int)pkt->data.frame.sz, NULL, 0);
                
                // Get decoded frames
                vpx_codec_iter_t dec_iter = NULL;
                while (vpx_codec_get_frame(&decoder, &dec_iter) != NULL) {
                    // Consume frames
                }
            }
        }

        // Flush encoder
        vpx_codec_encode(&encoder, NULL, 0, 1, 0, VPX_DL_REALTIME);
        enc_iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            // Consume remaining packets
        }

        // Test various control codes
        int ctrl_values[] = {
            VP8E_SET_CPUUSED,
            VP8E_SET_ENABLEAUTOALTREF,
            VP8E_SET_ARNR_MAXFRAMES,
            VP8E_SET_ARNR_STRENGTH,
        };
        
        for (size_t i = 0; i < sizeof(ctrl_values)/sizeof(ctrl_values[0]); i++) {
            if (i < Size) {
                int value = Data[i] % 10;
                vpx_codec_control_(&encoder, ctrl_values[i % 4], value);
            }
        }

        vpx_img_free(&image);
    }

    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}