// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"

static void init_dummy_encoder_config(aom_codec_enc_cfg_t *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_error_resilient = 0;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->g_profile = 0;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_threads = 1;
}

static void init_dummy_codec_ctx(aom_codec_ctx_t *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->name = "dummy";
    ctx->err = AOM_CODEC_OK;
    ctx->err_detail = NULL;
    ctx->init_flags = 0;
    ctx->priv = NULL;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Test aom_codec_err_to_string with various error codes
    if (selector & 0x01) {
        // Use remaining data to generate error codes
        for (size_t i = data_offset; i < Size && i < data_offset + 10; i++) {
            aom_codec_err_t err = (aom_codec_err_t)Data[i];
            const char *err_str = aom_codec_err_to_string(err);
            (void)err_str; // Use result to avoid optimization
        }
    }
    
    // Test aom_codec_destroy
    if (selector & 0x02) {
        aom_codec_ctx_t ctx;
        init_dummy_codec_ctx(&ctx);
        
        // Test with various contexts
        aom_codec_err_t destroy_result = aom_codec_destroy(&ctx);
        (void)destroy_result;
        
        // Test with NULL
        if (Size > data_offset && (Data[data_offset] & 0x01)) {
            aom_codec_destroy(NULL);
        }
    }
    
    // Test aom_codec_error_detail
    if (selector & 0x04) {
        aom_codec_ctx_t ctx;
        init_dummy_codec_ctx(&ctx);
        
        // Set some error details if we have enough data
        if (Size > data_offset + 10) {
            ctx.err_detail = (const char *)&Data[data_offset];
        }
        
        const char *detail = aom_codec_error_detail(&ctx);
        (void)detail;
        
        // Test with NULL context
        if (Size > data_offset + 1 && (Data[data_offset + 1] & 0x01)) {
            aom_codec_error_detail(NULL);
        }
    }
    
    // Test aom_codec_decode
    if (selector & 0x08) {
        aom_codec_ctx_t ctx;
        init_dummy_codec_ctx(&ctx);
        
        // Initialize decoder if we have enough data
        if (Size > data_offset + 100) {
            // Get AV1 decoder interface
            extern aom_codec_iface_t aom_codec_av1_dx_algo;
            aom_codec_iface_t *decoder = &aom_codec_av1_dx_algo;
            
            if (decoder) {
                // Try to initialize decoder with default config
                aom_codec_dec_cfg_t cfg = {0};
                cfg.threads = 1;
                
                if (aom_codec_dec_init(&ctx, decoder, &cfg, 0) == AOM_CODEC_OK) {
                    // Use remaining data for decode
                    size_t decode_size = Size - data_offset;
                    if (decode_size > 0) {
                        const uint8_t *decode_data = &Data[data_offset];
                        aom_codec_err_t decode_result = aom_codec_decode(&ctx, 
                                                                        decode_data, 
                                                                        decode_size, 
                                                                        NULL);
                        (void)decode_result;
                        
                        // Test error functions after decode
                        const char *error_str = aom_codec_error(&ctx);
                        (void)error_str;
                        const char *error_detail = aom_codec_error_detail(&ctx);
                        (void)error_detail;
                    }
                    
                    // Clean up
                    aom_codec_destroy(&ctx);
                }
            }
        }
    }
    
    // Test aom_codec_error
    if (selector & 0x10) {
        aom_codec_ctx_t ctx;
        init_dummy_codec_ctx(&ctx);
        
        // Set different error states
        if (Size > data_offset) {
            ctx.err = (aom_codec_err_t)Data[data_offset];
        }
        
        const char *error_str = aom_codec_error(&ctx);
        (void)error_str;
        
        // Test with NULL context
        if (Size > data_offset + 1 && (Data[data_offset + 1] & 0x01)) {
            aom_codec_error(NULL);
        }
    }
    
    // Test aom_codec_enc_init_ver
    if (selector & 0x20) {
        aom_codec_ctx_t ctx;
        init_dummy_codec_ctx(&ctx);
        
        aom_codec_enc_cfg_t cfg;
        init_dummy_encoder_config(&cfg);
        
        // Modify config with fuzzer data if available
        if (Size > data_offset) {
            size_t cfg_offset = 0;
            for (size_t i = data_offset; i < Size && cfg_offset < sizeof(cfg); i++) {
                ((uint8_t *)&cfg)[cfg_offset++] = Data[i];
            }
        }
        
        // Get AV1 encoder interface
        extern aom_codec_iface_t aom_codec_av1_cx_algo;
        aom_codec_iface_t *encoder = &aom_codec_av1_cx_algo;
        
        if (encoder) {
            aom_codec_err_t init_result = aom_codec_enc_init_ver(&ctx, 
                                                                encoder, 
                                                                &cfg, 
                                                                0, 
                                                                AOM_ENCODER_ABI_VERSION);
            (void)init_result;
            
            // If initialization succeeded, test other functions
            if (init_result == AOM_CODEC_OK) {
                const char *error_str = aom_codec_error(&ctx);
                (void)error_str;
                
                // Clean up
                aom_codec_destroy(&ctx);
            }
        }
    }
    
    return 0;
}