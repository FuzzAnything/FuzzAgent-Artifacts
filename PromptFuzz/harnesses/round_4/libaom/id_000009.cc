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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 35
//<Quality> {"density":8,"unique_branches":{"av1_calculate_ref_frame_side":[[1006,9,1006,20,0,0,4,0],[1008,9,1008,75,0,0,4,0],[1010,14,1010,42,0,0,4,1]],"read_uncompressed_header":[[4652,11,4652,41,0,0,4,0],[4660,11,4660,35,0,0,4,1],[4664,11,4664,35,0,0,4,1],[4745,9,4745,50,0,0,4,0],[4750,11,4750,37,0,0,4,1],[4752,11,4752,29,0,0,4,1],[4757,11,4757,29,0,0,4,1],[4773,23,4773,37,0,0,4,0],[4773,23,4773,37,0,0,4,1],[4774,13,4774,55,0,0,4,1],[4779,15,4779,57,0,0,4,1],[4841,11,4841,53,0,0,4,0],[4845,11,4845,27,0,0,4,1],[4865,13,4865,24,0,0,4,1],[4865,28,4865,57,0,0,4,0],[4865,28,4865,57,0,0,4,1],[4866,15,4866,26,0,0,4,0],[4893,15,4899,21,0,0,4,1],[4944,11,4944,47,0,0,4,0],[4944,51,4944,75,0,0,4,0],[4944,51,4944,75,0,0,4,1],[5169,44,5169,75,0,0,4,1]],"av1_dc_quant_QTX":[[203,5,203,21,0,0,4,0]],"decrease_ref_count":[[469,7,469,18,0,0,4,0],[478,9,478,28,0,0,4,0],[478,9,478,28,0,0,4,1],[478,32,478,58,0,0,4,0]],"av1_setup_frame_buf_refs":[[853,9,853,20,0,0,4,0]],"release_pending_output_frames":[[575,24,575,50,0,0,4,0]],"av1_setup_frame_sign_bias":[[865,62,865,73,0,0,4,0],[868,11,869,70,0,0,4,0],[868,11,869,70,0,0,4,1]]},"library_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
	//fuzzer shim end}




    // Get library version
    int version = aom_codec_version();
    (void)version; // Use version to avoid unused variable warning
    
    // Early return if input is too small
    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from input data
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
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;  // Default width
    dec_cfg.h = 128;  // Default height
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, 
                                                 AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read input data in chunks and decode
    std::vector<uint8_t> buffer(4096);
    size_t bytes_read;
    aom_codec_iter_t iter = nullptr;
    
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), in_file)) > 0) {
        // Decode the chunk
        res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        if (res != AOM_CODEC_OK) {
            // Continue anyway for fuzzing
        }
        
        // Get decoded frames
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create encoder configuration
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2);
            if (res != AOM_CODEC_OK) continue;
            
            // Set encoder configuration based on decoded image
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_threads = 1;
            enc_cfg.g_profile = 0;
            enc_cfg.g_bit_depth = static_cast<aom_bit_depth_t>(img->bit_depth);
            enc_cfg.g_input_bit_depth = img->bit_depth;
            enc_cfg.rc_end_usage = AOM_Q;
            enc_cfg.rc_min_quantizer = 0;
            enc_cfg.rc_max_quantizer = 63;
            enc_cfg.g_error_resilient = 0;
            enc_cfg.g_pass = AOM_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            // Initialize encoder
            res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_3, 
                                        AOM_ENCODER_ABI_VERSION);
            if (res != AOM_CODEC_OK) continue;
            
            // Apply encoder configuration
            res = aom_codec_enc_config_set(&encoder, &enc_cfg);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&encoder);
                continue;
            }
            
            // Prepare output buffer for encoded data
            aom_fixed_buf_t output_buf;
            output_buf.buf = malloc(4096);
            output_buf.sz = 4096;
            
            if (output_buf.buf) {
                // Set output buffer for encoder
                res = aom_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_4, fuzz_uint32_t_5);
                if (res != AOM_CODEC_OK) {
                    free(output_buf.buf);
                    aom_codec_destroy(&encoder);
                    continue;
                }
                
                // Create a wrapped image from decoded frame data
                aom_image_t wrapped_img;
                unsigned int stride_align = 1;
                
                // Wrap the image data (using first plane as example)
                aom_image_t *wrapped = aom_img_wrap(&wrapped_img, img->fmt, 
                                                   img->d_w, img->d_h, 
                                                   fuzz_uint32_t_6, 
                                                   img->planes[0]);
                
                if (wrapped) {
                    // Copy metadata from original image
                    wrapped->bit_depth = img->bit_depth;
                    wrapped->monochrome = img->monochrome;
                    
                    // Encode the wrapped image
                    res = aom_codec_encode(&encoder, wrapped, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                }
                
                free(output_buf.buf);
            }
            
            // Destroy encoder
            aom_codec_destroy(&encoder);
        }
        iter = nullptr;  // Reset iterator for next chunk
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}