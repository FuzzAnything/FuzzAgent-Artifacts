// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"

// Declare the AV1 encoder interface function
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static void dummy_callback(void *user_priv, const aom_codec_cx_pkt_t *pkt) {
    (void)user_priv;
    (void)pkt;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Test aom_codec_err_to_string with various error codes
    for (int i = 0; i < 10; i++) {
        const char *err_string = aom_codec_err_to_string((aom_codec_err_t)i);
        (void)err_string;
    }

    // Test with NULL context
    const char *null_error = aom_codec_error(NULL);
    const char *null_detail = aom_codec_error_detail(NULL);
    (void)null_error;
    (void)null_detail;
    
    aom_codec_destroy(NULL);

    // Try to initialize encoder if we have enough data
    if (Size > 50) {
        // Create encoder configuration
        aom_codec_enc_cfg_t cfg;
        aom_codec_iface_t *iface = aom_codec_av1_cx();
        
        if (iface != NULL) {
            aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, 0);
            
            if (err == AOM_CODEC_OK) {
                // Set some basic configuration from fuzzer input
                cfg.g_w = 64 + (Data[0] % 192);
                cfg.g_h = 64 + (Data[1] % 192);
                cfg.g_timebase.num = 1;
                cfg.g_timebase.den = 30;
                cfg.rc_target_bitrate = 1000;
                cfg.g_pass = AOM_RC_ONE_PASS;
                cfg.g_lag_in_frames = 0;

                // Test aom_codec_enc_init_ver
                err = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
                
                if (err == AOM_CODEC_OK) {
                    // Test aom_codec_set_option with fuzzer data
                    if (Size > 4) {
                        size_t name_len = Data[2] % 32;
                        if (name_len > 0 && name_len < Size - 5) {
                            char name[33];
                            size_t value_len = Data[3] % 32;
                            
                            if (name_len + value_len + 5 < Size) {
                                memcpy(name, Data + 4, name_len);
                                name[name_len] = '\0';
                                
                                char value[33];
                                memcpy(value, Data + 4 + name_len, value_len);
                                value[value_len] = '\0';
                                
                                aom_codec_set_option(&ctx, name, value);
                            }
                        }
                    }

                    // Test aom_codec_error and aom_codec_error_detail
                    const char *error_str = aom_codec_error(&ctx);
                    const char *error_detail = aom_codec_error_detail(&ctx);
                    (void)error_str;
                    (void)error_detail;

                    // Test aom_codec_destroy
                    aom_codec_destroy(&ctx);
                } else {
                    // Test error functions when init fails
                    const char *error_str = aom_codec_error(&ctx);
                    const char *error_detail = aom_codec_error_detail(&ctx);
                    (void)error_str;
                    (void)error_detail;
                }
            }
        }
    }

    return 0;
}