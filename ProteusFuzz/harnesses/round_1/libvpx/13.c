// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 > size) return -1;
    int val = (int)data[*offset];
    *offset += 1;
    return val;
}

static int parse_enum(const uint8_t *data, size_t size, size_t *offset, int max_val) {
    int val = parse_int(data, size, offset);
    if (val < 0 || val > max_val) {
        val = 0;
    }
    return val;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    size_t offset = 0;

    // 1. VP9E_SET_SVC_INTER_LAYER_PRED__value
    // Assuming it expects an integer (0 or 1)
    int svc_pred = parse_int(Data, Size, &offset);
    if (svc_pred != -1) {
        vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, svc_pred);
    }

    // 2. VP9E_SET_NOISE_SENSITIVITY__value
    // Assuming it expects an integer value
    int noise_sens = parse_int(Data, Size, &offset);
    if (noise_sens != -1) {
        vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sens);
    }

    // 3. VP9E_SET_TUNE_CONTENT__value
    // Valid values are typically VP9E_CONTENT_DEFAULT, VP9E_CONTENT_SCREEN, VP9E_CONTENT_FILM
    // We map input to a safe range [0, 2]
    int tune_content = parse_enum(Data, Size, &offset, 2);
    if (tune_content != -1) {
        vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, tune_content);
    }

    // 4. VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value
    // Assuming it expects an integer (0 or 1)
    int disable_overshoot = parse_int(Data, Size, &offset);
    if (disable_overshoot != -1) {
        vpx_codec_control(&ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);
    }

    vpx_codec_destroy(&ctx);

    return 0;
}