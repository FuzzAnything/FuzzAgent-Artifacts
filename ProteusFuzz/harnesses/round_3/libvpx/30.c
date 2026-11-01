// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed allow_highbitdepth as it is not a member in this struct version

    if (vpx_codec_dec_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static vpx_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to configure parameters safely
    cfg.g_w = (size_t)(data[0] & 0x3F) + 16;
    cfg.g_h = (size_t)(data[1] & 0x3F) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (size_t)(data[2] & 0xFF) + 1;
    cfg.rc_min_quantizer = data[3] & 0x3F;
    cfg.rc_max_quantizer = (data[4] & 0x3F) + cfg.rc_min_quantizer;
    cfg.rc_target_bitrate = (data[5] * 256 + data[6]) * 1000;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static vpx_image_t *create_test_image(const uint8_t *data, size_t size) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                     (data[0] & 0x3F) + 16, 
                                     (data[1] & 0x3F) + 16, 
                                     1);
    if (!img) return NULL;
    
    // Fill with fuzzing data
    for (int p = 0; p < 3; p++) {
        if (img->planes[p]) {
            size_t plane_size = (size_t)img->h * (size_t)img->stride[p];
            if (plane_size > 0) {
                memcpy(img->planes[p], data, size < plane_size ? size : plane_size);
            }
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Test Decoder
    vpx_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        vpx_codec_iter_t iter = NULL;
        
        // Decode with fuzzing data
        vpx_codec_decode(dec_ctx, Data, Size, NULL, 0);
        
        // Get frames
        while (vpx_codec_get_frame(dec_ctx, &iter) != NULL) {
            // Frame retrieved successfully
        }
        
        // Flush decoder
        vpx_codec_decode(dec_ctx, NULL, 0, NULL, 0);
        while (vpx_codec_get_frame(dec_ctx, &iter) != NULL) {
            // Flush frames
        }
        
        // Test error retrieval
        const char *err = vpx_codec_error(dec_ctx);
        (void)err; // Suppress unused warning
        
        vpx_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    // Test Encoder
    vpx_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        vpx_image_t *img = create_test_image(Data, Size);
        vpx_codec_iter_t iter = NULL;
        vpx_codec_pts_t pts = 0;
        unsigned long duration = (Data[7] & 0xFF) + 1;
        vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Data[8] & 0xFF);
        vpx_enc_deadline_t deadline = (Data[9] & 0xFF) * 1000;

        if (img) {
            // Encode frame
            vpx_codec_encode(enc_ctx, img, pts, duration, flags, deadline);
            pts += duration;
            
            // Get encoded packets
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(enc_ctx, &iter)) != NULL) {
                // Process packet
            }
            
            vpx_img_free(img);
        }

        // Flush encoder
        vpx_codec_encode(enc_ctx, NULL, pts, duration, flags, deadline);
        while (vpx_codec_get_cx_data(enc_ctx, &iter) != NULL) {
            // Flush packets
        }

        // Test preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(enc_ctx);
        (void)preview;

        // Test error retrieval
        const char *err = vpx_codec_error(enc_ctx);
        (void)err;

        vpx_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    return 0;
}