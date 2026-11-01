// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static int create_test_image(const uint8_t *data, size_t size, aom_image_t *img) {
    if (size < 2) return 0;

    unsigned int width = data[0] ? data[0] : 16;
    unsigned int height = data[1] ? data[1] : 16;

    // Ensure reasonable bounds to prevent allocation failures or huge allocations
    if (width > 1920 || height > 1080) {
        width = 1920;
        height = 1080;
    }
    if (width == 0 || height == 0) {
        width = 16;
        height = 16;
    }

    // AOM_IMG_OK is not defined in older versions or specific headers, use AOM_CODEC_OK or check return directly
    // aom_img_alloc returns AOM_CODEC_OK (which is 0) on success in many contexts, 
    // but strictly speaking it returns aom_img_alloc return type. 
    // In libaom, aom_img_alloc returns AOM_CODEC_OK (0) on success.
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != AOM_CODEC_OK) {
        return 0;
    }

    // Fill image data with fuzz input
    size_t plane_size = width * height;
    size_t total_size = plane_size + (plane_size / 2) + (plane_size / 2);
    size_t copy_size = (size > total_size) ? total_size : size;

    if (copy_size > 0) {
        memcpy(img->planes[AOM_PLANE_Y], data, copy_size);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    aom_image_t img;
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    int frame_count = 0;
    int64_t pts = 0;
    unsigned long duration = 1;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default("av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 1) cfg.g_w = Data[0] ? Data[0] : 16;
    if (Size > 2) cfg.g_h = Data[1] ? Data[1] : 16;
    if (Size > 3) cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    if (Size > 4) cfg.g_timebase.den = Data[3] ? Data[3] : 1;
    if (Size > 5) cfg.rc_target_bitrate = Data[4] * 1000;
    if (Size > 6) cfg.g_pass = Data[5] & 0x03; // 0, 1, 2, or 3

    // Ensure valid dimensions
    if (cfg.g_w == 0) cfg.g_w = 16;
    if (cfg.g_h == 0) cfg.g_h = 16;

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Create a test image from fuzz data if we have enough data
    int img_created = create_test_image(Data, Size, &img);

    // Encode frames loop
    // We perform a limited number of iterations to avoid infinite loops in fuzzing
    // but enough to exercise the encoder state machine
    int max_frames = 10;
    int i;
    for (i = 0; i < max_frames; i++) {
        const aom_image_t *img_ptr = NULL;
        int64_t current_pts = pts;
        unsigned long current_duration = duration;

        // Alternate between encoding a frame and flushing based on fuzz data
        if (i < 5 && img_created) {
            img_ptr = &img;
            // Ensure strictly increasing PTS
            current_pts = i * 100; 
            // Ensure non-zero duration
            current_duration = (Size > 6) ? (Data[6] % 100) + 1 : 1;
        } else {
            // Flush
            img_ptr = NULL;
            current_pts = 0;
            current_duration = 0;
        }

        aom_enc_frame_flags_t flags = 0;
        if (Size > 7) flags = Data[7];

        err = aom_codec_encode(&ctx, img_ptr, current_pts, current_duration, flags);
        if (err != AOM_CODEC_OK) {
            // If encoding fails, we might still want to try to flush or just break
            // Depending on the error, we might continue to drain
            if (err == AOM_CODEC_INVALID_PARAM) {
                // Try flushing to clean up
                aom_codec_encode(&ctx, NULL, 0, 0, 0);
            }
            // Continue to drain packets
        }

        // Drain packets
        do {
            pkt = aom_codec_get_cx_data(&ctx, &iter);
            if (pkt) {
                // Process packet (ignore for fuzzing, just ensure no crash)
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Valid frame packet
                }
            }
        } while (pkt != NULL);

        // If we flushed and got no data, we are done
        if (img_ptr == NULL && pkt == NULL && iter == NULL) {
            // Check if there is more data by calling get_cx_data again with fresh iter
            iter = NULL;
            pkt = aom_codec_get_cx_data(&ctx, &iter);
            if (pkt == NULL) break;
        }
    }

    // Final flush to ensure encoder is clean
    iter = NULL;
    do {
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        pkt = aom_codec_get_cx_data(&ctx, &iter);
    } while (pkt != NULL);

    // Cleanup
    aom_codec_destroy(&ctx);
    if (img_created) {
        aom_img_free(&img);
    }

    return 0;
}