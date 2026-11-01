// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t **ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_ctx_t *codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return -1;

    aom_codec_err_t err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(codec);
        return -1;
    }

    *ctx = codec;
    return 0;
}

static int init_decoder(aom_codec_ctx_t **ctx, const aom_codec_dec_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_ctx_t *codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return -1;

    aom_codec_err_t err = aom_codec_dec_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(codec);
        return -1;
    }

    *ctx = codec;
    return 0;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int use_encoder = Data[0] % 2;
    int test_control = Data[1] % 4;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg_enc;
    aom_codec_dec_cfg_t cfg_dec = {0};

    if (use_encoder) {
        aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;
        
        aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg_enc, 0);
        if (err != AOM_CODEC_OK) return 0;

        cfg_enc.g_w = (Data[0] % 1024) + 16;
        cfg_enc.g_h = (Data[1] % 1024) + 16;
        cfg_enc.g_timebase.num = 1;
        cfg_enc.g_timebase.den = 30;
        cfg_enc.rc_target_bitrate = (Data[0] + Data[1]) * 1000;
        
        if (init_encoder(&ctx, &cfg_enc) != 0) return 0;
    } else {
        cfg_dec.threads = (Data[0] % 8) + 1;
        if (init_decoder(&ctx, &cfg_dec) != 0) return 0;
    }

    int val = (Data[2] % 2) ? 1 : 0;
    int val_int = (int)(Data[2] + Data[3]);
    
    aom_codec_err_t err = AOM_CODEC_OK;

    if (use_encoder) {
        switch (test_control) {
            case 0: 
                err = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
                break;
            case 1: 
                err = aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, val);
                break;
            case 2: 
                err = aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, val_int);
                break;
            case 3: 
                err = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
                break;
            default:
                err = aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);
                break;
        }
    } else {
        if (test_control == 3) {
            err = aom_codec_control(ctx, AV1D_SET_IS_ANNEXB, val);
        } else {
            // For other decoder cases, we just return to avoid undefined behavior
            // or missing control enums.
            (void)val;
        }
    }

    (void)err;

    cleanup_codec(ctx);
    return 0;
}