// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static void cleanup_headers(aom_fixed_buf_t *headers) {
    if (headers && headers->buf) {
        free(headers->buf);
    }
    if (headers) {
        free(headers);
    }
}

static int create_test_image(const uint8_t *data, size_t size, aom_image_t *img, int width, int height, int bit_depth) {
    if (width <= 0 || height <= 0 || bit_depth <= 0) {
        return 0;
    }
    
    // AOM_IMG_FMT_I42016 is used for bit_depth > 8
    aom_img_fmt_t fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
    
    if (aom_img_alloc(img, fmt, width, height, 1) != AOM_RC_OK) {
        return 0;
    }
    
    img->bit_depth = bit_depth;

    size_t y_size = (size_t)width * height;
    size_t uv_size = (size_t)(width / 2) * (height / 2);
    
    size_t total_needed = y_size + 2 * uv_size;
    if (size < total_needed) {
        // Fill with pattern if data is insufficient
        memset(img->planes[0], 0, y_size);
        memset(img->planes[1], 0, uv_size);
        memset(img->planes[2], 0, uv_size);
    } else {
        // Copy data from input
        memcpy(img->planes[0], data, y_size);
        memcpy(img->planes[1], data + y_size, uv_size);
        memcpy(img->planes[2], data + y_size + uv_size, uv_size);
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_285(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    int ret = 0;
    int width, height, bit_depth;
    int64_t pts;
    unsigned long duration;
    aom_enc_frame_flags_t flags;
    int use_null_img;
    int get_headers;

    // Parse input to drive fuzzing logic
    width = Data[0];
    height = Data[1];
    bit_depth = Data[2] % 5 + 8; // 8, 9, 10, 11, 12
    pts = (int64_t)Data[3];
    duration = (unsigned long)(Data[4] % 100 + 1);
    flags = (aom_enc_frame_flags_t)(Data[5] % 100);
    use_null_img = (Data[6] % 2);
    get_headers = (Data[7] % 2);

    // Sanity checks
    if (width == 0 || height == 0) {
        width = 64;
        height = 64;
    }
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Ensure duration fits in uint32_t as per docs
    if (duration > UINT32_MAX) {
        duration = UINT32_MAX;
    }

    // Get the AV1 encoder interface explicitly to ensure it's valid
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_RC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_VBR;
    cfg.g_profile = 0;

    // Initialize codec context
    memset(&ctx, 0, sizeof(ctx));
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_RC_OK) {
        return 0;
    }

    // Prepare image if not flushing
    if (!use_null_img) {
        if (!create_test_image(Data, Size, &img, width, height, bit_depth)) {
            cleanup_codec(&ctx);
            return 0;
        }
    }

    // Call aom_codec_encode
    ret = aom_codec_encode(&ctx, use_null_img ? NULL : &img, pts, duration, flags);
    
    // Handle return value
    if (ret != AOM_CODEC_OK) {
        // Error handling is part of fuzzing state exploration
    }

    // Try to get global headers
    if (get_headers) {
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
        if (headers) {
            // Just access to ensure no crash, then free
            cleanup_headers(headers);
        }
    }

    // Flush the encoder
    while (1) {
        ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (ret != AOM_CODEC_OK) {
            break;
        }
        
        // Drain output packets
        aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Consume packet to prevent memory leaks in internal buffers
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Do nothing, just consume
            }
        }
        
        // Check if encoder is done
        if (pkt == NULL && ret == AOM_CODEC_OK) {
            // One more check to be sure
             if (aom_codec_get_cx_data(&ctx, NULL) == NULL) {
                 break;
             }
        }
    }

    // Cleanup
    if (!use_null_img) {
        cleanup_image(&img);
    }
    cleanup_codec(&ctx);

    return 0;
}