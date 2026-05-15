// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"

// Declare the AV1 decoder and encoder interface functions
extern const aom_codec_iface_t *aom_codec_av1_dx(void);
extern const aom_codec_iface_t *aom_codec_av1_cx(void);

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Fuzz aom_codec_err_to_string
    {
        // Use first byte as error code
        aom_codec_err_t err = (aom_codec_err_t)Data[0];
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Use result to avoid unused variable warning
    }

    // Fuzz aom_codec_destroy with NULL context
    {
        aom_codec_destroy(NULL);
    }

    // Initialize decoder context for other functions
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Get decoder interface
    const aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (dec_iface && aom_codec_dec_init(&decoder, dec_iface, &dec_cfg, 0) == AOM_CODEC_OK) {
        // Fuzz aom_codec_decode
        {
            // Use fuzzer input as compressed data
            aom_codec_decode(&decoder, Data, Size, NULL);
        }

        // Fuzz aom_codec_control with various control IDs
        {
            // Use data bytes to generate control IDs
            for (size_t i = 0; i < Size && i < 10; i++) {
                int ctrl_id = Data[i];
                if (ctrl_id != 0) {
                    int value = (i < Size - 1) ? Data[i + 1] : 0;
                    aom_codec_control(&decoder, ctrl_id, value);
                }
            }
        }

        // Fuzz aom_codec_set_option
        {
            // Create key-value strings from fuzzer data
            char key[256] = {0};
            char value[256] = {0};
            
            size_t key_len = Size < 256 ? Size : 255;
            size_t val_len = Size < 512 ? (Size > 256 ? Size - 256 : 0) : 255;
            
            if (key_len > 0) {
                memcpy(key, Data, key_len);
                key[key_len] = '\0';
            }
            
            if (val_len > 0 && Size > 256) {
                memcpy(value, Data + 256, val_len);
                value[val_len] = '\0';
            }
            
            if (key_len > 0) {
                aom_codec_set_option(&decoder, key, value);
            }
        }

        // Fuzz aom_codec_get_stream_info
        {
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            aom_codec_get_stream_info(&decoder, &si);
        }

        // Clean up decoder
        aom_codec_destroy(&decoder);
    }

    // Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get encoder interface
    const aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (enc_iface && aom_codec_enc_config_default(enc_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        if (aom_codec_enc_init(&encoder, enc_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
            // Fuzz aom_codec_control on encoder
            {
                // Use data bytes to generate control IDs
                for (size_t i = 0; i < Size && i < 5; i++) {
                    int ctrl_id = Data[i] | 0x100; // Use higher range for encoder controls
                    if (ctrl_id != 0) {
                        int value = (i < Size - 1) ? Data[i + 1] : 0;
                        aom_codec_control(&encoder, ctrl_id, value);
                    }
                }
            }

            // Fuzz aom_codec_set_option on encoder
            {
                char key[256] = {0};
                char value[256] = {0};
                
                size_t offset = Size / 3;
                size_t key_len = (offset < 256) ? offset : 255;
                size_t val_len = (Size - offset < 256) ? (Size - offset) : 255;
                
                if (key_len > 0) {
                    memcpy(key, Data, key_len);
                    key[key_len] = '\0';
                }
                
                if (val_len > 0 && Size > offset) {
                    memcpy(value, Data + offset, val_len);
                    value[val_len] = '\0';
                }
                
                if (key_len > 0) {
                    aom_codec_set_option(&encoder, key, value);
                }
            }

            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }

    return 0;
}

#ifdef __cplusplus
}
#endif