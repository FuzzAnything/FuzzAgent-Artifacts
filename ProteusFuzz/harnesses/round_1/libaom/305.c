// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, const aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static aom_codec_ctx_t *init_decoder(aom_codec_iface_t *iface, const aom_codec_dec_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    if (aom_codec_dec_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_305(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    
    if (!enc_iface || !dec_iface) return 0;

    // Encoder setup
    aom_codec_enc_cfg_t cfg_enc;
    if (aom_codec_enc_config_default(enc_iface, &cfg_enc, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use input data to configure encoder parameters
    cfg_enc.g_w = (Size > 0) ? Data[0] + 16 : 16;
    cfg_enc.g_h = (Size > 1) ? Data[1] + 16 : 16;
    cfg_enc.g_timebase.num = (Size > 2) ? Data[2] + 1 : 1;
    cfg_enc.g_timebase.den = (Size > 3) ? Data[3] + 1 : 1;
    cfg_enc.rc_target_bitrate = (Size > 4) ? Data[4] * 1000 : 1000;

    aom_codec_ctx_t *enc_ctx = init_encoder(enc_iface, &cfg_enc);
    if (!enc_ctx) return 0;

    // Decoder setup
    aom_codec_dec_cfg_t cfg_dec;
    memset(&cfg_dec, 0, sizeof(cfg_dec));
    cfg_dec.threads = (Size > 5) ? Data[5] % 8 + 1 : 1;
    // Removed decoder_mode and AOM_DECODER_MODE_REALTIME as they are not valid in this version

    aom_codec_ctx_t *dec_ctx = init_decoder(dec_iface, &cfg_dec);
    if (!dec_ctx) {
        cleanup_encoder(enc_ctx);
        return 0;
    }

    // Fuzzing control functions for encoder
    int val_int = (Size > 6) ? Data[6] : 0;
    
    // 1. AV1E_SET_QM_MAX
    aom_codec_control(enc_ctx, AV1E_SET_QM_MAX, val_int);
    
    // 2. AV1E_SET_TIMING_INFO_TYPE
    aom_codec_control(enc_ctx, AV1E_SET_TIMING_INFO_TYPE, val_int);
    
    // 3. AV1E_SET_DELTAQ_MODE
    aom_codec_control(enc_ctx, AV1E_SET_DELTAQ_MODE, val_int);
    
    // 5. AOME_SET_CQ_LEVEL
    aom_codec_control(enc_ctx, AOME_SET_CQ_LEVEL, val_int);
    
    // 6. AV1E_SET_ALLOW_REF_FRAME_MVS
    aom_codec_control(enc_ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val_int);

    // Fuzzing control function for decoder
    // 4. AOMD_GET_SCREEN_CONTENT_TOOLS_INFO
    aom_screen_content_tools_info info;
    memset(&info, 0, sizeof(info));
    aom_codec_control(dec_ctx, AOMD_GET_SCREEN_CONTENT_TOOLS_INFO, &info);

    // Cleanup
    cleanup_encoder(enc_ctx);
    cleanup_decoder(dec_ctx);

    return 0;
}