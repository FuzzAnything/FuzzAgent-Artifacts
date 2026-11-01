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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t **ctx) {
    *ctx = NULL;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;
    
    aom_codec_ctx_t *codec = malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return -1;
    
    memset(codec, 0, sizeof(aom_codec_ctx_t));
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec);
        return -1;
    }
    
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec);
        return -1;
    }
    
    *ctx = codec;
    return 0;
}

int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    aom_codec_ctx_t *ctx = NULL;
    
    if (init_encoder_context(&ctx) != 0) {
        return 0;
    }
    
    uint8_t mode = Data[0] % 7;
    
    if (mode == 0) {
        int num_ops = 0;
        aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
    } else if (mode == 1) {
        int32_t value = (Size > 1) ? (int32_t)(Data[1] * 256 + Data[0]) : 0;
        aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, value);
    } else if (mode == 2) {
        if (Size > 0) {
            int32_t dummy = Data[0];
            aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &dummy);
        }
    } else if (mode == 3) {
        int32_t value = (Size > 1) ? (int32_t)(Data[1] * 256 + Data[0]) : 0;
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, value);
    } else if (mode == 4) {
        int32_t value = (Size > 1) ? (int32_t)(Data[1] * 256 + Data[0]) : 0;
        aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, value);
    } else if (mode == 5) {
        int32_t value = (Size > 1) ? (int32_t)(Data[1] * 256 + Data[0]) : 0;
        aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, value);
    } else {
        int num_ops = 0;
        aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
        
        if (Size > 0) {
            int32_t val1 = Data[0];
            aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val1);
        }
        
        if (Size > 1) {
            int32_t val2 = Data[1];
            aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val2);
        }
    }
    
    cleanup_codec(ctx);
    return 0;
}