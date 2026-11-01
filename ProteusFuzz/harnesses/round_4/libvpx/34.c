// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// VP9E_GET_ACTIVEMAP__value at vp8cx.h:1056:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

// Fuzzing entry point
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration manually
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Use input data to configure encoder parameters fuzzily
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 1;
    cfg.g_lag_in_frames = (Data[5] % 20);
    cfg.g_error_resilient = (Data[6] % 2);

    // Initialize codec using the correct interface function
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz vpx_codec_control_ with various control IDs
    int ctrl_id = Data[7] % 1000;
    vpx_codec_err_t res;

    // Try VP8E_SET_CPUUSED
    if (ctrl_id == 0) {
        int cpu_used = Data[8] % 9; // 0-8
        res = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
        (void)res;
    }
    // Try VP8E_SET_FRAME_FLAGS
    else if (ctrl_id == 1) {
        int frame_flags = Data[8] % 100;
        res = vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags);
        (void)res;
    }
    // Try VP8E_SET_NOISE_SENSITIVITY
    else if (ctrl_id == 2) {
        int noise_sensitivity = Data[8] % 100;
        res = vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
        (void)res;
    }
    // Try VP8E_SET_ACTIVEMAP
    else if (ctrl_id == 3) {
        vpx_active_map_t active_map;
        active_map.rows = Data[8] % 10 + 1;
        active_map.cols = Data[9] % 10 + 1;
        size_t map_size = active_map.rows * active_map.cols;
        active_map.active_map = (unsigned char *)malloc(map_size);
        if (active_map.active_map) {
            memcpy(active_map.active_map, Data + 10, map_size < Size - 10 ? map_size : Size - 10);
            res = vpx_codec_control(&ctx, VP8E_SET_ACTIVEMAP, &active_map);
            free(active_map.active_map);
        }
        (void)res;
    }
    // Try VP9E_GET_ACTIVEMAP (if supported)
    else if (ctrl_id == 4) {
        vpx_active_map_t active_map;
        active_map.rows = Data[8] % 10 + 1;
        active_map.cols = Data[9] % 10 + 1;
        size_t map_size = active_map.rows * active_map.cols;
        active_map.active_map = (unsigned char *)malloc(map_size);
        if (active_map.active_map) {
            res = vpx_codec_control(&ctx, VP9E_GET_ACTIVEMAP, &active_map);
            free(active_map.active_map);
        }
        (void)res;
    }
    // Generic control with random arguments (only one extra arg allowed by macro)
    else {
        // Use the internal function directly to avoid macro expansion issues with variable ctrl_id
        res = vpx_codec_control_(&ctx, ctrl_id, Data[8]);
        (void)res;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}