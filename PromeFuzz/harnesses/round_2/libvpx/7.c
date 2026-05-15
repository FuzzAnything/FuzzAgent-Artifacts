// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

static void fuzz_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        return;
    }
    
    memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
    
    cfg->g_w = cfg->g_w % 1920 + 1;
    cfg->g_h = cfg->g_h % 1080 + 1;
    cfg->g_threads = cfg->g_threads % 8;
    cfg->rc_target_bitrate = cfg->rc_target_bitrate % 10000;
    cfg->rc_min_quantizer = cfg->rc_min_quantizer % 64;
    cfg->rc_max_quantizer = cfg->rc_max_quantizer % 64;
    if (cfg->rc_max_quantizer < cfg->rc_min_quantizer) {
        cfg->rc_max_quantizer = cfg->rc_min_quantizer;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = cfg->kf_max_dist % 100;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
    if (size < 100) {
        return 0;
    }
    
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }
    
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    fuzz_config(&cfg, data, size);
    
    size_t offset = sizeof(vpx_codec_enc_cfg_t) % size;
    
    if (vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    int ctrl_id = data[offset] % 100;
    offset = (offset + 1) % size;
    
    vpx_codec_control_(&ctx, ctrl_id, data[offset]);
    
    vpx_codec_enc_config_set(&ctx, &cfg);
    
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
    }
    
    vpx_codec_ctx_t multi_ctx[2];
    vpx_codec_enc_cfg_t multi_cfg[2];
    vpx_rational_t dsf[2] = {{1, 1}, {1, 1}};
    
    for (int i = 0; i < 2; i++) {
        if (vpx_codec_enc_config_default(iface, &multi_cfg[i], 0) != VPX_CODEC_OK) {
            break;
        }
        fuzz_config(&multi_cfg[i], data + offset, size - offset);
        offset = (offset + sizeof(vpx_codec_enc_cfg_t)) % size;
    }
    
    vpx_codec_enc_init_multi_ver(multi_ctx, iface, multi_cfg, 2, 0, dsf, VPX_ENCODER_ABI_VERSION);
    
    for (int i = 0; i < 2; i++) {
        vpx_codec_destroy(&multi_ctx[i]);
    }
    
    vpx_codec_destroy(&ctx);
    
    return 0;
}