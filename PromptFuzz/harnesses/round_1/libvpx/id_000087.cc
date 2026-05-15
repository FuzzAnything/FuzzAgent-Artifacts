#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"vpx_codec_enc_init_ver":[[35,7,35,37,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a fuzz driver that uses the specified libvpx APIs to decode and re-encode VP8 video data:

*/


extern "C" {
}

// Frame buffer callback functions
static int  id_000087_get_frame_buffer(void *cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000087_release_frame_buffer(void *cb_priv, 
                               vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

// Put frame callback
static void  id_000087_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Callback placeholder - no actual implementation needed for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // 1. Get encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // 2. Get interface name
    const char *iface_name = vpx_codec_iface_name(encoder_iface);
    (void)iface_name;
    
    // 3. Set up encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_err_t enc_err = vpx_codec_enc_config_default(
        encoder_iface, &enc_cfg, 0);
    if (enc_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // 4. Initialize multi-encoder context (using num_enc = 1)
    vpx_codec_ctx_t enc_ctx_multi;
    vpx_rational_t dsf = {1, 1};
    enc_err = vpx_codec_enc_init_multi_ver(&enc_ctx_multi, encoder_iface, 
                                          &enc_cfg, 1, 0, &dsf, 4);
    if (enc_err != VPX_CODEC_OK) {
        // Try single encoder initialization
        vpx_codec_ctx_t enc_ctx;
        enc_err = vpx_codec_enc_init_ver(&enc_ctx, encoder_iface, &enc_cfg, 
                                        0, 4);
        if (enc_err != VPX_CODEC_OK) {
            return 0;
        }
        
        // 5. Set frame buffer functions
        enc_err = vpx_codec_set_frame_buffer_functions(&enc_ctx, 
             id_000087_get_frame_buffer,  id_000087_release_frame_buffer, nullptr);
        
        // 6. Update encoder configuration
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
        enc_err = vpx_codec_enc_config_set(&enc_ctx, &enc_cfg);
        
        // 7. Allocate image
        vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                         640, 480, 1);
        if (!img) {
            vpx_codec_destroy(&enc_ctx);
            return 0;
        }
        
        // 8. Wrap image data
        vpx_image_t wrapped_img;
        uint8_t img_data[640 * 480 * 3 / 2]; // YUV420
        vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 640, 480, 1, img_data);
        
        // 9. Set image rectangle
        vpx_img_set_rect(&wrapped_img, 0, 0, 320, 240);
        
        // 10. Flip image
        vpx_img_flip(&wrapped_img);
        
        // 11. Set encoder control
        vpx_codec_control_(&enc_ctx, VP8E_SET_CPUUSED, 8);
        
        // 12. Get global headers
        vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&enc_ctx);
        if (headers) {
            free(headers->buf);
        }
        
        // 13. Get frame from decoder (setup decoder first)
        vpx_codec_ctx_t dec_ctx;
        vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
        vpx_codec_err_t dec_err = vpx_codec_dec_init_ver(&dec_ctx, 
            decoder_iface, nullptr, 0, 4);
        
        if (dec_err == VPX_CODEC_OK) {
            // Decode input data
            dec_err = vpx_codec_decode(&dec_ctx, data, size, nullptr, 0);
            
            vpx_codec_iter_t iter = nullptr;
            const vpx_image_t *decoded_img = vpx_codec_get_frame(&dec_ctx, 
                                                                 &iter);
            
            // 14. Register put frame callback
            vpx_codec_register_put_frame_cb(&dec_ctx,  id_000087_put_frame_callback, 
                                            nullptr);
            
            if (decoded_img && img) {
                // Copy decoded data to encoder input image
                memcpy(img->planes[0], data, 
                       std::min(size, (size_t)(640 * 480)));
            }
            
            vpx_codec_destroy(&dec_ctx);
        }
        
        // 15. Encode the image
        enc_err = vpx_codec_encode(&enc_ctx, img, 0, 1, 0, 0);
        
        // 16. Get error detail
        const char *error_detail = vpx_codec_error_detail(&enc_ctx);
        (void)error_detail;
        
        // 17. Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &enc_iter)) != nullptr) {
            // Process encoded packets
            (void)pkt;
        }
        
        // 18. Free image
        vpx_img_free(img);
        
        // 19. Destroy encoder
        vpx_codec_destroy(&enc_ctx);
        
        vpx_codec_destroy(&enc_ctx_multi);
    }
    
    return 0;
}