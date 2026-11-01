// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/*
 * Fuzz driver for libaom encoder API.
 * Focuses on: aom_codec_encode, aom_codec_enc_config_set.
 */

static aom_codec_ctx_t *g_ctx = NULL;
static aom_codec_enc_cfg_t g_cfg;
static int g_initialized = 0;

static void cleanup(void) {
    if (g_ctx) {
        aom_codec_destroy(g_ctx);
        g_ctx = NULL;
    }
    g_initialized = 0;
}

static int init_encoder(const aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &g_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (cfg) {
        memcpy(&g_cfg, cfg, sizeof(aom_codec_enc_cfg_t));
    }

    if (aom_codec_enc_init(&g_ctx, iface, &g_cfg, 0) != AOM_CODEC_OK) {
        cleanup();
        return 0;
    }

    g_initialized = 1;
    return 1;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Reset state for each input to ensure independence */
    cleanup();

    /* Parse input to drive behavior */
    int action = Data[0] % 5;
    int use_custom_cfg = (Data[0] % 2);
    int set_config = (Data[0] % 2);
    int flush_mode = (Data[0] % 2);
    int randomize_flags = (Data[0] % 2);

    aom_codec_enc_cfg_t custom_cfg;
    memset(&custom_cfg, 0, sizeof(custom_cfg));

    if (use_custom_cfg && Size > 1) {
        /* Use input data to seed configuration */
        custom_cfg.g_w = (Data[1] % 256) + 16;
        custom_cfg.g_h = (Data[1] % 256) + 16;
        if (custom_cfg.g_w > 2048) custom_cfg.g_w = 2048;
        if (custom_cfg.g_h > 2048) custom_cfg.g_h = 2048;
        
        custom_cfg.g_bit_depth = AOM_BITS_8;
        if (Size > 2 && Data[2] % 3 == 1) custom_cfg.g_bit_depth = AOM_BITS_10;
        if (Size > 2 && Data[2] % 3 == 2) custom_cfg.g_bit_depth = AOM_BITS_12;
        
        custom_cfg.g_timebase.num = 1;
        custom_cfg.g_timebase.den = (Data[3] % 100) + 1;
        custom_cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
        custom_cfg.g_threads = (Data[5] % 8) + 1;
        custom_cfg.g_profile = Data[6] % 4;
        custom_cfg.g_usage = Data[7] % 3;
        custom_cfg.rc_end_usage = AOM_VBR;
        if (Size > 8) custom_cfg.rc_end_usage = (Data[8] % 4);
        
        custom_cfg.rc_min_quantizer = Data[9] % 64;
        custom_cfg.rc_max_quantizer = (Data[10] % 64) + custom_cfg.rc_min_quantizer;
        if (custom_cfg.rc_max_quantizer > 63) custom_cfg.rc_max_quantizer = 63;

        custom_cfg.kf_mode = AOM_KF_AUTO;
        custom_cfg.kf_min_dist = (Data[11] % 100) + 1;
        custom_cfg.kf_max_dist = (Data[12] % 100) + custom_cfg.kf_min_dist;

        custom_cfg.g_limit = 10;
    }

    /* Initialize encoder */
    if (!init_encoder(use_custom_cfg ? &custom_cfg : NULL)) {
        return 0;
    }

    /* Optionally reconfigure */
    if (set_config && Size > 13) {
        aom_codec_enc_cfg_t re_cfg;
        memset(&re_cfg, 0, sizeof(re_cfg));
        re_cfg.g_w = (Data[13] % 256) + 16;
        re_cfg.g_h = (Data[14] % 256) + 16;
        re_cfg.g_bit_depth = g_cfg.g_bit_depth;
        re_cfg.g_timebase = g_cfg.g_timebase;
        re_cfg.rc_target_bitrate = (Data[15] % 1000) + 100;
        re_cfg.g_threads = (Data[16] % 8) + 1;
        re_cfg.g_profile = Data[17] % 4;
        re_cfg.rc_end_usage = (Data[18] % 4);
        re_cfg.rc_min_quantizer = Data[19] % 64;
        re_cfg.rc_max_quantizer = (Data[20] % 64) + re_cfg.rc_min_quantizer;
        if (re_cfg.rc_max_quantizer > 63) re_cfg.rc_max_quantizer = 63;

        aom_codec_enc_config_set(g_ctx, &re_cfg);
    }

    /* Create image for encoding */
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    int w = g_cfg.g_w;
    int h = g_cfg.g_h;
    int bd = g_cfg.g_bit_depth;
    int bytes_per_pixel = (bd == 8) ? 1 : 2;
    int plane_size = w * h * bytes_per_pixel;
    
    img.fmt = AOM_IMG_FMT_I420;
    if (bd > 8) img.fmt = AOM_IMG_FMT_I42016;
    img.w = w;
    img.h = h;
    img.bit_depth = bd;
    img.d_w = w;
    img.d_h = h;
    img.r_w = w;
    img.r_h = h;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.stride[0] = w * bytes_per_pixel;
    img.stride[1] = (w / 2) * bytes_per_pixel;
    img.stride[2] = (w / 2) * bytes_per_pixel;
    
    /* Allocate planes */
    size_t total_size = plane_size + (plane_size / 4) + (plane_size / 4);
    unsigned char *data = (unsigned char *)malloc(total_size);
    if (!data) return 0;

    img.planes[0] = data;
    img.planes[1] = data + plane_size;
    img.planes[2] = data + plane_size + (plane_size / 4);
    img.sz = total_size;

    /* Fill image with fuzz data */
    size_t data_idx = 21;
    for (size_t i = 0; i < total_size; i++) {
        if (data_idx < Size) {
            unsigned char val = Data[data_idx];
            if (bd == 8) {
                img.planes[0][i] = val;
            } else {
                // For 10/12 bit, we need to construct 16-bit values
                // Simple approach: use byte as lower bits
                if (i % 2 == 0) {
                    int16_t *p = (int16_t*)(img.planes[0] + i);
                    *p = val;
                }
            }
            data_idx++;
        } else {
            img.planes[0][i] = 0;
        }
    }

    /* Perform encoding */
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (randomize_flags && Size > 22) {
        flags = Data[22];
    }

    /* Encode some frames */
    int frames_to_encode = 3;
    for (int f = 0; f < frames_to_encode; f++) {
        if (flush_mode && f == 2) {
            /* Flush */
            aom_codec_encode(g_ctx, NULL, pts, 0, flags);
        } else {
            /* Encode frame */
            if (Size > 23) {
                pts += (Data[23] % 10) + 1;
                duration = (Data[24] % 10) + 1;
            } else {
                pts++;
                duration = 1;
            }
            
            aom_codec_err_t res = aom_codec_encode(g_ctx, &img, pts, duration, flags);
            if (res != AOM_CODEC_OK) {
                /* Expected on error, continue */
            }
        }
    }

    /* Flush remaining */
    while (1) {
        aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(g_ctx, NULL);
        if (!pkt) break;
        /* Process packet if needed, but for fuzzing we just consume */
        aom_codec_destroy(g_ctx);
        g_ctx = NULL;
        break;
    }

    /* Cleanup */
    free(data);
    cleanup();

    return 0;
}