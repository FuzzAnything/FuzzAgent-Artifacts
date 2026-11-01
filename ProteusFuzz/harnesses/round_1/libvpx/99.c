// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure encoder based on input data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_threads = (Data[2] % 16) + 1;
    cfg.g_error_resilient = (Data[3] % 2);
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[6] % 60) + 30;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz VP8E_SET_CPUUSED
    int cpu_used = Data[7] % 9;
    if (vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used) != VPX_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    int max_intra_pct = Data[8] % 101;
    if (vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Fuzz VP8E_SET_FRAME_FLAGS
    int frame_flags = Data[9] & 0xFF;
    if (vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Fuzz VP8E_SET_TEMPORAL_LAYER_ID
    int layer_id = (Data[10] % 8) + 1;
    if (vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Fuzz VP8E_SET_ACTIVEMAP
    if (Size > 12) {
        unsigned int rows = (Data[10] % 32) + 1;
        unsigned int cols = (Data[11] % 32) + 1;
        size_t map_size = rows * cols;
        
        // Ensure we have enough data for the map
        if (Size >= 12 + map_size) {
            vpx_active_map_t active_map;
            active_map.active_map = (unsigned char *)malloc(map_size);
            if (active_map.active_map) {
                memcpy(active_map.active_map, &Data[12], map_size);
                active_map.rows = rows;
                active_map.cols = cols;

                if (vpx_codec_control(&ctx, VP8E_SET_ACTIVEMAP, &active_map) != VPX_CODEC_OK) {
                    // Ignore error
                }
                free(active_map.active_map);
            }
        }
    }

    // Note: VP9E_GET_ACTIVEMAP is not applicable to VP8 encoder context and 
    // the symbol might not be defined in vp8cx.h. Skipping to ensure compilation.

    vpx_codec_destroy(&ctx);

    return 0;
}