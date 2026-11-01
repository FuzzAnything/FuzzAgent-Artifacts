// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_encoder(ctx);
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_encoder(ctx);
        return 0;
    }

    uint8_t val = Data[0] % 256;
    unsigned int unsigned_val = val;

    // Fuzz VP8E_SET_CPUUSED__value (0-8 range typically)
    if (Data[1] < 9) {
        vpx_codec_control(ctx, VP8E_SET_CPUUSED, Data[1]);
    } else {
        vpx_codec_control(ctx, VP8E_SET_CPUUSED, 0);
        vpx_codec_control(ctx, VP8E_SET_CPUUSED, 8);
    }

    // Fuzz VP8E_SET_FRAME_FLAGS__value
    // Frame flags are typically bitmasks; try various combinations
    unsigned int flags = 0;
    if (Size > 2) flags |= (Data[2] & 0xFF);
    if (Size > 3) flags |= ((Data[3] & 0xFF) << 8);
    vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, flags);

    // Fuzz VP8E_SET_NOISE_SENSITIVITY__value (typically 0-127)
    if (Size > 4) {
        int noise_sens = Data[4] % 128;
        vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sens);
    }

    // Fuzz VP8E_SET_TEMPORAL_LAYER_ID__value (typically 0-7)
    if (Size > 5) {
        int layer_id = Data[5] % 8;
        vpx_codec_control(ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);
    }

    // Fuzz VP8E_SET_ACTIVEMAP__value
    if (Size > 6) {
        vpx_active_map_t active_map;
        unsigned int rows = (Data[6] % 2) + 1; // 1 or 2
        unsigned int cols = (Data[6] % 2) + 1; // 1 or 2
        unsigned int map_size = rows * cols;
        
        active_map.rows = rows;
        active_map.cols = cols;
        active_map.active_map = malloc(map_size);
        
        if (active_map.active_map) {
            for (unsigned int i = 0; i < map_size; i++) {
                if (Size > 7 + i) {
                    active_map.active_map[i] = Data[7 + i] % 2;
                } else {
                    active_map.active_map[i] = 0;
                }
            }
            vpx_codec_control(ctx, VP8E_SET_ACTIVEMAP, &active_map);
            free(active_map.active_map);
        }
    }

    // Fuzz VP9E_GET_ACTIVEMAP__value (if available, though signature was empty)
    // Since signature is unknown, we skip direct invocation to avoid compilation errors
    // but the context is prepared.

    cleanup_encoder(ctx);
    return 0;
}