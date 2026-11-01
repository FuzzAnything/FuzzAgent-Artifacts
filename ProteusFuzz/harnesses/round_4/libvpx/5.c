// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int parse_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    uint32_t val = (uint32_t)data[0] | ((uint32_t)data[1] << 8) | 
                   ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

static int parse_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    int val = (int)data[0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_iface_t *encoder = vpx_codec_vp8_dx();
    if (!encoder) return 0;

    vpx_codec_enc_cfg_t cfg;
    // Fixed: Use 0 instead of undefined VPX_CODEC_USE_ENCODE
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Parse values from fuzz input for configuration controls
    uint32_t val1 = parse_uint32(&ptr, &remaining);
    uint32_t val2 = parse_uint32(&ptr, &remaining);
    uint32_t val3 = parse_uint32(&ptr, &remaining);
    uint32_t val4 = parse_uint32(&ptr, &remaining);

    // 1. VP8E_SET_TOKEN_PARTITIONS__value
    if (vpx_codec_control(&ctx, VP8E_SET_TOKEN_PARTITIONS, val1) != VPX_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // 2. VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    if (vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val2) != VPX_CODEC_OK) {
        // Ignore error
    }

    // 3. VP9E_SET_SVC_LAYER_ID__value
    vpx_svc_layer_id_t svc_layer_id;
    svc_layer_id.spatial_layer_id = parse_uint8(&ptr, &remaining);
    svc_layer_id.temporal_layer_id = parse_uint8(&ptr, &remaining);
    // Fill the array with fuzz data if available
    for (int i = 0; i < VPX_SS_MAX_LAYERS && remaining > 0; i++) {
        svc_layer_id.temporal_layer_id_per_spatial[i] = parse_uint8(&ptr, &remaining);
    }
    
    // Attempt to set SVC layer ID (might fail if encoder is VP8, but that's fine for fuzzing)
    if (vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer_id) != VPX_CODEC_OK) {
        // Ignore error
    }

    // 4. VP8E_SET_TEMPORAL_LAYER_ID__value
    if (vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, val4) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Create a test image
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 32, 32, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data if available
    if (remaining > 0) {
        size_t plane_size = (size_t)img->planes[0][0] * (size_t)img->stride[0]; // Rough estimate for Y
        size_t copy_size = (remaining < plane_size) ? remaining : plane_size;
        if (img->planes[0]) {
            memcpy(img->planes[0], ptr, copy_size);
        }
    }

    vpx_codec_pts_t pts = parse_uint32(&ptr, &remaining);
    unsigned long duration = (unsigned long)parse_uint32(&ptr, &remaining);
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)parse_uint32(&ptr, &remaining);
    vpx_enc_deadline_t deadline = (vpx_enc_deadline_t)parse_uint32(&ptr, &remaining);

    // 5. vpx_codec_encode
    vpx_codec_err_t err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    
    // Flush the stream
    vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);

    // 6. vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume packet data to avoid leaks or state issues in some scenarios
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do nothing, just iterate
        }
    }

    // 7. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 8. vpx_img_free
    vpx_img_free(img);

    return 0;
}