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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000065_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This is just a stub callback for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get codec interfaces
    vpx_codec_iface_t *vp9_decoder = vpx_codec_vp9_dx();
    vpx_codec_iface_t *vp8_encoder = vpx_codec_vp8_cx();
    
    // 3. Get codec capabilities
    vpx_codec_caps_t vp9_caps = vpx_codec_get_caps(vp9_decoder);
    vpx_codec_caps_t vp8_caps = vpx_codec_get_caps(vp8_encoder);
    (void)vp9_caps; (void)vp8_caps;  // Mark as used
    
    // 4. Get interface names
    const char *vp9_name = vpx_codec_iface_name(vp9_decoder);
    const char *vp8_name = vpx_codec_iface_name(vp8_encoder);
    (void)vp9_name; (void)vp8_name;  // Mark as used
    
    // 5. Try to peek stream info to determine if it's valid VP9 data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try with first part of data for peeking
    size_t peek_size = size > 1000 ? 1000 : size;
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        vp9_decoder, data, (unsigned int)peek_size, &stream_info);
    
    // 6. Initialize decoder context
    vpx_codec_ctx_t decoder;
    memset(&decoder, 0, sizeof(decoder));
    
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    // Only initialize decoder if peek was successful or we want to try anyway
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, vp9_decoder, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err == VPX_CODEC_OK) {
        // 7. Register put frame callback
        vpx_codec_register_put_frame_cb(&decoder,  id_000065_put_frame_callback, NULL);
        
        // 8. Decode the input data
        vpx_codec_err_t decode_err = vpx_codec_decode(
            &decoder, data, (unsigned int)size, NULL, 0);
        
        // 9. Get stream info from decoder context
        vpx_codec_stream_info_t dec_stream_info;
        memset(&dec_stream_info, 0, sizeof(dec_stream_info));
        dec_stream_info.sz = sizeof(dec_stream_info);
        vpx_codec_get_stream_info(&decoder, &dec_stream_info);
        
        // 10. Get decoded frames
        vpx_codec_iter_t iter = NULL;
        const vpx_image_t *decoded_frame;
        while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
            // Got a frame - do nothing for fuzzing
        }
        
        // 11. Get preview frame (might be NULL)
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
        (void)preview_frame;  // Mark as used
        
        // 12. If we got valid dimensions from decoding, try to encode
        if (dec_stream_info.w > 0 && dec_stream_info.h > 0) {
            // Allocate image for encoder input
            vpx_image_t *img = vpx_img_alloc(
                NULL, VPX_IMG_FMT_I420, 
                dec_stream_info.w, dec_stream_info.h, 1);
            
            if (img) {
                // Initialize encoder context
                vpx_codec_ctx_t encoder;
                memset(&encoder, 0, sizeof(encoder));
                
                vpx_codec_enc_cfg_t enc_cfg;
                memset(&enc_cfg, 0, sizeof(enc_cfg));
                
                // Get default configuration
                vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
                    vp8_encoder, &enc_cfg, 0);
                
                if (cfg_err == VPX_CODEC_OK) {
                    // Set encoder dimensions
                    enc_cfg.g_w = dec_stream_info.w;
                    enc_cfg.g_h = dec_stream_info.h;
                    enc_cfg.rc_target_bitrate = 1000;
                    
                    // Initialize encoder
                    vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(
                        &encoder, vp8_encoder, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
                    
                    if (enc_init_err == VPX_CODEC_OK) {
                        // Create output buffer for encoder
                        vpx_fixed_buf_t output_buf;
                        output_buf.buf = malloc(100000);  // Arbitrary size
                        output_buf.sz = 100000;
                        
                        if (output_buf.buf) {
                            // Set output buffer
                            vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                            
                            // Try to encode one frame
                            vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                            
                            free(output_buf.buf);
                        }
                        
                        // Destroy encoder
                        vpx_codec_destroy(&encoder);
                    }
                }
                
                // Free allocated image
                vpx_img_free(img);
            }
        }
        
        // 13. Destroy decoder
        vpx_codec_destroy(&decoder);
    }
    
    // 14. Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}