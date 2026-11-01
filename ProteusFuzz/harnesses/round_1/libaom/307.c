// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void setup_image_data(aom_image_t *img, const uint8_t *data, size_t size) {
    // Initialize image structure with safe defaults
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->bit_depth = 8;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_UNKNOWN;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->bps = 8;

    // Allocate planes
    size_t y_size = 16 * 16;
    size_t uv_size = 8 * 8;
    
    img->planes[0] = malloc(y_size);
    img->stride[0] = 16;
    img->planes[1] = malloc(uv_size);
    img->stride[1] = 8;
    img->planes[2] = malloc(uv_size);
    img->stride[2] = 8;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        img->planes[0] = NULL;
        img->planes[1] = NULL;
        img->planes[2] = NULL;
        return;
    }

    // Initialize planes to zero first to ensure safety
    memset(img->planes[0], 0, y_size);
    memset(img->planes[1], 0, uv_size);
    memset(img->planes[2], 0, uv_size);

    // Fill with fuzz data, clamping to valid range [0, 255]
    // Only copy up to the available size for each plane
    size_t y_copy = (size < y_size) ? size : y_size;
    memcpy(img->planes[0], data, y_copy);
    
    size_t remaining_size = (size > y_size) ? size - y_size : 0;
    size_t uv_copy = (remaining_size < uv_size) ? remaining_size : uv_size;
    if (uv_copy > 0) {
        memcpy(img->planes[1], data + y_size, uv_copy);
    }
    
    remaining_size = (size > y_size + uv_size) ? size - (y_size + uv_size) : 0;
    uv_copy = (remaining_size < uv_size) ? remaining_size : uv_size;
    if (uv_copy > 0) {
        memcpy(img->planes[2], data + y_size + uv_size, uv_copy);
    }
    
    // Ensure data is within valid range for 8-bit depth
    for (size_t i = 0; i < y_size; i++) {
        img->planes[0][i] = (img->planes[0][i] & 0xFF);
    }
    for (size_t i = 0; i < uv_size; i++) {
        img->planes[1][i] = (img->planes[1][i] & 0xFF);
        img->planes[2][i] = (img->planes[2][i] & 0xFF);
    }
    
    img->sz = y_size + 2 * uv_size;
}

static void cleanup_image(aom_image_t *img) {
    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    aom_fixed_buf_t *global_headers = NULL;

    // Initialize encoder context
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_bit_depth = 8;
    cfg.g_input_bit_depth = 8;
    cfg.g_profile = 0;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzzing logic
    uint64_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    // Parse input to control fuzzing behavior
    int num_frames = 1;
    if (Size > 0) {
        num_frames = (Data[0] % 10) + 1;
    }
    if (Size > 1) {
        duration = (Data[1] % 100) + 1;
    }
    if (Size > 2) {
        flags = (Data[2] % 256);
    }

    for (int i = 0; i < num_frames; i++) {
        setup_image_data(&img, Data, Size);
        
        // Encode frame
        ret = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (ret != AOM_CODEC_OK) {
            // Continue to flush
        }

        // Collect output packets
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet if needed, here we just consume it
            (void)pkt; 
        }

        cleanup_image(&img);
        
        // Ensure strictly increasing PTS
        pts += duration;
    }

    // Flush the encoder
    while (1) {
        ret = aom_codec_encode(&ctx, NULL, pts, duration, flags);
        if (ret != AOM_CODEC_OK) {
            // Check if it's just no more data
            if (aom_codec_get_cx_data(&ctx, NULL) == NULL) {
                break;
            }
        }

        // Collect remaining packets
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Consume packet
            (void)pkt;
        }

        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) {
            break;
        }
    }

    // Try to get global headers
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers && global_headers->buf) {
        free(global_headers->buf);
        free(global_headers);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}