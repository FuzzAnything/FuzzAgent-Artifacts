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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for any meaningful processing
    if (size < 10) {
        return 0;
    }

    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading (though not strictly needed here)
    int fd_in = fuzz_fileno(in_file);

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;

    // Initialize with VP9 decoder interface
    vpx_codec_err_t err = vpx_codec_dec_init_ver(
        &decoder, 
        vpx_codec_vp9_dx(), 
        &dec_cfg, 
        0, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (err != VPX_CODEC_OK) {
        // Try VP8 decoder if VP9 fails
        err = vpx_codec_dec_init_ver(
            &decoder, 
            vpx_codec_vp8_dx(), 
            &dec_cfg, 
            0, 
            VPX_DECODER_ABI_VERSION
        );
    }

    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Read and decode the compressed data
    uint8_t *compressed_data = (uint8_t*)malloc(size);
    if (!compressed_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    size_t bytes_read = fread(compressed_data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read == 0) {
        free(compressed_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Decode the frame
    err = vpx_codec_decode(&decoder, compressed_data, (unsigned int)bytes_read, NULL, 0);
    
    // Check for error details
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail; // Use error_detail to satisfy API usage requirement
    
    // Get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t si_err = vpx_codec_get_stream_info(&decoder, &stream_info);
    (void)si_err; // Use si_err to satisfy API usage requirement

    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();

    // Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        free(compressed_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Modify configuration based on stream info (if available)
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        // Use small default values
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.rc_end_usage = VPX_VBR;

    // Initialize encoder
    err = vpx_codec_enc_init_ver(
        &encoder, 
        encoder_iface, 
        &enc_cfg, 
        0, 
        VPX_ENCODER_ABI_VERSION
    );
    
    if (err == VPX_CODEC_OK) {
        // Try to set the encoder configuration
        vpx_codec_err_t set_err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
        (void)set_err; // Use set_err to satisfy API usage requirement
    }

    // Try to get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // We got a frame, but we don't need to do anything with it for fuzzing
        // Just using the API as required
        (void)img;
    }

    // Clean up
    if (vpx_codec_destroy(&decoder) != VPX_CODEC_OK) {
        // Error during decoder destruction
    }
    
    if (err == VPX_CODEC_OK) {
        if (vpx_codec_destroy(&encoder) != VPX_CODEC_OK) {
            // Error during encoder destruction
        }
    }

    free(compressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}