// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t* init_encoder_context(int width, int height, int fps_num, int fps_den) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return NULL;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    // Fixed: Use g_timebase instead of g_time_base
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz data
    int width = (Data[0] % 20) + 16; // Ensure at least 16
    int height = (Data[1] % 20) + 16; // Ensure at least 16
    int fps_num = (Data[2] % 30) + 1;
    int fps_den = (Data[3] % 30) + 1;
    
    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    aom_codec_ctx_t *ctx = init_encoder_context(width, height, fps_num, fps_den);
    if (!ctx) return 0;

    int val = 0;
    int strength = 0;

    // 1. AV1E_SET_ENABLE_DIFF_WTD_COMP
    // Fixed: Use AV1E_SET_ENABLE_DIFF_WTD_COMP
    val = (Data[4] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);

    // 2. AV1E_SET_ENABLE_FLIP_IDTX
    // Fixed: Use AV1E_SET_ENABLE_FLIP_IDTX
    val = (Data[5] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);

    // 3. AV1E_SET_DISABLE_TRELLIS_QUANT
    // Fixed: Use AV1E_SET_DISABLE_TRELLIS_QUANT
    val = (Data[6] % 2);
    aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val);

    // 4. AV1E_SET_ENABLE_GLOBAL_MOTION
    // Fixed: Use AV1E_SET_ENABLE_GLOBAL_MOTION
    val = (Data[7] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);

    // 5. AV1E_SET_ALLOW_REF_FRAME_MVS
    // Fixed: Use AV1E_SET_ALLOW_REF_FRAME_MVS
    val = (Data[8] % 2);
    aom_codec_control(ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val);

    // 6. AV1E_GET_LUMA_CDEF_STRENGTH
    // Fixed: Use AV1E_GET_LUMA_CDEF_STRENGTH
    aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &strength);

    // Additional exploration: Try invalid values or large values
    if (Size > 9) {
        int large_val = (Data[9] * 256 + Data[10]) % 1000;
        // Try setting with a larger value to test bounds
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, large_val);
        aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, large_val);
        aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, large_val);
        aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, large_val);
        aom_codec_control(ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, large_val);
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}