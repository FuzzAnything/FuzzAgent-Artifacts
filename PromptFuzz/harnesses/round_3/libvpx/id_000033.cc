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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get stream information
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    res = vpx_codec_peek_stream_info(decoder_iface, data, std::min(size, (size_t)1000), &si);
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_2);
    
    // Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    
    if (preview_frame) {
        // Create a copy of the preview frame data
        size_t img_size = preview_frame->w * preview_frame->h * 3 / 2; // YUV420 size
        std::vector<unsigned char> img_data(img_size);
        
        // Wrap the image data
        vpx_image_t wrapped_img;
        vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                           preview_frame->w, preview_frame->h, 
                                           fuzz_uint32_t_3, img_data.data());
        
        if (wrapped) {
            // Set a rectangle on the image (using safe coordinates)
            unsigned int x = std::min(10u, preview_frame->w - 1);
            unsigned int y = std::min(10u, preview_frame->h - 1);
            unsigned int w = std::min(100u, preview_frame->w - x);
            unsigned int h = std::min(100u, preview_frame->h - y);
            
            vpx_img_set_rect(wrapped, x, y, w, h);
            
            // Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
            
            vpx_codec_enc_cfg_t enc_cfg;
            vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4);
            enc_cfg.g_w = wrapped->w;
            enc_cfg.g_h = wrapped->h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_5, VPX_ENCODER_ABI_VERSION);
            
            if (res == VPX_CODEC_OK) {
                // Prepare output buffer
                vpx_fixed_buf_t output_buf;
                output_buf.buf = malloc(4096);
                output_buf.sz = 4096;
                
                if (output_buf.buf) {
                    // Set output buffer for encoder
                    vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_6, fuzz_uint32_t_7);
                    
                    // Encode the image
                    vpx_codec_encode(&encoder, wrapped, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                    
                    // Get encoded data
                    vpx_codec_iter_t iter = NULL;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    free(output_buf.buf);
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}