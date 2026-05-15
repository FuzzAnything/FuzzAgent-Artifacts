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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, 
        vpx_codec_vp9_dx(), 
        &dec_cfg, 
        0, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode the input data
    uint8_t *encoded_data = new uint8_t[size];
    size_t bytes_read = fread(encoded_data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != size) {
        delete[] encoded_data;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_decode(&decoder, encoded_data, size, nullptr, 0);
    
    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
        // Create output file for encoded data
        FILE *out_file = fopen("output_file", "wb");
        if (!out_file) {
            delete[] encoded_data;
            vpx_codec_destroy(&decoder);
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Use VP9 encoder
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Get default encoder configuration
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set dimensions from decoded frame
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        
        vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
            &encoder,
            encoder_iface,
            &enc_cfg,
            0,
            VPX_ENCODER_ABI_VERSION
        );
        
        if (enc_init_result == VPX_CODEC_OK) {
            // Create a wrapped image from decoded frame data
            vpx_image_t wrapped_image;
            vpx_img_wrap(
                &wrapped_image,
                decoded_frame->fmt,
                decoded_frame->d_w,
                decoded_frame->d_h,
                1,  // stride alignment
                decoded_frame->planes[0]
            );
            
            // Set a rectangle on the image (using entire frame for simplicity)
            vpx_img_set_rect(
                &wrapped_image,
                0,  // x
                0,  // y
                decoded_frame->d_w,  // width
                decoded_frame->d_h   // height
            );
            
            // Encode the frame
            vpx_codec_encode(
                &encoder,
                &wrapped_image,
                0,  // pts
                1,  // duration
                0,  // flags
                VPX_DL_REALTIME  // deadline
            );
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write encoded data to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Check for any encoder errors
            const char *error_detail = vpx_codec_error_detail(&encoder);
            if (error_detail) {
                // Error detail available, but we continue anyway for fuzzing
            }
            
            vpx_codec_destroy(&encoder);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Check for any decoder errors
    const char *error_detail = vpx_codec_error_detail(&decoder);
    if (error_detail) {
        // Error detail available, but we continue anyway for fuzzing
    }
    
    // Cleanup
    delete[] encoded_data;
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}