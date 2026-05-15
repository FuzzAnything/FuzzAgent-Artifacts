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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/



// Callback to handle decoded frames
static void  id_000028_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Store frame info or process it (for fuzzing, we just note it was called)
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<124) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_17, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_18, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_19, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_20, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file (will be deleted after use)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;
    (void)out_fd;
    
    // Initialize decoder for VP9
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, 
                                                VPX_DECODER_ABI_VERSION);
    
    // Check decoder initialization
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_str;
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000028_put_frame_callback, nullptr);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        (void)error_str;
    }
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    
    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
        // Initialize encoder for VP9
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
        if (res != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(res);
            (void)error_str;
        } else {
            // Set encoder configuration based on decoded frame
            enc_cfg.g_w = decoded_frame->d_w;
            enc_cfg.g_h = decoded_frame->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            
            // Initialize encoder
            res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, 
                                        VPX_ENCODER_ABI_VERSION);
            
            if (res == VPX_CODEC_OK) {
                // Use codec control (example: set CPU used)
                vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
                
                // Set encoder configuration again
                vpx_codec_enc_config_set(&encoder, &enc_cfg);
                
                // Create image for encoding
                vpx_image_t *img_for_enc = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                        decoded_frame->d_w, 
                                                        decoded_frame->d_h, fuzz_uint32_t_5);
                
                if (img_for_enc) {
                    // Copy decoded frame data (simplified - just use the decoded frame)
                    // In real usage, we'd copy the actual image data
                    (void)img_for_enc;
                    
                    // Set a rectangle on the image
                    vpx_img_set_rect(img_for_enc, fuzz_uint32_t_6, fuzz_uint32_t_7, 
                                    decoded_frame->d_w, 
                                    decoded_frame->d_h);
                    
                    // Encode the frame
                    res = vpx_codec_encode(&encoder, img_for_enc, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                    
                    if (res == VPX_CODEC_OK) {
                        // Get encoded data
                        vpx_codec_iter_t enc_iter = nullptr;
                        const vpx_codec_cx_pkt_t *pkt;
                        
                        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                // Write encoded data to file (simulated)
                                (void)out_file;
                            }
                        }
                    }
                    
                    // Free the allocated image
                    vpx_img_free(img_for_enc);
                }
                
                // Create another image and test more encoding
                vpx_image_t *img2 = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_12);
                if (img2) {
                    // Set rectangle
                    vpx_img_set_rect(img2, fuzz_uint32_t_13, fuzz_uint32_t_14, fuzz_uint32_t_15, fuzz_uint32_t_16);
                    
                    // Encode again
                    res = vpx_codec_encode(&encoder, img2, fuzz_int64_t_17, fuzz_uint64_t_18, fuzz_int64_t_19, fuzz_uint64_t_20);
                    
                    if (res == VPX_CODEC_OK) {
                        vpx_codec_iter_t enc_iter2 = nullptr;
                        const vpx_codec_cx_pkt_t *pkt2;
                        
                        while ((pkt2 = vpx_codec_get_cx_data(&encoder, &enc_iter2)) != nullptr) {
                            if (pkt2->kind == VPX_CODEC_CX_FRAME_PKT) {
                                // Process encoded packet
                                (void)pkt2;
                            }
                        }
                    }
                    
                    vpx_img_free(img2);
                }
                
                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // Close files
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