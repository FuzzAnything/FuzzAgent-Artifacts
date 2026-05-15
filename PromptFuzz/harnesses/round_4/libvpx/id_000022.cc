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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"validate_config":[[125,33,125,49,1,0,4,1]],"vpx_codec_enc_init_multi_ver":[[122,13,122,16,0,0,4,0],[123,38,123,47,0,0,4,0],[129,18,129,19,0,0,4,1],[23,32,23,37,8,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000022_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is called when a frame is decoded
    // We don't need to do anything specific for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the entire input into a buffer
    uint8_t *input_buffer = (uint8_t *)malloc(size);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(input_buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    for (int codec_idx = 0; codec_idx < 2; codec_idx++) {
        vpx_codec_ctx_t decoder;
        vpx_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 1;
        
        // Initialize decoder
        vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, 
            decoder_ifaces[codec_idx], &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
        
        if (err != VPX_CODEC_OK) continue;
        
        // Get stream info from the input data
        vpx_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.sz = sizeof(si);
        
        err = vpx_codec_peek_stream_info(decoder_ifaces[codec_idx], 
            input_buffer, (unsigned int)size, &si);
        
        if (err == VPX_CODEC_OK && si.w > 0 && si.h > 0) {
            // Now get stream info using the decoder context
            vpx_codec_get_stream_info(&decoder, &si);
            
            // Initialize multiple encoders (2 encoders)
            vpx_codec_ctx_t encoders[2];
            vpx_codec_enc_cfg_t enc_cfg;
            vpx_codec_iface_t *encoder_iface = (codec_idx == 0) ? 
                vpx_codec_vp8_cx() : vpx_codec_vp9_cx();
            
            // Get default encoder configuration
            vpx_codec_err_t enc_err = vpx_codec_enc_config_default(
                encoder_iface, &enc_cfg, fuzz_uint32_t_2);
            
            if (enc_err == VPX_CODEC_OK) {
                // Configure encoder settings
                enc_cfg.g_w = si.w;
                enc_cfg.g_h = si.h;
                enc_cfg.g_threads = 1;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                
                // Downsampling factors for multiple encoders
                vpx_rational_t dsf[2];
                dsf[0].num = 1; dsf[0].den = 1;  // Full resolution
                dsf[1].num = 1; dsf[1].den = 2;  // Half resolution
                
                // Initialize multiple encoders
                enc_err = vpx_codec_enc_init_multi_ver(&encoders[0], 
                    encoder_iface, &enc_cfg, 2, fuzz_int64_t_3, dsf, VPX_ENCODER_ABI_VERSION);
                
                if (enc_err == VPX_CODEC_OK) {
                    // Register put_frame callback for each encoder
                    for (int i = 0; i < 2; i++) {
                        vpx_codec_register_put_frame_cb(&encoders[i], 
                             id_000022_put_frame_callback, nullptr);
                    }
                    
                    // Decode the input data
                    vpx_codec_decode(&decoder, input_buffer, 
                        (unsigned int)size, nullptr, fuzz_int64_t_4);
                    
                    // Get decoded frames
                    vpx_codec_iter_t iter = nullptr;
                    vpx_image_t *decoded_img;
                    int frame_count = 0;
                    
                    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                        if (frame_count++ > 10) break; // Limit frames for fuzzing
                        
                        // Wrap the decoded image using vpx_img_wrap
                        vpx_image_t wrapped_img;
                        unsigned char *img_data = decoded_img->img_data;
                        
                        if (img_data) {
                            vpx_img_wrap(&wrapped_img, decoded_img->fmt,
                                decoded_img->d_w, decoded_img->d_h,
                                fuzz_uint32_t_5, img_data);
                            
                            // Encode the wrapped image with both encoders
                            for (int i = 0; i < 2; i++) {
                                vpx_codec_encode(&encoders[i], &wrapped_img,
                                    fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
                            }
                        }
                    }
                    
                    // Clean up encoders
                    for (int i = 0; i < 2; i++) {
                        vpx_codec_destroy(&encoders[i]);
                    }
                }
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        
        // If we successfully processed this codec, break
        if (si.w > 0 && si.h > 0) {
            break;
        }
    }
    
    // Clean up
    free(input_buffer);
    
    assert_file_closed(&in_file);
	return 0;
}