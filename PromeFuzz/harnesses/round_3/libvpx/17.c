// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"

// Declare the encoder interface functions that should be available
extern vpx_codec_iface_t *vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_cx(void);

static vpx_codec_iface_t* get_codec_iface(void) {
    // Use the VP8 encoder interface which is available in libvpx
    return vpx_codec_vp8_cx();
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to determine parameters
    if (Size < 4) return 0;
    
    // Extract parameters from fuzzer input
    unsigned int width = (Data[0] << 8) | Data[1];
    unsigned int height = (Data[2] << 8) | Data[3];
    
    // Ensure reasonable dimensions for fuzzing
    if (width == 0) width = 64;
    if (height == 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Get alignment from input
    unsigned int align = 16;
    if (Size > 4) align = Data[4] % 65; // Limit to reasonable alignment
    if (align == 0) align = 1;
    
    // Get image format from input
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 5) {
        int fmt_idx = Data[5] % 8;
        switch (fmt_idx) {
            case 0: fmt = VPX_IMG_FMT_YV12; break;
            case 1: fmt = VPX_IMG_FMT_I420; break;
            case 2: fmt = VPX_IMG_FMT_I422; break;
            case 3: fmt = VPX_IMG_FMT_I444; break;
            case 4: fmt = VPX_IMG_FMT_I440; break;
            case 5: fmt = VPX_IMG_FMT_NV12; break;
            case 6: fmt = VPX_IMG_FMT_I42016; break;
            case 7: fmt = VPX_IMG_FMT_I42216; break;
        }
    }
    
    // 1. vpx_img_alloc
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return 0;
    
    // 2. vpx_codec_iface_name
    vpx_codec_iface_t *iface = get_codec_iface();
    if (!iface) {
        vpx_img_free(img);
        return 0;
    }
    
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use result to avoid unused variable warning
    
    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    // This may fail, but we continue with default values
    if (err != VPX_CODEC_OK) {
        // Initialize cfg with safe defaults manually
        memset(&cfg, 0, sizeof(cfg));
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256;
        cfg.g_error_resilient = 0;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        cfg.rc_min_quantizer = 2;
        cfg.rc_max_quantizer = 56;
    } else {
        // Update config with fuzzer-provided dimensions
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.rc_target_bitrate = 256; // Reasonable default
    }
    
    // 4. vpx_codec_enc_init_ver
    vpx_codec_ctx_t codec;
    vpx_codec_flags_t flags = 0;
    
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // 5. vpx_codec_control_
    // Try different control IDs based on fuzzer input
    if (Size > 7) {
        int ctrl_id = Data[7] % 256; // Try various control IDs
        
        // Try with different argument types based on control ID
        if (ctrl_id < 128) {
            // Try with integer argument
            int arg = (Size > 8) ? (int)Data[8] : 0;
            vpx_codec_control_(&codec, ctrl_id, arg);
        } else {
            // Try with pointer argument (NULL for safety)
            void *arg = NULL;
            vpx_codec_control_(&codec, ctrl_id, arg);
        }
        
        // Try another control with different argument
        if (Size > 9) {
            int ctrl_id2 = Data[9] % 256;
            unsigned int arg2 = (Size > 10) ? (unsigned int)Data[10] : 0;
            vpx_codec_control_(&codec, ctrl_id2, arg2);
        }
    }
    
    // 6. vpx_img_free
    vpx_img_free(img);
    
    // 7. vpx_codec_destroy
    vpx_codec_destroy(&codec);
    
    return 0;
}