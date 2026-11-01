// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vp8cx.h"
#include "vp8dx.h"

static int fuzz_vp9_encoder(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where safe
    if (Size > 0) {
        cfg.g_w = (Data[0] % 256) + 16;
        cfg.g_h = (Data[1] % 256) + 16;
        cfg.g_timebase.num = (Data[2] % 100) + 1;
        cfg.g_timebase.den = (Data[3] % 1000) + 1;
        cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
        cfg.kf_mode = (Data[5] % 3);
        cfg.kf_max_dist = (Data[6] % 200) + 1;
        cfg.kf_min_dist = (Data[7] % cfg.kf_max_dist) + 1;
        cfg.rc_dropframe_thresh = (Data[8] % 100);
        cfg.rc_2pass_vbr_bias_pct = (Data[9] % 100);
        cfg.rc_resize_allowed = (Data[10] % 2);
        cfg.g_lag_in_frames = (Data[11] % 256);
        cfg.g_error_resilient = (Data[12] % 3);
        cfg.g_pass = (Data[13] % 3);
        cfg.g_threads = (Data[14] % 8) + 1;
        cfg.g_usage = (Data[15] % 2);
        cfg.g_profile = (Data[16] % 4);
    }

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare input frame
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 16);
    
    // Fill image data with fuzzed input if available
    if (Size > 0) {
        size_t plane_size = img.planes[0] ? img.stride[0] * img.h : 0;
        size_t copy_size = (Size < plane_size) ? Size : plane_size;
        if (img.planes[0]) {
            memcpy(img.planes[0], Data, copy_size);
        }
        
        // Copy to UV planes if they exist and data is sufficient
        if (img.planes[1] && Size > plane_size) {
            size_t uv_size = img.planes[1] ? img.stride[1] * (img.h / 2) : 0;
            size_t uv_copy = (Size - plane_size < uv_size) ? Size - plane_size : uv_size;
            memcpy(img.planes[1], Data + plane_size, uv_copy);
        }
        
        if (img.planes[2]) {
            size_t uv2_size = img.planes[2] ? img.stride[2] * (img.h / 2) : 0;
            size_t offset = plane_size;
            if (img.planes[1]) {
                offset += (img.planes[1] ? img.stride[1] * (img.h / 2) : 0);
            }
            
            if (Size > offset) {
                size_t uv2_copy = (Size - offset < uv2_size) ? Size - offset : uv2_size;
                memcpy(img.planes[2], Data + offset, uv2_copy);
            }
        }
    }

    // Test VP9E_SET_ALT_REF_AQ (expects int, not int*)
    int alt_ref_aq = (Data[0] % 2);
    vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, alt_ref_aq);

    // Test VP9E_SET_COLOR_RANGE (expects int, not int*)
    int color_range = (Data[1] % 2);
    vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, color_range);

    // Test VP9E_SET_MAX_GF_INTERVAL (expects unsigned int, not int*)
    unsigned int max_gf_interval = (Data[2] % 32) + 1;
    vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);

    // Test VP9E_SET_POSTENCODE_DROP (expects unsigned int, not int*)
    unsigned int postencode_drop = (Data[3] % 2);
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, postencode_drop);

    // Test VP9E_SET_QUANTIZER_ONE_PASS (expects int, not int*)
    int quantizer_one_pass = (Data[4] % 2);
    vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, quantizer_one_pass);

    // Test VP9E_SET_TARGET_LEVEL (expects int, not int*)
    int target_level = (Data[5] % 6) + 1;
    vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, target_level);

    // Encode frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }
    fuzz_vp9_encoder(Data, Size);
    return 0;
}