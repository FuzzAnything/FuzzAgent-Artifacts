// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static void dummy_user_priv(void) {
    // Empty function for user_priv callback
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Use first byte to determine flags
    aom_codec_flags_t flags = (Data[0] & 0x0F);
    
    // Use second byte to determine if we should use config
    int use_config = (Size > 1) ? (Data[1] & 0x01) : 0;
    
    aom_codec_dec_cfg_t cfg;
    aom_codec_dec_cfg_t *cfg_ptr = NULL;
    
    if (use_config && Size > 5) {
        // Initialize config with some values from fuzzer input
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (Data[2] & 0x3F) + 1;  // 1-64 threads
        cfg.w = (Data[3] << 8) | Data[4];
        cfg.h = (Data[5] << 8) | Data[6];
        if (cfg.w == 0) cfg.w = 128;
        if (cfg.h == 0) cfg.h = 128;
        cfg_ptr = &cfg;
    }

    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, cfg_ptr, flags, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Initialization failed, clean up and return
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Try to set some options based on fuzzer input
    if (Size > 10) {
        // Use some bytes to create option key-value pairs
        char key[32];
        char value[32];
        
        // First option
        snprintf(key, sizeof(key), "option_%u", Data[7] & 0x7F);
        snprintf(value, sizeof(value), "%u", Data[8]);
        aom_codec_set_option(&ctx, key, value);
        
        // Second option if we have enough data
        if (Size > 20) {
            snprintf(key, sizeof(key), "opt_%u", Data[9] & 0x3F);
            snprintf(value, sizeof(value), "val_%u", Data[10]);
            aom_codec_set_option(&ctx, key, value);
        }
    }

    // Try some control operations
    if (Size > 15) {
        // Use a byte for control ID
        int ctrl_id = Data[11] & 0x7F;
        if (ctrl_id != 0) {
            // Try different parameter types based on input
            if ((Data[12] & 0x01)) {
                int param = (Data[13] << 8) | Data[14];
                aom_codec_control(&ctx, ctrl_id, param);
            } else {
                void *param = NULL;
                aom_codec_control(&ctx, ctrl_id, param);
            }
        }
    }

    // Calculate offset for decode data
    size_t data_offset = (Size > 30) ? 30 : Size/2;
    if (data_offset >= Size) {
        data_offset = Size/2;
    }
    
    const uint8_t *decode_data = Data + data_offset;
    size_t decode_size = Size - data_offset;
    
    if (decode_size > 0) {
        // Decode the data
        err = aom_codec_decode(&ctx, decode_data, decode_size, &dummy_user_priv);
        
        // Try to get stream info after decode
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        
        aom_codec_get_stream_info(&ctx, &si);
        
        // Try another decode with remaining data (if any)
        if (decode_size > 10) {
            // Decode a subset of the data
            aom_codec_decode(&ctx, decode_data, decode_size/2, NULL);
            
            // Get stream info again
            aom_codec_get_stream_info(&ctx, &si);
        }
    }

    // Clean up
    aom_codec_destroy(&ctx);

    return 0;
}