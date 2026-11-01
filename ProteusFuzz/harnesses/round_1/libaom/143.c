// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

// Helper to initialize encoder context
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Fuzz configuration values based on input data if size permits
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup encoder context
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Attempt to set some encoder options to vary state before getting headers
    // Using the correct control IDs for AV1 encoder
    if (Size > 0) {
        int cpu_usage = Data[0] % 8;
        aom_codec_control(ctx, AOME_SET_CPUUSED, cpu_usage);
    }
    // AV1E_SET_TUNE is not available in older versions or specific builds, 
    // using a valid alternative or skipping if unsure. 
    // AV1E_SET_ENABLE_TX_SIZE_SEARCH is one of the target functions, let's try to use it safely.
    if (Size > 1) {
        int enable_tx = (Data[1] % 2) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx);
    }
    
    // Try to encode a frame to ensure headers are generated
    // Create a dummy image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  (ctx->config.enc->g_w > 0 ? ctx->config.enc->g_w : 16), 
                  (ctx->config.enc->g_h > 0 ? ctx->config.enc->g_h : 16), 
                  1);
    
    // Fill image with fuzz data
    if (Size > 0) {
        size_t y_size = img.stride[0] * img.h;
        size_t u_size = img.stride[1] * (img.h / 2);
        size_t v_size = img.stride[2] * (img.h / 2);
        
        for (size_t i = 0; i < y_size; i++) {
            img.planes[0][i] = Data[i % Size];
        }
        for (size_t i = 0; i < u_size; i++) {
            img.planes[1][i] = Data[(i + 1) % Size];
        }
        for (size_t i = 0; i < v_size; i++) {
            img.planes[2][i] = Data[(i + 2) % Size];
        }
    }

    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t err = aom_codec_encode(ctx, &img, 0, 1, 0);
    
    if (err == AOM_CODEC_OK) {
        // Try to get global headers
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
        if (headers) {
            // Process the buffer (just check size to avoid infinite loops or crashes)
            if (headers->buf && headers->sz > 0) {
                // Do something with the data to ensure it's executed
                volatile uint8_t *p = headers->buf;
                for (size_t i = 0; i < headers->sz; i++) {
                    (void)p[i];
                }
            }
            // Free the buffer as per documentation
            if (headers->buf) {
                free(headers->buf);
            }
            free(headers);
        }
    }

    // Cleanup
    aom_img_free(&img);
    cleanup_encoder(ctx);

    return 0;
}