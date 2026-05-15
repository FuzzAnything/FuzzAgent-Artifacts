// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_vpx_codec_get_caps(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
    
    if (iface_vp8) {
        vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(iface_vp8);
        (void)caps_vp8;
    }
    
    if (iface_vp9) {
        vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(iface_vp9);
        (void)caps_vp9;
    }
}

static void fuzz_vpx_codec_enc_config_default(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t cfg;
    
    if (iface_vp8) {
        vpx_codec_err_t err = vpx_codec_enc_config_default(iface_vp8, &cfg, 0);
        (void)err;
    }
    
    if (iface_vp9) {
        vpx_codec_err_t err = vpx_codec_enc_config_default(iface_vp9, &cfg, 0);
        (void)err;
    }
}

static void fuzz_vpx_codec_enc_init_ver(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    if (iface_vp8) {
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(iface_vp8, &cfg, 0);
        if (cfg_err == VPX_CODEC_OK) {
            if (size > 0) {
                cfg.g_w = (data[0] % 128 + 1) * 16;
                cfg.g_h = (data[size > 1 ? 1 : 0] % 128 + 1) * 16;
                cfg.rc_target_bitrate = (data[size > 2 ? 2 : 0] % 1000 + 1);
            }
            vpx_codec_err_t init_err = vpx_codec_enc_init_ver(&ctx, iface_vp8, &cfg, 0, VPX_ENCODER_ABI_VERSION);
            if (init_err == VPX_CODEC_OK) {
                vpx_codec_destroy(&ctx);
            }
        }
    }
    
    if (iface_vp9) {
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(iface_vp9, &cfg, 0);
        if (cfg_err == VPX_CODEC_OK) {
            if (size > 0) {
                cfg.g_w = (data[0] % 128 + 1) * 16;
                cfg.g_h = (data[size > 1 ? 1 : 0] % 128 + 1) * 16;
                cfg.rc_target_bitrate = (data[size > 2 ? 2 : 0] % 1000 + 1);
            }
            vpx_codec_err_t init_err = vpx_codec_enc_init_ver(&ctx, iface_vp9, &cfg, 0, VPX_ENCODER_ABI_VERSION);
            if (init_err == VPX_CODEC_OK) {
                vpx_codec_destroy(&ctx);
            }
        }
    }
}

static void fuzz_vpx_codec_enc_init_multi_ver(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    if (!iface_vp8) return;
    
    const int max_encoders = 2;
    vpx_codec_ctx_t ctx[max_encoders];
    vpx_codec_enc_cfg_t cfg[max_encoders];
    vpx_rational_t dsf[max_encoders];
    
    for (int i = 0; i < max_encoders; i++) {
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(iface_vp8, &cfg[i], 0);
        if (cfg_err != VPX_CODEC_OK) return;
        
        if (size > i * 4) {
            size_t idx = i * 4;
            cfg[i].g_w = (data[idx % size] % 64 + 1) * 16;
            cfg[i].g_h = (data[(idx + 1) % size] % 64 + 1) * 16;
            cfg[i].rc_target_bitrate = (data[(idx + 2) % size] % 500 + 1);
        }
        
        dsf[i].num = (i + 1);
        dsf[i].den = (i + 2);
    }
    
    vpx_codec_err_t init_err = vpx_codec_enc_init_multi_ver(ctx, iface_vp8, cfg, max_encoders, 0, dsf, VPX_ENCODER_ABI_VERSION);
    if (init_err == VPX_CODEC_OK) {
        for (int i = 0; i < max_encoders; i++) {
            vpx_codec_destroy(&ctx[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t selector = data[0];
    const uint8_t *fuzz_data = size > 1 ? data + 1 : data;
    size_t fuzz_size = size > 1 ? size - 1 : 1;
    
    switch (selector % 4) {
        case 0:
            fuzz_vpx_codec_get_caps(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_vpx_codec_enc_config_default(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_vpx_codec_enc_init_ver(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_vpx_codec_enc_init_multi_ver(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}