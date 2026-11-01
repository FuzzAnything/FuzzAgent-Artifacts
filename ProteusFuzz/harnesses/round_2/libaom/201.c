// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_201(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int32_t seed = *(const int32_t *)Data;
    int disable_trellis = (seed & 0x1);
    seed >>= 1;
    int arnr_strength = seed & 0xFF;
    seed >>= 8;
    int arnr_maxframes = seed & 0xFF;
    seed >>= 8;
    int enable_flip_idtx = seed & 0x1;
    seed >>= 1;
    int cdf_update_mode = seed & 0x3;
    seed >>= 2;
    int delf_mode = seed & 0x3;

    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();

    if (!encoder_iface || !decoder_iface) return 0;

    aom_codec_ctx_t enc_ctx;
    aom_codec_ctx_t dec_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    memset(&dec_ctx, 0, sizeof(dec_ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    aom_codec_err_t res;
    res = aom_codec_enc_init(&enc_ctx, encoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    // Fixed: Removed invalid members allow_high_bitdepth, allow_multiref, allow_frameparallel_decoding
    // Using valid member allow_lowbitdepth if needed, but keeping it 0 by default for safety

    res = aom_codec_dec_init(&dec_ctx, decoder_iface, &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Fuzz encoder controls
    // Fixed: Use correct macro names AV1E_SET_* and AOME_SET_*
    if (disable_trellis) {
        fuzz_encoder_control(&enc_ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis);
    }
    if (arnr_strength >= 0 && arnr_strength <= 100) {
        fuzz_encoder_control(&enc_ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
    }
    if (arnr_maxframes >= 0 && arnr_maxframes <= 32) {
        fuzz_encoder_control(&enc_ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }
    if (enable_flip_idtx) {
        fuzz_encoder_control(&enc_ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);
    }
    if (cdf_update_mode >= 0 && cdf_update_mode <= 3) {
        fuzz_encoder_control(&enc_ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);
    }
    if (delf_mode >= 0 && delf_mode <= 3) {
        fuzz_encoder_control(&enc_ctx, AV1E_SET_DELTALF_MODE, delf_mode);
    }

    // Fuzz decoder controls (attempting encoder controls on decoder context)
    if (disable_trellis) {
        fuzz_decoder_control(&dec_ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis);
    }
    if (arnr_strength >= 0 && arnr_strength <= 100) {
        fuzz_decoder_control(&dec_ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
    }
    if (arnr_maxframes >= 0 && arnr_maxframes <= 32) {
        fuzz_decoder_control(&dec_ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }
    if (enable_flip_idtx) {
        fuzz_decoder_control(&dec_ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);
    }
    if (cdf_update_mode >= 0 && cdf_update_mode <= 3) {
        fuzz_decoder_control(&dec_ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);
    }
    if (delf_mode >= 0 && delf_mode <= 3) {
        fuzz_decoder_control(&dec_ctx, AV1E_SET_DELTALF_MODE, delf_mode);
    }

    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}