// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
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

/* Helper to safely initialize a decoder context */
static int init_decoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

/* Helper to safely initialize an encoder context */
static int init_encoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Use first byte to decide encoder vs decoder path */
    int use_encoder = (Data[0] & 0x01);
    int op = Data[1] % 5; /* 0..4 for different control types */

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    const aom_codec_iface_t *iface = NULL;

    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface || !init_encoder(&ctx, iface)) return 0;
    } else {
        iface = aom_codec_av1_dx();
        if (!iface || !init_decoder(&ctx, iface)) return 0;
    }

    /* Prepare arguments based on op code */
    int ret = AOM_CODEC_OK;
    int32_t val_int32 = 0;
    uint32_t val_uint32 = 0;
    int val_int = 0;

    if (Size > 2) {
        val_int32 = (int32_t)Data[2];
        val_uint32 = (uint32_t)Data[2];
        val_int = (int)Data[2];
    }

    /* Fuzz different control types */
    switch (op) {
        case 0:
            /* AOME_SET_SPATIAL_LAYER_ID */
            if (use_encoder) {
                ret = aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, val_int);
            }
            break;
        case 1:
            /* AV1D_SET_OPERATING_POINT */
            if (!use_encoder) {
                ret = aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, val_uint32);
            }
            break;
        case 2:
            /* AOME_SET_NUMBER_SPATIAL_LAYERS */
            if (use_encoder) {
                ret = aom_codec_control(&ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val_int);
            }
            break;
        case 3:
            /* AV1E_SET_SVC_FRAME_DROP_MODE */
            if (use_encoder) {
                ret = aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val_int);
            }
            break;
        case 4:
            /* AV1D_SET_SKIP_FILM_GRAIN */
            if (!use_encoder) {
                ret = aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, val_int);
            }
            break;
    }

    /* Additional fuzzing for AV1D_SET_OUTPUT_ALL_LAYERS if decoder */
    if (!use_encoder && Size > 3) {
        int skip_film_grain = (int)Data[3];
        ret = aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, skip_film_grain);
        ret = aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, (int)Data[3]);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}