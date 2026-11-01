// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_QM_MIN
 * - AV1E_SET_RENDER_SIZE
 * - AV1E_SET_ENABLE_PALETTE
 * - AV1E_SET_ENABLE_OVERLAY
 * - AV1E_GET_SEQ_LEVEL_IDX
 * - AV1E_SET_ENABLE_PAETH_INTRA
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    int width, height, val_int;
    int ret;

    /* Extract parameters from fuzz input */
    width = (Data[0] % 20) + 16; // Min 16, max 35
    height = (Data[1] % 20) + 16;
    
    // Ensure dimensions are multiples of 8 for AV1
    width = (width / 8) * 8;
    height = (height / 8) * 8;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    if (!init_encoder(&ctx, &cfg, width, height)) {
        return 0;
    }

    /*
     * 1. AV1E_SET_QM_MIN
     * Sets the minimum quantizer value.
     * Expected argument: int
     */
    val_int = (int)(Data[2] % 64); // Range 0-63
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MIN, val_int);
    (void)ret;

    /*
     * 2. AV1E_SET_RENDER_SIZE
     * Sets the render size (width, height).
     * Expected argument: pointer to struct containing width/height.
     */
    struct {
        unsigned int width;
        unsigned int height;
    } render_size;
    render_size.width = (unsigned int)(Data[3] % 100) + width;
    render_size.height = (unsigned int)(Data[4] % 100) + height;
    ret = aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, &render_size);
    (void)ret;

    /*
     * 3. AV1E_SET_ENABLE_PALETTE
     * Enables palette mode.
     * Expected argument: int (0 or 1)
     */
    val_int = (int)(Data[5] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val_int);
    (void)ret;

    /*
     * 4. AV1E_SET_ENABLE_OVERLAY
     * Enables overlay plane.
     * Expected argument: int (0 or 1)
     */
    val_int = (int)(Data[6] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val_int);
    (void)ret;

    /*
     * 5. AV1E_GET_SEQ_LEVEL_IDX
     * Gets the sequence level index.
     * Expected argument: int* (output)
     */
    int level_idx = 0;
    ret = aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
    (void)ret;
    (void)level_idx;

    /*
     * 6. AV1E_SET_ENABLE_PAETH_INTRA
     * Enables Paeth prediction mode.
     * Expected argument: int (0 or 1)
     */
    val_int = (int)(Data[7] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val_int);
    (void)ret;

    /*
     * Additional exploration: Try encoding a dummy frame to ensure state consistency
     * Fixed: Use AOME_SET_CPUUSED instead of AV1E_SET_CPUUSED
     */
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(&img.planes[0][0], 0, img.stride[0] * height);
    memset(&img.planes[1][0], 128, img.stride[1] * (height / 2));
    memset(&img.planes[2][0], 128, img.stride[2] * (height / 2));

    aom_codec_control(&ctx, AOME_SET_CPUUSED, 0);
    aom_codec_encode(&ctx, &img, 0, 1, 0);
    
    aom_img_free(&img);

    cleanup_encoder(&ctx);

    return 0;
}