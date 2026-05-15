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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"vpx_codec_error":[[55,10,55,15,0,0,4,1]]},"library_calls":["vpx_codec_error","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_error","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000016_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Just acknowledge we got a frame
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for input
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // May be used in more complex scenarios
    
    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_rational_t dsf = {1, 1};
    vpx_image_t* img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    
    // Use vpx_codec_error - check and store error string
    const char* error_str = vpx_codec_error(nullptr);
    (void)error_str;
    
    // First, try to decode the input data (assuming it's encoded video)
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000016_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        error_str = vpx_codec_error(&decoder);
        // Continue anyway
    }
    
    // Try to peek at stream info
    vpx_codec_stream_info_t si = {0};
    si.sz = sizeof(si);
    if (vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si) == VPX_CODEC_OK) {
        // Decode the frame
        err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
        if (err == VPX_CODEC_OK) {
            // Get decoded frame
            const vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
            if (decoded_img) {
                // Allocate new image for encoding
                img = vpx_img_alloc(nullptr, decoded_img->fmt, decoded_img->d_w, decoded_img->d_h, 16);
                if (img) {
                    // Copy the decoded image data
                    for (int plane = 0; plane < 3; plane++) {
                        if (decoded_img->planes[plane]) {
                            size_t plane_size = decoded_img->stride[plane] * 
                                              (plane == 0 ? decoded_img->d_h : 
                                               decoded_img->d_h >> decoded_img->y_chroma_shift);
                            if (img->planes[plane] && decoded_img->planes[plane]) {
                                memcpy(img->planes[plane], decoded_img->planes[plane], plane_size);
                            }
                        }
                    }
                    
                    // Use vpx_img_set_rect to set a rectangle on the image
                    // Use safe values within image bounds
                    unsigned int rect_w = decoded_img->d_w > 100 ? 100 : decoded_img->d_w;
                    unsigned int rect_h = decoded_img->d_h > 100 ? 100 : decoded_img->d_h;
                    vpx_img_set_rect(img, 0, 0, rect_w, rect_h);
                    
                    // Now set up encoding
                    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
                    
                    // Get default encoder configuration
                    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
                    if (err == VPX_CODEC_OK) {
                        cfg.g_w = rect_w;
                        cfg.g_h = rect_h;
                        cfg.g_timebase.num = 1;
                        cfg.g_timebase.den = 30;
                        cfg.rc_target_bitrate = 1000;
                        
                        // Initialize encoder using vpx_codec_enc_init_ver
                        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
                        if (err == VPX_CODEC_OK) {
                            // Also try multi-encoder init (though we only need one)
                            vpx_codec_ctx_t multi_encoder = {0};
                            vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
                            vpx_codec_destroy(&multi_encoder);
                            
                            // Use vpx_codec_control_ - set CPU used to fastest
                            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
                            
                            // Encode the image
                            err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                            if (err == VPX_CODEC_OK) {
                                // Get preview frame
                                const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder);
                                (void)preview; // May be null
                                
                                // Get encoded data
                                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                        // Got encoded frame
                                        break;
                                    }
                                }
                            }
                            
                            // Get error status
                            error_str = vpx_codec_error(&encoder);
                            
                            vpx_codec_destroy(&encoder);
                        }
                    }
                    
                    vpx_img_free(img);
                }
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}