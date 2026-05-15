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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
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
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
        return 0;
    }

    // Peek at stream info
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    if (vpx_codec_peek_stream_info(decoder_iface, data, static_cast<unsigned int>(size), &si) == VPX_CODEC_OK) {
        // Get stream info after decoding
        vpx_codec_get_stream_info(&decoder, &si);
    }

    // Decode the input
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&decoder);
        (void)detail; // Prevent unused variable warning
    }

    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img;
    std::vector<vpx_image_t*> decoded_frames;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_frames.push_back(img);
    }

    // If we have decoded frames, try to re-encode them
    if (!decoded_frames.empty()) {
        // Initialize encoder
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
        if (err == VPX_CODEC_OK) {
            // Configure encoder based on first decoded frame
            vpx_image_t *first_frame = decoded_frames[0];
            enc_cfg.g_w = first_frame->d_w;
            enc_cfg.g_h = first_frame->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_threads = 1;
            
            // Use vpx_codec_enc_init_multi_ver
            vpx_rational_t dsf = {1, 1};
            err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 1, fuzz_int64_t_4, &dsf, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Register put frame callback (dummy implementation)
                auto dummy_callback = [](void*, const vpx_image_t*) {};
                vpx_codec_register_put_frame_cb(&encoder, dummy_callback, nullptr);
                
                // Use control API
                vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
                vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
                
                // Get global headers
                vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&encoder);
                if (headers) {
                    free(headers->buf);
                }
                
                // Create input image for encoder
                vpx_image_t *enc_input = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                      enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_5);
                
                if (enc_input) {
                    // For simplicity, use first frame as input
                    // In real scenario, would copy from decoded frame
                    
                    // Set a rectangle on the image
                    vpx_img_set_rect(enc_input, fuzz_uint32_t_6, fuzz_uint32_t_7, 
                                    std::min(16u, enc_cfg.g_w), 
                                    std::min(16u, enc_cfg.g_h));
                    
                    // Encode the frame
                    for (int i = 0; i < 2; i++) { // Encode a couple of times
                        vpx_enc_frame_flags_t flags = 0;
                        if (i == 0) {
                            flags = VPX_EFLAG_FORCE_KF;
                        }
                        
                        err = vpx_codec_encode(&encoder, enc_input, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                        
                        if (err == VPX_CODEC_OK) {
                            // Get encoded data
                            vpx_codec_iter_t enc_iter = nullptr;
                            const vpx_codec_cx_pkt_t *pkt;
                            
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                // Process encoded packet
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Could write to file or process further
                                }
                            }
                        }
                    }
                    
                    vpx_img_free(enc_input);
                }
                
                // Create another image using vpx_img_wrap
                size_t img_size = enc_cfg.g_w * enc_cfg.g_h * 3 / 2;
                std::vector<unsigned char> img_data(img_size);
                vpx_image_t *wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420,
                                                       enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_12,
                                                       img_data.data());
                if (wrapped_img) {
                    vpx_img_free(wrapped_img);
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}