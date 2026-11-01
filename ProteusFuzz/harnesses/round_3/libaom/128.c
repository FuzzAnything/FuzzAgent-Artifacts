// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

// Helper to create a dummy image from fuzz input
static aom_image_t* create_dummy_image(const uint8_t *data, size_t size, int width, int height, int bit_depth) {
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth == 10 || bit_depth == 12) {
        fmt = AOM_IMG_FMT_I42016;
    }

    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;

    // Fill planes with fuzz data if available
    size_t plane_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    
    if (size >= plane_size) {
        memcpy(img->planes[AOM_PLANE_Y], data, plane_size);
    } else {
        memset(img->planes[AOM_PLANE_Y], 0, plane_size);
    }

    if (size >= plane_size + uv_size) {
        memcpy(img->planes[AOM_PLANE_U], data + plane_size, uv_size);
        memcpy(img->planes[AOM_PLANE_V], data + plane_size + uv_size, uv_size);
    } else {
        memset(img->planes[AOM_PLANE_U], 0, uv_size);
        memset(img->planes[AOM_PLANE_V], 0, uv_size);
    }

    return img;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input
    int width = (Data[0] % 16) + 16; // Min 16
    int height = (Data[1] % 16) + 16; // Min 16
    int bit_depth = (Data[2] % 3) == 0 ? 8 : (Data[2] % 2 == 0 ? 10 : 12);
    int num_frames = (Data[3] % 10) + 1;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    // Use aom_codec_av1_cx() to get the AV1 codec interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t *img = NULL;
    aom_codec_iter_t iter = NULL;
    int64_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    // Fuzz encoding loop
    for (int i = 0; i < num_frames; i++) {
        // Create image from fuzz data
        img = create_dummy_image(Data, Size, width, height, bit_depth);
        if (!img) {
            // Try encoding with NULL to flush if image creation fails
            if (aom_codec_encode(&ctx, NULL, pts, duration, flags) != AOM_CODEC_OK) {
                break;
            }
            continue;
        }

        // Encode frame
        if (aom_codec_encode(&ctx, img, pts, duration, flags) != AOM_CODEC_OK) {
            aom_img_free(img);
            break;
        }

        pts += duration;
        aom_img_free(img);
        img = NULL;
    }

    // Flush encoder
    while (aom_codec_encode(&ctx, NULL, pts, duration, flags) == AOM_CODEC_OK) {
        // Drain output
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Ignore unsupported packet types as per documentation
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Valid frame packet, do nothing (data valid until next call)
            } else if (pkt->kind == AOM_CODEC_STATS_PKT) {
                // Stats packet, ignore
            } else if (pkt->kind == AOM_CODEC_PSNR_PKT) {
                // PSNR packet, ignore
            }
            // Other types are silently ignored
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}