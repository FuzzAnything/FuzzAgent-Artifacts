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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
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

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    
    // Choose codec based on input data (simple heuristic)
    if (size > 4 && data[0] == 0x9D && data[1] == 0x01 && data[2] == 0x2A) {
        // VP8 keyframe signature
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        // Default to VP9
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    }

    // Initialize decoder
    dec_cfg.threads = 1;
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        &dec_cfg, 
        fuzz_int64_t_1, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read input data into a buffer
    uint8_t *input_buffer = (uint8_t *)malloc(size);
    if (!input_buffer) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_buffer, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, 
        input_buffer, 
        size, 
        nullptr, 
        fuzz_int64_t_2
    );
    
    free(input_buffer);

    // Get default encoder configuration
    vpx_codec_err_t enc_config_default_result = vpx_codec_enc_config_default(
        encoder_iface, 
        &enc_cfg, 
        fuzz_uint32_t_3
    );
    
    if (enc_config_default_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration (modify some values for fuzzing)
    if (size > 8) {
        enc_cfg.g_w = (data[0] % 640) + 1;  // Random width 1-640
        enc_cfg.g_h = (data[1] % 480) + 1;  // Random height 1-480
        enc_cfg.rc_target_bitrate = (data[2] % 1000) + 100;  // Bitrate 100-1099
    } else {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        enc_cfg.rc_target_bitrate = 500;
    }
    
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;

    // Initialize encoder
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
        &encoder, 
        encoder_iface, 
        &enc_cfg, 
        fuzz_int64_t_4, 
        VPX_ENCODER_ABI_VERSION
    );
    
    if (enc_init_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration (call the required API)
    vpx_codec_err_t enc_config_set_result = vpx_codec_enc_config_set(
        &encoder, 
        &enc_cfg
    );
    
    if (enc_config_set_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        vpx_codec_destroy(&encoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get global headers from encoder
    global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // Try to encode a dummy frame if we have valid dimensions
    if (enc_cfg.g_w > 0 && enc_cfg.g_h > 0) {
        // Create a dummy image for encoding
        vpx_image_t *img = vpx_img_alloc(
            nullptr, 
            VPX_IMG_FMT_I420, 
            enc_cfg.g_w, 
            enc_cfg.g_h, 
            fuzz_uint32_t_5
        );
        
        if (img) {
            // Fill with some data
            if (size > 16) {
                for (unsigned int i = 0; i < img->d_w * img->d_h; i++) {
                    if (i < size - 16) {
                        img->planes[0][i] = data[i % (size - 16)];
                    }
                }
            }
            
            // Encode the dummy frame
            vpx_codec_encode(
                &encoder, 
                img, 
                fuzz_int64_t_6, 
                fuzz_uint64_t_7, 
                fuzz_int64_t_8, 
                fuzz_uint64_t_9
            );
            
            vpx_img_free(img);
        }
    }

    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}