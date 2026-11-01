// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// VP9E_GET_ACTIVEMAP__value at vp8cx.h:1056:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

static vpx_codec_ctx_t *create_encoder(const vpx_codec_iface_t *codec) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    vpx_codec_err_t err = vpx_codec_enc_init(ctx, codec, NULL, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_iface_t *codec = vpx_codec_vp8_dx();
    if (!codec) return 0;

    vpx_codec_ctx_t *ctx = create_encoder(codec);
    if (!ctx) {
        return 0;
    }

    // Use input data to derive parameters
    uint8_t cpu_used = Data[0] % 9; // 0-8
    uint8_t temporal_layer_id = Data[1];
    uint32_t frame_flags = 0;
    if (Size > 4) {
        frame_flags = Data[2] | (Data[3] << 8) | (Size > 5 ? (Data[4] << 16) : 0) | (Size > 6 ? (Data[5] << 24) : 0);
    }
    
    // Attempt to set CPU used
    vpx_codec_err_t err = vpx_codec_control(ctx, VP8E_SET_CPUUSED, cpu_used);
    (void)err;

    // Attempt to set temporal layer ID
    err = vpx_codec_control(ctx, VP8E_SET_TEMPORAL_LAYER_ID, temporal_layer_id);
    (void)err;

    // Attempt to set frame flags
    err = vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, frame_flags);
    (void)err;

    // Prepare active map if enough data
    if (Size > 10) {
        vpx_active_map_t active_map;
        active_map.rows = (Data[6] % 32) + 1;
        active_map.cols = (Data[7] % 32) + 1;
        size_t map_size = active_map.rows * active_map.cols;
        if (map_size > 0 && map_size <= (Size - 8)) {
            active_map.active_map = (unsigned char *)malloc(map_size);
            if (active_map.active_map) {
                memcpy(active_map.active_map, Data + 8, map_size);
                
                // Try VP8E_SET_ACTIVEMAP
                err = vpx_codec_control(ctx, VP8E_SET_ACTIVEMAP, &active_map);
                (void)err;
                
                // Try VP9E_GET_ACTIVEMAP (if available, though likely not for VP8 encoder)
                vpx_active_map_t get_map;
                get_map.active_map = NULL;
                get_map.rows = 0;
                get_map.cols = 0;
                err = vpx_codec_control(ctx, VP9E_GET_ACTIVEMAP, &get_map);
                (void)err;
                
                free(active_map.active_map);
            }
        }
    }

    // Try static threshold if data allows
    if (Size > 12) {
        int static_threshold = (int)(Data[10] | (Data[11] << 8));
        err = vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, static_threshold);
        (void)err;
    }

    cleanup_encoder(ctx);
    return 0;
}