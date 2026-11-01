// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;
    int idx = 0;

    if (size < sizeof(int)) {
        return 0;
    }
    
    // AV1E_SET_ENABLE_ANGLE_DELTA
    val = *(const int *)(data + idx);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);
    (void)ret;
    idx += sizeof(int);

    if (size >= idx + sizeof(int)) {
        val = *(const int *)(data + idx);
        // AOME_SET_MAX_INTRA_BITRATE_PCT
        ret = aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val);
        (void)ret;
        idx += sizeof(int);
    }

    // AOME_SET_MAX_INTER_BITRATE_PCT is not a valid identifier in standard libaom.
    // Skipping to prevent compilation error.
    // If a specific custom build defines it, it would need to be included here.
    // For standard libaom, we skip this control or map to a valid one if intended.
    // Here we simply skip to ensure compilation.
    if (size >= idx + sizeof(int)) {
        idx += sizeof(int); 
    }

    if (size >= idx + sizeof(int)) {
        val = *(const int *)(data + idx);
        // AV1E_SET_ENABLE_DIST_WTD_COMP
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val);
        (void)ret;
        idx += sizeof(int);
    }

    if (size >= idx + sizeof(int)) {
        val = *(const int *)(data + idx);
        // AV1E_SET_GF_CBR_BOOST_PCT
        ret = aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, val);
        (void)ret;
        idx += sizeof(int);
    }

    if (size >= idx + sizeof(int)) {
        val = *(const int *)(data + idx);
        // AV1E_SET_QUANT_B_ADAPT
        ret = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }
    
    // Initialize default encoder configuration
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    
    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}