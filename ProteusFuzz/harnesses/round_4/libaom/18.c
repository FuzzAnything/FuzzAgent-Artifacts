// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control commands based on input data
    // We map input bytes to different control types and values
    for (size_t i = 0; i < Size; i += 4) {
        if (i + 4 > Size) break;
        
        uint32_t val = 0;
        memcpy(&val, Data + i, sizeof(val));
        
        // AV1E_SET_DENOISE_BLOCK_SIZE
        if (aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
        
        // AV1E_SET_NOISE_SENSITIVITY
        if (aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
        
        // AV1E_SET_QM_MAX
        if (aom_codec_control(&ctx, AV1E_SET_QM_MAX, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
        
        // AV1E_SET_ENABLE_INTRABC
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
        
        // AV1E_SET_CHROMA_SUBSAMPLING_Y
        if (aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
        
        // AV1E_SET_GF_CBR_BOOST_PCT
        if (aom_codec_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, val & 0xFF) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }
    }

    cleanup_encoder(&ctx);
    return 0;
}