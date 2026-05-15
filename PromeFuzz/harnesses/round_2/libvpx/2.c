// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface functions that are typically in vp8dx.h
extern vpx_codec_iface_t vpx_codec_vp8_dx_algo;
extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Use the VP8 decoder interface
    vpx_codec_iface_t *iface = &vpx_codec_vp8_dx_algo;
    
    // Call vpx_codec_iface_name
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Avoid unused variable warning
    
    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    // Initialize decoder with version check
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result == VPX_CODEC_OK) {
        // Decode the input data
        vpx_codec_err_t decode_result = vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
        (void)decode_result; // Avoid unused variable warning
        
        // Get decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Do nothing with the frame, just iterate through them
            (void)img;
        }
        
        // Destroy the decoder context
        vpx_codec_err_t destroy_result = vpx_codec_destroy(&ctx);
        (void)destroy_result; // Avoid unused variable warning
    }
    
    // Test with NULL configuration
    vpx_codec_ctx_t ctx_null_cfg;
    vpx_codec_err_t init_null = vpx_codec_dec_init_ver(&ctx_null_cfg, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (init_null == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx_null_cfg, Data, (unsigned int)Size, NULL, 0);
        vpx_codec_iter_t iter_null = NULL;
        while (vpx_codec_get_frame(&ctx_null_cfg, &iter_null) != NULL) {
            // Iterate
        }
        vpx_codec_destroy(&ctx_null_cfg);
    }
    
    // Test with different ABI versions (should fail but we want to test the code path)
    vpx_codec_ctx_t ctx_wrong_ver;
    vpx_codec_err_t init_wrong = vpx_codec_dec_init_ver(&ctx_wrong_ver, iface, &cfg, 0, 0);
    if (init_wrong == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx_wrong_ver, Data, (unsigned int)Size, NULL, 0);
        vpx_codec_destroy(&ctx_wrong_ver);
    }
    
    // Test with empty input data
    vpx_codec_ctx_t ctx_empty;
    if (vpx_codec_dec_init_ver(&ctx_empty, iface, &cfg, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx_empty, NULL, 0, NULL, 0); // Flush decoder
        vpx_codec_iter_t iter_empty = NULL;
        while (vpx_codec_get_frame(&ctx_empty, &iter_empty) != NULL) {
            // Iterate
        }
        vpx_codec_destroy(&ctx_empty);
    }
    
    // Also test with VP9 decoder interface
    vpx_codec_iface_t *iface_vp9 = &vpx_codec_vp9_dx_algo;
    vpx_codec_ctx_t ctx_vp9;
    if (vpx_codec_dec_init_ver(&ctx_vp9, iface_vp9, &cfg, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx_vp9, Data, (unsigned int)Size, NULL, 0);
        vpx_codec_iter_t iter_vp9 = NULL;
        while (vpx_codec_get_frame(&ctx_vp9, &iter_vp9) != NULL) {
            // Iterate
        }
        vpx_codec_destroy(&ctx_vp9);
    }
    
    return 0;
}