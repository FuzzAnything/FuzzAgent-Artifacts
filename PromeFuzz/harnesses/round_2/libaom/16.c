// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

static void init_dummy_image(aom_image_t *img, const uint8_t *data, size_t size) {
    // Initialize a dummy image with fuzzer data
    static const int width = 64;
    static const int height = 64;
    
    // Use I420 format
    aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill planes with fuzzer data (or zeros if not enough)
    size_t offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = (plane == 0) ? width : width / 2;
        int plane_height = (plane == 0) ? height : height / 2;
        size_t plane_size = plane_width * plane_height;
        
        for (int y = 0; y < plane_height; y++) {
            size_t row_size = plane_width;
            size_t copy_size = (size - offset) < row_size ? (size - offset) : row_size;
            
            if (copy_size > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], data + offset, copy_size);
                offset += copy_size;
            } else {
                // Fill with zeros if no more data
                memset(img->planes[plane] + y * img->stride[plane], 0, row_size);
            }
        }
    }
}

static void cleanup_encoder(aom_codec_ctx_t *ctx, aom_image_t *img, aom_fixed_buf_t *buf) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
    if (img) {
        aom_img_free(img);
    }
    if (buf && buf->buf) {
        free(buf->buf);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_fixed_buf_t output_buf;
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    
    // Initialize structures
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));
    memset(&output_buf, 0, sizeof(output_buf));
    
    // Get encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some basic parameters from fuzzer data
    if (size > 0) {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256;
        cfg.g_pass = AOM_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        
        // Use some fuzzer data to influence configuration
        if (size > 4) {
            cfg.rc_end_usage = (enum aom_rc_mode)(data[0] % 4);
            cfg.g_profile = data[1] % 4;
            cfg.g_threads = (data[2] % 8) + 1;
            cfg.rc_min_quantizer = data[3] % 64;
            cfg.rc_max_quantizer = (data[3] % 64) + 10;
        }
    }
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Initialize dummy image
    init_dummy_image(&img, data, size);
    
    // Set some encoder controls using fuzzer data
    if (size > 8) {
        // Use different control IDs based on fuzzer data
        int control_ids[] = {
            7,    // AOME_SET_CPUUSED
            17,   // AOME_SET_ENABLEAUTOALTREF
            8,    // AOME_SET_SHARPNESS
            26,   // AOME_SET_STATIC_THRESHOLD
            5,    // AOME_SET_ARNR_MAXFRAMES
            6     // AOME_SET_ARNR_STRENGTH
        };
        
        for (int i = 0; i < sizeof(control_ids)/sizeof(control_ids[0]) && i + 8 < size; i++) {
            int value = data[8 + i] % 256;
            aom_codec_control(&ctx, control_ids[i], value);
        }
    }
    
    // Allocate output buffer
    size_t buf_size = 1024 * 1024; // 1MB buffer
    output_buf.buf = malloc(buf_size);
    output_buf.sz = buf_size;
    
    if (output_buf.buf) {
        // Set output buffer with padding from fuzzer data
        unsigned int pad_before = 0;
        unsigned int pad_after = 0;
        
        if (size > 16) {
            pad_before = data[16] % 256;
            pad_after = data[17] % 256;
        }
        
        aom_codec_set_cx_data_buf(&ctx, &output_buf, pad_before, pad_after);
    }
    
    // Encode frames
    int frame_count = 2;
    for (int i = 0; i < frame_count; i++) {
        aom_codec_pts_t pts = i * 1000;
        unsigned long duration = 1000;
        aom_enc_frame_flags_t flags = 0;
        
        // Use fuzzer data to influence flags
        if (size > 20 && (data[20] & (1 << (i % 8)))) {
            flags = 1; // Force keyframe flag
        }
        
        // Encode frame (use NULL for last frame to flush)
        const aom_image_t *frame_to_encode = (i == frame_count - 1) ? NULL : &img;
        aom_codec_encode(&ctx, frame_to_encode, pts, duration, flags);
        
        // Get encoded data
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet if needed
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Access packet data to exercise code paths
                volatile size_t packet_size = pkt->data.frame.sz;
                volatile aom_codec_pts_t packet_pts = pkt->data.frame.pts;
            }
        }
        iter = NULL;
    }
    
    // Test reconfiguration
    if (size > 24) {
        // Modify some configuration parameters
        cfg.rc_target_bitrate = (data[24] % 1000) + 100;
        cfg.g_w = 128;
        cfg.g_h = 128;
        
        aom_codec_enc_config_set(&ctx, &cfg);
        
        // Encode one more frame with new configuration
        aom_codec_encode(&ctx, &img, frame_count * 1000, 1000, 0);
        
        // Get any remaining data
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet
        }
    }
    
    // Cleanup
    cleanup_encoder(&ctx, &img, &output_buf);
    
    return 0;
}