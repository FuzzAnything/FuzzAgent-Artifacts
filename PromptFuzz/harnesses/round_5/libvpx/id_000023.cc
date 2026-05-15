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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000023_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Just a placeholder callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoder interface and print its name
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *decoder_name = vpx_codec_iface_name(decoder_iface);
    (void)decoder_name; // Prevent unused warning
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
        (void)error_str;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put_frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000023_put_frame_cb, nullptr);
    
    // Read and decode data
    uint8_t *buffer = new uint8_t[size];
    size_t bytes_read = fread(buffer, 1, size, in_file);
    
    if (bytes_read > 0) {
        err = vpx_codec_decode(&decoder, buffer, bytes_read, nullptr, 0);
        
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
        
        if (decoded_img) {
            // Set image rectangle (crop)
            int x = 0, y = 0;
            int w = decoded_img->d_w / 2;
            int h = decoded_img->d_h / 2;
            if (w > 0 && h > 0) {
                vpx_img_set_rect(decoded_img, x, y, w, h);
            }
            
            // Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
            
            // Get default encoder config
            vpx_codec_enc_cfg_t enc_cfg;
            unsigned int usage = 0; // Good quality
            err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
            
            // Set encoder dimensions
            enc_cfg.g_w = decoded_img->d_w;
            enc_cfg.g_h = decoded_img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            
            // Initialize multiple encoders (just one for fuzzing)
            vpx_rational_t dsf = {1, 1};
            int num_enc = 1;
            err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                              num_enc, 0, &dsf, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Set encoder config (again, just to use the API)
                err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
                
                // Set output buffer
                size_t output_buf_size = 1024 * 1024;
                uint8_t *output_buffer = new uint8_t[output_buf_size];
                vpx_fixed_buf_t buf = {output_buffer, output_buf_size};
                err = vpx_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
                
                // Encode the frame
                vpx_codec_pts_t pts = 0;
                unsigned long duration = 1;
                vpx_enc_frame_flags_t flags = 0;
                vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
                
                err = vpx_codec_encode(&encoder, decoded_img, pts, duration, flags, deadline);
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up encoder resources
                delete[] output_buffer;
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Clean up
    delete[] buffer;
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}