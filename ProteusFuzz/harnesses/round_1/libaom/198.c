// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aomcx.h"
#include "aomdx.h"

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    // Use rc_min_quantizer instead of rc_quantizer which does not exist
    cfg->rc_min_quantizer = 10;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_image_t create_image(int w, int h) {
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 16);
    memset(img.planes[0], 0, w * h);
    memset(img.planes[1], 128, (w * h) / 4);
    memset(img.planes[2], 128, (w * h) / 4);
    return img;
}

int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Create a dummy frame to ensure encoder is in a valid state
    aom_image_t img = create_image(64, 64);
    
    // Ensure we have a valid frame to encode at least once to initialize state
    aom_codec_frame_flags_t flags = 0;
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Extract control values from input data safely
    // We use modulo to keep values within reasonable ranges for specific controls
    int32_t val = (int32_t)Data[0];
    int32_t val2 = (int32_t)Data[1] % 2; // 0 or 1 for boolean flags
    int32_t val3 = (int32_t)Data[2] % 2;
    int32_t val4 = (int32_t)Data[3] % 2;
    int32_t val5 = (int32_t)Data[4] % 3; // Chroma subsampling usually 0, 1, 2
    int32_t val6 = (int32_t)Data[5] % 100;

    // Attempt to invoke controls corresponding to the requested functionality
    // 1. AV1E_SET_TIER_MASK (Tier 0 or 1)
    if (aom_codec_control(&ctx, AV1E_SET_TIER_MASK, val2) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing other states
    }

    // 2. AV1E_SET_QM_MIN (Quantizer minimum)
    if (aom_codec_control(&ctx, AV1E_SET_QM_MIN, val) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 3. AV1E_SET_ENABLE_OVERLAY (Boolean)
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val3) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 4. AV1E_SET_LOSSLESS (Boolean)
    if (aom_codec_control(&ctx, AV1E_SET_LOSSLESS, val4) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 5. AV1E_SET_CHROMA_SUBSAMPLING_X (0, 1, or 2)
    if (aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val5) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 6. AV1E_SET_MV_COST_UPD_FREQ (Frequency value)
    if (aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val6) != AOM_CODEC_OK) {
        // Ignore error
    }

    // Encode a frame to trigger processing with new settings
    aom_codec_encode(&ctx, &img, 1, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}