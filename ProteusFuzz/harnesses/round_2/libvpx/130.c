// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "vp8cx.h"
#include "vp8dx.h"

static int fuzz_vp8e_set_cpuused(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    int val = (int)(*data & 0x0F); // Constrain to 0-15, though typically 0-8
    return vpx_codec_control(ctx, VP8E_SET_CPUUSED, val);
}

static int fuzz_vp8e_set_frame_flags(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    uint32_t flags = 0;
    if (size >= 4) {
        memcpy(&flags, data, 4);
    } else if (size > 0) {
        flags = data[0];
    }
    return vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, flags);
}

static int fuzz_vp8e_set_temporal_layer_id(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    int layer_id = (int)(*data & 0x07); // Typically 0-7 for temporal layers
    return vpx_codec_control(ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);
}

static int fuzz_vp8e_set_noise_sensitivity(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    int val = (int)(*data & 0x0F); // Constrain to a reasonable range
    return vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, val);
}

static int fuzz_vp8e_set_activemap(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || size < 4) return 0;
    
    uint32_t rows = 0, cols = 0;
    memcpy(&rows, data, sizeof(uint32_t));
    memcpy(&cols, data + 4, sizeof(uint32_t));
    
    // Sanity check to prevent huge allocations
    if (rows > 4096 || cols > 4096 || rows == 0 || cols == 0) return 0;
    if (rows * cols > 1024 * 1024) return 0; // Max 1M pixels

    size_t map_size = rows * cols;
    if (size < 8 + map_size) return 0;

    vpx_active_map_t active_map;
    active_map.rows = rows;
    active_map.cols = cols;
    active_map.active_map = (unsigned char *)(data + 8);

    return vpx_codec_control(ctx, VP8E_SET_ACTIVEMAP, &active_map);
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize codec context with a dummy codec
    // We use VP8 as the base since the target APIs are VP8E_*
    // Note: In a real fuzzing harness, we would need a valid codec instance.
    // Since we cannot easily initialize a full encoder without complex setup,
    // we simulate the control calls. However, vpx_codec_control expects a valid ctx.
    // To make this runnable, we attempt to initialize with a dummy config.
    
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != 0) {
        return 0;
    }
    
    // Set minimal config to allow initialization
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    int init_res = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (init_res != 0) {
        return 0;
    }

    // Split input data to drive different functions
    // Byte 0: Function selector
    uint8_t func_selector = Data[0] % 5;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    switch (func_selector) {
        case 0:
            fuzz_vp8e_set_cpuused(&ctx, payload, payload_size);
            break;
        case 1:
            fuzz_vp8e_set_frame_flags(&ctx, payload, payload_size);
            break;
        case 2:
            fuzz_vp8e_set_temporal_layer_id(&ctx, payload, payload_size);
            break;
        case 3:
            fuzz_vp8e_set_noise_sensitivity(&ctx, payload, payload_size);
            break;
        case 4:
            fuzz_vp8e_set_activemap(&ctx, payload, payload_size);
            break;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    
    return 0;
}