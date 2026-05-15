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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 6
//<Quality> {"density":13,"unique_branches":{"vp9_left_block_mode":[[16,21,16,44,0,0,4,0]],"decode_tiles_mt":[[2509,12,2509,23,0,0,4,0],[24,23,24,34,1,0,4,0]],"vp9_release_frame_buffer":[[80,7,80,13,0,0,4,1]],"tile_worker_hook":[[2254,12,2254,36,0,0,4,0],[2254,40,2254,65,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_peek_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_peek_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
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
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use fd_in to avoid unused variable warning

    // Step 1: First try to decode the input as VP9 stream
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check decoder capabilities
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(decoder_iface);
    (void)dec_caps; // Use dec_caps
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    vpx_codec_err_t ret = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (ret != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use error_detail
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Read and decode frames
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        vpx_codec_err_t decode_ret = vpx_codec_decode(&decoder, buffer, bytes_read, NULL, 0);
        if (decode_ret != VPX_CODEC_OK) {
            // Continue anyway
        }
    }

    // Get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_frame = NULL;
    vpx_image_t *allocated_image = NULL;
    
    while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Use the decoded frame to create a new image
        allocated_image = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                       decoded_frame->d_w, decoded_frame->d_h, 16);
        if (allocated_image) {
            // Copy some data (simplified)
            if (decoded_frame->planes[0] && allocated_image->planes[0]) {
                size_t copy_size = decoded_frame->d_w * decoded_frame->d_h;
                if (copy_size > 0 && copy_size <= allocated_image->d_w * allocated_image->d_h) {
                    memcpy(allocated_image->planes[0], decoded_frame->planes[0], 
                           copy_size);
                }
            }
            vpx_img_free(allocated_image);
            allocated_image = NULL;
        }
    }

    // Try to get preview frame (though typically for encoder)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    (void)preview_frame;

    // Step 2: Now try to encode something
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default encoder config
    vpx_codec_enc_cfg_t enc_cfg;
    ret = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Configure encoder
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 1;

    // Initialize encoder
    ret = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (ret != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Also try multi-encoder init (though we won't use it extensively)
    vpx_codec_ctx_t multi_encoder;
    vpx_rational_t dsf = {1, 1};
    vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    vpx_codec_destroy(&multi_encoder);

    // Create a test image for encoding
    vpx_image_t test_image;
    vpx_img_alloc(&test_image, VPX_IMG_FMT_I420, 640, 480, 16);
    
    // Fill with some data from input
    size_t fill_size = test_image.d_w * test_image.d_h;
    if (fill_size > 0 && size > 0) {
        size_t to_copy = fill_size < size ? fill_size : size;
        if (test_image.planes[0]) {
            memcpy(test_image.planes[0], data, to_copy);
        }
    }

    // Encode the frame
    ret = vpx_codec_encode(&encoder, &test_image, 0, 1, 0, VPX_DL_REALTIME);
    if (ret != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
    }

    // Get encoded data
    vpx_codec_iter_t enc_iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do something with the packet
            (void)pkt->data.frame.sz;
        }
    }

    // Try peek stream info on original data
    vpx_codec_stream_info_t si = {0};
    si.sz = sizeof(si);
    vpx_codec_peek_stream_info(decoder_iface, data, 
                              size < 1000 ? size : 1000, &si);

    // Cleanup
    vpx_img_free(&test_image);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}