// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void free_image(aom_image_t *img) {
    if (img && img->img_data) {
        aom_img_free(img);
    }
}

static void free_codec_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void free_global_headers(aom_fixed_buf_t *buf) {
    if (buf) {
        if (buf->buf) {
            free(buf->buf);
        }
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Parse input for configuration
    unsigned int width = (Data[0] % 64) + 16;
    unsigned int height = (Data[1] % 64) + 16;
    unsigned int bit_depth = (Data[2] % 3) == 0 ? 8 : ((Data[2] % 3) == 1 ? 10 : 12);
    unsigned int threads = (Data[3] % 4) + 1;
    unsigned int target_bitrate = (Data[4] * 256 + Data[5]) + 100;
    unsigned int pad_before = Data[6];
    unsigned int pad_after = Data[7];
    unsigned int duration = (Data[8] % 100) + 1;
    unsigned int flags = Data[9];
    
    // Ensure alignment for width/height if necessary for AV1
    if (width % 2 != 0) width++;
    if (height % 2 != 0) height++;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free_codec_ctx(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free_codec_ctx(ctx);
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.g_bit_depth = (aom_bit_depth_t)bit_depth;
    cfg.g_input_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = target_bitrate;
    cfg.rc_end_usage = AOM_VBR;
    cfg.g_pass = AOM_RC_ONE_PASS;

    aom_codec_err_t ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free_codec_ctx(ctx);
        return 0;
    }

    // Fuzz aom_codec_set_option
    // Use a subset of data as key/value strings if possible, or fixed strings
    if (Size > 10) {
        char key[64], val[64];
        size_t k_len = Data[10] % 20;
        size_t v_len = Data[11] % 20;
        if (k_len > 0 && v_len > 0) {
            if (k_len > sizeof(key) - 1) k_len = sizeof(key) - 1;
            if (v_len > sizeof(val) - 1) v_len = sizeof(val) - 1;
            memcpy(key, Data + 12, k_len);
            key[k_len] = '\0';
            memcpy(val, Data + 12 + k_len, v_len);
            val[v_len] = '\0';
            aom_codec_set_option(ctx, key, val);
        }
    }

    // Fuzz aom_codec_enc_config_set
    aom_codec_enc_cfg_t new_cfg;
    memcpy(&new_cfg, &cfg, sizeof(new_cfg));
    new_cfg.g_w = (Data[12] % 128) + 16;
    new_cfg.g_h = (Data[13] % 128) + 16;
    new_cfg.g_threads = (Data[14] % 8) + 1;
    aom_codec_enc_config_set(ctx, &new_cfg);

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *global_hdr = aom_codec_get_global_headers(ctx);
    if (global_hdr) {
        // Do nothing with it, just leak check handled by cleanup
        free_global_headers(global_hdr);
    }

    // Prepare custom buffer for aom_codec_set_cx_data_buf
    unsigned int buf_size = (Size > 15) ? (Data[15] * 256 + Data[16]) : 1024;
    if (buf_size > 1024 * 1024) buf_size = 1024 * 1024; // Cap size
    unsigned char *custom_buf = (unsigned char *)malloc(buf_size);
    aom_fixed_buf_t cx_buf = { .buf = custom_buf, .sz = buf_size };

    if (custom_buf) {
        aom_codec_set_cx_data_buf(ctx, &cx_buf, pad_before, pad_after);
    }

    // Fuzz aom_codec_encode
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    if (!img.planes[0]) {
        free_codec_ctx(ctx);
        free(custom_buf);
        return 0;
    }
    img.bit_depth = bit_depth;

    // Fill image with fuzz data
    size_t plane_size = img.stride[0] * height;
    size_t total_size = 0;
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            total_size += img.stride[i] * img.h;
        }
    }

    if (Size > 16) {
        size_t data_offset = 16;
        size_t data_len = Size - data_offset;
        if (data_len > total_size) data_len = total_size;
        // Simple fill pattern
        for (size_t i = 0; i < data_len; i++) {
            // Write to Y plane primarily, wrap around if needed
            if (img.planes[0]) img.planes[0][i % plane_size] = Data[data_offset + i];
        }
    }

    aom_codec_pts_t pts = 0;
    int flush_count = 0;
    int max_iterations = 5; // Limit iterations to prevent infinite loops in fuzzer

    while (flush_count < max_iterations) {
        ret = aom_codec_encode(ctx, &img, pts, duration, flags);
        if (ret != AOM_CODEC_OK) {
            // Check error
            aom_codec_error(ctx);
            break;
        }

        // Consume output packets
        aom_codec_cx_pkt_t pkt;
        while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                // Packet consumed
            }
        }

        if (img.planes[0]) {
            // Prepare next frame
            pts += duration;
            // Modify image slightly for next frame
            if (img.planes[0]) {
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        img.planes[0][y * img.stride[0] + x] = (img.planes[0][y * img.stride[0] + x] + 1) % (1 << bit_depth);
                    }
                }
            }
        } else {
            // Flush
            ret = aom_codec_encode(ctx, NULL, 0, 0, 0);
            if (ret != AOM_CODEC_OK) break;
            
            aom_codec_cx_pkt_t pkt_flush;
            while (aom_codec_get_cx_data(ctx, &pkt_flush) == AOM_CODEC_OK) {
                // Consume flush packets
            }
            flush_count++;
            if (flush_count >= max_iterations) break;
        }
    }

    // Cleanup
    free_image(&img);
    free(custom_buf);
    free_codec_ctx(ctx);

    return 0;
}