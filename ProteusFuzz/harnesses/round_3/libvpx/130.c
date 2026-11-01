// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_GET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1080:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

// Helper to safely extract integers from fuzz data
static int get_int(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    int val = ((int)data[0] << 24) | ((int)data[1] << 16) | ((int)data[2] << 8) | (int)data[3];
    *data += 4;
    *size -= 4;
    return val;
}

// Helper to safely extract small integers (0-255)
static uint8_t get_byte(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = *data[0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize configuration for VP9 encoder
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzz data to tweak config slightly to avoid immediate errors
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_pass = 1; // Single pass

    // Initialize the encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare a minimal frame for encoding to ensure internal state is valid
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image with fuzz data if available
    if (Size > 0) {
        memset(img.planes[0], get_byte(&Data, &Size), img.stride[0] * cfg.g_h);
        memset(img.planes[1], get_byte(&Data, &Size), img.stride[1] * (cfg.g_h / 2));
        memset(img.planes[2], get_byte(&Data, &Size), img.stride[2] * (cfg.g_h / 2));
    }

    // Encode at least one frame to populate internal buffers
    // Using 0 for flags as VPX_EFLAG_NONE might not be defined in all versions
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Now we can safely test control commands
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // 1. Test VP9E_SET_SVC_LAYER_ID
    if (remaining >= 1) {
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        layer_id.spatial_layer_id = get_int(&ptr, &remaining) % 4;
        layer_id.temporal_layer_id = get_int(&ptr, &remaining) % 8;
        // Fill array safely
        for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining > 0; i++) {
            layer_id.temporal_layer_id_per_spatial[i] = get_int(&ptr, &remaining) % 8;
        }
        vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    }

    // 2. Test VP9E_SET_SVC_FRAME_DROP_LAYER
    if (remaining >= 1) {
        vpx_svc_frame_drop_t frame_drop;
        memset(&frame_drop, 0, sizeof(frame_drop));
        for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining > 0; i++) {
            frame_drop.framedrop_thresh[i] = get_int(&ptr, &remaining);
        }
        frame_drop.framedrop_mode = (SVC_LAYER_DROP_MODE)(get_byte(&ptr, &remaining) % 2);
        frame_drop.max_consec_drop = get_int(&ptr, &remaining);
        vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
    }

    // 3. Test VP9E_SET_SVC_INTER_LAYER_PRED
    if (remaining >= 1) {
        int enable = get_int(&ptr, &remaining) ? 1 : 0;
        vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, enable);
    }

    // 4. Test VP9E_SET_SVC_REF_FRAME_CONFIG
    if (remaining >= 1) {
        vpx_svc_ref_frame_config_t ref_cfg;
        memset(&ref_cfg, 0, sizeof(ref_cfg));
        for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining > 0; i++) {
            ref_cfg.lst_fb_idx[i] = get_int(&ptr, &remaining) % 8;
            ref_cfg.gld_fb_idx[i] = get_int(&ptr, &remaining) % 8;
            ref_cfg.alt_fb_idx[i] = get_int(&ptr, &remaining) % 8;
            ref_cfg.update_buffer_slot[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.update_last[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.update_golden[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.update_alt_ref[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.reference_last[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.reference_golden[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.reference_alt_ref[i] = get_int(&ptr, &remaining) ? 1 : 0;
            ref_cfg.duration[i] = (int64_t)get_int(&ptr, &remaining);
        }
        vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
    }

    // 5. Test VP9E_GET_SVC_REF_FRAME_CONFIG (if supported)
    // Note: This control might not exist in all versions or might be read-only.
    // We attempt to call it to see if it triggers any state.
    vpx_svc_ref_frame_config_t dummy_ref_cfg;
    memset(&dummy_ref_cfg, 0, sizeof(dummy_ref_cfg));
    vpx_codec_control(&ctx, VP9E_GET_SVC_REF_FRAME_CONFIG, &dummy_ref_cfg);

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}