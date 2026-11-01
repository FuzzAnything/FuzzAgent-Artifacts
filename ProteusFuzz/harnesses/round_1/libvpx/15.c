// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    int ret;
    vpx_svc_layer_id_t svc_config;

    if (Size < 6) {
        return 0;
    }

    // Initialize encoder configuration
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    ret = vpx_codec_enc_init(&encoder, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        return 0;
    }

    // 1. VP9E_SET_DISABLE_LOOPFILTER__value
    vpx_codec_control(&encoder, VP9E_SET_DISABLE_LOOPFILTER, Data[0] ? 1 : 0);

    // 2. VP9E_SET_ROW_MT__value (1st call)
    vpx_codec_control(&encoder, VP9E_SET_ROW_MT, Data[1] ? 1 : 0);

    // 3. VP9E_SET_ROW_MT__value (2nd call)
    vpx_codec_control(&encoder, VP9E_SET_ROW_MT, Data[2] ? 1 : 0);

    // 4. VP9E_SET_SVC__value
    vpx_codec_control(&encoder, VP9E_SET_SVC, Data[3] ? 1 : 0);

    // 5. VP9E_SET_SVC_PARAMETERS__value
    memset(&svc_config, 0, sizeof(svc_config));
    svc_config.spatial_layer_id = (Data[4] % 4) + 1;
    if (svc_config.spatial_layer_id > 4) svc_config.spatial_layer_id = 4;
    vpx_codec_control(&encoder, VP9E_SET_SVC_PARAMETERS, &svc_config);

    // 6. VP8E_SET_SCREEN_CONTENT_MODE__value
    vpx_codec_control(&encoder, VP8E_SET_SCREEN_CONTENT_MODE, Data[5] ? 1 : 0);

    // Cleanup
    vpx_codec_destroy(&encoder);

    return 0;
}