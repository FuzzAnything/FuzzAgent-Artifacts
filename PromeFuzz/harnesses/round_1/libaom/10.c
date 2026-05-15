// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Part 1: Test aom_codec_peek_stream_info without full decoder setup
    {
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        
        // Initialize is_annexb as required by the API
        si.is_annexb = (Data[0] & 0x01);
        
        aom_codec_err_t err = aom_codec_peek_stream_info(
            aom_codec_av1_dx(),
            Data,
            Size,
            &si
        );
        
        // Ignore return value - we're fuzzing, not validating functionality
        (void)err;
    }

    // Part 2: Test full decoder initialization and operations
    {
        aom_codec_ctx_t ctx;
        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        
        // Use some fuzz data to configure decoder
        if (Size >= 4) {
            cfg.w = (Data[0] << 8) | Data[1];
            cfg.h = (Data[2] << 8) | Data[3];
            cfg.threads = Data[0] % 8 + 1;
        } else {
            cfg.w = 128;
            cfg.h = 128;
            cfg.threads = 1;
        }
        
        // Initialize decoder with version checking
        aom_codec_err_t init_err = aom_codec_dec_init_ver(
            &ctx,
            aom_codec_av1_dx(),
            &cfg,
            0,
            AOM_DECODER_ABI_VERSION
        );
        
        if (init_err == AOM_CODEC_OK) {
            // Try to decode the data
            aom_codec_err_t decode_err = aom_codec_decode(
                &ctx,
                Data,
                Size,
                NULL
            );
            (void)decode_err;
            
            // Try to get stream info after decoding
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            si.is_annexb = (Data[0] & 0x01);
            
            aom_codec_err_t stream_info_err = aom_codec_get_stream_info(
                &ctx,
                &si
            );
            (void)stream_info_err;
            
            // Destroy the codec instance
            aom_codec_err_t destroy_err = aom_codec_destroy(&ctx);
            (void)destroy_err;
        } else {
            // Test aom_codec_destroy on uninitialized/partially initialized context
            // Create a context with some random data
            aom_codec_ctx_t bad_ctx;
            memset(&bad_ctx, 0, sizeof(bad_ctx));
            
            // Corrupt the context with fuzz data
            if (Size > 0) {
                memcpy(&bad_ctx, Data, Size > sizeof(bad_ctx) ? sizeof(bad_ctx) : Size);
            }
            
            aom_codec_err_t destroy_err = aom_codec_destroy(&bad_ctx);
            (void)destroy_err;
        }
    }

    // Part 3: Test edge cases with different configurations
    if (Size >= 2) {
        aom_codec_ctx_t ctx;
        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        
        // Try different thread counts based on fuzz data
        cfg.threads = Data[1] % 16 + 1;
        cfg.w = 64;
        cfg.h = 64;
        
        // Try with different flags - use raw fuzz data for flags
        aom_codec_flags_t flags = (aom_codec_flags_t)Data[0];
        
        aom_codec_err_t init_err = aom_codec_dec_init_ver(
            &ctx,
            aom_codec_av1_dx(),
            &cfg,
            flags,
            AOM_DECODER_ABI_VERSION
        );
        
        if (init_err == AOM_CODEC_OK) {
            // Try decoding with different user_priv pointers
            void *user_priv = NULL;
            if (Size > 8) {
                user_priv = (void *)(uintptr_t)Data[4];
            }
            
            aom_codec_err_t decode_err = aom_codec_decode(
                &ctx,
                Data,
                Size,
                user_priv
            );
            (void)decode_err;
            
            // Destroy
            aom_codec_destroy(&ctx);
        }
    }

    // Part 4: Test with NULL pointers and edge cases
    {
        // Test aom_codec_destroy with NULL context
        aom_codec_err_t err = aom_codec_destroy(NULL);
        (void)err;
        
        // Test aom_codec_peek_stream_info with NULL pointers
        if (Size > 10) {
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            si.is_annexb = 1;
            
            // Test with NULL iface
            aom_codec_err_t err1 = aom_codec_peek_stream_info(
                NULL,
                Data,
                Size,
                &si
            );
            (void)err1;
            
            // Test with NULL data but non-zero size
            aom_codec_err_t err2 = aom_codec_peek_stream_info(
                aom_codec_av1_dx(),
                NULL,
                Size,
                &si
            );
            (void)err2;
            
            // Test with NULL stream info
            aom_codec_err_t err3 = aom_codec_peek_stream_info(
                aom_codec_av1_dx(),
                Data,
                Size,
                NULL
            );
            (void)err3;
        }
    }

    return 0;
}