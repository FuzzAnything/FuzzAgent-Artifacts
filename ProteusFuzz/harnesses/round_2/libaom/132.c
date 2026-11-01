// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encode_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = AOM_CODEC_OK;
    uint32_t cmd;
    size_t offset = 0;

    if (size < 4) return 0;

    cmd = *(uint32_t *)(data + offset);
    offset += 4;

    switch (cmd) {
        case 0: // AOME_SET_CQ_LEVEL
            if (size >= offset + 4) {
                int cq_level = *(int32_t *)(data + offset);
                if (cq_level < 0) cq_level = 0;
                if (cq_level > 255) cq_level = 255;
                ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, cq_level);
            }
            break;

        case 1: // AV1E_SET_GF_MAX_PYRAMID_HEIGHT
            if (size >= offset + 4) {
                int height = *(int32_t *)(data + offset);
                if (height < 0) height = 0;
                if (height > 6) height = 6;
                ret = aom_codec_control(ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, height);
            }
            break;

        case 2: // AV1E_SET_MAX_GF_INTERVAL
            if (size >= offset + 4) {
                int interval = *(int32_t *)(data + offset);
                if (interval < 1) interval = 1;
                if (interval > 128) interval = 128;
                ret = aom_codec_control(ctx, AV1E_SET_MAX_GF_INTERVAL, interval);
            }
            break;

        case 3: // AV1E_SET_SVC_PARAMS
            if (size >= offset + sizeof(aom_svc_params_t)) {
                aom_svc_params_t svc_params;
                memset(&svc_params, 0, sizeof(svc_params));
                memcpy(&svc_params, data + offset, sizeof(svc_params));
                
                if (svc_params.number_spatial_layers < 1) svc_params.number_spatial_layers = 1;
                if (svc_params.number_spatial_layers > 3) svc_params.number_spatial_layers = 3;
                if (svc_params.number_temporal_layers < 1) svc_params.number_temporal_layers = 1;
                if (svc_params.number_temporal_layers > 3) svc_params.number_temporal_layers = 3;
                
                ret = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
            }
            break;

        case 4: // AV1E_SET_MIN_GF_INTERVAL
            if (size >= offset + 4) {
                int interval = *(int32_t *)(data + offset);
                if (interval < 1) interval = 1;
                if (interval > 128) interval = 128;
                ret = aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, interval);
            }
            break;

        case 5: // AV1E_SET_GF_MIN_PYRAMID_HEIGHT
            if (size >= offset + 4) {
                int height = *(int32_t *)(data + offset);
                if (height < 0) height = 0;
                if (height > 6) height = 6;
                ret = aom_codec_control(ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, height);
            }
            break;

        default:
            return 0;
    }

    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t *img;
    aom_codec_err_t res;
    int initialized = 0;

    if (Size < 1) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;
    initialized = 1;

    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    res = aom_codec_encode(&ctx, img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    fuzz_encode_control(&ctx, Data, Size);

    aom_img_free(img);
    if (initialized) {
        aom_codec_destroy(&ctx);
    }

    return 0;
}