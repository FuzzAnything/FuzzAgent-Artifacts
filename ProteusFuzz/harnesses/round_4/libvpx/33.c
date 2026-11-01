// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_image_t *create_fuzz_image(const uint8_t *data, size_t size, int width, int height) {
    if (size < 100 || width < 1 || height < 1) return NULL;
    
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    // Fill image planes with fuzz data
    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    
    if (size >= y_size + 2 * uv_size) {
        memcpy(img->planes[VPX_PLANE_Y], data, y_size);
        memcpy(img->planes[VPX_PLANE_U], data + y_size, uv_size);
        memcpy(img->planes[VPX_PLANE_V], data + y_size + uv_size, uv_size);
    } else {
        memset(img->planes[VPX_PLANE_Y], 0, y_size);
        memset(img->planes[VPX_PLANE_U], 0, uv_size);
        memset(img->planes[VPX_PLANE_V], 0, uv_size);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Use first byte to decide encoder vs decoder
    int mode = Data[0] % 2;
    uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;
    
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    
    if (mode == 0) {
        // Encoder path
        vpx_codec_enc_cfg_t cfg;
        iface = vpx_codec_vp8_cx();
        if (!iface) return 0;
        
        if (vpx_codec_enc_config_default(&cfg, iface, 0) != VPX_CODEC_OK) return 0;
        
        // Fuzz config parameters
        cfg.g_w = (payload[0] % 512) + 16;
        cfg.g_h = (payload[1] % 512) + 16;
        cfg.g_timebase.num = (payload[2] % 100) + 1;
        cfg.g_timebase.den = (payload[3] % 100) + 1;
        cfg.rc_target_bitrate = (payload[4] % 10000) + 1;
        
        if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        
        // Create test image
        int w = (payload[5] % 128) + 16;
        int h = (payload[6] % 128) + 16;
        vpx_image_t *img = create_fuzz_image(payload + 7, payload_size - 7, w, h);
        
        if (img) {
            // Encode frame
            vpx_codec_pts_t pts = (payload[7] % 1000);
            unsigned long duration = (payload[8] % 100) + 1;
            vpx_enc_frame_flags_t flags = (payload[9] % 16);
            vpx_enc_deadline_t deadline = (payload[10] % 1000000);
            
            res = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
            
            // Get output packets
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                // Process packet (ignore unsupported types)
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Valid frame packet
                }
            }
            
            vpx_img_free(img);
        }
        
        // Flush encoder
        res = vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
        vpx_codec_iter_t iter_flush = NULL;
        while (vpx_codec_get_cx_data(&ctx, &iter_flush) != NULL);
        
        // Test preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
        (void)preview;
        
        vpx_codec_destroy(&ctx);
        
    } else {
        // Decoder path
        vpx_codec_dec_cfg_t cfg;
        iface = vpx_codec_vp8_dx();
        if (!iface) return 0;
        
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (payload[0] % 4) + 1;
        
        if (vpx_codec_dec_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        
        // Decode frames
        vpx_codec_iter_t iter = NULL;
        size_t offset = 0;
        
        while (offset < payload_size) {
            unsigned int chunk_size = (payload[offset] % 1000) + 1;
            if (offset + chunk_size > payload_size) chunk_size = payload_size - offset;
            
            res = vpx_codec_decode(&ctx, payload + offset, chunk_size, NULL, 0);
            
            // Get decoded frames
            vpx_image_t *frame;
            while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                // Frame decoded successfully
            }
            
            offset += chunk_size;
        }
        
        // Flush decoder
        res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        iter = NULL;
        while (vpx_codec_get_frame(&ctx, &iter) != NULL);
        
        vpx_codec_destroy(&ctx);
    }
    
    // Test error handling
    const char *err = vpx_codec_error(&ctx);
    (void)err;
    
    return 0;
}