// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Declare encoder interface functions
extern vpx_codec_iface_t *vpx_codec_vp8_cx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_cx(void);

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize variables
    vpx_image_t *img = NULL;
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    
    // Extract parameters from fuzzer input with bounds checking
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;  // Default format
    
    // Use first byte for format selection if available
    if (Size > 0) {
        fmt = (Data[0] % 8) + 1;  // Range 1-8 for valid formats
    }
    
    // Use next bytes for dimensions (with reasonable limits)
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int align = 1;
    
    if (Size > 1) {
        width = 16 + (Data[1] % 256);
    }
    
    if (Size > 2) {
        height = 16 + (Data[2] % 256);
    }
    
    if (Size > 3) {
        align = 1 + (Data[3] % 32);  // Reasonable alignment
    }
    
    // Ensure dimensions don't exceed limits
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align > 65536) align = 65536;
    
    // 1. vpx_img_alloc
    img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        // If allocation fails, we can't proceed
        return 0;
    }
    
    // Set some image properties for diversity
    if (Size > 4) {
        img->cs = (Data[4] % 8);  // Color space
    }
    if (Size > 5) {
        img->range = (Data[5] % 2);  // Color range
    }
    
    // 2. vpx_codec_iface_name
    // Use VP8 encoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        // Try VP9 if VP8 not available
        iface = vpx_codec_vp9_cx();
    }
    
    if (iface) {
        const char *iface_name = vpx_codec_iface_name((vpx_codec_iface_t *)iface);
        (void)iface_name;  // Use result to avoid unused variable warning
    }
    
    // 3. vpx_codec_enc_config_default
    if (iface) {
        res = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (res != VPX_CODEC_OK) {
            // If config fails, clean up and exit
            vpx_img_free(img);
            return 0;
        }
        
        // Modify some config parameters based on fuzzer input for diversity
        cfg.g_w = width;
        cfg.g_h = height;
        
        if (Size > 6) {
            cfg.g_threads = 1 + (Data[6] % 4);
        }
        if (Size > 7) {
            cfg.rc_target_bitrate = 100 + (Data[7] % 1000);
        }
        if (Size > 8) {
            cfg.rc_min_quantizer = Data[8] % 64;
        }
        if (Size > 9) {
            cfg.rc_max_quantizer = 10 + (Data[9] % 54);
        }
    } else {
        vpx_img_free(img);
        return 0;
    }
    
    // 4. vpx_codec_enc_init_ver
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    res = vpx_codec_enc_init_ver(&codec_ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (res == VPX_CODEC_OK) {
        // 5. vpx_codec_control_
        // Try different control IDs based on fuzzer input
        if (Size > 10) {
            // Use a few common control IDs
            int ctrl_ids[] = {
                8,   // VP8E_SET_CPUUSED
                9,   // VP8E_SET_ENABLEAUTOALTREF
                17,  // VP8E_SET_NOISE_SENSITIVITY
                18,  // VP8E_SET_SHARPNESS
                19,  // VP8E_SET_STATIC_THRESHOLD
                20,  // VP8E_SET_TOKEN_PARTITIONS
                21,  // VP8E_SET_ARNR_MAXFRAMES
                22,  // VP8E_SET_ARNR_STRENGTH
                23,  // VP8E_SET_ARNR_TYPE
                24,  // VP8E_SET_TUNING
                25,  // VP8E_SET_CQ_LEVEL
            };
            
            int num_ctrls = sizeof(ctrl_ids) / sizeof(ctrl_ids[0]);
            int ctrl_idx = Data[10] % num_ctrls;
            int ctrl_value = 0;
            
            if (Size > 11) {
                ctrl_value = Data[11] % 256;
            }
            
            // Call vpx_codec_control_ with the selected control ID
            vpx_codec_control_(&codec_ctx, ctrl_ids[ctrl_idx], ctrl_value);
            
            // Try another control ID if we have enough data
            if (Size > 12) {
                ctrl_idx = Data[12] % num_ctrls;
                ctrl_value = 0;
                if (Size > 13) {
                    ctrl_value = Data[13] % 256;
                }
                vpx_codec_control_(&codec_ctx, ctrl_ids[ctrl_idx], ctrl_value);
            }
        }
        
        // 7. vpx_codec_destroy (called after vpx_img_free)
        vpx_codec_destroy(&codec_ctx);
    }
    
    // 6. vpx_img_free
    vpx_img_free(img);
    
    return 0;
}