// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
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
    if (ctx == NULL) return 0;
    
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int is_encoder = Data[0] & 1;
    int value = (Data[1] << 8) | (Size > 2 ? Data[2] : 0);
    
    if (value < 0) value = 0;
    if (value > 1000) value = 1000;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        struct aom_codec_enc_cfg cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        int control_id;
        switch (Data[0] % 6) {
            case 0: control_id = AV1E_SET_ENABLE_INTRABC; break;
            case 1: control_id = AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR; break;
            case 2: control_id = AV1E_SET_NOISE_SENSITIVITY; break;
            case 3: control_id = AOME_SET_TUNING; break;
            case 4: control_id = AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR; break;
            case 5: control_id = AV1E_SET_GF_CBR_BOOST_PCT; break;
            default: control_id = AV1E_SET_ENABLE_INTRABC; break;
        }

        fuzz_encoder_control(&ctx, control_id, value);
        cleanup_encoder(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        struct aom_codec_dec_cfg cfg;
        cfg.threads = 1;
        // Removed invalid fields: allow_highbitdepth, allow_multiref, allow_screencontenttools
        // Only setting valid fields based on the error message and standard struct definition
        cfg.allow_lowbitdepth = 1;

        err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        int control_id;
        switch (Data[0] % 6) {
            case 0: control_id = AV1E_SET_ENABLE_INTRABC; break;
            case 1: control_id = AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR; break;
            case 2: control_id = AV1E_SET_NOISE_SENSITIVITY; break;
            case 3: control_id = AOME_SET_TUNING; break;
            case 4: control_id = AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR; break;
            case 5: control_id = AV1E_SET_GF_CBR_BOOST_PCT; break;
            default: control_id = AV1E_SET_ENABLE_INTRABC; break;
        }

        fuzz_decoder_control(&ctx, control_id, value);
        cleanup_decoder(&ctx);
    }

    return 0;
}