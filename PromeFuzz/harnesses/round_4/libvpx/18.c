// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

static void fuzz_vpx_codec_get_caps(void) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_dx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
    
    if (iface_vp8) {
        vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(iface_vp8);
        (void)caps_vp8;
    }
    
    if (iface_vp9) {
        vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(iface_vp9);
        (void)caps_vp9;
    }
}

static void fuzz_vpx_codec_peek_stream_info(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_dx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
    
    vpx_codec_stream_info_t si;
    si.sz = sizeof(vpx_codec_stream_info_t);
    
    if (iface_vp8 && size > 0) {
        vpx_codec_peek_stream_info(iface_vp8, data, (unsigned int)size, &si);
        vpx_codec_peek_stream_info(iface_vp8, data, (unsigned int)size, NULL);
    }
    
    if (iface_vp9 && size > 0) {
        vpx_codec_peek_stream_info(iface_vp9, data, (unsigned int)size, &si);
        vpx_codec_peek_stream_info(iface_vp9, data, (unsigned int)size, NULL);
    }
}

static void fuzz_vpx_codec_dec_init_and_decode(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_dx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
    
    vpx_codec_ctx_t ctx_vp8, ctx_vp9;
    vpx_codec_dec_cfg_t cfg;
    
    memset(&ctx_vp8, 0, sizeof(vpx_codec_ctx_t));
    memset(&ctx_vp9, 0, sizeof(vpx_codec_ctx_t));
    memset(&cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    if (iface_vp8) {
        vpx_codec_err_t init_ret = vpx_codec_dec_init_ver(&ctx_vp8, iface_vp8, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (init_ret == VPX_CODEC_OK) {
            if (size > 0) {
                vpx_codec_decode(&ctx_vp8, data, (unsigned int)size, NULL, 0);
                vpx_codec_decode(&ctx_vp8, NULL, 0, NULL, 0);
            }
            vpx_codec_decode(&ctx_vp8, NULL, 0, NULL, 0);
            vpx_codec_destroy(&ctx_vp8);
        }
    }
    
    if (iface_vp9) {
        vpx_codec_err_t init_ret = vpx_codec_dec_init_ver(&ctx_vp9, iface_vp9, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (init_ret == VPX_CODEC_OK) {
            if (size > 0) {
                vpx_codec_decode(&ctx_vp9, data, (unsigned int)size, NULL, 0);
                vpx_codec_decode(&ctx_vp9, NULL, 0, NULL, 0);
            }
            vpx_codec_decode(&ctx_vp9, NULL, 0, NULL, 0);
            vpx_codec_destroy(&ctx_vp9);
        }
    }
    
    if (iface_vp8) {
        vpx_codec_err_t init_ret = vpx_codec_dec_init_ver(&ctx_vp8, iface_vp8, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (init_ret == VPX_CODEC_OK) {
            if (size > 0) {
                vpx_codec_decode(&ctx_vp8, data, (unsigned int)size, NULL, 0);
            }
            vpx_codec_destroy(&ctx_vp8);
        }
    }
    
    if (iface_vp9) {
        vpx_codec_err_t init_ret = vpx_codec_dec_init_ver(&ctx_vp9, iface_vp9, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (init_ret == VPX_CODEC_OK) {
            if (size > 0) {
                vpx_codec_decode(&ctx_vp9, data, (unsigned int)size, NULL, 0);
            }
            vpx_codec_destroy(&ctx_vp9);
        }
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_vpx_codec_get_caps();
    
    fuzz_vpx_codec_peek_stream_info(Data, Size);
    
    fuzz_vpx_codec_dec_init_and_decode(Data, Size);
    
    return 0;
}