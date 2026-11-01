// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Fuzz driver for libvpx encoder initialization and control.
 * Targets: vpx_codec_enc_init_ver, vpx_codec_control_
 *
 * Note: VP9E_SET_TUNE_CONTENT, VP8E_SET_TUNING, VP8E_SET_CPUUSED,
 * and VP8D_SET_DECRYPTOR are control IDs (enums), not standalone functions.
 * They are invoked via vpx_codec_control_.
 */

static int fuzz_init_and_control(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int ctrl_id;
    int arg_val;
    int cpuused_val;
    int tuning_val;
    int tune_content_val;
    vpx_decrypt_init decrypt;

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&decrypt, 0, sizeof(decrypt));

    /* 
     * In libvpx, interfaces are typically registered via vpx_codec_iface_list().
     * Since we cannot easily iterate or lookup by name without internal headers or 
     * specific registration macros in the build, we rely on the standard exported 
     * interface pointers if available, or we assume the user has linked the 
     * specific encoder libraries. 
     * 
     * However, for a standalone fuzz driver, we often need to explicitly reference 
     * the external interface symbols provided by the library (e.g., vpx_codec_vp8_cx_iface).
     * If these symbols are not exported or if the build is static, we might need to 
     * use vpx_codec_iface_list() if available, but that function is not in the public API 
     * in all versions.
     * 
     * A robust approach for fuzzing specific codecs is to assume the standard 
     * external symbols exist (vp8, vp9) or use the first available encoder.
     * Since vpx_codec_iface_by_name is not a public API, we will try to access 
     * the standard interface pointers directly if they are declared in the headers.
     * 
     * If the headers provided do not declare vpx_codec_vp8_cx_iface, we must rely 
     * on the fact that the linker will resolve it if the library is linked, or 
     * we simply cannot fuzz without a valid interface.
     * 
     * Let's try to use the standard interface pointers usually defined in the 
     * encoder headers (vp8cx.h, vp9cx.h).
     */
    
    /* Attempt to get VP8 interface */
    #ifdef VPX_CODEC_VP8_CX_IFACE
    iface = &vpx_codec_vp8_cx_iface;
    #endif
    
    /* Fallback to VP9 if VP8 not found or not defined */
    if (!iface) {
        #ifdef VPX_CODEC_VP9_CX_IFACE
        iface = &vpx_codec_vp9_cx_iface;
        #endif
    }

    /* If no interface is found, we cannot proceed */
    if (!iface) {
        return 0; 
    }

    /* Configure encoder settings based on fuzz input */
    if (Size > 0) {
        cfg.g_w = (Data[0] % 256) + 16;
        cfg.g_h = (Data[1] % 256) + 16;
        cfg.g_threads = (Data[2] % 8) + 1;
        cfg.g_profile = Data[3] % 4;
        
        /* Map bit depth safely */
        int bd_idx = Data[4] % 3;
        if (bd_idx == 0) cfg.g_bit_depth = VPX_BITS_8;
        else if (bd_idx == 1) cfg.g_bit_depth = VPX_BITS_10;
        else cfg.g_bit_depth = VPX_BITS_12;

        cfg.g_input_bit_depth = cfg.g_bit_depth;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[5] % 60) + 1;
        cfg.rc_target_bitrate = (Data[6] % 1000) + 100;
        cfg.rc_min_quantizer = Data[7] % 64;
        cfg.rc_max_quantizer = (Data[8] % 64) + cfg.rc_min_quantizer;
        cfg.rc_end_usage = (enum vpx_rc_mode)(Data[9] % 4);
        cfg.kf_mode = (enum vpx_kf_mode)(Data[10] % 3);
        cfg.kf_min_dist = (Data[11] % 120) + 1;
        cfg.kf_max_dist = (Data[12] % 120) + cfg.kf_min_dist;
    } else {
        /* Default configuration */
        cfg.g_w = 160;
        cfg.g_h = 120;
        cfg.g_threads = 1;
        cfg.g_profile = 0;
        cfg.g_bit_depth = VPX_BITS_8;
        cfg.g_input_bit_depth = VPX_BITS_8;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;
        cfg.rc_min_quantizer = 0;
        cfg.rc_max_quantizer = 63;
        cfg.rc_end_usage = VPX_VBR;
        cfg.kf_mode = VPX_KF_AUTO;
        cfg.kf_min_dist = 120;
        cfg.kf_max_dist = 120;
    }

    /* Initialize encoder */
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0; /* Initialization failed, nothing to fuzz */
    }

    /* Prepare control arguments from input data */
    if (Size > 13) {
        ctrl_id = Data[13] % 1000;
        arg_val = (int)(Data[14] % 256);
        cpuused_val = (int)(Data[15] % 9);
        tuning_val = (int)(Data[16] % 100);
        tune_content_val = (int)(Data[17] % 4);
    } else {
        ctrl_id = 0;
        arg_val = 0;
        cpuused_val = 0;
        tuning_val = 0;
        tune_content_val = 0;
    }

    /*
     * Invoke vpx_codec_control_ with various control IDs.
     */

    /* 1. Try VP8E_SET_CPUUSED */
    if (ctrl_id == 0 || ctrl_id == 1) {
        res = vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpuused_val);
        (void)res;
    }

    /* 2. Try VP8E_SET_TUNING */
    if (ctrl_id == 0 || ctrl_id == 2) {
        res = vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning_val);
        (void)res;
    }

    /* 3. Try VP9E_SET_TUNE_CONTENT */
    if (ctrl_id == 0 || ctrl_id == 3) {
        res = vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, tune_content_val);
        (void)res;
    }

    /* 4. Try VP8D_SET_DECRYPTOR (Decoder control on encoder context - expected to fail or be ignored) */
    if (ctrl_id == 0 || ctrl_id == 4) {
        decrypt.decrypt_cb = NULL;
        decrypt.decrypt_state = NULL;
        res = vpx_codec_control_(&ctx, VP8D_SET_DECRYPTOR, &decrypt);
        (void)res;
    }

    /* 5. Try a generic control with random ID and value to explore error paths */
    if (Size > 13) {
        res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
        (void)res;
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    fuzz_init_and_control(Data, Size);
    return 0;
}