// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>

static void init_encoder_context(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg) {
    // Initialize config with defaults
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), cfg, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        return;
    }

    // Apply constraints from fuzzing logic later, but ensure basic validity
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    // Initialize the codec context
    if (vpx_codec_enc_init(ctx, vpx_codec_vp8_cx(), cfg, 0) != VPX_CODEC_OK) {
        return;
    }
}

static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 30) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    init_encoder_context(&ctx, &cfg);
    if (ctx.err != VPX_CODEC_OK) {
        return 0;
    }

    // Extract parameters from fuzz data
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    uint32_t align = (Data[4] << 8) | Data[5];
    uint32_t x_rect = Data[6];
    uint32_t y_rect = Data[7];
    uint32_t w_rect = (Data[8] << 8) | Data[9];
    uint32_t h_rect = (Data[10] << 8) | Data[11];
    uint8_t fmt_idx = Data[12] % 10; // Limit to known formats
    uint64_t pts = ((uint64_t)Data[13] << 40) | ((uint64_t)Data[14] << 32) | 
                   ((uint64_t)Data[15] << 24) | ((uint64_t)Data[16] << 16) | 
                   ((uint64_t)Data[17] << 8) | Data[18];
    uint64_t duration = Data[19];
    uint64_t deadline = (Data[20] << 16) | (Data[21] << 8) | Data[22];
    uint32_t flags = (Data[23] << 24) | (Data[24] << 16) | (Data[25] << 8) | Data[26];

    // Constrain dimensions to valid ranges
    if (width == 0) width = 64;
    if (height == 0) height = 64;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align == 0) align = 1;
    if (align > 65536) align = 65536;

    // Map format index to enum
    vpx_img_fmt_t fmts[] = {
        VPX_IMG_FMT_YV12, VPX_IMG_FMT_I420, VPX_IMG_FMT_I422, VPX_IMG_FMT_I444,
        VPX_IMG_FMT_I440, VPX_IMG_FMT_NV12, VPX_IMG_FMT_I42016, VPX_IMG_FMT_I42216,
        VPX_IMG_FMT_I44416, VPX_IMG_FMT_I44016
    };
    vpx_img_fmt_t fmt = fmts[fmt_idx];

    vpx_image_t *img = NULL;
    unsigned char *img_data = NULL;

    // Test vpx_img_alloc
    img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (img) {
        // Fill with fuzz data if possible
        if (Size > 27) {
            size_t data_len = Size - 27;
            // Safe copy to avoid overflow, just fill first plane if exists
            if (img->planes[0]) {
                size_t plane_size = (size_t)img->h * (size_t)img->stride[0];
                if (plane_size > 0 && data_len > 0) {
                    size_t copy_len = (plane_size < data_len) ? plane_size : data_len;
                    memcpy(img->planes[0], Data + 27, copy_len);
                }
            }
        }

        // Test vpx_img_set_rect
        // Ensure rect is within bounds
        if (x_rect < width && y_rect < height && w_rect <= (width - x_rect) && h_rect <= (height - y_rect)) {
            vpx_img_set_rect(img, x_rect, y_rect, w_rect, h_rect);
        }

        // Test vpx_img_flip
        vpx_img_flip(img);

        // Test vpx_codec_encode with valid image
        vpx_codec_encode(&ctx, img, (vpx_codec_pts_t)pts, (unsigned long)duration, 
                         (vpx_enc_frame_flags_t)flags, (vpx_enc_deadline_t)deadline);

        // Retrieve and discard output data to prevent memory leak in some contexts
        vpx_codec_cx_pkt_t pkt;
        while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
            if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
                // Data is managed by libvpx, no need to free manually here usually, 
                // but we just consume it.
            }
        }

        vpx_img_free(img);
    }

    // Test vpx_img_wrap with external buffer
    if (Size > 30) {
        // Calculate minimum buffer size roughly (simplified for fuzzing)
        // We allocate a safe buffer size based on dimensions
        size_t buffer_size = (size_t)width * (size_t)height * 2; // Rough estimate for YUV
        if (buffer_size > 0 && buffer_size < SIZE_MAX / 2) {
            img_data = (unsigned char *)malloc(buffer_size);
            if (img_data) {
                if (Size > 30) {
                    size_t fill_size = (Size - 30 < buffer_size) ? (Size - 30) : buffer_size;
                    memcpy(img_data, Data + 30, fill_size);
                }

                vpx_image_t img_wrap;
                memset(&img_wrap, 0, sizeof(img_wrap));
                
                // Wrap the buffer
                vpx_img_wrap(&img_wrap, fmt, width, height, align, img_data);
                
                // Test vpx_img_set_rect on wrapped image
                if (x_rect < width && y_rect < height && w_rect <= (width - x_rect) && h_rect <= (height - y_rect)) {
                    vpx_img_set_rect(&img_wrap, x_rect, y_rect, w_rect, h_rect);
                }

                // Test vpx_img_flip on wrapped image
                vpx_img_flip(&img_wrap);

                // Test vpx_codec_encode with wrapped image
                vpx_codec_encode(&ctx, &img_wrap, (vpx_codec_pts_t)pts, (unsigned long)duration, 
                                 (vpx_enc_frame_flags_t)flags, (vpx_enc_deadline_t)deadline);
                
                // Flush output
                vpx_codec_cx_pkt_t pkt;
                while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
                    // Consume
                }

                // Free wrapped image (only frees descriptor, not img_data)
                vpx_img_free(&img_wrap);
                free(img_data);
            }
        }
    }

    // Flush the encoder with NULL image
    vpx_codec_encode(&ctx, NULL, (vpx_codec_pts_t)pts, (unsigned long)duration, 
                     (vpx_enc_frame_flags_t)flags, (vpx_enc_deadline_t)deadline);
    
    // Drain remaining packets
    vpx_codec_cx_pkt_t pkt;
    while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
        // Consume
    }

    cleanup_encoder_context(&ctx);
    return 0;
}