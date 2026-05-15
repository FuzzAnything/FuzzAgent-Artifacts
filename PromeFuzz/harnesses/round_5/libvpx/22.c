// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface functions that are missing from the headers
extern vpx_codec_iface_t* vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t* vpx_codec_vp9_dx(void);

static void fuzz_decoder_init(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = NULL;
    
    // Choose decoder interface based on input data
    if (size > 0) {
        if (data[0] % 2 == 0) {
            iface = vpx_codec_vp8_dx();
        } else {
            iface = vpx_codec_vp9_dx();
        }
    } else {
        iface = vpx_codec_vp8_dx();
    }
    
    vpx_codec_dec_cfg_t cfg;
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        // Ensure reasonable values to avoid excessive memory allocation
        cfg.threads = cfg.threads % 16 + 1;
        cfg.w = cfg.w % 4096 + 1;
        cfg.h = cfg.h % 4096 + 1;
    } else {
        cfg.threads = 1;
        cfg.w = 320;
        cfg.h = 240;
    }
    
    vpx_codec_flags_t flags = 0;
    if (size > 0) {
        flags = (data[0] & 0x01) ? VPX_CODEC_USE_POSTPROC : 0;
    }
    
    // Initialize decoder with different ABI versions to test validation
    int abi_version = VPX_DECODER_ABI_VERSION;
    if (size > 1) {
        // Test with potentially invalid ABI versions
        abi_version = data[1];
    }
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, abi_version);
    
    // Test error handling functions regardless of initialization result
    const char *error_str = vpx_codec_error(&ctx);
    const char *error_detail = vpx_codec_error_detail(&ctx);
    const char *err_to_str = vpx_codec_err_to_string(init_result);
    
    // Test frame iterator even if decoder not fully initialized
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
    
    // Clean up if initialization was successful
    if (init_result == VPX_CODEC_OK) {
        vpx_codec_err_t destroy_result = vpx_codec_destroy(&ctx);
        // Test error conversion for destroy result too
        const char *destroy_err_str = vpx_codec_err_to_string(destroy_result);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test with different configurations
    for (int i = 0; i < 2; i++) {
        fuzz_decoder_init(data, size);
    }
    
    return 0;
}