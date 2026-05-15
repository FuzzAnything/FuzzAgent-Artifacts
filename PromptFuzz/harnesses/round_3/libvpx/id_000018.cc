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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Step 1: Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Read the input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    std::vector<uint8_t> input_buffer(file_size);
    size_t bytes_read = fread(input_buffer.data(), 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != static_cast<size_t>(file_size)) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Try to peek stream info to determine codec
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first, then VP8
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(), 
        input_buffer.data(), 
        static_cast<unsigned int>(input_buffer.size()), 
        &stream_info
    );
    
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (peek_result == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp8_dx(), 
            input_buffer.data(), 
            static_cast<unsigned int>(input_buffer.size()), 
            &stream_info
        );
        if (peek_result == VPX_CODEC_OK) {
            decoder_iface = vpx_codec_vp8_dx();
            encoder_iface = vpx_codec_vp8_cx();
        }
    }
    
    // Get error string for peek result
    const char *peek_error_str = vpx_codec_err_to_string(peek_result);
    (void)peek_error_str; // Prevent unused variable warning
    
    if (!decoder_iface) {
        assert_file_closed(&in_file);
	return 0; // Couldn't identify codec
    }

    // Step 4: Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        nullptr, 
        fuzz_int64_t_1, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Decode the input
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, 
        input_buffer.data(), 
        static_cast<unsigned int>(input_buffer.size()), 
        nullptr, 
        fuzz_int64_t_2
    );
    
    // Get decoder error if any
    const char *decoder_error = vpx_codec_error(&decoder);
    (void)decoder_error; // Prevent unused variable warning

    // Step 6: Try to get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
        // Step 7: Initialize multi-encoder for re-encoding
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t encoder_config;
        
        // Get default encoder configuration
        if (vpx_codec_enc_config_default(encoder_iface, &encoder_config, fuzz_uint32_t_3) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder configuration based on decoded frame
        encoder_config.g_w = decoded_frame->d_w;
        encoder_config.g_h = decoded_frame->d_h;
        encoder_config.g_timebase.num = 1;
        encoder_config.g_timebase.den = 30;
        encoder_config.rc_target_bitrate = 1000;
        encoder_config.g_pass = VPX_RC_ONE_PASS;
        encoder_config.g_lag_in_frames = 0;
        
        // Initialize multi-encoder (2 encoders in this case)
        vpx_rational_t dsf[2] = {{1, 1}, {1, 1}};
        vpx_codec_err_t enc_init_result = vpx_codec_enc_init_multi_ver(
            &encoder,
            encoder_iface,
            &encoder_config,
            2, // num_enc
            fuzz_int64_t_4, // flags
            dsf,
            VPX_ENCODER_ABI_VERSION
        );
        
        if (enc_init_result == VPX_CODEC_OK) {
            // Step 8: Encode the decoded frame
            vpx_codec_err_t encode_result = vpx_codec_encode(
                &encoder,
                decoded_frame,
                fuzz_int64_t_5, // pts
                fuzz_uint64_t_6, // duration
                fuzz_int64_t_7, // flags
                fuzz_uint64_t_8  // deadline
            );
            
            // Get encoder error if any
            const char *encoder_error = vpx_codec_error(&encoder);
            (void)encoder_error; // Prevent unused variable warning
            
            // Step 9: Cleanup encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Step 10: Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}