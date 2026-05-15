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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
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
	
    
    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t encoder_config = {0};
    vpx_image_t *image = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    std::vector<uint8_t> input_buffer;
    std::vector<uint8_t> encoded_buffer;
    
    // Determine codec type from input data
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Try VP9 first
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(), data, size, &stream_info);
    
    if (peek_result == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        // Try VP8
        stream_info.sz = sizeof(stream_info);
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp8_dx(), data, size, &stream_info);
        
        if (peek_result == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
            decoder_iface = vpx_codec_vp8_dx();
            encoder_iface = vpx_codec_vp8_cx();
        } else {
            // Cannot determine codec, clean up and return
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Initialize decoder
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, data, size, nullptr, 0);
    
    if (decode_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream info from decoder
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t stream_info_result = vpx_codec_get_stream_info(
        &decoder, &stream_info);
    
    if (stream_info_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    image = vpx_codec_get_frame(&decoder, &iter);
    
    if (!image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder with default config
    vpx_codec_err_t enc_config_result = vpx_codec_enc_config_default(
        encoder_iface, &encoder_config, 0);
    
    if (enc_config_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on decoded frame
    encoder_config.g_w = image->d_w;
    encoder_config.g_h = image->d_h;
    encoder_config.g_timebase.num = 1;
    encoder_config.g_timebase.den = 30;
    encoder_config.rc_target_bitrate = 1000;
    encoder_config.g_pass = VPX_RC_ONE_PASS;
    encoder_config.g_lag_in_frames = 0;
    
    // Initialize encoder
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
        &encoder, encoder_iface, &encoder_config, 0, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use vpx_codec_control_ to set encoder properties
    // Set deadline (quality) parameter
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    
    // Get global headers from encoder
    global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Encode the decoded frame
    vpx_codec_err_t encode_result = vpx_codec_encode(
        &encoder, image, 0, 1, 0, VPX_DL_REALTIME);
    
    if (encode_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}