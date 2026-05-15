// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static aom_image_t *create_test_image(size_t width, size_t height, aom_img_fmt_t fmt, unsigned int bit_depth) {
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    
    // Fill planes with dummy data
    for (int plane = 0; plane < 3; ++plane) {
        if (img->planes[plane]) {
            size_t plane_height = (height >> (plane ? img->y_chroma_shift : 0));
            size_t plane_size = img->stride[plane] * plane_height;
            for (size_t i = 0; i < plane_size; ++i) {
                img->planes[plane][i] = (uint8_t)(i % 256);
            }
        }
    }
    
    return img;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx, aom_image_t *img) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
    if (img) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize encoder configuration with defaults
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use fuzzer data to modify some configuration parameters, but check bounds
    cfg.g_w = 64 + ((Size > 0 ? Data[0] : 0) % 64);
    cfg.g_h = 64 + ((Size > 1 ? Data[1] : 0) % 64);
    cfg.g_bit_depth = (Size > 2 && (Data[2] & 1)) ? AOM_BITS_10 : AOM_BITS_8;
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.rc_target_bitrate = 100 + ((Size > 3 ? Data[3] : 0) % 900);
    cfg.g_pass = (Size > 4 && (Data[4] & 1)) ? AOM_RC_FIRST_PASS : AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = (Size > 5 ? Data[5] : 0) % 16;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some control parameters based on fuzzer data
    if (Size > 6) {
        int cpu_used = Data[6] % 9;
        aom_codec_control(&ctx, AOME_SET_CPUUSED, cpu_used);
    }
    
    if (Size > 7) {
        int enable_denoise = Data[7] & 1;
        aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, enable_denoise ? 1 : 0);
    }
    
    // Create test image
    aom_img_fmt_t fmt = (Size > 8 && (Data[8] & 1)) ? AOM_IMG_FMT_I420 : AOM_IMG_FMT_I444;
    if (cfg.g_bit_depth == AOM_BITS_10) {
        fmt = (fmt == AOM_IMG_FMT_I420) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I44416;
    }
    
    aom_image_t *img = create_test_image(cfg.g_w, cfg.g_h, fmt, cfg.g_bit_depth);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Set output buffer if we have enough data
    aom_fixed_buf_t output_buf;
    unsigned char buffer[4096];
    if (Size > 100) {
        output_buf.buf = buffer;
        output_buf.sz = sizeof(buffer);
        unsigned int pad_before = (Size > 9 ? Data[9] : 0) % 64;
        unsigned int pad_after = (Size > 10 ? Data[10] : 0) % 64;
        aom_codec_set_cx_data_buf(&ctx, &output_buf, pad_before, pad_after);
    }
    
    // Encode frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    // Encode first frame
    if (aom_codec_encode(&ctx, img, pts, duration, flags) != AOM_CODEC_OK) {
        cleanup_encoder(&ctx, img);
        return 0;
    }
    
    // Get encoded data
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process different packet types
        switch (pkt->kind) {
            case AOM_CODEC_CX_FRAME_PKT:
                // Access frame data
                if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                    // Touch the data to ensure it's valid
                    volatile uint8_t dummy = *((uint8_t*)pkt->data.frame.buf);
                    (void)dummy;
                }
                break;
            case AOM_CODEC_STATS_PKT:
                // Access stats data
                if (pkt->data.twopass_stats.buf && pkt->data.twopass_stats.sz > 0) {
                    volatile uint8_t dummy = *((uint8_t*)pkt->data.twopass_stats.buf);
                    (void)dummy;
                }
                break;
            default:
                break;
        }
    }
    
    // Encode a few more frames with varying parameters
    for (int i = 0; i < 3 && (i * 16 + 12) < (int)Size; i++) {
        pts += 1;
        flags = (Size > (11 + i) && (Data[11 + i] & 1)) ? AOM_EFLAG_FORCE_KF : 0;
        
        // Modify image data slightly
        if (img->planes[0]) {
            size_t offset = (i * 16) % (img->stride[0] * img->h);
            if (offset < img->stride[0] * img->h) {
                img->planes[0][offset] = Size > (12 + i) ? Data[12 + i] : 0;
            }
        }
        
        if (aom_codec_encode(&ctx, img, pts, duration, flags) != AOM_CODEC_OK) {
            break;
        }
        
        // Get encoded data
        iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Just iterate through packets
        }
    }
    
    // Flush encoder
    iter = NULL;
    while (aom_codec_encode(&ctx, NULL, pts, duration, 0) == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process remaining packets
        }
        iter = NULL;
    }
    
    // Final get of any remaining data
    iter = NULL;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process final packets
    }
    
    cleanup_encoder(&ctx, img);
    return 0;
}