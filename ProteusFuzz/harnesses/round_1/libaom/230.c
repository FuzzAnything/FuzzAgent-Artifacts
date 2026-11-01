// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) {
        return 0;
    }

    int ret = 0;
    int val_int;

    // Helper to read int safely
    #define READ_INT(offset) \
        do { \
            if (size > (offset) + sizeof(int)) { \
                memcpy(&val_int, data + (offset), sizeof(int)); \
            } else { \
                val_int = 0; \
            } \
        } while(0)

    // Fuzz AV1E_SET_QM_MAX
    READ_INT(0);
    ret = aom_codec_control(ctx, AV1E_SET_QM_MAX, val_int);
    (void)ret;

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    READ_INT(4);
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, val_int);
    (void)ret;

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    READ_INT(8);
    ret = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val_int);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    READ_INT(12);
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val_int);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_Y
    READ_INT(16);
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val_int);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIST_8X8 (Note: If this macro is not defined in the specific libaom version, 
    // the code might fail to compile if the macro truly doesn't exist. 
    // However, based on the error log, the user asked to fuzz this specific name. 
    // If the macro is missing in the header, we cannot use it. 
    // The error log suggests 'AV1E_SET_ENABLE_RECT_TX' exists. 
    // Assuming the user wants to test the specific string provided in the prompt, 
    // but since it's a macro, we must rely on the header. 
    // If the header doesn't have it, we skip or use a placeholder if allowed. 
    // Given the strict requirement to fix compilation, and the error says 'undeclared', 
    // it implies the macro is missing in the installed headers. 
    // However, the prompt explicitly lists it as a target. 
    // Let's assume the macro might be named differently or the user's environment is specific. 
    // To ensure compilation, we will comment out the line that causes the error if the macro is truly missing, 
    // but since I cannot check the user's specific header version at runtime, 
    // I will assume the macro name in the prompt is correct for the target version 
    // and the error might be due to a mismatch. 
    // Wait, the error log explicitly says 'use of undeclared identifier'. 
    // This means the macro AV1E_SET_ENABLE_DIST_8X8 does NOT exist in the provided headers. 
    // To fix the compilation error, I must remove or comment out this specific call 
    // or replace it with a valid one if the intent is to test similar functionality. 
    // But the task is to fuzz the specific API. If the API doesn't exist in the headers, 
    // I cannot call it. 
    // I will remove the line to fix the compilation error as requested.
    // READ_INT(20);
    // ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_8X8, val_int);
    // (void)ret;

    // Try to invoke with different data sizes and patterns to explore states
    if (size > 24) {
        READ_INT(24);
        ret = aom_codec_control(ctx, AV1E_SET_QM_MAX, val_int);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_230(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal valid configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    // Fix: AOM_CQP -> AOM_CQ
    cfg.rc_end_usage = AOM_CQ;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;
    cfg.g_bit_depth = 8;
    // Fix: Removed g_subsample_bit_depth as it doesn't exist in the struct
    // cfg.g_subsample_bit_depth = 8; 
    cfg.g_input_bit_depth = 8;
    // Fix: Removed g_input_sample_bits as it doesn't exist in the struct
    // cfg.g_input_sample_bits = 8;
    cfg.g_pass = 0;
    cfg.g_usage = 0;
    cfg.g_lag_in_frames = 0;

    // Initialize the codec
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz the target functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}