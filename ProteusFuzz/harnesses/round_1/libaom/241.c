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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to vary configuration slightly if possible, 
    // but keep it within valid bounds to ensure initialization succeeds.
    cfg.g_w = (Size % 256) + 16;
    cfg.g_h = (Size % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size % 1000) + 1;
    cfg.rc_target_bitrate = (Size % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Helper to safely call control functions with fuzzed values
    int ret;

    // Parse a small portion of the input to decide which function to test and what value to use
    int idx = (Size > 0) ? Data[0] % 6 : 0;
    int value = (Size > 1) ? (Data[1] & 0xFF) : 0;
    
    switch (idx) {
        case 0: // AV1E_SET_QM_U
            {
                int val_int = value;
                ret = aom_codec_control(&ctx, AV1E_SET_QM_U, &val_int);
                (void)ret;
            }
            break;
        case 1: // AV1E_SET_INTER_DCT_ONLY
            {
                int val_int = value ? 1 : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, &val_int);
                (void)ret;
            }
            break;
        case 2: // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
            {
                int val_int = value; // milliseconds
                ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, &val_int);
                (void)ret;
            }
            break;
        case 3: // AV1E_SET_ENABLE_INTRABC
            {
                int val_int = value ? 1 : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, &val_int);
                (void)ret;
            }
            break;
        case 4: // AV1E_SET_ENABLE_CHROMA_DELTAQ
            {
                int val_int = value ? 1 : 0;
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, &val_int);
                (void)ret;
            }
            break;
        case 5: // AOME_SET_TUNING
            {
                // Tuning usually expects an enum like AOM_TUNE_PSNR or AOM_TUNE_SSIM
                int val_int = value % 2; // 0 or 1
                ret = aom_codec_control(&ctx, AOME_SET_TUNING, &val_int);
                (void)ret;
            }
            break;
        default:
            break;
    }

    cleanup_codec(&ctx);
    return 0;
}