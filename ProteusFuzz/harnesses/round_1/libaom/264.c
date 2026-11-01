// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
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

static void setup_image_data(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img || !data || size == 0) return;

    img->fmt = AOM_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->bit_depth = 8;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_COLOCATED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;

    // Allocate planes if not already allocated
    if (img->planes[0] == NULL) {
        size_t y_size = img->w * img->h;
        size_t uv_size = (img->w / 2) * (img->h / 2);
        size_t total_size = y_size + 2 * uv_size;

        if (total_size > 0) {
            img->img_data = (unsigned char *)malloc(total_size);
            if (img->img_data) {
                img->img_data_owner = 1;
                img->self_allocd = 1;
                img->planes[0] = img->img_data;
                img->planes[1] = img->img_data + y_size;
                img->planes[2] = img->img_data + y_size + uv_size;
                img->stride[0] = img->w;
                img->stride[1] = img->w / 2;
                img->stride[2] = img->w / 2;
                img->sz = total_size;
                img->bps = 8;

                // Fill with fuzz data or pattern
                if (size >= total_size) {
                    memcpy(img->img_data, data, total_size);
                } else {
                    memset(img->img_data, 0x80, total_size);
                    if (size > 0) {
                        memcpy(img->img_data, data, size);
                    }
                }
            }
        }
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img && img->img_data && img->img_data_owner) {
        free(img->img_data);
        img->img_data = NULL;
        img->img_data_owner = 0;
        img->self_allocd = 0;
        img->planes[0] = NULL;
        img->planes[1] = NULL;
        img->planes[2] = NULL;
    }
}

int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    aom_codec_cx_pkt_t *pkt;
    aom_enc_frame_flags_t flags = 0;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;

    // Initialize image structure
    memset(&img, 0, sizeof(img));

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (Size > 0) cfg.g_w = (Data[0] % 256) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 256) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 255) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 255) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    if (Size > 5) cfg.g_threads = (Data[5] % 4) + 1;
    if (Size > 6) cfg.g_error_resilient = Data[6] % 3;
    if (Size > 7) cfg.g_lag_in_frames = Data[7] % 10;

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control commands (some may fail if not supported or invalid)
    // AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    int gf_max_pyramid = (Size > 8) ? (Data[8] % 4) : 0;
    aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, gf_max_pyramid);

    // AV1E_SET_FORCE_VIDEO_MODE
    int force_video_mode = (Size > 9) ? (Data[9] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);

    // AV1E_SET_MAX_GF_INTERVAL
    int max_gf_interval = (Size > 10) ? (Data[10] % 32) + 1 : 1;
    aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);

    // AOME_SET_CQ_LEVEL
    int cq_level = (Size > 11) ? (Data[11] % 64) : 32;
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);

    // AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    int gf_min_pyramid = (Size > 12) ? (Data[12] % 3) : 0;
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyramid);

    // Prepare image data
    setup_image_data(&img, Data, Size);

    // Encode frames
    while (1) {
        // Fuzz PTS and duration
        if (Size > 13) pts = (int64_t)(Data[13] * 256 + (Size > 14 ? Data[14] : 0));
        if (Size > 15) duration = (unsigned long)(Data[15] % 100) + 1;
        if (Size > 16) flags = (aom_enc_frame_flags_t)(Data[16] % 4);

        // Encode frame
        res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (res != AOM_CODEC_OK) {
            // Continue to flush even on error
        }

        // Get output packets
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // Process packet if needed
            // The packet data is managed by the codec context.
            // We simply consume the packet.
            // Do not call aom_codec_cx_pkt_free as it does not exist.
            // The memory is freed when the context is destroyed or when the packet is no longer needed.
            (void)pkt; // Suppress unused variable warning
        }

        // Flush encoder
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;

        // Check if flushing is complete
        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) {
            break;
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    cleanup_image(&img);

    return 0;
}