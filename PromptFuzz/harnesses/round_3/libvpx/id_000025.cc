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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000025_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is called when a frame is decoded
    // In fuzzing context, we don't need to do anything substantial
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing decoded frames
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();  // Use VP8 decoder
    
    // Get decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;  // Mark as used

    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;  // Single thread for fuzzing

    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000025_put_frame_callback, nullptr);
    (void)err;  // Mark as used

    // Get stream information
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (err == VPX_CODEC_OK) {
        // Try to get more detailed stream info after decode
        vpx_codec_get_stream_info(&decoder, &stream_info);
    }

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    
    // Try to use control API (example control)
    vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_UPDATES, nullptr);

    vpx_codec_iter_t iter = nullptr;
    
    // Get compressed data packets (if any)
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&decoder, &iter)) != nullptr) {
        // Process different packet types
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write frame data to output file
            if (out_file && pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }

    // Get decoded frames
    iter = nullptr;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Write image data to output file
        if (out_file && img->planes[0]) {
            // Write Y plane
            for (unsigned int y = 0; y < img->d_h; ++y) {
                fwrite(img->planes[0] + y * img->stride[0], 1, img->d_w, out_file);
            }
        }
    }

    // Try to get preview frame (more relevant for encoder, but we try anyway)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    if (preview_frame && out_file && preview_frame->planes[0]) {
        // Write preview frame data
        for (unsigned int y = 0; y < preview_frame->d_h; ++y) {
            fwrite(preview_frame->planes[0] + y * preview_frame->stride[0], 
                   1, preview_frame->d_w, out_file);
        }
    }

    // Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}