// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

// Helper to safely initialize an encoder context
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (ctx == NULL || cfg == NULL) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    // Set some basic config to ensure valid state
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

// Helper to safely clean up an encoder context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

// Helper to generate a dummy frame
static aom_image_t *create_dummy_frame(int width, int height) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (img == NULL) return NULL;

    // Fill with simple pattern
    for (int i = 0; i < img->d_h; i++) {
        for (int j = 0; j < img->d_w; j++) {
            img->planes[0][i * img->stride[0] + j] = (i + j) % 256;
        }
    }
    for (int i = 0; i < (img->d_h + 1) / 2; i++) {
        for (int j = 0; j < (img->d_w + 1) / 2; j++) {
            img->planes[1][i * img->stride[1] + j] = 128;
            img->planes[2][i * img->stride[2] + j] = 128;
        }
    }
    return img;
}

// Safe helper to read 16-bit value from buffer
static uint16_t read_u16(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 2) return 0;
    return (Data[offset] << 8) | Data[offset + 1];
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    // Extract parameters from fuzz input safely
    uint16_t val1 = read_u16(Data, Size, 0);
    uint16_t val2 = read_u16(Data, Size, 2);
    uint16_t val3 = read_u16(Data, Size, 4);
    uint16_t val4 = read_u16(Data, Size, 6);
    uint16_t val5 = read_u16(Data, Size, 8);
    uint16_t val6 = read_u16(Data, Size, 10);

    // 1. AV1E_SET_TARGET_SEQ_LEVEL_IDX
    // Expected: int level_idx
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val1);

    // 2. AV1E_SET_ENABLE_REF_FRAME_MVS
    // Expected: int enable (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val2 & 1);

    // 3. AV1E_SET_MAX_REFERENCE_FRAMES
    // Expected: int max_refs (typically 0-7)
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val3 & 7);

    // 4. AV1E_SET_RTC_EXTERNAL_RC
    // Expected: int mode (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val4 & 1);

    // 5. AV1E_SET_S_FRAME_MODE
    // Expected: int mode (0, 1, or 2)
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val5 & 3);

    // 6. AV1E_SET_ENABLE_INTERINTRA_WEDGE
    // Expected: int enable (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val6 & 1);

    // Try to encode a frame to exercise the state
    aom_image_t *img = create_dummy_frame(cfg.g_w, cfg.g_h);
    if (img != NULL) {
        // aom_codec_encode signature: aom_codec_encode(ctx, img, pts, duration, flags)
        aom_codec_encode(&ctx, img, 0, 1, 0);
        aom_img_free(img);
    }

    cleanup_encoder_context(&ctx);
    return 0;
}