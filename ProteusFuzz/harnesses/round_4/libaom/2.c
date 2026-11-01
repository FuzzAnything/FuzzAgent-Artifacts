// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_integer.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static void init_encoder_cfg_fuzz(aom_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));
    
    // Use fuzzing data to set configuration parameters safely
    if (Size > 0) cfg->g_threads = Data[0] % 16 + 1;
    if (Size > 1) cfg->g_profile = Data[1] % 4;
    if (Size > 2) cfg->g_w = (Data[2] * 16) + 16; // Ensure minimum size
    if (Size > 3) cfg->g_h = (Data[3] * 16) + 16;
    if (Size > 4) cfg->g_limit = Data[4] + 1;
    if (Size > 5) cfg->g_bit_depth = (Data[5] % 3 == 0) ? AOM_BITS_8 : ((Data[5] % 3 == 1) ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 6) cfg->g_input_bit_depth = cfg->g_bit_depth;
    
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (Size > 7) ? (Data[7] + 1) : 30;
    
    if (Size > 8) cfg->rc_target_bitrate = (Data[8] * 100) + 100;
    if (Size > 9) cfg->rc_min_quantizer = Data[9] % 64;
    if (Size > 10) cfg->rc_max_quantizer = (Data[10] % 63) + cfg->rc_min_quantizer;
    
    cfg->rc_end_usage = (Size > 11) ? (enum aom_rc_mode)(Data[11] % 4) : AOM_VBR;
    cfg->kf_mode = (Size > 12) ? (enum aom_kf_mode)(Data[12] % 3) : AOM_KF_DISABLED;
    if (Size > 13) cfg->kf_min_dist = Data[13] + 1;
    if (Size > 14) cfg->kf_max_dist = (Data[14] * 10) + cfg->kf_min_dist;
    
    // Initialize cfg_options
    memset(&cfg->encoder_cfg, 0, sizeof(cfg_options_t));
    if (Size > 15) cfg->encoder_cfg.super_block_size = (Data[15] % 3 == 0) ? 64 : ((Data[15] % 3 == 1) ? 128 : 0);
}

static void init_decoder_cfg_fuzz(aom_codec_dec_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(aom_codec_dec_cfg_t));
    if (Size > 0) cfg->threads = Data[0] % 16 + 1;
    if (Size > 1) cfg->w = (Data[1] * 16) + 16;
    if (Size > 2) cfg->h = (Data[2] * 16) + 16;
    if (Size > 3) cfg->allow_lowbitdepth = Data[3] % 2;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Get interfaces
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    
    // Test aom_codec_iface_name with valid and NULL pointers
    if (enc_iface) {
        const char *name = aom_codec_iface_name(enc_iface);
        (void)name; // Suppress unused warning, just exercising the path
    }
    const char *invalid_name = aom_codec_iface_name(NULL);
    (void)invalid_name;

    // Test aom_codec_get_caps with valid and NULL pointers
    if (enc_iface) {
        aom_codec_caps_t caps = aom_codec_get_caps(enc_iface);
        (void)caps;
    }
    aom_codec_caps_t null_caps = aom_codec_get_caps(NULL);
    (void)null_caps;

    // Prepare Encoder Context
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_enc_cfg_t enc_cfg;
    init_encoder_cfg_fuzz(&enc_cfg, Data, Size);
    
    // Initialize Encoder
    aom_codec_err_t enc_res = aom_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // If init failed, we don't need to destroy, but we can try with different flags
    if (enc_res != AOM_CODEC_OK) {
        // Try again with different flags if data allows
        if (Size > 20) {
            aom_codec_flags_t flags = (aom_codec_flags_t)Data[20];
            aom_codec_err_t enc_res2 = aom_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, flags, AOM_ENCODER_ABI_VERSION);
            (void)enc_res2;
        }
    } else {
        // Cleanup encoder if initialized successfully
        aom_codec_destroy(&enc_ctx);
    }

    // Prepare Decoder Context
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_dec_cfg_t dec_cfg;
    init_decoder_cfg_fuzz(&dec_cfg, Data, Size);
    
    // Initialize Decoder
    aom_codec_err_t dec_res = aom_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_res != AOM_CODEC_OK) {
        // Try with different flags
        if (Size > 21) {
            aom_codec_flags_t flags = (aom_codec_flags_t)Data[21];
            aom_codec_err_t dec_res2 = aom_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, flags, AOM_DECODER_ABI_VERSION);
            (void)dec_res2;
        }
    } else {
        // Cleanup decoder if initialized successfully
        aom_codec_destroy(&dec_ctx);
    }

    // Test with NULL interface pointers to trigger error handling paths
    aom_codec_err_t null_enc_res = aom_codec_enc_init_ver(&enc_ctx, NULL, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    (void)null_enc_res;
    
    aom_codec_err_t null_dec_res = aom_codec_dec_init_ver(&dec_ctx, NULL, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    (void)null_dec_res;

    // Test with NULL context pointers
    aom_codec_err_t null_ctx_enc_res = aom_codec_enc_init_ver(NULL, enc_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    (void)null_ctx_enc_res;
    
    aom_codec_err_t null_ctx_dec_res = aom_codec_dec_init_ver(NULL, dec_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    (void)null_ctx_dec_res;

    // Test with NULL config pointers (allowed for decoder)
    aom_codec_err_t null_cfg_dec_res = aom_codec_dec_init_ver(&dec_ctx, dec_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
    if (null_cfg_dec_res == AOM_CODEC_OK) {
        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}