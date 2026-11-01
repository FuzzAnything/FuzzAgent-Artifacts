// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_ctx(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

static vpx_codec_ctx_t* create_encoder_ctx(const vpx_codec_iface_t *iface, vpx_codec_flags_t flags) {
    vpx_codec_ctx_t ctx_local;
    vpx_codec_ctx_t *ctx = &ctx_local;
    
    // Initialize the context structure to zero to ensure safety
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    if (vpx_codec_enc_init_ver(ctx, iface, NULL, flags, VPX_CODEC_ABI_VERSION) != VPX_CODEC_OK) {
        return NULL;
    }
    return ctx;
}

static vpx_image_t* create_test_image(unsigned int w, unsigned int h, vpx_img_fmt_t fmt) {
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, w, h, 16);
    if (!img) return NULL;

    // Fill planes with deterministic data based on coordinates
    for (int p = 0; p < 4; p++) {
        if (img->planes[p]) {
            int stride = img->stride[p];
            int w_p = (p == 0) ? w : (w + 1) / 2;
            int h_p = (p == 0) ? h : (h + 1) / 2;
            for (int y = 0; y < h_p; y++) {
                for (int x = 0; x < w_p; x++) {
                    img->planes[p][y * stride + x] = (x + y) & 0xFF;
                }
            }
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Parse input to determine behavior
    uint32_t seed = *(uint32_t *)Data;
    Data += 4;
    Size -= 4;

    // Select codec interface based on seed (Only VP8 is guaranteed available with provided headers)
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) return 0;

    vpx_codec_ctx_t *ctx = create_encoder_ctx(iface, 0);
    if (!ctx) return 0;

    // Initialize configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_ctx(ctx);
        return 0;
    }

    // Fuzz configuration parameters
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_threads = (Data[2] % 8) + 1;
    cfg.g_profile = (Data[3] % 4);
    cfg.g_bit_depth = VPX_BITS_8; // Fixed to 8 for VP8 compatibility in this context
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[5] % 60) + 1;
    cfg.rc_target_bitrate = (Data[6] % 1000) + 100;
    cfg.rc_min_quantizer = (Data[7] % 64);
    cfg.rc_max_quantizer = (Data[8] % 64) + 10;
    cfg.kf_mode = (Data[9] % 3); // VPX_KF_FIXED, AUTO, DISABLED
    cfg.kf_min_dist = (Data[10] % 100) + 1;
    cfg.kf_max_dist = (Data[11] % 100) + 1;

    // Apply configuration
    if (vpx_codec_enc_config_set(ctx, &cfg) != VPX_CODEC_OK) {
        cleanup_ctx(ctx);
        return 0;
    }

    // Initialize encoder
    if (vpx_codec_enc_init_ver(ctx, iface, &cfg, 0, VPX_CODEC_ABI_VERSION) != VPX_CODEC_OK) {
        cleanup_ctx(ctx);
        return 0;
    }

    // Create test image
    vpx_image_t *img = create_test_image(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420);
    if (!img) {
        cleanup_ctx(ctx);
        return 0;
    }

    // Fuzz encoding loop
    int frame_count = (Data[12] % 10) + 1;
    int64_t pts = 0;
    unsigned long duration = (Data[13] % 100) + 1;
    vpx_enc_frame_flags_t flags = (Data[14] % 4);
    vpx_enc_deadline_t deadline = (Data[15] % 3 == 0) ? 0 : (Data[15] % 100000) + 1000;

    for (int i = 0; i < frame_count; i++) {
        // Fuzz image content slightly per frame
        if (img->planes[0]) {
            img->planes[0][0] = Data[16 + (i % 10)];
        }

        vpx_codec_err_t res = vpx_codec_encode(ctx, img, pts, duration, flags, deadline);
        if (res != VPX_CODEC_OK) {
            // Check for expected errors, continue to flush
        }

        // Collect output
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(ctx, NULL)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Process packet if needed
            }
            // vpx_codec_free_cx_data is not a valid API in standard libvpx.
            // The memory is managed by the codec context.
            // We simply iterate to drain the queue.
        }

        pts += duration;
    }

    // Flush encoder
    vpx_codec_encode(ctx, NULL, pts, duration, 0, 0);
    while (vpx_codec_get_cx_data(ctx, NULL) != NULL) {
        // Drain
    }

    // Test vpx_codec_set_cx_data_buf
    if (Size > 20) {
        size_t buf_size = (Data[20] % 1024) + 1024;
        uint8_t *custom_buf = (uint8_t *)malloc(buf_size);
        if (custom_buf) {
            vpx_fixed_buf_t buf = { .buf = custom_buf, .sz = buf_size };
            unsigned int pad_before = Data[21] % 16;
            unsigned int pad_after = Data[22] % 16;
            vpx_codec_set_cx_data_buf(ctx, &buf, pad_before, pad_after);

            // Encode one more frame with custom buffer
            vpx_codec_encode(ctx, img, pts + duration, duration, 0, 0);
            while (vpx_codec_get_cx_data(ctx, NULL) != NULL) {
                // Drain
            }

            // Restore default
            vpx_codec_set_cx_data_buf(ctx, NULL, 0, 0);
            free(custom_buf);
        }
    }

    // Cleanup
    vpx_img_free(img);
    cleanup_ctx(ctx);

    return 0;
}