// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test vpx_codec_peek_stream_info for both VP8 and VP9
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    // Try VP8
    vpx_codec_err_t err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), Data, (unsigned int)Size, &stream_info);
    (void)err; // Result can be ignored for fuzzing
    
    // Try VP9
    err = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), Data, (unsigned int)Size, &stream_info);
    (void)err;
    
    // Test vpx_codec_get_caps
    vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(vpx_codec_vp8_dx());
    vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(vpx_codec_vp9_dx());
    (void)caps_vp8;
    (void)caps_vp9;
    
    // Test decoder initialization and decoding for VP8
    vpx_codec_ctx_t ctx_vp8;
    vpx_codec_dec_cfg_t cfg_vp8;
    cfg_vp8.threads = 1;
    cfg_vp8.w = 640;
    cfg_vp8.h = 480;
    
    // Initialize VP8 decoder with different configurations
    for (int i = 0; i < 2; i++) {
        vpx_codec_flags_t flags = 0;
        if (i == 1) {
            flags = VPX_CODEC_USE_INPUT_FRAGMENTS;
        }
        
        err = vpx_codec_dec_init_ver(&ctx_vp8, vpx_codec_vp8_dx(), &cfg_vp8, flags, VPX_DECODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            // Decode the input data
            err = vpx_codec_decode(&ctx_vp8, Data, (unsigned int)Size, NULL, 0);
            (void)err;
            
            // Try decoding with NULL to flush
            err = vpx_codec_decode(&ctx_vp8, NULL, 0, NULL, 0);
            (void)err;
            
            vpx_codec_destroy(&ctx_vp8);
        }
    }
    
    // Test decoder initialization and decoding for VP9
    vpx_codec_ctx_t ctx_vp9;
    vpx_codec_dec_cfg_t cfg_vp9;
    cfg_vp9.threads = 2;
    cfg_vp9.w = 1280;
    cfg_vp9.h = 720;
    
    // Initialize VP9 decoder with different configurations
    for (int i = 0; i < 2; i++) {
        vpx_codec_flags_t flags = 0;
        if (i == 1) {
            flags = VPX_CODEC_USE_INPUT_FRAGMENTS | VPX_CODEC_USE_POSTPROC;
        }
        
        err = vpx_codec_dec_init_ver(&ctx_vp9, vpx_codec_vp9_dx(), &cfg_vp9, flags, VPX_DECODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            // Decode the input data in chunks to test fragmented input
            size_t chunk_size = Size / 4;
            if (chunk_size > 0) {
                for (size_t offset = 0; offset < Size; offset += chunk_size) {
                    size_t remaining = Size - offset;
                    size_t current_chunk = (remaining < chunk_size) ? remaining : chunk_size;
                    err = vpx_codec_decode(&ctx_vp9, Data + offset, (unsigned int)current_chunk, NULL, 0);
                    (void)err;
                }
            } else {
                err = vpx_codec_decode(&ctx_vp9, Data, (unsigned int)Size, NULL, 0);
                (void)err;
            }
            
            // Try decoding with NULL to flush
            err = vpx_codec_decode(&ctx_vp9, NULL, 0, NULL, 0);
            (void)err;
            
            vpx_codec_destroy(&ctx_vp9);
        }
    }
    
    // Test with NULL configuration
    vpx_codec_ctx_t ctx_null_cfg;
    err = vpx_codec_dec_init_ver(&ctx_null_cfg, vpx_codec_vp8_dx(), NULL, 0, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        err = vpx_codec_decode(&ctx_null_cfg, Data, (unsigned int)(Size > 100 ? 100 : Size), NULL, 0);
        (void)err;
        vpx_codec_destroy(&ctx_null_cfg);
    }
    
    return 0;
}