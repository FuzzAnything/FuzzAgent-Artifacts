// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
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

static void fuzz_vpx_codec_get_caps(vpx_codec_iface_t *iface) {
    if (iface) {
        vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
        (void)caps; // Use caps to avoid unused variable warning
    }
}

static void fuzz_vpx_codec_dec_init_ver(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    
    // Initialize configuration with fuzzer data if available
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        // Ensure reasonable values
        cfg.threads = cfg.threads % 64; // Limit threads
        cfg.w = cfg.w % 8192; // Limit width
        cfg.h = cfg.h % 8192; // Limit height
    } else {
        cfg.threads = 1;
        cfg.w = 0;
        cfg.h = 0;
    }
    
    vpx_codec_flags_t flags = 0;
    if (size > 0) {
        flags = data[0] & 0x0F; // Use some bits for flags
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }
}

static void fuzz_vpx_codec_peek_stream_info(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(iface, data, (unsigned int)size, &si);
    (void)err; // Use err to avoid unused variable warning
}

static void fuzz_vpx_codec_decode(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    
    // Initialize decoder with default config
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err == VPX_CODEC_OK) {
        // Decode the data
        vpx_codec_err_t decode_err = vpx_codec_decode(&ctx, data, (unsigned int)size, NULL, 0);
        (void)decode_err; // Use decode_err to avoid unused variable warning
        
        // Flush decoder
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        vpx_codec_destroy(&ctx);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Get VP8 decoder interface
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_dx();
    if (vp8_iface) {
        fuzz_vpx_codec_get_caps(vp8_iface);
        fuzz_vpx_codec_dec_init_ver(vp8_iface, data, size);
        fuzz_vpx_codec_peek_stream_info(vp8_iface, data, size);
        fuzz_vpx_codec_decode(vp8_iface, data, size);
    }
    
    // Get VP9 decoder interface
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_dx();
    if (vp9_iface) {
        fuzz_vpx_codec_get_caps(vp9_iface);
        fuzz_vpx_codec_dec_init_ver(vp9_iface, data, size);
        fuzz_vpx_codec_peek_stream_info(vp9_iface, data, size);
        fuzz_vpx_codec_decode(vp9_iface, data, size);
    }
    
    return 0;
}