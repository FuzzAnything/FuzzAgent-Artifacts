// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_with_config(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    const char *err_str;
    const char *err_detail;

    // Initialize config with defaults
    res = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz config fields based on input data
    if (size > 0) cfg.g_w = data[0] + 1;
    if (size > 1) cfg.g_h = data[1] + 1;
    if (size > 2) cfg.g_threads = data[2] % 16;
    if (size > 3) cfg.g_profile = data[3] % 4;
    if (size > 4) {
        int bd = data[4] % 3;
        if (bd == 0) cfg.g_bit_depth = AOM_BITS_8;
        else if (bd == 1) cfg.g_bit_depth = AOM_BITS_10;
        else cfg.g_bit_depth = AOM_BITS_12;
    } else {
        cfg.g_bit_depth = AOM_BITS_8;
    }
    
    cfg.g_timebase.num = 1;
    if (size > 5) cfg.g_timebase.den = data[5] + 1;
    
    if (size > 6) {
        cfg.rc_end_usage = (enum aom_rc_mode)(data[6] % 4);
    } else {
        cfg.rc_end_usage = AOM_VBR;
    }
    
    if (size > 7) cfg.rc_target_bitrate = data[7] * 100 + 100;
    if (size > 8) cfg.rc_min_quantizer = data[8] % 64;
    if (size > 9) cfg.rc_max_quantizer = (data[9] % 64) + cfg.rc_min_quantizer;

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        err_str = aom_codec_error(&ctx);
        err_detail = aom_codec_error_detail(&ctx);
        (void)err_str;
        (void)err_detail;
        return 0;
    }

    // Test aom_codec_enc_config_set
    res = aom_codec_enc_config_set(&ctx, &cfg);
    if (res != AOM_CODEC_OK) {
        err_str = aom_codec_error(&ctx);
        (void)err_str;
    }

    // Test aom_codec_error and aom_codec_err_to_string
    err_str = aom_codec_error(&ctx);
    const char *err_code_str = aom_codec_err_to_string(res);
    (void)err_str;
    (void)err_code_str;

    // Cleanup
    aom_codec_destroy(&ctx);
    return 1;
}

static int fuzz_error_handling(void) {
    const char *err_str;
    const char *err_code_str;
    
    // Test with NULL context
    err_str = aom_codec_error(NULL);
    (void)err_str;

    // Test error string conversion with various codes
    err_code_str = aom_codec_err_to_string(AOM_CODEC_OK);
    err_code_str = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    err_code_str = aom_codec_err_to_string(AOM_CODEC_INCAPABLE);
    err_code_str = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    err_code_str = aom_codec_err_to_string((aom_codec_err_t)9999); // Unrecognized
    (void)err_code_str;

    return 1;
}

int LLVMFuzzerTestOneInput_252(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return fuzz_error_handling();
    }

    // Use the first byte to decide which path to take
    if (Data[0] % 2 == 0) {
        fuzz_encoder_with_config(Data, Size);
    } else {
        fuzz_error_handling();
        // Also run encoder with potentially malformed data if size allows
        if (Size > 1) {
            fuzz_encoder_with_config(Data + 1, Size - 1);
        }
    }

    return 0;
}