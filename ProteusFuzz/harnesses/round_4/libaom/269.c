// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_269(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test aom_codec_err_to_string with various error codes
    // Using valid enum values defined in aom_codec.h
    aom_codec_err_t safe_err_codes[] = {
        AOM_CODEC_OK,
        AOM_CODEC_ERROR,
        AOM_CODEC_MEM_ERROR,
        AOM_CODEC_INVALID_PARAM,
        AOM_CODEC_UNSUP_BITSTREAM,
        AOM_CODEC_UNSUP_FEATURE,
        AOM_CODEC_CORRUPT_FRAME
    };
    const int num_safe_err_codes = 7;
    
    for (int i = 0; i < num_safe_err_codes; i++) {
        const char *err_str = aom_codec_err_to_string(safe_err_codes[i]);
        (void)err_str; // Use the string to avoid unused variable warning
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;

    aom_codec_err_t ret = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
        return 0;
    }

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&enc_ctx);
    if (global_headers) {
        if (global_headers->buf) {
            // Process the buffer if needed
        }
        free(global_headers->buf);
        free(global_headers);
    }

    // Fuzz aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER (if available)
    int quantizer = 0;
    ret = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS (if available)
    int quantizer_one_pass = (Data[0] % 64);
    ret = aom_codec_control(&enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, &quantizer_one_pass);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_STRENGTH (if available)
    int deltaq_strength = (Size > 1 ? Data[1] % 64 : 0);
    ret = aom_codec_control(&enc_ctx, AV1E_SET_DELTAQ_STRENGTH, &deltaq_strength);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
    }

    // Cleanup encoder
    cleanup_encoder(&enc_ctx);

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    aom_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX (if available)
    int base_q_idx = 0;
    ret = aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    if (ret != AOM_CODEC_OK) {
        aom_codec_err_to_string(ret);
    }

    // Cleanup decoder
    cleanup_decoder(&dec_ctx);

    return 0;
}