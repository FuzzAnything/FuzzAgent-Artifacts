// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();

    // Initialize encoder configuration
    res = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Size % 10000) + 100;

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare SVC reference frame config
    #ifndef VPX_SS_MAX_LAYERS
    #define VPX_SS_MAX_LAYERS 4
    #endif

    vpx_svc_ref_frame_config_t svc_config;
    memset(&svc_config, 0, sizeof(svc_config));

    // Fuzz SVC config fields
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        svc_config.lst_fb_idx[i] = (int8_t)(Data[4 + i] & 0x0F);
        svc_config.gld_fb_idx[i] = (int8_t)((Data[4 + i] >> 4) & 0x0F);
        svc_config.alt_fb_idx[i] = (int8_t)(Data[5 + i] & 0x0F);
        svc_config.update_buffer_slot[i] = (Data[6 + i] % 2);
        svc_config.update_last[i] = (Data[7 + i] % 2);
        svc_config.update_golden[i] = (Data[8 + i] % 2);
        svc_config.update_alt_ref[i] = (Data[9 + i] % 2);
        svc_config.reference_last[i] = (Data[10 + i] % 2);
        svc_config.reference_golden[i] = (Data[11 + i] % 2);
        svc_config.reference_alt_ref[i] = (Data[12 + i] % 2);
        svc_config.duration[i] = (int64_t)(Data[13 + i] * 10);
    }

    // Call VP9E_SET_SVC_REF_FRAME_CONFIG
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_config);
    (void)res;

    // Fuzz and call VP9E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = (int)(Data[14] % 100);
    res = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    (void)res;

    // Skip VP9E_SET_SVC_PARAMETERS as it requires a specific struct (vpx_svc_parameters_t)
    // which was not provided in the context, and passing a dummy pointer caused
    // a stack-use-after-return crash in the library's internal logic.
    // To fuzz this safely, we would need to construct a valid vpx_svc_parameters_t.
    // Since the struct definition is missing, we omit this call to prevent crashes.

    // Fuzz and call VP9E_SET_POSTENCODE_DROP
    int postencode_drop = (int)(Data[15] % 2);
    res = vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, postencode_drop);
    (void)res;

    // Fuzz and call VP9E_SET_FRAME_PARALLEL_DECODING
    int frame_parallel_decoding = (int)(Data[16] % 2);
    res = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding);
    (void)res;

    // Fuzz and call VP9E_SET_SVC_INTER_LAYER_PRED
    int svc_inter_layer_pred = (int)(Data[17] % 2);
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, svc_inter_layer_pred);
    (void)res;

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}