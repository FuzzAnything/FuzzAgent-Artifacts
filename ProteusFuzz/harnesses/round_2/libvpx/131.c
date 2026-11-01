// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx encoder initialization and control APIs.
 * Targets: vpx_codec_enc_init_ver, vpx_codec_control_
 */

static void init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg || !data) return;

    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));

    // Use fuzz data to populate configuration fields safely
    unsigned int val = 0;
    if (size >= sizeof(val)) {
        memcpy(&val, data, sizeof(val));
    }

    // Set basic dimensions (avoid 0 to prevent immediate errors)
    cfg->g_w = (val % 1024) + 16;
    cfg->g_h = (val % 1024) + 16;

    // Bit depth
    cfg->g_bit_depth = (val % 3) == 0 ? VPX_BITS_8 : ((val % 3) == 1 ? VPX_BITS_10 : VPX_BITS_12);
    cfg->g_input_bit_depth = cfg->g_bit_depth;

    // Timebase
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (val % 60) + 1;

    // Threads
    cfg->g_threads = (val % 4) + 1;

    // Rate control
    cfg->rc_end_usage = (val % 4); // VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q
    cfg->rc_target_bitrate = (val % 10000) + 100; // 100 to 10100 kbps
    cfg->rc_min_quantizer = (val % 64) + 1;
    cfg->rc_max_quantizer = (val % 63) + cfg->rc_min_quantizer + 1;

    // Keyframe settings
    cfg->kf_mode = (val % 3); // VPX_KF_FIXED, VPX_KF_AUTO, VPX_KF_DISABLED
    cfg->kf_min_dist = (val % 100) + 1;
    cfg->kf_max_dist = (val % 1000) + cfg->kf_min_dist;

    // Error resilience: Use 1 as a generic flag since specific macros might vary
    cfg->g_error_resilient = (val % 2) ? 1 : 0;

    // Pass
    cfg->g_pass = (val % 3); // VPX_RC_ONE_PASS, VPX_RC_FIRST_PASS, VPX_RC_LAST_PASS

    // Lag
    cfg->g_lag_in_frames = (val % 10);

    // Profile
    cfg->g_profile = (val % 4);
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int ctrl_id;
    int value;

    // Initialize config from fuzz data
    init_config(&cfg, Data, Size);

    // Try VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        // If VP8 is not available, we cannot proceed with this specific driver logic
        return 0;
    }

    // Initialize encoder
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Initialization failed, no need to destroy
        return 0;
    }

    // Extract control ID and value from fuzz data
    if (Size >= 8) {
        memcpy(&ctrl_id, Data, sizeof(ctrl_id));
        memcpy(&value, Data + sizeof(ctrl_id), sizeof(value));
    } else {
        ctrl_id = 0;
        value = 0;
    }

    // Fuzz various control IDs
    // VP8E_SET_CPUUSED
    if (ctrl_id == VP8E_SET_CPUUSED) {
        vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, value & 0x7); // Clamp to 0-7
    }
    // VP8E_SET_TUNING
    else if (ctrl_id == VP8E_SET_TUNING) {
        vpx_codec_control_(&ctx, VP8E_SET_TUNING, value);
    }
    // VP8D_GET_LAST_REF_UPDATES (Decoder control, might fail on encoder)
    else if (ctrl_id == VP8D_GET_LAST_REF_UPDATES) {
        int *ref_updates = NULL;
        vpx_codec_control_(&ctx, VP8D_GET_LAST_REF_UPDATES, ref_updates);
    }
    // Generic control ID fuzzing
    else {
        vpx_codec_control_(&ctx, ctrl_id, value);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}