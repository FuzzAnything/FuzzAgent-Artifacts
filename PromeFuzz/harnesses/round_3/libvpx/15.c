// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
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
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_init_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    
    // Initialize configuration with fuzzer data
    vpx_codec_dec_cfg_t cfg;
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        data += sizeof(cfg);
        size -= sizeof(cfg);
    } else {
        cfg.threads = 1;
        cfg.w = 640;
        cfg.h = 480;
    }
    
    // Use fuzzer data for flags if available
    vpx_codec_flags_t flags = 0;
    if (size >= sizeof(flags)) {
        memcpy(&flags, data, sizeof(flags));
        data += sizeof(flags);
        size -= sizeof(flags);
    }
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    
    // Call error functions regardless of initialization result
    const char *error_str = vpx_codec_error(&ctx);
    const char *error_detail = vpx_codec_error_detail(&ctx);
    const char *err_to_str = vpx_codec_err_to_string(err);
    
    // Try to get frames even if initialization failed (should return NULL)
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *frame;
    while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Access some frame properties to explore more states
        if (frame->fmt != VPX_IMG_FMT_NONE) {
            // Touch some fields
            (void)frame->w;
            (void)frame->h;
            (void)frame->d_w;
            (void)frame->d_h;
        }
    }
    
    // Clean up if initialization was successful
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }
}

static void fuzz_error_functions(const uint8_t *data, size_t size) {
    // Test error functions with various inputs
    for (size_t i = 0; i < size && i < 100; i++) {
        vpx_codec_err_t err = (vpx_codec_err_t)data[i];
        const char *str = vpx_codec_err_to_string(err);
        (void)str; // Use result to avoid unused variable warning
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Seed random number generator with fuzzer data for diverse execution
    unsigned int seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
        srand(seed);
    }
    
    // Call decoder initialization with different configurations
    fuzz_init_decoder(Data, Size);
    
    // Also test error functions separately
    fuzz_error_functions(Data, Size);
    
    return 0;
}