// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"

// Declare the VP8 encoder interface function since it's not in the provided headers
extern vpx_codec_iface_t* vpx_codec_vp8_cx(void);

static vpx_codec_iface_t* get_codec_iface() {
    // Use VP8 encoder interface for fuzzing
    return vpx_codec_vp8_cx();
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // We need at least some data to make decisions
    if (Size < 1) {
        return 0;
    }

    // 1. vpx_img_alloc
    // Use fuzzer data to determine image parameters
    unsigned int width = (Data[0] % 64) + 1;  // 1-64 pixels
    unsigned int height = (Data[1 % Size] % 64) + 1;  // 1-64 pixels
    unsigned int align = (Data[2 % Size] % 32) + 1;  // 1-32 alignment
    
    // Choose image format based on fuzzer data
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    switch (Data[3 % Size] % 4) {
        case 0: fmt = VPX_IMG_FMT_I420; break;
        case 1: fmt = VPX_IMG_FMT_I422; break;
        case 2: fmt = VPX_IMG_FMT_I444; break;
        case 3: fmt = VPX_IMG_FMT_NV12; break;
    }
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return 0;  // Allocation failed
    }

    // 2. vpx_codec_iface_name
    vpx_codec_iface_t *iface = get_codec_iface();
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name;  // Use the result to avoid unused variable warning

    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // Update config with fuzzer-provided dimensions
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = (Data[4 % Size] % 1000) + 1;  // 1-1000 kbps

    // 4. vpx_codec_enc_init_ver
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // 5. vpx_codec_control_
    // Try different control IDs based on fuzzer data
    if (Size >= 5) {
        // Use generic control IDs that should be available
        // These are defined in vpx/vpx_codec.h
        int ctrl_id = Data[5 % Size] % 10;  // Try different control IDs
        int value = Data[6 % Size];
        
        // Use the control function with different parameter types
        // Using generic control IDs that are likely to be defined
        switch (ctrl_id) {
            case 0:
                // VP8E_SET_CPUUSED - typical value for VP8
                vpx_codec_control_(&codec, 1, value % 16);  // CPUUSED typically 0-16
                break;
            case 1:
                // VP8E_SET_ENABLEAUTOALTREF
                vpx_codec_control_(&codec, 2, value & 1);
                break;
            case 2:
                // VP8E_SET_NOISE_SENSITIVITY
                vpx_codec_control_(&codec, 3, value % 10);
                break;
            case 3:
                // VP8E_SET_SHARPNESS
                vpx_codec_control_(&codec, 4, value % 8);
                break;
            case 4:
                // VP8E_SET_STATIC_THRESHOLD
                vpx_codec_control_(&codec, 5, value % 1000);
                break;
            case 5:
                // VP8E_SET_TOKEN_PARTITIONS
                vpx_codec_control_(&codec, 6, value % 4);
                break;
            case 6:
                // VP8E_SET_ARNR_MAXFRAMES
                vpx_codec_control_(&codec, 7, value % 16);
                break;
            case 7:
                // VP8E_SET_ARNR_STRENGTH
                vpx_codec_control_(&codec, 8, value % 7);
                break;
            case 8:
                // Generic control with different value type
                vpx_codec_control_(&codec, ctrl_id, value);
                break;
            case 9:
                // Another generic control
                vpx_codec_control_(&codec, ctrl_id + 10, value);
                break;
        }
    }

    // 6. vpx_img_free
    vpx_img_free(img);

    // 7. vpx_codec_destroy
    vpx_codec_destroy(&codec);

    return 0;
}