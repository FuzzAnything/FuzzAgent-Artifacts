// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9_DECODE_SVC_SPATIAL_LAYER__value at vp8dx.h:212:1 in vp8dx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

// Helper to safely read an integer from the fuzz input
static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

// Helper to safely read a single byte
static int read_byte(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    int val = **data;
    (*data)++;
    (*size)--;
    return val;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Determine which API to test based on the first byte
    int api_choice = read_byte(&ptr, &remaining);

    // Initialize encoder context
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    // Use VP9_PROFILE_HIGH if defined, otherwise 0
    #ifdef VP9_PROFILE_HIGH
    vpx_codec_enc_config_default(&cfg, VP9_PROFILE_HIGH, 0);
    #else
    vpx_codec_enc_config_default(&cfg, 0, 0);
    #endif

    // Configure encoder
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_pass = 0;
    cfg.g_usage = 0;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize decoder context
    vpx_codec_iface_t *d_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t d_ctx;
    if (vpx_codec_dec_init(&d_ctx, d_iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Prepare a dummy frame for encoding
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 64, 64, 16);
    memset(img.planes[0], 0, 64 * 64);
    memset(img.planes[1], 128, 32 * 32);
    memset(img.planes[2], 128, 32 * 32);

    int ret = 0;

    switch (api_choice % 6) {
        case 0: // VP9E_SET_SVC
            {
                int enable_svc = read_int(&ptr, &remaining) % 2;
                ret = vpx_codec_control(&ctx, VP9E_SET_SVC, enable_svc);
            }
            break;

        case 1: // VP9E_SET_SVC_FRAME_DROP_LAYER
            {
                // This control expects a pointer to vpx_svc_frame_drop_t
                vpx_svc_frame_drop_t drop_params;
                memset(&drop_params, 0, sizeof(drop_params));
                
                // Read thresholds for layers
                for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining >= sizeof(int); i++) {
                    drop_params.framedrop_thresh[i] = read_int(&ptr, &remaining);
                }
                
                // Read mode
                if (remaining >= sizeof(int)) {
                    int mode = read_int(&ptr, &remaining);
                    // Use raw integer values for the enum if constants are missing
                    // 0 = SVC_LAYER_DROP_MODE_LAYER_BASED
                    // 1 = SVC_LAYER_DROP_MODE_CONSTRAINED
                    drop_params.framedrop_mode = (SVC_LAYER_DROP_MODE)mode;
                }
                
                // Read max consecutive drops
                if (remaining >= sizeof(int)) {
                    drop_params.max_consec_drop = read_int(&ptr, &remaining);
                }
                
                ret = vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_params);
            }
            break;

        case 2: // VP9_DECODE_SVC_SPATIAL_LAYER
            {
                int layer = read_int(&ptr, &remaining);
                if (layer < 0) layer = 0;
                if (layer > 7) layer = 7;
                ret = vpx_codec_control(&d_ctx, VP9_DECODE_SVC_SPATIAL_LAYER, layer);
            }
            break;

        case 3: // VP9E_SET_SVC_LAYER_ID
            {
                vpx_svc_layer_id_t layer_id;
                memset(&layer_id, 0, sizeof(layer_id));
                if (remaining >= sizeof(int)) {
                    layer_id.spatial_layer_id = read_int(&ptr, &remaining);
                }
                if (remaining >= sizeof(int)) {
                    layer_id.temporal_layer_id = read_int(&ptr, &remaining);
                }
                for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining >= sizeof(int); i++) {
                    layer_id.temporal_layer_id_per_spatial[i] = read_int(&ptr, &remaining);
                }
                ret = vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
            }
            break;

        case 4: // VP9E_SET_SVC_PARAMETERS
            {
                // This control typically expects a pointer to a structure or specific config
                // Since exact structure isn't provided, we pass a dummy int pointer if needed
                // or handle as per typical usage. Assuming it might take an int or struct.
                // For safety, we'll use a local int and pass its address.
                int dummy_val = read_int(&ptr, &remaining);
                ret = vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, &dummy_val);
            }
            break;

        case 5: // VP9E_SET_SVC_INTER_LAYER_PRED
            {
                int flag = read_int(&ptr, &remaining) % 2;
                ret = vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, flag);
            }
            break;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_codec_destroy(&d_ctx);
    vpx_img_free(&img);

    return 0;
}