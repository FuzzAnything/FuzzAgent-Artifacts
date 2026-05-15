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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_error","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_fixed_buf_t* global_headers = nullptr;
    vpx_image_t* image = nullptr;
    
    // Determine codec interface based on first byte
    vpx_codec_iface_t* decoder_iface = nullptr;
    vpx_codec_iface_t* encoder_iface = nullptr;
    
    if (data[0] & 0x01) {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    }

    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Use vpx_codec_error
        const char* error_str = vpx_codec_error(&decoder);
        (void)error_str; // Prevent unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&decoder);
        (void)error_str;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get stream information
    stream_info.sz = sizeof(stream_info);
    err = vpx_codec_get_stream_info(&decoder, &stream_info);
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&decoder);
        (void)error_str;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set encoder configuration based on stream info
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;

    // Initialize encoder
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&encoder);
        (void)error_str;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get global headers
    global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Use the global headers data
        (void)global_headers->buf;
        (void)global_headers->sz;
    }

    // Create and free an image to use vpx_img_free
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 16, 16, fuzz_uint32_t_5);
    if (image) {
        // Use vpx_img_free
        vpx_img_free(image);
        image = nullptr;
    }

    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}