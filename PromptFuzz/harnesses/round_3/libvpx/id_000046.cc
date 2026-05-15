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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"copy_segment_id":[[124,11,124,27,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

// Global callback for put_slice (minimal implementation)
static void  id_000046_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, 
                               const vpx_image_rect_t* rect2) {
    // Minimal implementation for fuzzing
    (void)user_priv; (void)img; (void)rect; (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	  // Need minimum data
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decoder setup
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 128;
    dec_cfg.h = 128;

    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                 &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put slice callback for decoder
    vpx_codec_register_put_slice_cb(&decoder,  id_000046_put_slice_callback, nullptr);

    // Read and decode data
    uint8_t* buffer = new uint8_t[size];
    size_t bytes_read = fread(buffer, 1, size, in_file);
    
    if (bytes_read > 0) {
        // Decode the input data
        vpx_codec_decode(&decoder, buffer, bytes_read, nullptr, fuzz_int64_t_2);
        
        // Try to get a decoded frame
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* img = vpx_codec_get_frame(&decoder, &iter);
        
        if (img) {
            // Use vpx_img_set_rect on the decoded image
            vpx_img_set_rect(img, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint32_t_6);
            
            // Encoder setup (multi-encoder)
            vpx_codec_ctx_t encoder;
            vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
            vpx_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder config
            vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_7);
            
            // Configure encoder
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_threads = 2;
            
            // Setup downsampling factors for multi-encoder
            vpx_rational_t dsf[2] = {{1, 1}, {2, 2}};
            
            // Initialize multi-encoder
            res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg,
                                               2, fuzz_int64_t_8, dsf, VPX_ENCODER_ABI_VERSION);
            
            if (res == VPX_CODEC_OK) {
                // Use vpx_codec_control_ to set encoder properties
                vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
                vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);
                
                // Get global headers from encoder
                vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&encoder);
                if (headers && headers->buf && headers->sz > 0) {
                    fwrite(headers->buf, 1, headers->sz, out_file);
                }
                
                // Encode the image
                vpx_codec_encode(&encoder, img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
                
                // Get encoded data
                const vpx_codec_cx_pkt_t* pkt;
                vpx_codec_iter_t enc_iter = nullptr;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    delete[] buffer;
    
    // Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}