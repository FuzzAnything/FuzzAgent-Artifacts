// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (size < 2) return 0;

    ret = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = data[0] + 16;
    cfg.g_h = data[1] + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz AOME_SET_SHARPNESS
    if (size > 2) {
        int sharpness = (int)(data[2] % 7);
        aom_codec_control(&ctx, AOME_SET_SHARPNESS, sharpness);
    }

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    if (size > 3) {
        int noise_level = (int)(data[3] % 100);
        aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, noise_level);
    }

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    if (size > 4) {
        int enable_gm = (data[4] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_gm);
    }

    // Fuzz AOME_SET_MAX_INTRA_BITRATE_PCT
    if (size > 5) {
        int max_intra_pct = (int)(data[5] % 101);
        aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);
    }

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    if (size > 6) {
        int arnr_maxframes = (int)(data[6] % 33) + 1;
        aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }

    // Fuzz AOME_SET_ARNR_STRENGTH
    if (size > 7) {
        int arnr_strength = (int)(data[7] % 64);
        aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
    }

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    int ret;

    if (size < 1) return 0;

    // Initialize decoder config manually. 
    // Removed invalid members: allow_highbitdepth, allow_intrabc, allow_screen_content_tools
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Only using members that exist in the standard aom_codec_dec_cfg_t
    // cfg.allow_lowbitdepth is available but optional to set.
    
    ret = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // The specific functions provided are encoder-specific, so we initialize the context
    // to ensure coverage of the codec control infrastructure if any generic checks exist.
    // No specific decoder controls are fuzzed here as per the target list.

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to decide which path to take.
    // The provided functions are encoder-specific, so we prioritize that.
    if (Data[0] % 2 == 0) {
        fuzz_encoder_control(Data, Size);
    } else {
        fuzz_decoder_control(Data, Size);
    }

    return 0;
}