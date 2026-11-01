// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

// Helper function to initialize the encoder context
static aom_codec_ctx_t *init_encoder(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set basic configuration to ensure valid state
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_bit_depth = 8;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to create a dummy image
static aom_image_t *create_dummy_image(uint32_t width, uint32_t height, uint8_t bit_depth) {
    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return NULL;

    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->bit_depth = bit_depth;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->stride[0] = width;
    img->stride[1] = (width + 1) / 2;
    img->stride[2] = (width + 1) / 2;

    size_t y_size = width * height;
    size_t uv_size = ((width + 1) / 2) * ((height + 1) / 2);

    img->planes[0] = (unsigned char *)malloc(y_size);
    img->planes[1] = (unsigned char *)malloc(uv_size);
    img->planes[2] = (unsigned char *)malloc(uv_size);

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Fill with deterministic data based on bit_depth
    uint8_t max_val = (1 << bit_depth) - 1;
    for (size_t i = 0; i < y_size; i++) img->planes[0][i] = (i % (max_val + 1));
    for (size_t i = 0; i < uv_size; i++) img->planes[1][i] = (i % (max_val + 1));
    for (size_t i = 0; i < uv_size; i++) img->planes[2][i] = (i % (max_val + 1));

    img->sz = y_size + 2 * uv_size;
    img->bps = 8;

    return img;
}

// Helper to free image
static void free_image(aom_image_t *img) {
    if (!img) return;
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
    free(img);
}

// Helper to free context
static void free_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size) {
    // Minimum size check to safely access required bytes
    if (Size < 15) return 0;

    // Parse input data to control fuzzing behavior
    uint32_t width = (Data[0] % 256) + 16;
    uint32_t height = (Data[1] % 256) + 16;
    uint8_t bit_depth = (Data[2] % 5) + 8; // 8, 9, 10, 11, 12
    uint32_t pts = Data[3] | (Data[4] << 8) | (Data[5] << 16) | (Data[6] << 24);
    unsigned long duration = Data[7] | (Data[8] << 8);
    if (duration == 0) duration = 1; // Ensure non-zero duration for valid frames
    aom_enc_frame_flags_t flags = Data[9];

    // Initialize encoder
    aom_codec_ctx_t *ctx = init_encoder();
    if (!ctx) return 0;

    // Create image
    aom_image_t *img = create_dummy_image(width, height, bit_depth);
    if (!img) {
        free_ctx(ctx);
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE
    // Using the correct constant AV1E_SET_DENOISE_BLOCK_SIZE
    int denoise_block_size = Data[10] % 16;
    aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_QM
    // Using the correct constant AV1E_SET_ENABLE_QM
    int enable_qm = Data[11] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_QM, enable_qm);

    // Fuzz aom_codec_control_typechecked_AOME_SET_ENABLEAUTOBWDREF
    // Using the correct constant AOME_SET_ENABLEAUTOBWDREF
    int enable_autobwdref = Data[12] % 2;
    aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, enable_autobwdref);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
    // Using the correct constant AV1E_SET_DELTAQ_MODE
    int deltaq_mode = Data[13] % 4; // Assuming 4 modes
    aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_TPL_MODEL
    // Using the correct constant AV1E_SET_ENABLE_TPL_MODEL
    int enable_tpl_model = Data[14] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl_model);

    // Encode frame
    aom_codec_err_t res = aom_codec_encode(ctx, img, pts, duration, flags);
    (void)res; // Ignore result for fuzzing, just ensure no crash

    // Flush encoder
    while (1) {
        aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(ctx, NULL);
        if (!pkt) break;
        // Consume data to prevent memory leak in loop
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // In a real app we'd process this, here we just consume
        }
    }

    // Cleanup
    free_image(img);
    free_ctx(ctx);

    return 0;
}