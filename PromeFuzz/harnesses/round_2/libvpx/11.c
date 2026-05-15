// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

static void test_vp8_decoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) return;
    
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps;
    
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_peek_stream_info(iface, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), &si);
    
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = si.w;
    cfg.h = si.h;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), NULL, 0);
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        vpx_codec_destroy(&ctx);
    }
}

static void test_vp9_decoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) return;
    
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps;
    
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_peek_stream_info(iface, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), &si);
    
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = si.w;
    cfg.h = si.h;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err == VPX_CODEC_OK) {
        vpx_codec_decode(&ctx, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), NULL, 0);
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        vpx_codec_destroy(&ctx);
    }
}

static void test_decoder_with_config(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) return;
    
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    cfg.threads = (data[0] % 8) + 1;
    cfg.w = ((data[1] << 8) | data[2]) % 4096;
    cfg.h = ((data[3] << 8) | (size > 4 ? data[4] : 0)) % 4096;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err == VPX_CODEC_OK) {
        const uint8_t *decode_data = size > 5 ? data + 5 : data;
        size_t decode_size = size > 5 ? size - 5 : size;
        vpx_codec_decode(&ctx, decode_data, (unsigned int)(decode_size > 0xFFFFFFFF ? 0xFFFFFFFF : decode_size), NULL, 0);
        vpx_codec_destroy(&ctx);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    test_vp8_decoder(data, size);
    test_vp9_decoder(data, size);
    test_decoder_with_config(data, size);
    
    return 0;
}