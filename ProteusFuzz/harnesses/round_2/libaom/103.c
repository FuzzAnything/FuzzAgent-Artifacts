// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom AV1 encoder control commands.
 * Targets:
 * - AV1E_SET_MAX_PARTITION_SIZE
 * - AV1E_SET_REDUCED_REFERENCE_SET
 * - AV1E_SET_ENABLE_RECT_PARTITIONS
 * - AV1E_SET_ENABLE_AB_PARTITIONS
 * - AV1E_SET_MIN_PARTITION_SIZE
 * - AV1E_SET_ENABLE_1TO4_PARTITIONS
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->g_pass = 0;
    cfg->rc_target_bitrate = 500;

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_err_t res;
    aom_codec_cx_pkt_t pkt;

    /* Flush the encoder */
    while ((res = aom_codec_encode(ctx, NULL, 0, 1, 0)) == AOM_CODEC_OK) {
        /* Drain packets */
        while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                /* pkt.data.frame is an aom_encoded_frame_t. 
                 * We do not free it directly here as the encoder manages its lifecycle.
                 */
            }
        }
    }

    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int width, height;
    int val;
    int ret;

    if (Size < 8) return 0;

    /* Extract parameters from fuzz input */
    width = Data[0] + 1;
    height = Data[1] + 1;
    val = Data[2];

    /* Initialize encoder */
    if (init_encoder(&ctx, &cfg, width, height) != 0) {
        return 0;
    }

    /* Fuzz various control commands */
    /* AV1E_SET_MAX_PARTITION_SIZE: expects an int (partition size) */
    if (Size > 3) {
        int max_part = Data[3];
        if (max_part > 0 && max_part <= 64) {
            aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, max_part);
        }
    }

    /* AV1E_SET_REDUCED_REFERENCE_SET: expects an int (0 or 1) */
    if (Size > 4) {
        int reduced_ref = Data[4] % 2;
        aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);
    }

    /* AV1E_SET_ENABLE_RECT_PARTITIONS: expects an int (0 or 1) */
    if (Size > 5) {
        int enable_rect = Data[5] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect);
    }

    /* AV1E_SET_ENABLE_AB_PARTITIONS: expects an int (0 or 1) */
    if (Size > 6) {
        int enable_ab = Data[6] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab);
    }

    /* AV1E_SET_MIN_PARTITION_SIZE: expects an int (partition size) */
    if (Size > 7) {
        int min_part = Data[7];
        if (min_part > 0 && min_part <= 64) {
            aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part);
        }
    }

    /* AV1E_SET_ENABLE_1TO4_PARTITIONS: expects an int (0 or 1) */
    if (Size > 8) {
        int enable_1to4 = Data[8] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, enable_1to4);
    }

    /* Encode a dummy frame to exercise the encoder state */
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    
    /* Fill the image planes with safe data using the correct stride and height */
    /* Plane 0 (Y): stride * height */
    memset(img.planes[0], 0, img.stride[0] * height);
    /* Plane 1 (U): stride * (height/2 + 1) for I420, but aom_img_alloc handles alignment. 
     * Safe to use stride * (height + 1) only if height is large enough, but strictly 
     * the allocated size is stride * (height + 1) for the plane buffer.
     * However, to avoid overflow, we use the exact allocated size logic or just memset the whole buffer.
     * The error occurred because we wrote stride * (height + 1) but the buffer might be smaller 
     * if alignment was different or if the calculation was wrong. 
     * Actually, aom_img_alloc allocates stride * (height + 1) for each plane to allow for alignment.
     * The error in the previous run suggests the calculation img.stride[1] * (height + 1) exceeded the allocation.
     * This happens if height is very large or if the stride calculation is off.
     * Let's use a safer approach: memset the entire plane buffer up to the known allocated size.
     * The allocated size for a plane is img.stride[i] * (img.d_h / (1 << (fmt->subsampling_y[i] + fmt->subsampling_x[i])) + 1).
     * For I420, U and V are height/2.
     * Let's just memset the first 'height' rows for Y and 'height/2 + 1' for UV to be safe and simple.
     */
    
    /* Y plane */
    memset(img.planes[0], 0, img.stride[0] * height);
    /* U plane */
    int uv_height = (height + 1) / 2;
    memset(img.planes[1], 128, img.stride[1] * uv_height);
    /* V plane */
    memset(img.planes[2], 128, img.stride[2] * uv_height);

    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    aom_img_free(&img);

    /* Drain any output packets */
    if (ret == AOM_CODEC_OK) {
        aom_codec_cx_pkt_t pkt;
        while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                /* Do not call aom_img_free here. pkt.data.frame is an aom_encoded_frame_t.
                 * The memory for the frame content is managed by the encoder or the application
                 * after copying. In a fuzzing context, we just consume the packet.
                 */
            }
        }
    }

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}