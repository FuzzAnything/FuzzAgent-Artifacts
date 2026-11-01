// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static int fuzz_vp9_encoder_controls(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return -1;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;

    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return -1;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_pass = 0;
    cfg.g_profile = 0;
    cfg.g_usage = 0;

    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return -1;

    // Prepare dummy input frame
    vpx_image_t img;
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1) != 0) {
        vpx_codec_destroy(&ctx);
        return -1;
    }
    
    // Initialize planes safely using stride and height
    if (img.planes[0]) {
        memset(img.planes[0], 0, img.stride[0] * img.d_h);
    }
    if (img.planes[1]) {
        memset(img.planes[1], 0, img.stride[1] * img.d_h);
    }
    if (img.planes[2]) {
        memset(img.planes[2], 0, img.stride[2] * img.d_h);
    }

    size_t offset = 0;
    int max_ops = 100;
    int op_count = 0;

    while (offset < size && op_count < max_ops) {
        int op = 0;
        if (size - offset >= sizeof(int)) {
            memcpy(&op, &data[offset], sizeof(int));
            offset += sizeof(int);
            op = op % 6;
        }

        switch (op) {
            case 0: {
                // VP9E_SET_SVC
                int svc_enable = 0;
                if (size - offset >= sizeof(int)) {
                    memcpy(&svc_enable, &data[offset], sizeof(int));
                    offset += sizeof(int);
                    svc_enable = svc_enable ? 1 : 0;
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC, svc_enable);
                break;
            }
            case 1: {
                // VP9E_SET_SVC_LAYER_ID
                vpx_svc_layer_id_t layer_id = {0};
                if (size - offset >= sizeof(layer_id)) {
                    memcpy(&layer_id, &data[offset], sizeof(layer_id));
                    offset += sizeof(layer_id);
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
                break;
            }
            case 2: {
                // VP9E_SET_SVC_FRAME_DROP_LAYER
                vpx_svc_frame_drop_t frame_drop = {0};
                if (size - offset >= sizeof(frame_drop)) {
                    memcpy(&frame_drop, &data[offset], sizeof(frame_drop));
                    offset += sizeof(frame_drop);
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
                break;
            }
            case 3: {
                // VP9E_SET_SVC_REF_FRAME_CONFIG
                vpx_svc_ref_frame_config_t ref_cfg = {0};
                if (size - offset >= sizeof(ref_cfg)) {
                    memcpy(&ref_cfg, &data[offset], sizeof(ref_cfg));
                    offset += sizeof(ref_cfg);
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
                break;
            }
            case 4: {
                // VP9E_SET_SVC_PARAMETERS
                int param_int = 0;
                if (size - offset >= sizeof(int)) {
                    memcpy(&param_int, &data[offset], sizeof(int));
                    offset += sizeof(int);
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, &param_int);
                break;
            }
            case 5: {
                // VP9E_SET_SVC_INTER_LAYER_PRED
                int inter_pred = 0;
                if (size - offset >= sizeof(int)) {
                    memcpy(&inter_pred, &data[offset], sizeof(int));
                    offset += sizeof(int);
                    inter_pred = inter_pred ? 1 : 0;
                }
                vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_pred);
                break;
            }
        }

        // Encode a frame to trigger processing of controls
        // Use 0 for flags
        vpx_codec_err_t enc_err = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
        (void)enc_err;

        op_count++;
    }

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    fuzz_vp9_encoder_controls(Data, Size);
    return 0;
}