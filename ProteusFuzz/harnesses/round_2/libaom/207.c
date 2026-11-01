// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int get_int(const uint8_t *d, size_t *off, size_t max) {
    if (*off + sizeof(int) > max) return 0;
    int val;
    memcpy(&val, d + *off, sizeof(int));
    *off += sizeof(int);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    size_t off = 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_X
    if (off + sizeof(int) <= size) {
        int val = get_int(data, &off, size);
        if (val >= 0 && val <= 2) {
            aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
        }
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_8X8
    // Fixed: The identifier AV1E_SET_ENABLE_DIST_8X8 does not exist in the provided headers.
    // Based on the error message suggesting AV1E_SET_ENABLE_RECT_TX, and the prompt's context,
    // we will skip this specific control if the constant is not defined to avoid compilation errors,
    // or use a valid alternative if intended. Since the prompt specifically asked for this name
    // but the compiler says it's missing, we must omit it or use a macro check.
    // Given the strict requirement to compile, we will comment out the invalid constant usage
    // or assume the user meant a valid constant. However, to strictly follow the "fuzz the API"
    // instruction while compiling, we will skip the undefined constant.
    // Note: If the user insists on fuzzing this specific name, they must define it or the library version must support it.
    // For this fix, we remove the line causing the error.
    // If AV1E_SET_ENABLE_DIST_8X8 was a typo in the prompt for AV1E_SET_ENABLE_RECT_TX, 
    // one could uncomment the line below and change the constant.
    // For now, we skip to ensure compilation.
    // aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_8X8, val); 

    // 3. aom_codec_control_typechecked_AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    if (off + sizeof(int) <= size) {
        int val = get_int(data, &off, size);
        if (val == 0 || val == 1) {
            aom_codec_control(ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, val);
        }
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_QM_U
    if (off + sizeof(int) <= size) {
        int val = get_int(data, &off, size);
        aom_codec_control(ctx, AV1E_SET_QM_U, val);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_TIER_MASK
    if (off + sizeof(int) <= size) {
        int val = get_int(data, &off, size);
        aom_codec_control(ctx, AV1E_SET_TIER_MASK, val);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_FP_MT
    if (off + sizeof(int) <= size) {
        int val = get_int(data, &off, size);
        if (val == 0 || val == 1) {
            aom_codec_control(ctx, AV1E_SET_FP_MT, val);
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_207(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    if (!encoder) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set some basic config parameters
    cfg.g_w = (Size > 1) ? (Data[0] % 512 + 16) : 128;
    cfg.g_h = (Size > 2) ? (Data[1] % 512 + 16) : 128;
    // Fixed: g_time_base -> g_timebase
    cfg.g_timebase.num = (Size > 3) ? (Data[2] % 100 + 1) : 1;
    cfg.g_timebase.den = (Size > 4) ? (Data[3] % 100 + 1) : 1000;
    cfg.g_threads = (Size > 5) ? (Data[4] % 8 + 1) : 1;
    cfg.rc_target_bitrate = (Size > 6) ? (Data[5] % 10000 + 100) : 1000;
    cfg.g_error_resilient = (Size > 7) ? (Data[6] % 3) : 0;
    cfg.g_lag_in_frames = (Size > 8) ? (Data[7] % 10) : 0;

    if (aom_codec_enc_init(&ctx, encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}