// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_RENDER_SIZE__value at vp8cx.h:1062:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int call_fuzzable_api(vpx_codec_ctx_t *ctx, int api_id, const uint8_t *data, size_t size) {
    if (!ctx || size < 4) {
        return 0;
    }

    int32_t value;
    memcpy(&value, data, sizeof(int32_t));

    // Ensure value is within a reasonable range to avoid immediate crashes on invalid flags
    if (value < 0 || value > 100) {
        value = (value % 101);
    }

    switch (api_id) {
        case 0: // VP9E_SET_DISABLE_LOOPFILTER__value
            return vpx_codec_control(ctx, VP9E_SET_DISABLE_LOOPFILTER, value);
        case 1: // VP9E_SET_SVC__value
            return vpx_codec_control(ctx, VP9E_SET_SVC, value);
        case 2: // VP9E_SET_SVC_PARAMETERS__value
            // This control requires a pointer to a struct (vpx_svc_parameters_t).
            // Since we don't have the struct definition in the prompt and it's complex,
            // we skip this specific API or pass a dummy pointer if we could construct it.
            // For safety in this fuzz driver without struct defs, we return 0 to skip.
            return 0; 
        case 3: // VP9E_SET_RENDER_SIZE__value
            // Requires int* (width, height). We pass address of value.
            return vpx_codec_control(ctx, VP9E_SET_RENDER_SIZE, &value);
        case 4: // VP9E_SET_TARGET_LEVEL__value
            return vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, value);
        case 5: // VP9E_SET_FRAME_PARALLEL_DECODING__value
            return vpx_codec_control(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, value);
        default:
            return 0;
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    int api_id = 0;
    int ret = 0;

    // Initialize config with defaults
    memset(&cfg, 0, sizeof(cfg));
    
    // Use correct member names for vpx_codec_enc_cfg_t
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 500;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    // Initialize encoder context
    err = vpx_codec_enc_init(&ctx, vpx_codec_vp9_cx(), &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Parse input to determine API to test and value
    // First 4 bytes: API ID (0-5)
    // Remaining bytes: Value (used as int32_t)
    if (Size >= 4) {
        uint32_t id_val;
        memcpy(&id_val, Data, sizeof(uint32_t));
        api_id = id_val % 6; // 0 to 5
    }

    // Use the rest of the data for the value
    if (Size > 4) {
        ret = call_fuzzable_api(&ctx, api_id, Data + 4, Size - 4);
    } else {
        // Fallback if data is too small for a value, use 0
        ret = call_fuzzable_api(&ctx, api_id, Data, 4);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}