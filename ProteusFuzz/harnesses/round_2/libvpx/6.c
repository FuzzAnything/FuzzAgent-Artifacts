// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static int fuzz_vpx_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_image_t *img = NULL;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_err_t res;
    int ret = 0;

    // Initialize configuration
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters safely
    if (size > 0) {
        cfg.g_w = data[0] > 0 ? data[0] * 16 : 16;
        cfg.g_h = data[1] > 0 ? data[1] * 16 : 16;
        cfg.g_timebase.num = data[2] + 1;
        cfg.g_timebase.den = data[3] + 1;
        cfg.g_lag_in_frames = data[4] % 10;
        cfg.rc_target_bitrate = (data[5] * 256) + data[6];
    }

    // Get VP9 encoder interface
    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // 1. VP9E_SET_DISABLE_LOOPFILTER__value
    int disable_loopfilter = data[size > 0 ? 7 % size : 0] % 2;
    res = vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    // Ignore error for fuzzing robustness

    // 2. VP9E_SET_SVC_FRAME_DROP_LAYER__value
    vpx_svc_frame_drop_t drop_cfg;
    memset(&drop_cfg, 0, sizeof(drop_cfg));
    if (size > 8) {
        drop_cfg.framedrop_thresh[0] = data[8];
        drop_cfg.framedrop_mode = (data[9] % 2);
        drop_cfg.max_consec_drop = data[10] % 10;
    }
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_cfg);
    // Ignore error for fuzzing robustness

    // 3. VP9E_SET_SVC_LAYER_ID__value
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    if (size > 11) {
        layer_id.spatial_layer_id = data[11] % 4;
        layer_id.temporal_layer_id = data[12] % 4;
    }
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    // Ignore error for fuzzing robustness

    // 4. VP9E_SET_SVC_REF_FRAME_CONFIG__value
    vpx_svc_ref_frame_config_t ref_cfg;
    memset(&ref_cfg, 0, sizeof(ref_cfg));
    if (size > 13) {
        ref_cfg.lst_fb_idx[0] = data[13];
        ref_cfg.gld_fb_idx[0] = data[14];
        ref_cfg.alt_fb_idx[0] = data[15];
        ref_cfg.reference_last[0] = data[16] % 2;
        ref_cfg.reference_golden[0] = data[17] % 2;
        ref_cfg.reference_alt_ref[0] = data[18] % 2;
    }
    res = vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
    // Ignore error for fuzzing robustness

    // Create a dummy image to encode
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    if (size > 0) {
        size_t img_size = img->d_w * img->d_h * 3 / 2;
        size_t copy_size = size < img_size ? size : img_size;
        memcpy(img->planes[VPX_PLANE_Y], data, copy_size);
        if (copy_size < img_size) {
            memset(img->planes[VPX_PLANE_Y] + copy_size, 0, img_size - copy_size);
        }
    }

    // Encode a frame
    res = vpx_codec_encode(&ctx, img, 0, 1, 0, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 5. vpx_codec_get_cx_data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame packet if needed, but for fuzzing just iterate
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // 6. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 7. vpx_img_free
    vpx_img_free(img);

    return 1;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    fuzz_vpx_encoder(Data, Size);
    return 0;
}