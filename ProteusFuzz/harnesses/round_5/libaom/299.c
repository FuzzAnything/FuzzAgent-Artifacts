// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static int fuzz_encode_frame(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    aom_codec_cx_pkt_t *pkt;
    int frame_idx = 0;
    int flush_count = 0;
    int max_flush = 10;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default("av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Parse input to configure encoder
    if (size >= 20) {
        cfg.g_w = *(uint32_t *)(data + 0) % 4096;
        cfg.g_h = *(uint32_t *)(data + 4) % 4096;
        cfg.g_timebase.num = *(uint32_t *)(data + 8) % 1000 + 1;
        cfg.g_timebase.den = *(uint32_t *)(data + 12) % 1000 + 1;
        cfg.g_bit_depth = 8 + (*(uint8_t *)(data + 16) % 4); // 8, 10, 12, 14
        cfg.g_threads = *(uint8_t *)(data + 17) % 16 + 1;
        cfg.rc_target_bitrate = *(uint32_t *)(data + 18) % 1000000 + 100;
        cfg.g_error_resilient = *(uint8_t *)(data + 22) % 2;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_bit_depth = 8;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    }

    // Ensure dimensions are multiples of 2
    cfg.g_w = (cfg.g_w / 2) * 2;
    cfg.g_h = (cfg.g_h / 2) * 2;
    if (cfg.g_w < 2 || cfg.g_h < 2) {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Allocate image
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) == NULL) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz encoding loop
    size_t offset = 0;
    while (offset < size && frame_idx < 50) {
        // Fill image planes with fuzz data
        size_t plane_size = (size_t)img.planes[0][0] * (size_t)img.stride[0];
        if (offset + plane_size <= size) {
            memcpy(img.planes[0], data + offset, plane_size);
            memcpy(img.planes[1], data + offset + plane_size, plane_size / 4);
            memcpy(img.planes[2], data + offset + plane_size * 5 / 4, plane_size / 4);
            offset += plane_size * 7 / 4;
        } else {
            // Fill with pattern if not enough data
            memset(img.planes[0], frame_idx & 0xFF, img.w * img.h);
            memset(img.planes[1], (frame_idx >> 8) & 0xFF, img.w * img.h / 4);
            memset(img.planes[2], (frame_idx >> 16) & 0xFF, img.w * img.h / 4);
            offset = size; // Force exit after this frame
        }

        aom_codec_pts_t pts = frame_idx * 100;
        unsigned long duration = 100;
        aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)(frame_idx % 4);

        res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (res != AOM_CODEC_OK) {
            // Continue to flush even on error
        }

        // Collect output
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            aom_codec_destroy(&ctx); // Just to free memory quickly in fuzz
            aom_img_free(&img);
            return 0;
        }

        frame_idx++;
    }

    // Flush encoder
    while (flush_count < max_flush) {
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) {
            // Might be expected if no more data
        }

        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) {
            break;
        }
        flush_count++;
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Try different encoding scenarios
    fuzz_encode_frame(Data, Size);

    return 0;
}