// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    
    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    
    // Initialize encoder context
    if (vpx_codec_enc_init(&ctx, vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Prepare a dummy image
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 32, 32, 1) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    
    // Fuzz various encoder controls based on input data
    if (Size >= 1) {
        uint8_t control_type = Data[0] % 6;
        
        if (Size >= 3) {
            uint16_t value;
            memcpy(&value, Data + 1, sizeof(value));
            
            switch (control_type) {
                case 0: // VP8E_SET_CPUUSED
                    vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, value % 9); // 0-8
                    break;
                case 1: // VP8E_SET_NOISE_SENSITIVITY
                    vpx_codec_control_(&ctx, VP8E_SET_NOISE_SENSITIVITY, value % 100);
                    break;
                case 2: // VP8E_SET_MAX_INTRA_BITRATE_PCT
                    vpx_codec_control_(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, value % 101); // 0-100
                    break;
                case 3: // VP8E_SET_TUNING
                    vpx_codec_control_(&ctx, VP8E_SET_TUNING, value % 2); // PSNR or SSIM
                    break;
                case 4: // VP8E_SET_TEMPORAL_LAYER_ID
                    vpx_codec_control_(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, value % 4); // 0-3
                    break;
                case 5: // VP8E_SET_FRAME_FLAGS
                    vpx_codec_control_(&ctx, VP8E_SET_FRAME_FLAGS, value);
                    break;
            }
        }
    }
    
    // Attempt to encode a frame
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    
    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    
    return 0;
}