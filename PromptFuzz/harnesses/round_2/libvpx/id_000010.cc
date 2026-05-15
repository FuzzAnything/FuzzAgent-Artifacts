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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000010_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This callback receives decoded frames
    // For fuzzing, we don't need to do anything special
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Get extra version string
    const char* extra_version = vpx_codec_version_extra_str();
    (void)extra_version; // Use to avoid unused variable warning
    
    // 2. Try to peek stream info to determine if this is VP8/VP9 data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 interfaces
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp8_dx(), data, size, &stream_info);
    
    if (peek_result != VPX_CODEC_OK) {
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp9_dx(), data, size, &stream_info);
    }
    
    // 3. Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t init_result;
    
    // Try VP9 first, then VP8
    if (peek_result == VPX_CODEC_OK) {
        // We'll use VP9 if peek succeeded with VP9, otherwise VP8
        init_result = vpx_codec_dec_init_ver(&decoder, 
            (peek_result == VPX_CODEC_OK) ? vpx_codec_vp9_dx() : vpx_codec_vp8_dx(),
            nullptr, 0, VPX_DECODER_ABI_VERSION);
    } else {
        // Default to VP8 if peek failed
        init_result = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(),
            nullptr, 0, VPX_DECODER_ABI_VERSION);
    }
    
    if (init_result != VPX_CODEC_OK) {
        // Cleanup and return if init failed
        return 0;
    }
    
    // 4. Register put_frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000010_put_frame_callback, nullptr);
    
    // 5. Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    // 6. Get error message (if any)
    const char* error_msg = vpx_codec_error(&decoder);
    (void)error_msg; // Use to avoid unused variable warning
    
    // 7. Initialize encoder context to test preview frame functionality
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    // Use a small fixed size for encoding to avoid excessive memory usage
    unsigned int width = 64;
    unsigned int height = 64;
    
    // Initialize encoder config
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) == VPX_CODEC_OK) {
        cfg.g_w = width;
        cfg.g_h = height;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, vpx_codec_vp8_cx(), &cfg, 0,
                                   VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // 8. Get global headers (for encoder)
            vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers) {
                free(global_headers->buf);
            }
            
            // 9. Create a test image and encode
            vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
            if (img) {
                // Fill with some test data
                for (int plane = 0; plane < 3; ++plane) {
                    unsigned char* plane_ptr = img->planes[plane];
                    int stride = img->stride[plane];
                    int plane_height = (plane == 0) ? height : height / 2;
                    int plane_width = (plane == 0) ? width : width / 2;
                    
                    for (int y = 0; y < plane_height; ++y) {
                        for (int x = 0; x < plane_width; ++x) {
                            plane_ptr[y * stride + x] = (x + y) & 0xFF;
                        }
                    }
                }
                
                // Encode the image
                vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                
                // 10. Try to get preview frame (for encoder)
                const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&encoder);
                (void)preview_frame; // Use to avoid unused variable warning
                
                // Cleanup image
                vpx_img_free(img);
            }
            
            // Cleanup encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 11. Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}