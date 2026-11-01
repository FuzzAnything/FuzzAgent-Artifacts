// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

/*
 * Helper function to create a valid image from fuzzing data.
 * Returns NULL if data is insufficient.
 */
static aom_image_t *create_fuzz_image(const uint8_t *data, size_t size, int width, int height) {
    if (width <= 0 || height <= 0) return NULL;
    if (size < (size_t)width * height * 2) return NULL; // Minimum for Y + UV

    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;

    // Fill Y plane
    for (int y = 0; y < height; y++) {
        size_t row_len = (size_t)width;
        size_t offset = (size_t)y * width;
        if (offset + row_len > size) break;
        memcpy(img->planes[0] + y * img->stride[0], data + offset, row_len);
    }

    // Fill U and V planes (scaled down)
    int uv_h = (height + 1) / 2;
    int uv_w = (width + 1) / 2;
    for (int y = 0; y < uv_h; y++) {
        size_t row_len = (size_t)uv_w;
        size_t offset = (size_t)width * height + (size_t)y * uv_w;
        if (offset + row_len > size) break;
        memcpy(img->planes[1] + y * img->stride[1], data + offset, row_len);
        memcpy(img->planes[2] + y * img->stride[2], data + offset, row_len);
    }

    return img;
}

/*
 * Helper to initialize encoder context
 */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    if (aom_codec_enc_init(ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper to initialize decoder context
 */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    if (aom_codec_dec_init(ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Cleanup helper function
 */
static void cleanup_context(aom_codec_ctx_t **ctx_ptr, aom_image_t **img_ptr) {
    if (ctx_ptr && *ctx_ptr) {
        aom_codec_destroy(*ctx_ptr);
        free(*ctx_ptr);
        *ctx_ptr = NULL;
    }
    if (img_ptr && *img_ptr) {
        aom_img_free(*img_ptr);
        *img_ptr = NULL;
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Parse simple parameters from data to vary test scenarios
    int width = (Data[0] % 64) + 16;
    int height = (Data[1] % 64) + 16;
    int mode = Data[2] % 3; // 0: Encoder, 1: Decoder, 2: Mixed/Control

    aom_codec_ctx_t *ctx = NULL;
    aom_image_t *img = NULL;
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    aom_image_t *dec_img = NULL;
    aom_codec_err_t res;

    if (mode == 0) {
        // Encoder Path
        ctx = init_encoder(Data, Size);
        if (!ctx) return 0;

        img = create_fuzz_image(Data, Size, width, height);
        if (!img) {
            cleanup_context(&ctx, &img);
            return 0;
        }

        // Encode a frame
        aom_codec_pts_t pts = (aom_codec_pts_t)(Data[3] * 256 + Data[4]);
        unsigned long duration = (unsigned long)(Data[5] + 1);
        aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)(Data[6]);

        res = aom_codec_encode(ctx, img, pts, duration, flags);
        
        if (res == AOM_CODEC_OK) {
            // Iterate encoded packets
            while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Process frame packet
                    if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                        // Just touch the data to ensure it's valid
                        volatile uint8_t x = *(uint8_t*)pkt->data.frame.buf;
                        (void)x;
                    }
                } else if (pkt->kind == AOM_CODEC_STATS_PKT) {
                    // Process stats
                }
                // Ignore other types as per spec
            }
        }

        // Flush encoder
        while (aom_codec_encode(ctx, NULL, 0, 0, 0) == AOM_CODEC_OK) {
            while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
                // Consume flush packets
            }
        }

        cleanup_context(&ctx, &img);

    } else if (mode == 1) {
        // Decoder Path
        ctx = init_decoder(Data, Size);
        if (!ctx) return 0;

        // Decode the data as a stream (assuming it's valid AV1 or garbage)
        const uint8_t *data_ptr = Data;
        size_t data_size = Size;
        
        // aom_codec_decode signature: aom_codec_decode(ctx, data, data_sz, user_priv)
        res = aom_codec_decode(ctx, data_ptr, data_size, NULL);

        if (res == AOM_CODEC_OK) {
            while ((dec_img = aom_codec_get_frame(ctx, &iter)) != NULL) {
                // Process decoded frame
                if (dec_img && dec_img->planes[0]) {
                    volatile uint8_t x = dec_img->planes[0][0];
                    (void)x;
                }
            }
        }

        cleanup_context(&ctx, NULL);

    } else {
        // Mixed / Control Path
        ctx = init_encoder(Data, Size);
        if (!ctx) return 0;

        // Just do a minimal encode/decode cycle to exercise the context
        img = create_fuzz_image(Data, Size, width, height);
        if (img) {
            res = aom_codec_encode(ctx, img, 0, 1, 0);
            if (res == AOM_CODEC_OK) {
                aom_codec_get_cx_data(ctx, &iter);
            }
            aom_img_free(img);
            img = NULL;
        }
        
        cleanup_context(&ctx, &img);
    }

    return 0;
}