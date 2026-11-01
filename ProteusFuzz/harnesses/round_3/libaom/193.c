// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize an encoder context with fuzzed configuration.
 * Returns 0 on success, non-zero on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                                const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (!ctx || !cfg) return 1;

    // Initialize config with defaults
    if (aom_codec_enc_config_default("av1", cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    // Fuzz configuration parameters based on input data
    if (size > 0) {
        cfg->g_w = (data[0] % 4096) + 16; // Width: 16-4111
        cfg->g_h = (data[1] % 4096) + 16; // Height: 16-4111
        cfg->g_timebase.num = (data[2] % 100) + 1;
        cfg->g_timebase.den = (data[3] % 100) + 1;
        cfg->g_threads = (data[4] % 16) + 1;
        cfg->rc_target_bitrate = (data[5] % 10000) + 100;
        cfg->g_error_resilient = (data[6] % 2);
        cfg->g_lag_in_frames = (data[7] % 30) + 1;
        
        // Use valid fields for AV1 encoder config
        cfg->kf_mode = (data[8] % 3);
        cfg->kf_max_dist = (data[9] % 200) + 1;
        cfg->kf_min_dist = (data[10] % 100);
        
        // Use valid rate control fields
        cfg->rc_min_quantizer = (data[11] % 64);
        cfg->rc_max_quantizer = (data[12] % 64) + cfg->rc_min_quantizer;
        cfg->rc_undershoot_pct = (data[13] % 100);
        cfg->rc_overshoot_pct = (data[14] % 100);
    }

    // Ensure alignment
    cfg->g_w = (cfg->g_w + 15) & ~15;
    cfg->g_h = (cfg->g_h + 15) & ~15;

    iface = aom_codec_av1_cx();
    if (!iface) return 1;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_fixed_buf_t *global_headers = NULL;
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Initialize encoder
    if (init_encoder_context(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    // Create a dummy image for encoding
    // aom_img_alloc returns 0 on success
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    // img.planes[i] is a pointer to uint8_t (the plane data)
    // img.planes[i] is not a struct, so we access width/height from img directly or use stride
    // The width and height of the image are stored in img.d_w and img.d_h (or w/h in newer versions)
    // Actually, aom_image_t has .d_w and .d_h for display width/height, and .w and .h for allocation
    // But usually we use .w and .h. Let's check the struct definition in aom_image.h
    // struct aom_image { ... uint16_t w, h; ... uint8_t *planes[4]; ... }
    // So img.w and img.h are the dimensions.
    // img.planes[0] is uint8_t*. To access [i][j], we need to know the stride.
    // aom_image_t has .stride[4] which holds the stride for each plane.
    
    uint16_t width = img.w;
    uint16_t height = img.h;
    int stride = img.stride[0];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            img.planes[0][i * stride + j] = Data[(i * width + j) % Size];
        }
    }

    // Encode one frame
    // Signature: aom_codec_err_t aom_codec_encode(aom_codec_ctx_t *ctx, const aom_image_t *img,
    //                                            aom_codec_pts_t pts, unsigned long duration,
    //                                            aom_enc_frame_flags_t flags);
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Try to get global headers
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers != NULL) {
        // Process the buffer (just read size to ensure it's valid)
        size_t sz = global_headers->sz;
        (void)sz; // Suppress unused warning
        free(global_headers->buf);
        free(global_headers);
    }

    // Collect packets
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // Process packet if needed
        aom_codec_destroy(&ctx); // Just to explore state
        break;
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}