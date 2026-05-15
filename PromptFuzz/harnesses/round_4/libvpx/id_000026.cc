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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

// Callback function for put_frame
static void  id_000026_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback receives decoded frames
    // For fuzzing, we'll just count frames
    static int frame_count = 0;
    frame_count++;
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
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

    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Use if needed

    // Initialize variables
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = NULL;
    vpx_codec_err_t res;
    
    // Get VP9 decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize decoder
    res = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Use error reporting APIs
        const char *error_str = vpx_codec_err_to_string(res);
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_str;
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Register put_frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000026_put_frame_callback, NULL);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_error(&decoder);
        (void)error_str;
    }

    // Decode the input data
    res = vpx_codec_decode(&decoder, data, (unsigned int)size, NULL, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        (void)error_str;
    }

    // Get decoded frames
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Frame received - could process it here
        (void)img;
    }

    // Initialize encoder with multi-encoder setup
    // First get default configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx(); // Using VP9 encoder
    if (!encoder_iface) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    res = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set some basic parameters
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    // Initialize multi-encoder (using 1 encoder instance)
    vpx_rational_t dsf = {1, 1}; // Downsampling factor
    res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, fuzz_int64_t_4, &dsf, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_str;
        (void)error_detail;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get global headers from encoder
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Global headers available - could use them
        (void)global_headers->buf;
        (void)global_headers->sz;
    }

    // Create an output file for writing (as per requirement)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor for writing
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use if needed
        
        // Write something minimal
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
    }

    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}