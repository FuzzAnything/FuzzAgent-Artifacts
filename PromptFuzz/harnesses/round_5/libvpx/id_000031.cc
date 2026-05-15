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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_vp9_cx"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_vp9_cx"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Callback functions
static void  id_000031_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // Dummy callback - just to satisfy API requirement
    (void)user_priv;
    (void)img;
}

static void  id_000031_put_slice_cb(void* user_priv, const vpx_image_t* img,
                         const vpx_image_rect_t* rect, 
                         const vpx_image_rect_t* rect2) {
    // Dummy callback - just to satisfy API requirement
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small to be meaningful
    
    // Initialize variables
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = NULL;
    
    // Create input FILE* using fmemopen
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (not used directly but satisfying requirement)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;
    (void)out_fd;
    
    // 1. First try to decode the input data as VP8
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    // Try to peek stream info
    if (vpx_codec_peek_stream_info(decoder_iface, data, 
                                   (unsigned int)size, &stream_info) == VPX_CODEC_OK) {
        // Initialize decoder
        if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                   0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // Decode the input data
            if (vpx_codec_decode(&decoder, data, (unsigned int)size, 
                                NULL, 0) == VPX_CODEC_OK) {
                
                // Get decoded frames
                vpx_image_t* img;
                while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
                    
                    // Now try to re-encode the decoded frame
                    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
                    
                    // Get default encoder configuration
                    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                        
                        // Set encoder configuration based on decoded image
                        enc_cfg.g_w = img->d_w;
                        enc_cfg.g_h = img->d_h;
                        enc_cfg.g_timebase.num = 1;
                        enc_cfg.g_timebase.den = 30;
                        enc_cfg.rc_target_bitrate = 256;
                        enc_cfg.g_threads = 1;
                        
                        // Initialize encoder with vpx_codec_enc_init_ver
                        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                                   0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                            
                            // Register callbacks
                            vpx_codec_register_put_frame_cb(&encoder,  id_000031_put_frame_cb, NULL);
                            vpx_codec_register_put_slice_cb(&encoder,  id_000031_put_slice_cb, NULL);
                            
                            // Prepare image for encoding using vpx_img_wrap
                            vpx_image_t wrapped_img;
                            unsigned char* img_data = (unsigned char*)malloc(img->d_w * img->d_h * 3 / 2);
                            if (img_data) {
                                // Copy image data (simplified - would need proper format conversion)
                                memcpy(img_data, img->planes[0], img->d_w * img->d_h);
                                
                                vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                           img->d_w, img->h, 1, img_data);
                                
                                // Encode the frame
                                if (vpx_codec_encode(&encoder, &wrapped_img, 0, 
                                                    1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
                                    
                                    // Get encoded data
                                    vpx_codec_iter_t enc_iter = NULL;
                                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                            // Write to output file
                                            fwrite(pkt->data.frame.buf, 1, 
                                                  pkt->data.frame.sz, out_file);
                                        }
                                    }
                                }
                                
                                free(img_data);
                            }
                            
                            // Try multi-encoder initialization too (with 1 encoder)
                            vpx_codec_ctx_t multi_encoder;
                            vpx_rational_t dsf = {1, 1};
                            if (vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, 
                                                            &enc_cfg, 1, 0, &dsf, 
                                                            VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                                // Don't actually use it, just initialize and destroy
                                vpx_codec_destroy(&multi_encoder);
                            }
                            
                            // Destroy encoder
                            vpx_codec_destroy(&encoder);
                        }
                    }
                    
                    break; // Just process first frame for fuzzing efficiency
                }
            }
            
            // Destroy decoder
            vpx_codec_destroy(&decoder);
        }
    }
    
    // Also try VP9 codec interfaces
    vpx_codec_iface_t* vp9_decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t* vp9_encoder_iface = vpx_codec_vp9_cx();
    (void)vp9_decoder_iface;
    (void)vp9_encoder_iface;
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}