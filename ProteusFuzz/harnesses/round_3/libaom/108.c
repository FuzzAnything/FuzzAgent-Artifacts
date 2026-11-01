// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

// Helper to safely initialize an encoder context
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return 0;

    // Initialize config with default values
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some config parameters safely
    if (size > 0) {
        cfg->g_w = 16 + (data[0] % 256);
        cfg->g_h = 16 + (data[1] % 256);
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 1 + (data[2] % 256);
        cfg->g_threads = 1 + (data[3] % 4);
        cfg->rc_target_bitrate = 100 + (data[4] % 10000);
    }

    // Initialize the codec context
    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

// Helper to safely destroy an encoder context
static void destroy_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder
    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Extract values from fuzzing data to use as parameters
    int val1 = Data[0] & 0xFF;
    int val2 = Data[1] & 0xFF;
    int val3 = Data[2] & 0xFF;
    int val4 = Data[3] & 0xFF;
    int val5 = Data[4] & 0xFF;
    int val6 = Data[5] & 0xFF;

    // Invoke control functions using the correct enum names from aomcx.h
    // AV1E_SET_DENOISE_BLOCK_SIZE
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val1);
    
    // AV1E_SET_ENABLE_CDEF
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val2);
    
    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val3);
    
    // AV1E_SET_SKIP_POSTPROC_FILTERING
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val4);
    
    // AV1E_SET_ENABLE_KEYFRAME_FILTERING
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val5);
    
    // AV1E_SET_QM_U
    aom_codec_control(&ctx, AV1E_SET_QM_U, val6);

    // Cleanup
    destroy_encoder(&ctx);

    return 0;
}