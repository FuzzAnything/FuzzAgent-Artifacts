// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// VP9_DECODE_SVC_SPATIAL_LAYER__value at vp8dx.h:212:1 in vp8dx.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_integer.h"

/*
 * Note: The provided API signatures in the prompt were empty.
 * This fuzzer implements standard libvpx SVC control interfaces based on
 * the provided data structures (vpx_svc_frame_drop_t, vpx_svc_layer_id_t)
 * and standard libvpx API conventions.
 *
 * Controls used:
 * - VP9E_SET_SVC
 * - VP9E_SET_SVC_LAYER_ID
 * - VP9E_SET_SVC_FRAME_DROP_LAYER
 * - VP9E_SET_SVC_INTER_LAYER_PRED
 * - VP9E_SET_SVC_PARAMETERS
 *
 * Decoding side:
 * - VP9_DECODE_SVC_SPATIAL_LAYER
 */

// Define missing enum values based on typical libvpx SVC definitions
// SVC_LAYER_DROP_MODE is an enum, typically 0 for layer based, 1 for constrained
#ifndef SVC_LAYER_DROP_LAYER_BASED
#define SVC_LAYER_DROP_LAYER_BASED 0
#endif
#ifndef SVC_LAYER_DROP_CONSTRAINED
#define SVC_LAYER_DROP_CONSTRAINED 1
#endif

static int fuzz_encoder(const uint8_t *data, size_t size) {
    if (size < 20) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
    if (!encoder) return 0;

    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != 0) {
        return 0;
    }

    // Configure basic encoding parameters
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;

    // Enable SVC
    cfg.g_threads = 1;
    cfg.rc_end_usage = VPX_CBR;
    cfg.g_pass = 0;

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Prepare a minimal frame
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, fmt, 128, 128, 1);
    
    // Fill image with fuzzing data
    size_t plane_size = 128 * 128;
    size_t uv_size = plane_size / 4;
    if (size >= plane_size + 2 * uv_size) {
        memcpy(img.planes[0], data, plane_size);
        memcpy(img.planes[1], data + plane_size, uv_size);
        memcpy(img.planes[2], data + plane_size + uv_size, uv_size);
    } else {
        memset(img.planes[0], 0x80, plane_size);
        memset(img.planes[1], 0x80, uv_size);
        memset(img.planes[2], 0x80, uv_size);
    }

    // 1. VP9E_SET_SVC: Enable SVC (value 1)
    int svc_val = (data[0] % 2) == 1 ? 1 : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_val);

    // 2. VP9E_SET_SVC_LAYER_ID
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    if (svc_val) {
        layer_id.spatial_layer_id = data[1] % 4; // Max layers usually 4
        layer_id.temporal_layer_id = data[2] % 4;
        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            layer_id.temporal_layer_id_per_spatial[i] = data[3 + i] % 4;
        }
        vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    }

    // 3. VP9E_SET_SVC_FRAME_DROP_LAYER
    if (svc_val) {
        vpx_svc_frame_drop_t drop_params;
        memset(&drop_params, 0, sizeof(drop_params));
        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            drop_params.framedrop_thresh[i] = data[10 + i] % 100;
        }
        // Use the defined macros for enum values
        drop_params.framedrop_mode = (data[10 + VPX_SS_MAX_LAYERS] % 2) == 1 ? SVC_LAYER_DROP_CONSTRAINED : SVC_LAYER_DROP_LAYER_BASED;
        drop_params.max_consec_drop = data[10 + VPX_SS_MAX_LAYERS + 1] % 10;
        vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_params);
    }

    // 4. VP9E_SET_SVC_INTER_LAYER_PRED
    if (svc_val) {
        int inter_pred = data[10 + VPX_SS_MAX_LAYERS + 2] % 2;
        vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_pred);
    }

    // 5. VP9E_SET_SVC_PARAMETERS
    // Reusing layer_id as a potential parameter set if the API expects it
    if (svc_val) {
        vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, &layer_id);
    }

    // Encode a frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *decoder = vpx_codec_vp9_dx();
    if (!decoder) return 0;

    if (vpx_codec_dec_init(&ctx, decoder, NULL, 0) != 0) {
        return 0;
    }

    // Decode a frame
    // vpx_codec_decode requires 5 arguments: ctx, data, size, user_priv, deadline
    // deadline is typically VPX_DL_REALTIME or VPX_DL_GOOD_QUALITY
    vpx_codec_decode(&ctx, data, size, NULL, VPX_DL_REALTIME);

    // 6. VP9_DECODE_SVC_SPATIAL_LAYER
    // This control usually requests a specific spatial layer for decoding
    // We try different layer indices based on fuzz data
    int layer_idx = data[0] % 4;
    vpx_codec_control(&ctx, VP9_DECODE_SVC_SPATIAL_LAYER, layer_idx);

    // Try to get the frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *frame;
    while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame if needed
        (void)frame;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz Encoder
    fuzz_encoder(Data, Size);

    // Fuzz Decoder
    fuzz_decoder(Data, Size);

    return 0;
}