// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_vpx_codec_get_caps(vpx_codec_iface_t *iface) {
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps;
}

static void test_vpx_codec_peek_stream_info(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_peek_stream_info(iface, data, (unsigned int)size, &si);
}

static void test_vpx_codec_dec_init_ver(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
    } else {
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.w = 640;
        cfg.h = 480;
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        size_t decode_size = size > 0 ? (size % 1024) : 0;
        if (decode_size > 0) {
            vpx_codec_decode(&ctx, data, (unsigned int)decode_size, NULL, 0);
        }
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        vpx_codec_destroy(&ctx);
    }
}

static void test_vpx_codec_decode(vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        if (size > 0) {
            vpx_codec_decode(&ctx, data, (unsigned int)size, NULL, 0);
        }
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        vpx_codec_destroy(&ctx);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_dx();
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_dx();
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector % 8) {
        case 0:
            test_vpx_codec_get_caps(vp8_iface);
            break;
        case 1:
            test_vpx_codec_get_caps(vp9_iface);
            break;
        case 2:
            if (fuzz_size > 0) {
                test_vpx_codec_peek_stream_info(vp8_iface, fuzz_data, fuzz_size);
            }
            break;
        case 3:
            if (fuzz_size > 0) {
                test_vpx_codec_peek_stream_info(vp9_iface, fuzz_data, fuzz_size);
            }
            break;
        case 4:
            test_vpx_codec_dec_init_ver(vp8_iface, fuzz_data, fuzz_size);
            break;
        case 5:
            test_vpx_codec_dec_init_ver(vp9_iface, fuzz_data, fuzz_size);
            break;
        case 6:
            if (fuzz_size > 0) {
                test_vpx_codec_decode(vp8_iface, fuzz_data, fuzz_size);
            }
            break;
        case 7:
            if (fuzz_size > 0) {
                test_vpx_codec_decode(vp9_iface, fuzz_data, fuzz_size);
            }
            break;
    }
    
    return 0;
}