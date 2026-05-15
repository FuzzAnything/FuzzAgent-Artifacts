// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Dummy callback function
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (iface == NULL) {
        return 0;
    }
    
    vpx_codec_dec_cfg_t cfg = {0};
    
    // Initialize decoder
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register callback
    vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    
    // Use first byte to determine how to split data for decode calls
    size_t split_point = (Data[0] % (Size > 1 ? Size : 1));
    if (split_point == 0 && Size > 1) split_point = 1;
    
    // First decode call with part of data
    if (split_point > 0 && split_point <= Size) {
        vpx_codec_decode(&ctx, Data, (unsigned int)split_point, NULL, 0);
    }
    
    // Try to get stream info
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&ctx, &si);
    
    // Try various control IDs based on input data
    if (Size > 2) {
        // Use second byte for control ID (mask to reasonable range)
        int ctrl_id = Data[1] % 64;
        // Use third byte for control value
        int ctrl_value = Data[2];
        
        // Try different control types based on control ID
        switch (ctrl_id % 4) {
            case 0:
                vpx_codec_control_(&ctx, ctrl_id, ctrl_value);
                break;
            case 1:
                vpx_codec_control_(&ctx, ctrl_id, (void*)&ctrl_value);
                break;
            case 2:
                vpx_codec_control_(&ctx, ctrl_id, (int*)&ctrl_value);
                break;
            case 3:
                vpx_codec_control_(&ctx, ctrl_id, (unsigned int)ctrl_value);
                break;
        }
    }
    
    // Second decode call with remaining data (if any)
    if (Size > split_point) {
        vpx_codec_decode(&ctx, Data + split_point, (unsigned int)(Size - split_point), NULL, 0);
    }
    
    // Flush decoder
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    // Get all decoded frames
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Access some image properties to explore more states
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
    }
    
    // Clean up
    vpx_codec_destroy(&ctx);
    
    return 0;
}