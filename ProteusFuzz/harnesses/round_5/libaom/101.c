// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *data, size_t data_size) {
    if (ctx == NULL) return 0;

    int ret = 0;
    switch (control_id) {
        case AV1E_SET_ENABLE_INTRABC: {
            if (data_size >= sizeof(int)) {
                int val = *(int *)data;
                ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
            }
            break;
        }
        case AV1E_SET_MAX_REFERENCE_FRAMES: {
            if (data_size >= sizeof(int)) {
                int val = *(int *)data;
                ret = aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val);
            }
            break;
        }
        case AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR: {
            if (data_size >= sizeof(int)) {
                int val = *(int *)data;
                ret = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val);
            }
            break;
        }
        case AV1E_SET_TARGET_SEQ_LEVEL_IDX: {
            if (data_size >= sizeof(int)) {
                int val = *(int *)data;
                ret = aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val);
            }
            break;
        }
        case AOME_SET_TUNING: {
            if (data_size >= sizeof(int)) {
                int val = *(int *)data;
                ret = aom_codec_control(ctx, AOME_SET_TUNING, val);
            }
            break;
        }
        case AV1E_SET_SVC_PARAMS: {
            if (data_size >= sizeof(aom_svc_params_t)) {
                aom_svc_params_t *svc_params = (aom_svc_params_t *)data;
                if (svc_params->number_spatial_layers < 1 || svc_params->number_spatial_layers > 3) {
                    svc_params->number_spatial_layers = 1;
                }
                if (svc_params->number_temporal_layers < 1 || svc_params->number_temporal_layers > 3) {
                    svc_params->number_temporal_layers = 1;
                }
                ret = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, svc_params);
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int control_id = Data[0];
    int param_int = 0;
    aom_svc_params_t svc_params = {0};

    if (control_id == 0) control_id = AV1E_SET_ENABLE_INTRABC;
    else if (control_id == 1) control_id = AV1E_SET_MAX_REFERENCE_FRAMES;
    else if (control_id == 2) control_id = AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR;
    else if (control_id == 3) control_id = AV1E_SET_TARGET_SEQ_LEVEL_IDX;
    else if (control_id == 4) control_id = AOME_SET_TUNING;
    else if (control_id == 5) control_id = AV1E_SET_SVC_PARAMS;
    else {
        control_id = AV1E_SET_ENABLE_INTRABC;
    }

    if (Size > 4) {
        param_int = *(int *)(Data + 1);
        if (control_id == AV1E_SET_ENABLE_INTRABC) {
            param_int = (param_int & 1) ? 1 : 0;
        } else if (control_id == AV1E_SET_MAX_REFERENCE_FRAMES) {
            param_int = (param_int % 8) + 1;
        } else if (control_id == AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR) {
            param_int = (param_int % 100) + 1;
        } else if (control_id == AV1E_SET_TARGET_SEQ_LEVEL_IDX) {
            param_int = (param_int % 7) + 1;
        } else if (control_id == AOME_SET_TUNING) {
            param_int = (param_int % 2) ? AOM_TUNE_PSNR : AOM_TUNE_SSIM;
        }
    }

    if (control_id == AV1E_SET_SVC_PARAMS) {
        if (Size >= sizeof(aom_svc_params_t)) {
            memcpy(&svc_params, Data + 1, sizeof(aom_svc_params_t));
            fuzz_codec_control(&ctx, control_id, &svc_params, sizeof(svc_params));
        } else {
            svc_params.number_spatial_layers = 1;
            svc_params.number_temporal_layers = 1;
            fuzz_codec_control(&ctx, control_id, &svc_params, sizeof(svc_params));
        }
    } else {
        fuzz_codec_control(&ctx, control_id, &param_int, sizeof(param_int));
    }

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1);
    aom_enc_frame_flags_t flags = 0;

    if (aom_codec_encode(&ctx, &img, 0, 1, flags) != AOM_CODEC_OK) {
        aom_img_free(&img);
        cleanup_codec(&ctx);
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process packet if needed
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}