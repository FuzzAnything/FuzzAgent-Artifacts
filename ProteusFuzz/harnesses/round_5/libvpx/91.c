// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_encode(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    const vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    vpx_codec_flags_t flags = 0;
    
    /* Use VP8 interface. The correct function name is vpx_codec_vp8_cx */
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        /* Fallback to VP9 if VP8 is not available. 
           Note: vpx_codec_vp9_cx requires libvpx to be built with VP9 support.
           If not available, this will be NULL and we return early. */
        iface = vpx_codec_vp9_cx();
    }
    
    if (!iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(&cfg, iface, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Fuzz configuration parameters based on input data */
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (size > 1 ? (data[1] % 512) + 16 : 16);
        cfg.g_threads = (size > 2 ? (data[2] % 8) + 1 : 1);
        cfg.g_profile = (size > 3 ? data[3] % 4 : 0);
        
        /* Handle bit depth carefully */
        if (size > 4) {
            int bd = data[4] % 3;
            if (bd == 0) cfg.g_bit_depth = VPX_BITS_8;
            else if (bd == 1) cfg.g_bit_depth = VPX_BITS_10;
            else cfg.g_bit_depth = VPX_BITS_12;
        } else {
            cfg.g_bit_depth = VPX_BITS_8;
        }
        
        cfg.rc_target_bitrate = (size > 5 ? (data[5] * 100) : 1000);
        cfg.rc_min_quantizer = (size > 6 ? data[6] % 64 : 0);
        cfg.rc_max_quantizer = (size > 7 ? data[7] % 63 : 63);
        cfg.kf_mode = (size > 8 ? data[8] % 3 : VPX_KF_AUTO);
        cfg.kf_min_dist = (size > 9 ? data[9] % 300 : 120);
        cfg.kf_max_dist = (size > 10 ? data[10] % 300 : 120);
        
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (size > 11 ? (data[11] % 60) + 1 : 30);
    }

    /* Initialize encoder context */
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* Fuzz control commands */
    if (size > 12) {
        int ctrl_id = data[12];
        int value = 0;
        
        /* Map input byte to valid control IDs */
        switch (ctrl_id % 5) {
            case 0:
                /* VP8E_SET_CPUUSED */
                value = (size > 13 ? data[13] % 9 : 0);
                res = vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, value);
                break;
            case 1:
                /* VP8E_SET_TUNING */
                value = (size > 13 ? data[13] % 10 : 0);
                res = vpx_codec_control_(&ctx, VP8E_SET_TUNING, value);
                break;
            case 2:
                /* VP9E_SET_TUNE_CONTENT */
                value = (size > 13 ? data[13] % 3 : 0);
                res = vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, value);
                break;
            case 3:
                /* VP8E_SET_STATIC_THRESHOLD */
                value = (size > 13 ? data[13] * 10 : 0);
                res = vpx_codec_control_(&ctx, VP8E_SET_STATIC_THRESHOLD, value);
                break;
            case 4:
                /* VP8E_SET_TOKEN_PARTITIONS */
                value = (size > 13 ? data[13] % 4 : 0);
                res = vpx_codec_control_(&ctx, VP8E_SET_TOKEN_PARTITIONS, value);
                break;
        }
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);
    
    return 0;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    return fuzz_encode(Data, Size);
}