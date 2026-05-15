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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Looking at the APIs provided, I'll create a fuzz driver that processes libvpx encoded video data through decoding and re-encoding. Here's a comprehensive fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = NULL;
    vpx_image_t *wrapped_img = NULL;
    vpx_fixed_buf_t encoded_buf = {0};
    vpx_codec_stream_info_t stream_info = {0};
    int ret = 0;
    
    // Use vpx_codec_iface_name to get codec interface name
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    if (!iface_name) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use vpx_codec_get_caps to get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Try to peek stream info from input data
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Initialize decoder
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Use vpx_codec_err_to_string to convert error
        const char *err_str = vpx_codec_err_to_string(err);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, NULL, 0);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
    }
    
    // Get stream info after decoding
    stream_info.sz = sizeof(stream_info);
    err = vpx_codec_get_stream_info(&decoder, &stream_info);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
    }
    
    // Get decoded frames
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        if (img->d_w > 0 && img->d_h > 0) {
            // Try to wrap image data for re-encoding
            size_t img_data_size = img->d_w * img->d_h * 3; // Estimate for RGB
            unsigned char *img_data = (unsigned char *)malloc(img_data_size);
            if (img_data) {
                // Fill with some data
                memset(img_data, 128, img_data_size);
                
                // Use vpx_img_wrap to wrap the image data
                wrapped_img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, 
                                          img->d_w, img->d_h, 1, img_data);
                
                if (wrapped_img) {
                    // Initialize encoder configuration
                    vpx_codec_enc_cfg_t cfg = {0};
                    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
                    
                    // Get default encoder configuration
                    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
                    if (err == VPX_CODEC_OK) {
                        cfg.g_w = img->d_w;
                        cfg.g_h = img->d_h;
                        cfg.g_timebase.num = 1;
                        cfg.g_timebase.den = 30;
                        cfg.rc_target_bitrate = 1000;
                        cfg.g_error_resilient = 0;
                        
                        // Initialize encoder with multi_ver API
                        // Use vpx_codec_enc_init_multi_ver
                        vpx_rational_t dsf = {1, 1};
                        err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, 
                                                          &cfg, 1, 0, &dsf, 
                                                          VPX_ENCODER_ABI_VERSION);
                        
                        if (err == VPX_CODEC_OK) {
                            // Prepare output buffer
                            encoded_buf.buf = malloc(size * 2); // Allocate larger buffer
                            encoded_buf.sz = size * 2;
                            
                            if (encoded_buf.buf) {
                                // Use vpx_codec_set_cx_data_buf to set output buffer
                                err = vpx_codec_set_cx_data_buf(&encoder, &encoded_buf, 0, 0);
                                
                                if (err == VPX_CODEC_OK) {
                                    // Try to encode the wrapped image
                                    err = vpx_codec_encode(&encoder, wrapped_img, 
                                                          0, 1, 0, VPX_DL_REALTIME);
                                    
                                    if (err == VPX_CODEC_OK) {
                                        // Try to get preview frame
                                        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                                        
                                        // Iterate through encoded packets
                                        const vpx_codec_cx_pkt_t *pkt = NULL;
                                        vpx_codec_iter_t enc_iter = NULL;
                                        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                                // Process encoded frame
                                            }
                                        }
                                    }
                                }
                                
                                free(encoded_buf.buf);
                            }
                            
                            // Destroy encoder
                            vpx_codec_destroy(&encoder);
                        }
                    }
                    
                    // Free wrapped image (doesn't free img_data since we allocated it)
                    free(img_data);
                }
            }
        }
    }
    
    // Clean up
    if (wrapped_img) {
        // vpx_img_free should not be called on wrapped images with external buffers
    }
    
    // Destroy decoder
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
}