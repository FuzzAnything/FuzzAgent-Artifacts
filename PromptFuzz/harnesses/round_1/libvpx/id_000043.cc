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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    vpx_codec_ctx_t encoder = {};
    vpx_codec_ctx_t decoder = {};
    vpx_image_t img = {};
    vpx_codec_enc_cfg_t cfg = {};
    vpx_codec_stream_info_t si = {};
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    const vpx_image_t *frame = nullptr;
    
    // Get codec interface for VP9 encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // 1. Get codec interface name and capabilities
    const char *iface_name = vpx_codec_iface_name(encoder_iface);
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    
    // If we have enough data, try to decode it
    if (size > 0) {
        // Try to peek at stream info
        if (vpx_codec_peek_stream_info(decoder_iface, data, size, &si) == VPX_CODEC_OK) {
            // Initialize decoder
            if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
                // Decode the input data
                if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
                    // Get decoded frame(s)
                    while ((frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                        // Process decoded frame here if needed
                    }
                }
                vpx_codec_destroy(&decoder);
            }
        }
    }
    
    // 2. Initialize encoder
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
        // Set some basic configuration
        cfg.g_w = 320;
        cfg.g_h = 240;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            // 3. Get global headers
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            
            // 4. Create and encode some frames using the input data
            if (size > 0) {
                // Allocate image buffer
                unsigned int align = 1;
                unsigned int width = 320;
                unsigned int height = 240;
                vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
                
                // Calculate required size for I420 image
                size_t y_size = width * height;
                size_t uv_size = y_size / 4;
                size_t total_size = y_size + 2 * uv_size;
                
                // Allocate buffer for image data
                unsigned char *img_data = (unsigned char *)malloc(total_size);
                if (img_data) {
                    // Fill image data with input data (repeating if necessary)
                    size_t bytes_copied = 0;
                    while (bytes_copied < total_size && size > 0) {
                        size_t to_copy = (total_size - bytes_copied < size) ? 
                                        (total_size - bytes_copied) : size;
                        memcpy(img_data + bytes_copied, data, to_copy);
                        bytes_copied += to_copy;
                    }
                    
                    // 5. Wrap the allocated buffer in vpx_image_t
                    vpx_image_t *wrapped_img = vpx_img_wrap(&img, fmt, width, height, align, img_data);
                    
                    if (wrapped_img) {
                        // Encode the frame
                        vpx_codec_encode(&encoder, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        // 6. Get compressed data
                        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                            // Process compressed packets here if needed
                        }
                        
                        // 7. Get preview frame if available
                        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                        // Preview frame could be nullptr if not available
                        
                        // Free the image data buffer
                        free(img_data);
                    }
                }
            }
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Clean up any remaining resources
    if (img.img_data_owner && img.img_data) {
        free(img.img_data);
    }
    
    return 0;
}