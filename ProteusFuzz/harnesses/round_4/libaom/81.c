// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int create_test_image(aom_image_t *img, int width, int height, int bit_depth, const uint8_t *data, size_t data_size) {
    if (!img || width <= 0 || height <= 0 || bit_depth <= 0) return 0;

    // Ensure dimensions are aligned to avoid potential issues
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) return 0;
    if (!img->planes[0]) return 0;

    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;

    // Fill planes with fuzzing data if available
    size_t total_plane_size = 0;
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            // Calculate actual plane size based on stride and height
            size_t plane_size = (size_t)img->stride[i] * img->h;
            
            if (data && data_size > 0) {
                size_t copy_size = (data_size > plane_size) ? plane_size : data_size;
                memcpy(img->planes[i], data, copy_size);
                data_size -= copy_size;
                if (data) data += copy_size;
            } else {
                // Safe memset using the calculated plane_size
                memset(img->planes[i], 0x80, plane_size);
            }
            total_plane_size += plane_size;
        }
    }
    img->sz = total_plane_size;
    return 1;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Extract parameters from input
    int width = (Data[0] % 64) + 16;
    int height = (Data[1] % 64) + 16;
    int bit_depth = (Data[2] % 4) + 8; // 8, 10, 12, 14
    int frame_count = (Data[3] % 5) + 1;
    int use_custom_buf = (Data[4] % 2);
    int pad_before = (Data[5] % 100);
    int pad_after = (Data[6] % 100);
    int flags = Data[7];
    int duration = (Data[8] % 100) + 1;
    
    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 10;
    cfg.rc_overshoot_pct = 10;
    cfg.g_error_resilient = 0;

    aom_codec_ctx_t *ctx_ptr = &ctx;
    if (aom_codec_enc_init(ctx_ptr, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Prepare image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Prepare custom buffer if needed
    aom_fixed_buf_t custom_buf;
    uint8_t *custom_buf_data = NULL;
    if (use_custom_buf && Size > 10) {
        size_t available_data = Size - 10;
        size_t buf_size = (available_data > 1024) ? 1024 : available_data;
        custom_buf_data = (uint8_t *)malloc(buf_size);
        if (custom_buf_data) {
            // Ensure we don't read past the end of the input buffer
            memcpy(custom_buf_data, Data + 10, buf_size);
            custom_buf.buf = custom_buf_data;
            custom_buf.sz = buf_size;
        }
    }

    aom_codec_pts_t pts = 0;

    for (int i = 0; i < frame_count; i++) {
        // Set custom buffer if requested
        if (use_custom_buf && custom_buf_data) {
            aom_codec_set_cx_data_buf(ctx_ptr, &custom_buf, pad_before, pad_after);
        } else {
            aom_codec_set_cx_data_buf(ctx_ptr, NULL, 0, 0);
        }

        // Create image for this frame
        if (!create_test_image(&img, width, height, bit_depth, Data + 10, Size - 10)) {
            // If image creation fails, try to free previous allocation if any
            if (img.planes[0]) aom_img_free(&img);
            memset(&img, 0, sizeof(img));
            continue;
        }

        // Encode frame
        aom_codec_encode(ctx_ptr, &img, pts, duration, flags);
        pts += duration;

        // Get output data
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(ctx_ptr, NULL)) != NULL) {
            // Consume packet to avoid memory leaks
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Data is in pkt->data.frame.buf
                // No explicit free needed for the packet structure here.
            }
        }
    }

    // Flush encoder
    while (1) {
        aom_codec_encode(ctx_ptr, NULL, 0, 0, 0);
        const aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(ctx_ptr, NULL)) != NULL) {
            has_data = 1;
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Data is in pkt->data.frame.buf
            }
        }
        if (!has_data) break;
    }

    // Cleanup
    aom_codec_destroy(ctx_ptr);
    if (img.planes[0]) aom_img_free(&img);
    if (custom_buf_data) free(custom_buf_data);

    return 0;
}