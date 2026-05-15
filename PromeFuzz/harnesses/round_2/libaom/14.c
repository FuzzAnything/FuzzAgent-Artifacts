// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static aom_codec_ctx_t g_ctx;
static const aom_codec_iface_t *g_iface = NULL;
static int g_initialized = 0;

static void initialize_once(void) {
    if (g_iface == NULL) {
        g_iface = aom_codec_av1_dx();
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    initialize_once();
    
    if (g_iface == NULL) {
        return 0;
    }
    
    // Initialize decoder if not already initialized
    if (!g_initialized) {
        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        
        aom_codec_err_t init_err = aom_codec_dec_init_ver(&g_ctx, g_iface, &cfg, 0, 
                                                          AOM_DECODER_ABI_VERSION);
        if (init_err == AOM_CODEC_OK) {
            g_initialized = 1;
        } else {
            return 0;
        }
    }
    
    // Test aom_codec_set_option with fuzzed key-value pairs
    if (Size >= 2) {
        // Split data into key and value parts
        size_t key_len = Data[0] % (Size - 1);
        if (key_len == 0) key_len = 1;
        
        size_t value_len = Size - key_len - 1;
        if (value_len > 0) {
            char *key = (char *)malloc(key_len + 1);
            char *value = (char *)malloc(value_len + 1);
            
            if (key && value) {
                memcpy(key, Data + 1, key_len);
                key[key_len] = '\0';
                
                memcpy(value, Data + 1 + key_len, value_len);
                value[value_len] = '\0';
                
                aom_codec_set_option(&g_ctx, key, value);
                
                free(key);
                free(value);
            }
        }
    }
    
    // Test aom_codec_decode with fuzzed data
    if (Size > 0) {
        aom_codec_decode(&g_ctx, Data, Size, NULL);
    }
    
    // Test aom_codec_get_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&g_ctx, &si);
    
    // Test aom_codec_control with various control IDs
    if (Size >= 4) {
        int ctrl_id = 0;
        memcpy(&ctrl_id, Data, sizeof(int));
        
        int ctrl_arg = 0;
        if (Size >= 8) {
            memcpy(&ctrl_arg, Data + 4, sizeof(int));
        }
        
        // Only try control if ID is non-zero
        if (ctrl_id != 0) {
            aom_codec_control(&g_ctx, ctrl_id, ctrl_arg);
        }
    }
    
    // Occasionally destroy and reinitialize to test different states
    if (Size > 0 && (Data[0] % 100 == 0)) {
        aom_codec_destroy(&g_ctx);
        g_initialized = 0;
        
        // Reinitialize
        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        
        aom_codec_err_t init_err = aom_codec_dec_init_ver(&g_ctx, g_iface, &cfg, 0,
                                                          AOM_DECODER_ABI_VERSION);
        if (init_err == AOM_CODEC_OK) {
            g_initialized = 1;
        }
    }
    
    return 0;
}