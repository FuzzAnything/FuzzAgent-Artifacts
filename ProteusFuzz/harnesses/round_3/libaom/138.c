// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/* 
 * Helper function to initialize the encoder context.
 * Returns 0 on success, non-zero on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (!ctx || !cfg) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

/*
 * Helper function to create a dummy image frame for encoding.
 */
static aom_image_t *create_dummy_image(int width, int height) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;

    // Fill with simple pattern using row pointers
    // img->planes[plane] is a pointer to a buffer, stride is img->stride[plane]
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            img->planes[0][i * img->stride[0] + j] = (i + j) % 256;
        }
    }
    // Fill UV planes (simplified)
    int uv_height = height / 2;
    int uv_width = width / 2;
    for (int i = 0; i < uv_height; i++) {
        for (int j = 0; j < uv_width; j++) {
            img->planes[1][i * img->stride[1] + j] = 128;
            img->planes[2][i * img->stride[2] + j] = 128;
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int width = 0, height = 0;
    int val = 0;

    // Extract parameters from input data
    width = Data[0] + 1;
    height = Data[1] + 1;
    
    // Limit dimensions to reasonable sizes to prevent OOM
    if (width > 1920 || height > 1080) return 0;
    if (width < 16 || height < 16) {
        width = 16;
        height = 16;
    }

    // Initialize encoder
    if (init_encoder(&ctx, &cfg, width, height) != 0) {
        return 0;
    }

    // Create a dummy image to ensure encoder is in a valid state
    aom_image_t *img = create_dummy_image(width, height);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Encode one frame to ensure context is active
    err = aom_codec_encode(&ctx, img, 0, 1, 0);
    aom_img_free(img);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz the target control functions
    // We use the input data to determine which function to call and with what value
    int choice = (Size > 2) ? Data[2] % 6 : 0;

    switch (choice) {
        case 0:
            // AV1E_SET_ENABLE_CFL_INTRA
            val = (Size > 3) ? Data[3] % 2 : 0;
            aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
            break;
        case 1:
            // AV1E_SET_FRAME_PARALLEL_DECODING
            val = (Size > 3) ? Data[3] % 2 : 0;
            aom_codec_control(&ctx, AV1E_SET_FRAME_PARALLEL_DECODING, val);
            break;
        case 2:
            // AV1E_SET_CHROMA_SAMPLE_POSITION
            val = (Size > 3) ? Data[3] % 3 : 0; // 0, 1, 2
            aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
            break;
        case 3:
            // AV1E_SET_ENABLE_INTRABC
            val = (Size > 3) ? Data[3] % 2 : 0;
            aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
            break;
        case 4:
            // AV1E_SET_TILE_ROWS
            // Tile rows usually range from 0 to 31 (or similar power of 2 minus 1)
            val = (Size > 3) ? (Data[3] % 32) : 0;
            aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
            break;
        case 5:
            // AV1E_SET_QM_U
            // QM values are typically integers, let's fuzz within a reasonable range
            val = (Size > 3) ? (Data[3] % 256) : 0;
            aom_codec_control(&ctx, AV1E_SET_QM_U, val);
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}