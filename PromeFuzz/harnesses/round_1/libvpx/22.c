// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

static int get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return 0;
}

static int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    
    // Initialize configuration with fuzzer data
    cfg.threads = (Data[0] % 8) + 1;
    cfg.w = (Data[1] % 256) + 1;
    cfg.h = (Data[2] % 256) + 1;
    
    // Choose codec interface based on input
    vpx_codec_iface_t *iface;
    if (Data[3] % 2 == 0) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp9_dx();
    }
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Try to set frame buffer functions with some probability
    if (Size > 8 && (Data[4] % 3 == 0)) {
        void *cb_priv = NULL;
        if (Size > 16) {
            cb_priv = (void *)(Data + 8);
        }
        vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb, 
                                           release_frame_buffer_cb, cb_priv);
    }
    
    // Try various control operations
    if (Size > 12) {
        int ctrl_id = Data[5] % 256;
        int ctrl_value = Data[6];
        
        // Try different control IDs with different parameter types
        switch (ctrl_id % 4) {
            case 0:
                vpx_codec_control(&ctx, VP8D_GET_LAST_REF_UPDATES, &ctrl_value);
                break;
            case 1:
                vpx_codec_control(&ctx, VP8D_GET_FRAME_CORRUPTED, &ctrl_value);
                break;
            case 2:
                vpx_codec_control(&ctx, VP9D_GET_DISPLAY_SIZE, &ctrl_value);
                break;
            case 3:
                vpx_codec_control(&ctx, VP8D_SET_DECRYPTOR, NULL);
                break;
        }
    }
    
    // Try to get preview frame (might fail for decoder, but that's OK)
    img = vpx_codec_get_preview_frame(&ctx);
    (void)img;  // Ignore result
    
    // Try to get frames from iterator
    if (Size > 20) {
        int max_iterations = Data[7] % 10;
        for (int i = 0; i < max_iterations; i++) {
            vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
            if (!frame) break;
        }
    }
    
    // Clean up
    vpx_codec_destroy(&ctx);
    
    return 0;
}