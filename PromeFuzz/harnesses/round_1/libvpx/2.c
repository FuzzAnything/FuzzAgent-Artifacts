// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"

// Declare the encoder interface functions that are missing from headers
// These are typically declared in vpx_encoder.h but we need to ensure they're visible
extern vpx_codec_iface_t *vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_cx(void);

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Use fuzz data to generate parameters
    if (Size < 4) return 0;
    
    // Extract parameters from fuzz data
    unsigned int width = (Data[0] % 64) + 1;  // Limit to reasonable size
    unsigned int height = (Data[1] % 64) + 1;
    unsigned int align = (Data[2] % 32) + 1;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    
    // 1. vpx_img_alloc
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return 0;
    
    // Get encoder interface
    vpx_codec_iface_t *iface = NULL;
    
    // Try to get VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        // Try VP9 encoder interface if VP8 is not available
        iface = vpx_codec_vp9_cx();
        if (!iface) {
            vpx_img_free(img);
            return 0;
        }
    }
    
    // 2. vpx_codec_iface_name
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use variable to avoid unused warning
    
    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Update config with fuzz data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 256; // Reasonable default
    
    // 4. vpx_codec_enc_init_ver
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // 5. vpx_codec_control_
    // Use vpx_codec_control_ directly with arbitrary control IDs
    if (Size > 8) {
        // Extract control ID and value from fuzz data
        int ctrl_id = (int)Data[3];
        int value = (int)Data[4];
        
        // Try various control IDs based on fuzz data
        for (int i = 0; i < 5 && (i + 5) < (int)Size; i++) {
            int test_ctrl_id = (int)Data[3 + i];
            int test_value = (int)Data[4 + i];
            
            // Call vpx_codec_control_ with variable arguments
            // We'll pass different types of arguments based on control ID
            if (test_ctrl_id % 3 == 0) {
                vpx_codec_control_(&codec, test_ctrl_id, test_value);
            } else if (test_ctrl_id % 3 == 1) {
                vpx_codec_control_(&codec, test_ctrl_id, (void*)(intptr_t)test_value);
            } else {
                vpx_codec_control_(&codec, test_ctrl_id, &test_value);
            }
        }
    }
    
    // 6. vpx_img_free
    vpx_img_free(img);
    
    // 7. vpx_codec_destroy
    vpx_codec_destroy(&codec);
    
    return 0;
}