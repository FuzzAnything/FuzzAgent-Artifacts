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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_enc_config_set","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_control_","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_enc_config_set","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_control_","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*To achieve the described event using the provided libvpx APIs with a byte stream of libvpx output data as input, I'll create a fuzzer that:

1. Decodes compressed video data
2. Potentially re-encodes it (though encoding would normally require additional data)
3. Exercises various encoding/decoding pathways

Here's the complete fuzzer implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // We'll use VP9 codec for this fuzzer
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Temporary file for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Output file for encoded data (won't actually be used much)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_image_t *img = nullptr;
    vpx_rational_t dsf = {1, 1}; // Downsampling factor
    
    int rc = 0;
    
    // Step 1: Try to peek at stream info to see if it's valid VP8/VP9 data
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                    (unsigned int)(size > 1024 ? 1024 : size), 
                                                    &si);
    // We continue even if peek fails - input might be corrupted
    
    // Step 2: Initialize decoder
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, 
                                 VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Get stream info from decoder context
    err = vpx_codec_get_stream_info(&decoder, &si);
    // Ignore error - stream might be corrupted
    
    // Step 4: Try to decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    
    // Step 5: Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    // Step 6: If we got a decoded frame, try to encode it
    if (decoded_img) {
        // Initialize encoder configuration
        err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        
        if (err == VPX_CODEC_OK) {
            // Set reasonable dimensions based on decoded image
            cfg.g_w = decoded_img->d_w > 0 ? decoded_img->d_w : 640;
            cfg.g_h = decoded_img->d_h > 0 ? decoded_img->d_h : 480;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.rc_target_bitrate = 1000;
            cfg.g_threads = 1;
            
            // Step 7: Initialize encoder with multi_ver API
            err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 
                                              1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Step 8: Use control API to set some parameters
                // Try to set a control (e.g., set noise sensitivity)
                vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);
                
                // Step 9: Allocate an image for encoding
                img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
                
                if (img) {
                    // Copy some data from decoded image or use test pattern
                    // For fuzzing, we'll just fill with some data
                    if (size > 0 && decoded_img->planes[0]) {
                        // Copy minimal data to avoid overflow
                        size_t copy_size = cfg.g_w * cfg.g_h;
                        if (copy_size > size) copy_size = size;
                        
                        for (int i = 0; i < 3; i++) {
                            if (img->planes[i]) {
                                size_t plane_size = img->stride[i] * 
                                                  (i == 0 ? cfg.g_h : cfg.g_h / 2);
                                memset(img->planes[i], i * 64, plane_size);
                            }
                        }
                    }
                    
                    // Step 10: Try to encode the image
                    err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                    
                    // Step 11: Try to get preview frame
                    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                    (void)preview; // Just to use the API
                    
                    // Step 12: Set encoder configuration (even though already set)
                    err = vpx_codec_enc_config_set(&encoder, &cfg);
                    
                    // Clean up allocated image
                    vpx_img_free(img);
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Step 13: Try to encode even without decoded image (using random data)
    if (!decoded_img || size > 100) {
        // Initialize encoder
        err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        
        if (err == VPX_CODEC_OK) {
            cfg.g_w = 64;
            cfg.g_h = 64;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.rc_target_bitrate = 1000;
            cfg.g_threads = 1;
            
            err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                                        VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Allocate test image
                img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
                
                if (img) {
                    // Fill with some data from input
                    for (int i = 0; i < 3; i++) {
                        if (img->planes[i]) {
                            size_t plane_size = img->stride[i] * 
                                              (i == 0 ? cfg.g_h : cfg.g_h / 2);
                            for (size_t j = 0; j < plane_size && j < size; j++) {
                                img->planes[i][j] = data[j % size];
                            }
                        }
                    }
                    
                    // Try to encode
                    err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                    
                    // Try control API again
                    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
                    
                    vpx_img_free(img);
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    
    // Write some output (minimal)
    if (out_file) {
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}