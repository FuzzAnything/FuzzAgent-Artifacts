// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "vpx_encoder.h"
#include "vpx_codec.h"
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx_image.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // 1. Prepare the environment
    // Get the VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Invoke vpx_codec_iface_name to explore interface name state
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use to avoid unused variable warning

    // Prepare encoder configuration
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    // Invoke vpx_codec_enc_config_default
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // If default config fails, we can still test error handling
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        return 0;
    }

    // Diversify program state by modifying config based on input data if Size is sufficient
    if (Size > 0) {
        // Limit width and height to safe values (max 2^27 as per docs, but keep reasonable for fuzzing)
        unsigned int w = (Data[0] % 1024) + 16;
        unsigned int h = (Data[1 % Size] % 1024) + 16;
        cfg.g_w = w;
        cfg.g_h = h;
        
        // Set other parameters safely
        cfg.g_threads = (Data[2 % Size] % 4) + 1;
        cfg.g_profile = (Data[3 % Size] % 4);
        cfg.g_bit_depth = VPX_BITS_8;
        cfg.g_input_bit_depth = 8;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_error_resilient = 0;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        cfg.rc_target_bitrate = (Data[4 % Size] % 5000) + 100;
        cfg.rc_min_quantizer = 0;
        cfg.rc_max_quantizer = 63;
        cfg.rc_end_usage = VPX_VBR;
        cfg.kf_mode = VPX_KF_AUTO;
        cfg.kf_min_dist = 0;
        cfg.kf_max_dist = 0;
    }

    // Call vpx_codec_vp8_cx again to ensure we have a valid interface pointer for subsequent steps
    // (Though we already have it, this satisfies the sequence requirement)
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    if (!iface2) {
        return 0;
    }

    // Invoke vpx_codec_err_to_string with a known error code to test error handling path
    const char *err_msg = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)err_msg;

    // Test with an unknown error code
    vpx_codec_err_t unknown_err = (vpx_codec_err_t)9999;
    const char *unknown_err_msg = vpx_codec_err_to_string(unknown_err);
    (void)unknown_err_msg;

    // 2. Allocate image
    // Use input data to determine image parameters safely
    vpx_image_t *img = NULL;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int d_w = 64;
    unsigned int d_h = 64;
    unsigned int align = 16;

    if (Size >= 4) {
        // Fuzz dimensions but keep within limits (<= 2^27)
        d_w = (Data[0] % 512) + 16;
        d_h = (Data[1] % 512) + 16;
        align = (Data[2] % 64) + 16; // Keep alignment reasonable
        // Select format based on input
        int fmt_idx = Data[3] % 10;
        switch (fmt_idx) {
            case 0: fmt = VPX_IMG_FMT_YV12; break;
            case 1: fmt = VPX_IMG_FMT_I420; break;
            case 2: fmt = VPX_IMG_FMT_I422; break;
            case 3: fmt = VPX_IMG_FMT_I444; break;
            case 4: fmt = VPX_IMG_FMT_NV12; break;
            case 5: fmt = VPX_IMG_FMT_I42016; break;
            case 6: fmt = VPX_IMG_FMT_I42216; break;
            case 7: fmt = VPX_IMG_FMT_I44416; break;
            case 8: fmt = VPX_IMG_FMT_I440; break;
            case 9: fmt = VPX_IMG_FMT_I44016; break;
            default: fmt = VPX_IMG_FMT_I420; break;
        }
    }

    // Allocate image
    img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);

    // Cleanup
    if (img) {
        vpx_img_free(img);
    }

    return 0;
}