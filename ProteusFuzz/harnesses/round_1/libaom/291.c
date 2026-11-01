// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static void free_image(aom_image_t *img) {
    if (img == NULL) return;
    if (img->img_data_owner) {
        aom_img_free(img);
    }
}

static int create_test_image(const uint8_t *data, size_t size, aom_image_t *img, int width, int height, int bit_depth) {
    if (width <= 0 || height <= 0 || bit_depth <= 0 || bit_depth > 16) return 0;
    
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) fmt = AOM_IMG_FMT_I42016;

    if (aom_img_alloc(img, fmt, width, height, 16) == NULL) return 0;

    // Fill image with data if available, otherwise random
    size_t y_size = img->stride[0] * height;
    size_t u_size = img->stride[1] * (height / 2);
    size_t v_size = img->stride[2] * (height / 2);
    size_t total_needed = y_size + u_size + v_size;

    if (size > 0) {
        size_t copy_size = (size < total_needed) ? size : total_needed;
        memcpy(img->planes[0], data, copy_size);
        // Fill remaining planes with 0 or pattern if data is insufficient
        if (copy_size < y_size) memset(img->planes[0] + copy_size, 0, y_size - copy_size);
        if (copy_size < y_size + u_size) {
            size_t u_start = copy_size - y_size;
            size_t u_copy = (copy_size < y_size + u_size) ? (copy_size - y_size) : u_size;
            if (u_copy > 0) memcpy(img->planes[1], data + y_size, u_copy);
            if (u_copy < u_size) memset(img->planes[1] + u_copy, 0, u_size - u_copy);
        }
        if (copy_size < y_size + u_size + v_size) {
            size_t v_start = copy_size - y_size - u_size;
            size_t v_copy = (copy_size < y_size + u_size + v_size) ? (copy_size - y_size - u_size) : v_size;
            if (v_copy > 0) memcpy(img->planes[2], data + y_size + u_size, v_copy);
            if (v_copy < v_size) memset(img->planes[2] + v_copy, 0, v_size - v_copy);
        }
    } else {
        memset(img->planes[0], 0, y_size);
        memset(img->planes[1], 0, u_size);
        memset(img->planes[2], 0, v_size);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_291(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    int frame_count = 0;
    int max_frames = 10; // Limit frames to prevent infinite loops in fuzzer

    // 1. Prepare environment
    // Select AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz config parameters based on input data
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16; // Width 16-1039
        cfg.g_h = (Data[1] % 1024) + 16; // Height 16-1039
        cfg.g_timebase.num = (Data[2] % 255) + 1;
        cfg.g_timebase.den = (Data[3] % 255) + 1;
        cfg.g_threads = (Data[4] % 8) + 1;
        cfg.g_pass = AOM_RC_ONE_PASS; // Fixed: AOM_ONE_PASS -> AOM_RC_ONE_PASS
        cfg.rc_target_bitrate = (Data[5] % 1000) + 100;
        cfg.g_bit_depth = (Data[6] % 4) + 8; // 8, 9, 10, 11
        // cfg.g_color_range removed as it's not a member of aom_codec_enc_cfg in this version
        cfg.g_profile = (Data[8] % 4);
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_pass = AOM_RC_ONE_PASS; // Fixed
        cfg.rc_target_bitrate = 500;
        cfg.g_bit_depth = 8;
        // cfg.g_color_range removed
        cfg.g_profile = 0;
    }

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // 2. Execute target function logic
    // We will try to encode frames and then flush
    // Use input data to determine how many frames to encode and their properties
    
    int num_frames = (Size > 0) ? (Data[0] % 5) : 1;
    if (num_frames == 0) num_frames = 1;
    
    // Ensure we don't exceed max_frames
    if (num_frames > max_frames) num_frames = max_frames;

    for (int i = 0; i < num_frames; i++) {
        // Create image
        int w = cfg.g_w;
        int h = cfg.g_h;
        int bd = cfg.g_bit_depth;
        
        if (!create_test_image(Data, Size, &img, w, h, bd)) {
            // If image creation fails, try with smaller dimensions or default
            if (!create_test_image(Data, Size, &img, 64, 64, 8)) {
                break; 
            }
        }

        // Set PTS and duration
        pts = i * 100; // Strictly increasing
        duration = 100; // Non-zero

        // Fuzz flags
        aom_enc_frame_flags_t flags = 0;
        if (Size > 10) flags = Data[10];

        // Call target API: aom_codec_encode
        res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        
        // Handle result
        if (res != AOM_CODEC_OK) {
            // If error, we might still want to flush or stop
            // For fuzzing, we continue to see if recovery is possible or if it crashes
        }

        free_image(&img);
        frame_count++;
    }

    // 3. Flush the encoder (img = NULL)
    // Call aom_codec_encode with img=NULL until no output remains
    while (frame_count < max_frames * 2) { // Safety limit
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;

        // Check for output packets
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Consume packet to prevent memory leaks
            // Fixed: aom_codec_cx_pkt_free is not a public API in some versions, 
            // but aom_codec_get_cx_data returns a pointer to internal data.
            // We must not free it manually if it's managed by the context.
            // However, the standard pattern is to just consume it. 
            // If the API requires freeing, it's usually aom_codec_cx_pkt_free.
            // If that function is missing, we assume the context manages it or it's a read-only view.
            // In newer libaom, the packet data is owned by the context until destroy.
            // We just advance the iterator implicitly by calling get_cx_data again.
            // No explicit free needed for the packet struct itself in standard usage.
        }

        // If no more data, we are done flushing
        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) break;
        
        frame_count++;
    }

    // 4. Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}