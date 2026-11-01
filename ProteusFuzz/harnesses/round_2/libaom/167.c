// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

// Helper function to initialize the encoder context
static aom_codec_ctx_t* init_encoder_context(int width, int height, int fps_num, int fps_den) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Use the correct function name from aomcx.h
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 1000;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to create a dummy image frame
static aom_image_t* create_dummy_image(int width, int height) {
    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        free(img);
        return NULL;
    }

    // Fill with some data
    for (int i = 0; i < img->d_w * img->d_h; i++) {
        img->planes[0][i] = (uint8_t)(i % 256);
    }
    // Simplified subsampling for chroma
    int ch_w = (img->d_w + 1) / 2;
    int ch_h = (img->d_h + 1) / 2;
    for (int i = 0; i < ch_w * ch_h; i++) {
        img->planes[1][i] = 128;
        img->planes[2][i] = 128;
    }

    return img;
}

// Helper to destroy image
static void destroy_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    // Extract parameters from fuzz data
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int fps_num = Data[2] + 1;
    int fps_den = Data[3] + 1;
    
    // Ensure reasonable bounds
    if (width > 4096 || height > 4096) return 0;
    if (width < 16 || height < 16) { width = 16; height = 16; }

    aom_codec_ctx_t *ctx = init_encoder_context(width, height, fps_num, fps_den);
    if (!ctx) return 0;

    aom_image_t *img = create_dummy_image(width, height);
    if (!img) {
        aom_codec_destroy(ctx);
        free(ctx);
        return 0;
    }

    // Prepare control commands based on fuzz data
    // We will try to invoke the target control functions using aom_codec_control
    
    // Test 1: AV1E_SET_CHROMA_SAMPLE_POSITION
    // Value: 0, 1, 2, 3 (TOPLEFT, TOP, BOTTOMLEFT, CENTER)
    int chroma_pos = Data[4] % 4;
    aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

    // Test 2: AV1E_SET_REDUCED_REFERENCE_SET
    // Value: 0 or 1
    int reduced_ref = (Data[5] % 2);
    aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);

    // Test 3: AV1E_SET_ENABLE_RECT_PARTITIONS
    // Value: 0 or 1
    int rect_parts = (Data[6] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, rect_parts);

    // Test 4: AV1E_SET_ENABLE_AB_PARTITIONS
    // Value: 0 or 1
    int ab_parts = (Data[7] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, ab_parts);

    // Test 5: AV1E_SET_ENABLE_1TO4_PARTITIONS
    // Value: 0 or 1
    int one_to_four_parts = (Data[8] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, one_to_four_parts);

    // Test 6: AV1E_SET_TILE_ROWS
    // Value: 0 to 8 (max usually 8)
    int tile_rows = (Data[9] % 9);
    aom_codec_control(ctx, AV1E_SET_TILE_ROWS, tile_rows);

    // Encode one frame to ensure the context is active and controls took effect
    // Fixed type name: aom_codec_frame_flags_t
    aom_codec_frame_flags_t flags = 0;
    if (Size > 10) flags = Data[10];
    
    aom_codec_encode(ctx, img, 0, 1, flags);

    // Cleanup
    destroy_image(img);
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}