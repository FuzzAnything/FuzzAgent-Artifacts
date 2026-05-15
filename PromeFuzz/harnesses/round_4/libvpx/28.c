// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

static void fuzz_vpx_codec_get_frame(vpx_codec_ctx_t *ctx) {
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    
    while ((img = vpx_codec_get_frame(ctx, &iter)) != NULL) {
        // Access some fields to potentially trigger issues
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->planes[0];
        (void)img->stride[0];
    }
}

static void fuzz_vpx_codec_control(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Try different control IDs based on fuzzed input
    int ctrl_id = data[0] % 256;
    
    // Use vpx_codec_control_ directly with appropriate arguments
    // We'll try different argument patterns based on the control ID
    
    // Pattern 1: No additional arguments
    if (ctrl_id % 3 == 0) {
        vpx_codec_control_(ctx, ctrl_id);
    }
    // Pattern 2: One integer argument
    else if (ctrl_id % 3 == 1) {
        int int_arg = 0;
        vpx_codec_control_(ctx, ctrl_id, &int_arg);
    }
    // Pattern 3: Two integer arguments
    else {
        int int_arg1 = 0, int_arg2 = 0;
        vpx_codec_control_(ctx, ctrl_id, &int_arg1, &int_arg2);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Initialize VP9 decoder
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) return 0;
    
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Check error functions even on failure
        const char *error_str = vpx_codec_error(&ctx);
        const char *detail = vpx_codec_error_detail(&ctx);
        const char *err_to_str = vpx_codec_err_to_string(err);
        (void)error_str;
        (void)detail;
        (void)err_to_str;
        return 0;
    }
    
    // Decode the fuzzed data
    err = vpx_codec_decode(&ctx, data, size, NULL, 0);
    
    // Always check error functions
    const char *error_str = vpx_codec_error(&ctx);
    const char *detail = vpx_codec_error_detail(&ctx);
    const char *err_to_str = vpx_codec_err_to_string(err);
    (void)error_str;
    (void)detail;
    (void)err_to_str;
    
    // Only iterate frames if decode was successful
    if (err == VPX_CODEC_OK) {
        fuzz_vpx_codec_get_frame(&ctx);
    }
    
    // Try codec control with fuzzed data
    fuzz_vpx_codec_control(&ctx, data, size);
    
    // Clean up
    vpx_codec_destroy(&ctx);
    
    return 0;
}