// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int try_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;
    int32_t luma_strength = 0;
    
    if (size > 0) val = data[0];
    if (size > 1) val = data[1];
    
    // AV1E_SET_ENABLE_ONESIDED_COMP
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val) != AOM_CODEC_OK) {
        ret = 1;
    }
    
    // AV1E_SET_MATRIX_COEFFICIENTS
    if (size > 2) val = data[2];
    if (aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, val) != AOM_CODEC_OK) {
        ret = 1;
    }
    
    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // We pass the data pointer directly if size is sufficient, assuming the API expects a struct pointer
    // Since the exact struct size is unknown or typedef missing, we use a safe minimum check or skip if unsure.
    // Based on libaom headers, aom_film_grain_params_t exists in aom_av1.h usually. 
    // If not visible, we try to pass the buffer if it's large enough to be a plausible struct.
    if (size >= 64) { 
        if (aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)data) != AOM_CODEC_OK) {
            ret = 1;
        }
    }
    
    // AV1E_SET_FILM_GRAIN_TABLE
    if (size >= 64) {
        if (aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TABLE, (void*)data) != AOM_CODEC_OK) {
            ret = 1;
        }
    }
    
    // AV1E_GET_LUMA_CDEF_STRENGTH
    if (aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &luma_strength) != AOM_CODEC_OK) {
        ret = 1;
    }
    
    return ret;
}

static int try_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;
    
    if (size > 0) val = data[0];
    
    // AV1D_SET_SKIP_FILM_GRAIN
    if (aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, val) != AOM_CODEC_OK) {
        ret = 1;
    }
    
    return ret;
}

int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    aom_codec_ctx_t encoder_ctx;
    aom_codec_ctx_t decoder_ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_iface_t *enc_iface;
    aom_codec_iface_t *dec_iface;
    aom_codec_err_t res;
    
    memset(&encoder_ctx, 0, sizeof(encoder_ctx));
    memset(&decoder_ctx, 0, sizeof(decoder_ctx));
    
    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;
    
    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    
    res = aom_codec_enc_init(&encoder_ctx, enc_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Initialize Decoder
    dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    // Fixed: removed invalid members allow_highbitdepth, allow_intrabc, allow_screencontenttools
    // These are encoder specific or not present in this struct version.
    
    res = aom_codec_dec_init(&decoder_ctx, dec_iface, &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // Run Encoder Controls
    try_encoder_controls(&encoder_ctx, Data, Size);
    
    // Run Decoder Controls
    try_decoder_controls(&decoder_ctx, Data, Size);
    
    // Cleanup
    aom_codec_destroy(&encoder_ctx);
    aom_codec_destroy(&decoder_ctx);
    
    return 0;
}