// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static int create_test_image(aom_image_t *img, int width, int height, int bit_depth) {
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) fmt = AOM_IMG_FMT_I42016;

    if (aom_img_alloc(img, fmt, width, height, 32) != 0) return 1;
    img->bit_depth = bit_depth;
    
    // Fill with deterministic data based on width/height to ensure coverage
    for (int p = 0; p < 3; p++) {
        int plane_h = (p == 0) ? height : (height + 1) / 2;
        int plane_w = (p == 0) ? width : (width + 1) / 2;
        for (int i = 0; i < plane_h; i++) {
            for (int j = 0; j < plane_w; j++) {
                unsigned int val = (i * j + width + height) % (1 << bit_depth);
                if (bit_depth > 8) {
                    ((uint16_t*)img->planes[p])[i * img->stride[p] + j] = val;
                } else {
                    img->planes[p][i * img->stride[p] + j] = (uint8_t)val;
                }
            }
        }
    }
    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t res;

    // Parse input for dimensions and bit depth
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    int bit_depth = 8;
    if (Size > 4) bit_depth = (Data[4] % 5) + 8; // 8, 10, 12, 14, 16

    // Sanity checks for dimensions
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64; height = 64;
    }
    if (width % 2 != 0) width++;
    if (height % 2 != 0) height++;

    if (init_encoder(&ctx, &cfg, width, height) != 0) return 0;

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    int val = (Size > 5) ? Data[5] : 0;
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    val = (Size > 6) ? Data[6] : 0;
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);

    // Fuzz AV1E_GET_TARGET_SEQ_LEVEL_IDX
    int level_idx;
    aom_codec_control(&ctx, AV1E_GET_TARGET_SEQ_LEVEL_IDX, &level_idx);

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    val = (Size > 7) ? (Data[7] % 7) : 0; // Limit to valid range roughly
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val);

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    val = (Size > 8) ? (Data[8] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);

    // Create image and encode
    if (create_test_image(&img, width, height, bit_depth) == 0) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = (Size > 9) ? Data[9] : 1;
        if (duration == 0) duration = 1;
        if (duration > UINT32_MAX) duration = UINT32_MAX;

        // Encode a frame
        res = aom_codec_encode(&ctx, &img, pts, duration, 0);
        if (res == AOM_CODEC_OK) {
            // Drain the encoder
            while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                // Consume packet
                // No explicit free needed for the packet structure itself, 
                // aom_codec_get_cx_data handles the lifecycle or it's read-only.
                // If it contains a buffer that needs freeing, it's usually handled internally
                // or via aom_codec_cx_pkt_data_free if available in newer versions, 
                // but standard practice is just to read and discard.
                // In older libaom versions, we just ignore the data.
            }
        }

        // Flush encoder
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        while (res == AOM_CODEC_OK) {
            while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                // Consume packet
            }
            res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        }
    }

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}