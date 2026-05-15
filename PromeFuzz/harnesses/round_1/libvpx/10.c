// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t err;
    const char *err_detail;
    
    // Initialize configuration with fuzzed data
    cfg.threads = Size > 0 ? (Data[0] % 8) + 1 : 1;
    cfg.w = Size > 1 ? (Data[1] * 4) : 320;
    cfg.h = Size > 2 ? (Data[2] * 4) : 240;
    
    // Try to initialize decoder with VP8 interface
    err = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp8_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP9 if VP8 fails
        err = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp9_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    }
    
    if (err == VPX_CODEC_OK) {
        // Decode the fuzzed data
        err = vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
        
        // Get error details if decode failed
        if (err != VPX_CODEC_OK) {
            err_detail = vpx_codec_error_detail(&ctx);
            // err_detail may be NULL, that's fine
        }
        
        // Only call vpx_codec_control_ with valid control IDs
        if (Size > 0) {
            // Use a small set of known valid control IDs to avoid crashes
            // These are common control IDs from vpx_decoder.h
            int valid_ctrl_ids[] = {
                0,  // VP8D_GET_LAST_REF_UPDATES
                1,  // VP8D_GET_FRAME_CORRUPTED
                2,  // VP8D_GET_LAST_REF_USED
                256, // VP8D_SET_REFERENCE
                257, // VP8D_COPY_REFERENCE
                258, // VP8D_SET_POSTPROC
                259  // VP8D_SET_DBG_COLOR_REF_FRAME
            };
            
            int num_valid_ids = sizeof(valid_ctrl_ids) / sizeof(valid_ctrl_ids[0]);
            int ctrl_id_index = Data[0] % num_valid_ids;
            int ctrl_id = valid_ctrl_ids[ctrl_id_index];
            
            // Prepare parameters based on control ID
            if (ctrl_id == 0 || ctrl_id == 1 || ctrl_id == 2) {
                // These controls expect int* parameters
                // Use a static variable to avoid stack-use-after-scope
                static int param;
                param = Size > 1 ? Data[1] : 0;
                vpx_codec_control_(&ctx, ctrl_id, &param);
            } else if (ctrl_id == 256 || ctrl_id == 257) {
                // These controls expect vpx_ref_frame_t* parameters
                // Create a static structure to avoid stack-use-after-scope
                static struct vpx_ref_frame {
                    int frame_type;
                    vpx_image_t img;
                } ref_frame;
                
                memset(&ref_frame, 0, sizeof(ref_frame));
                ref_frame.frame_type = Size > 1 ? Data[1] % 4 : 0;
                vpx_codec_control_(&ctx, ctrl_id, &ref_frame);
            } else {
                // For other controls, pass NULL
                vpx_codec_control_(&ctx, ctrl_id, NULL);
            }
        }
        
        // Get decoded frames
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Access some image properties to explore more states
            int dummy = img->w + img->h + img->fmt;
            dummy = dummy;  // Avoid unused variable warning
        }
        
        // Try to decode again with NULL data to flush
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        // Get frames again after flush
        iter = NULL;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            // Access image properties
            int dummy = img->d_w + img->d_h;
            dummy = dummy;
        }
        
        // Clean up
        vpx_codec_destroy(&ctx);
    }
    
    return 0;
}