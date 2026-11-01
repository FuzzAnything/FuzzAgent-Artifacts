// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    // Use fuzzing data to randomize configuration within safe bounds
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    if (size > 5) cfg.g_threads = (data[5] % 8) + 1;
    if (size > 6) cfg.g_error_resilient = (data[6] % 2);

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Prepare a dummy frame to ensure encoder is in a valid state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, ctx->config.enc->g_w, ctx->config.enc->g_h, 1);
    
    // Fill image with fuzzing data if possible
    if (Size > 0 && img.planes[0]) {
        size_t plane_size = img.stride[0] * img.d_h;
        size_t copy_size = (Size < plane_size) ? Size : plane_size;
        memcpy(img.planes[0], Data, copy_size);
    }

    // Encode the frame to ensure the encoder is active and configured
    // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    // Call target API: aom_codec_get_global_headers
    // This should be called after configuration is finalized
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
    if (headers) {
        // Fuzzing does not need to process the data, just ensure it's freed
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}