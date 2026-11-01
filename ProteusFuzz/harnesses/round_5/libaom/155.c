// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * 1. AV1E_SET_MIN_PARTITION_SIZE
 * 2. AV1E_SET_REDUCED_TX_TYPE_SET
 * 3. AV1E_SET_ENABLE_OVERLAY
 * 4. AV1E_SET_ENABLE_ANGLE_DELTA
 * 5. AV1E_SET_ENABLE_1TO4_PARTITIONS
 * 6. AV1E_SET_ENABLE_DNL_DENOISING
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        int width, int height, int fps_num, int fps_den) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (!ctx || !cfg) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = fps_num;
    cfg->g_timebase.den = fps_den;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_end_usage = AOM_CBR;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int width, height, fps_num, fps_den;
    int val;
    aom_codec_err_t ret;

    if (Size < 16) return 0;

    // Parse input data to generate test parameters
    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    fps_num = Data[4] + 1;
    fps_den = Data[5] + 1;
    val = (int)(Data[6]); // Value for boolean/integer settings

    // Ensure valid dimensions
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    // Initialize encoder
    if (!init_encoder(&ctx, &cfg, width, height, fps_num, fps_den)) {
        return 0;
    }

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    // Takes an integer (partition size)
    int min_part_size = Data[7];
    ret = aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part_size);
    (void)ret; // Ignore return for fuzzing

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    // Takes an integer (flag)
    int reduced_tx = Data[8];
    ret = aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    // Takes an integer (flag)
    int enable_overlay = Data[9];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    // Takes an integer (flag)
    int enable_angle_delta = Data[10];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, enable_angle_delta);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    // Takes an integer (flag)
    int enable_1to4 = Data[11];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, enable_1to4);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    // Takes an integer (flag)
    int enable_dnl = Data[12];
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_dnl);
    (void)ret;

    // Try to encode a frame to ensure state consistency
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill with fuzz data
    memset(img.planes[0], Data[13], width * height);
    memset(img.planes[1], Data[14], (width * height) / 4);
    memset(img.planes[2], Data[15], (width * height) / 4);

    aom_codec_frame_flags_t flags = 0;
    aom_codec_ctx_t *enc_ctx = &ctx;
    
    // Attempt encoding
    ret = aom_codec_encode(enc_ctx, &img, 0, 1, flags);
    (void)ret;

    // Cleanup image
    aom_img_free(&img);

    // Cleanup encoder
    cleanup_encoder(&ctx);

    return 0;
}