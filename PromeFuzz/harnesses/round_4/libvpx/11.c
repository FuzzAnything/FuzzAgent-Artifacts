// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    
    // Get VP8 decoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }
    
    // Initialize with zeros
    memset(&ctx, 0, sizeof(ctx));
    
    // Test vpx_codec_error with uninitialized context
    const char *error_str = vpx_codec_error(&ctx);
    (void)error_str;
    
    // Test vpx_codec_error_detail with uninitialized context
    const char *error_detail = vpx_codec_error_detail(&ctx);
    (void)error_detail;
    
    // Try to initialize decoder if we have enough data
    if (Size >= sizeof(vpx_codec_dec_cfg_t) + 4) {
        vpx_codec_dec_cfg_t cfg;
        memcpy(&cfg, Data, sizeof(cfg));
        
        // Ensure reasonable values
        cfg.threads = cfg.threads % 8 + 1;  // 1-8 threads
        cfg.w = cfg.w % 4096 + 1;          // 1-4096 width
        cfg.h = cfg.h % 4096 + 1;          // 1-4096 height
        
        // Initialize decoder
        vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        
        // Test error functions after init
        error_str = vpx_codec_error(&ctx);
        error_detail = vpx_codec_error_detail(&ctx);
        (void)error_str;
        (void)error_detail;
        
        // Try to decode some data if initialization succeeded
        if (init_err == VPX_CODEC_OK && Size > sizeof(cfg)) {
            size_t decode_size = Size - sizeof(cfg);
            if (decode_size > 1024) decode_size = 1024; // Limit size
            
            vpx_codec_err_t decode_err = vpx_codec_decode(&ctx, 
                Data + sizeof(cfg), 
                (unsigned int)decode_size, 
                NULL, 0);
            
            // Test error functions after decode attempt
            error_str = vpx_codec_error(&ctx);
            error_detail = vpx_codec_error_detail(&ctx);
            (void)error_str;
            (void)error_detail;
            
            // Try to get frames even if decode failed
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img = vpx_codec_get_frame(&ctx, &iter);
            (void)img;
            
            // Test vpx_codec_control_ with various control IDs
            // Use fuzzed data to generate control IDs
            if (Size >= sizeof(cfg) + 8) {
                int ctrl_id;
                memcpy(&ctrl_id, Data + sizeof(cfg), sizeof(int));
                
                // Try different argument types
                int int_arg = 0;
                if (Size >= sizeof(cfg) + 12) {
                    memcpy(&int_arg, Data + sizeof(cfg) + 4, sizeof(int));
                }
                
                // Call vpx_codec_control_ with the fuzzed control ID
                vpx_codec_control_(&ctx, ctrl_id, int_arg);
                
                // Test error functions after control call
                error_str = vpx_codec_error(&ctx);
                error_detail = vpx_codec_error_detail(&ctx);
                (void)error_str;
                (void)error_detail;
            }
        }
        
        // Clean up if initialized
        if (ctx.priv) {
            vpx_codec_destroy(&ctx);
        }
    }
    
    return 0;
}