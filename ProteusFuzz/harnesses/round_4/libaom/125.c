// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    // Need at least 8 bytes to safely access Data[0] through Data[7]
    if (Size < 8) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Initialize default encoder config
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure width/height within reasonable bounds
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize codec
    res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int val;

    // 1. AV1E_SET_QM_V (Quantization Matrix Vertical)
    val = (int)(Data[2] % 128) - 64; // Range -64 to 63
    aom_codec_control(&ctx, AV1E_SET_QM_V, &val);

    // 2. AV1E_SET_ENABLE_QM (Enable Quantization Matrices)
    val = (Data[3] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, &val);

    // 3. AV1E_SET_SUPERBLOCK_SIZE (Superblock Size)
    // Valid sizes are typically 64 or 128
    if (Data[4] % 2 == 0) {
        val = 64;
    } else {
        val = 128;
    }
    aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, &val);

    // 4. AV1E_SET_ENABLE_CDEF (Enable Constrained Directional Enhancement Filter)
    val = (Data[5] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, &val);

    // 5. AV1E_SET_ENABLE_KEYFRAME_FILTERING (Enable Keyframe Filtering)
    val = (Data[6] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, &val);

    // 6. AV1E_SET_FORCE_VIDEO_MODE (Force Video Mode)
    val = (Data[7] % 2) ? 1 : 0;
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, &val);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}