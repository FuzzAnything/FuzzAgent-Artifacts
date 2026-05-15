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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 7
//<Quality> {"density":24,"unique_branches":{"vpx_realloc_frame_buffer":[[162,7,162,33,0,0,4,0],[162,37,162,65,0,0,4,0]],"vp9_copy_and_extend_frame":[[134,27,134,61,0,0,4,0]],"encoder_encode":[[1459,7,1459,38,0,0,4,1],[1481,27,1481,70,0,0,4,0]],"alloc_util_frame_buffers":[[1305,7,1311,49,0,0,4,0]],"update_initial_width":[[2368,9,2368,39,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_img_flip","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_img_flip","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000037_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This callback is called when a frame is decoded
    // We can process the frame here if needed
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_rational_t dsf = {1, 1}; // Downsampling factor
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* img = nullptr;
    std::vector<uint8_t> buffer;
    
    // Get decoder interface for VP9
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get encoder interface for VP9
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Mark as used
    
    // Peek at stream info to get dimensions
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(
        decoder_iface, data, static_cast<unsigned int>(size), &stream_info);
    
    if (err != VPX_CODEC_OK) {
        // If we can't peek stream info, use default values
        stream_info.w = 640;
        stream_info.h = 480;
    }
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
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
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000037_put_frame_callback, nullptr);
    (void)err; // Continue even if callback registration fails
    
    // Allocate image for decoded frame
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                       stream_info.w, stream_info.h, 1);
    if (!img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set a rectangle in the image (top-left 100x100 region)
    vpx_img_set_rect(img, 0, 0, 100, 100);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), 
                          nullptr, 0);
    
    // Get decoded frames
    const vpx_image_t* decoded_frame;
    while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Copy decoded frame to our allocated image
        if (decoded_frame->fmt == VPX_IMG_FMT_I420) {
            for (int plane = 0; plane < 3; plane++) {
                int width = (plane == 0) ? decoded_frame->d_w : 
                           (decoded_frame->d_w + 1) >> 1;
                int height = (plane == 0) ? decoded_frame->d_h : 
                            (decoded_frame->d_h + 1) >> 1;
                
                for (int y = 0; y < height; y++) {
                    memcpy(img->planes[plane] + y * img->stride[plane],
                          decoded_frame->planes[plane] + y * decoded_frame->stride[plane],
                          width);
                }
            }
        }
    }
    
    // Initialize encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 500;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Initialize encoder (using multi version with single encoder)
    err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                       1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        // Try single encoder initialization
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                     0, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            vpx_img_free(img);
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }
    
    // Encode the frame
    err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Flip the image for next potential use
    vpx_img_flip(img);
    
    // Get encoded data
    iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write encoded data to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}