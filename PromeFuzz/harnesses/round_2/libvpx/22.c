// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"

static vpx_image_t *create_test_image(unsigned int width, unsigned int height, 
                                       vpx_img_fmt_t fmt, unsigned int bit_depth) {
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill with some data
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int stride = img->stride[plane];
            int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
            for (int y = 0; y < plane_height; y++) {
                memset(img->planes[plane] + y * stride, y % 256, stride);
            }
        }
    }
    
    return img;
}

static void fuzz_encoder_functions(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Try both VP8 and VP9 codecs
    vpx_codec_iface_t *iface = NULL;
    if (size > 0 && (data[0] % 2 == 0)) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }
    
    if (!iface) return;
    
    // Initialize with default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Use fuzz data to modify some configuration parameters
    if (size > 0) {
        cfg.g_w = 64 + (data[0] % 192);  // 64-256 width
        cfg.g_h = 64 + (data[1] % 192);  // 64-256 height
        cfg.rc_target_bitrate = 100 + (data[2] % 900);  // 100-1000 kbps
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
    }
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return;
    }
    
    // Test vpx_codec_enc_config_set with modified config
    if (size > 10) {
        cfg.rc_min_quantizer = data[3] % 64;
        cfg.rc_max_quantizer = 63;
        vpx_codec_enc_config_set(&codec, &cfg);
    }
    
    // Create test image
    vpx_image_t *img = create_test_image(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420, 8);
    if (!img) {
        vpx_codec_destroy(&codec);
        return;
    }
    
    // Test vpx_codec_set_cx_data_buf
    vpx_fixed_buf_t output_buf;
    output_buf.buf = NULL;
    output_buf.sz = 0;
    
    if (size > 20) {
        size_t buf_size = 1024 * 1024;  // 1MB buffer
        output_buf.buf = malloc(buf_size);
        if (output_buf.buf) {
            output_buf.sz = buf_size;
            unsigned int pad_before = data[4] % 16;
            unsigned int pad_after = data[5] % 16;
            vpx_codec_set_cx_data_buf(&codec, &output_buf, pad_before, pad_after);
        }
    }
    
    // Encode frames using fuzz data to vary parameters
    vpx_codec_pts_t pts = 0;
    for (int i = 0; i < 3; i++) {
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
        
        if (size > 30 + i * 10) {
            flags = data[10 + i] & 0xFF;
            if (data[20 + i] % 3 == 0) {
                deadline = VPX_DL_BEST_QUALITY;
            } else if (data[20 + i] % 3 == 1) {
                deadline = VPX_DL_GOOD_QUALITY;
            }
        }
        
        vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
        pts += duration;
        
        // Test vpx_codec_get_cx_data
        const vpx_codec_cx_pkt_t *pkt;
        vpx_codec_iter_t iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets
        }
    }
    
    // Flush encoder
    vpx_codec_encode(&codec, NULL, pts, 0, 0, 0);
    
    // Get remaining packets
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_iter_t iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process different packet types
        switch (pkt->kind) {
            case VPX_CODEC_CX_FRAME_PKT:
                // Compressed frame data
                break;
            case VPX_CODEC_STATS_PKT:
                // Two-pass statistics
                break;
            default:
                break;
        }
    }
    
    // Test vpx_codec_get_global_headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers) {
        // Headers available (VP9 only)
    }
    
    // Test setting buffer to NULL to restore default behavior
    vpx_codec_set_cx_data_buf(&codec, NULL, 0, 0);
    
    // Clean up
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
    // Use a small subset of data to avoid timeouts
    if (size > 1000) {
        size = 1000;
    }
    
    // Create a dummy file if needed (not used in this fuzzer)
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, dummy);
        }
        fclose(dummy);
    }
    
    fuzz_encoder_functions(data, size);
    
    return 0;
}