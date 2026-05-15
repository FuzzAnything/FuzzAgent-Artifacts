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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_fixed_buf_t output_buf = {0};
    vpx_image_t* wrapped_img = nullptr;
    unsigned char* img_data = nullptr;
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
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
    
    // Get decoder capabilities
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream information
    stream_info.sz = sizeof(stream_info);
    err = vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Get preview frame
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&decoder);
    
    if (preview_frame) {
        // Allocate image data for wrapped image
        size_t img_size = preview_frame->w * preview_frame->h * 3 / 2;
        img_data = (unsigned char*)malloc(img_size);
        if (!img_data) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Wrap the image data
        wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, 
                                   preview_frame->w, preview_frame->h, 
                                   fuzz_uint32_t_3, img_data);
        
        if (wrapped_img) {
            // Set a rectangle on the image
            unsigned int rect_w = preview_frame->w > 10 ? preview_frame->w / 2 : preview_frame->w;
            unsigned int rect_h = preview_frame->h > 10 ? preview_frame->h / 2 : preview_frame->h;
            vpx_img_set_rect(wrapped_img, fuzz_uint32_t_4, fuzz_uint32_t_5, rect_w, rect_h);
            
            // Initialize encoder
            vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
            vpx_codec_enc_cfg_t enc_cfg;
            
            err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_6);
            if (err == VPX_CODEC_OK) {
                enc_cfg.g_w = rect_w;
                enc_cfg.g_h = rect_h;
                
                err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_7, VPX_ENCODER_ABI_VERSION);
                if (err == VPX_CODEC_OK) {
                    // Allocate output buffer
                    output_buf.sz = size * 2;
                    output_buf.buf = malloc(output_buf.sz);
                    
                    if (output_buf.buf) {
                        // Set output buffer for encoded data
                        vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_8, fuzz_uint32_t_9);
                        
                        // Encode the wrapped image
                        vpx_codec_encode(&encoder, wrapped_img, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
                        
                        // Write output to file
                        if (output_buf.sz > 0 && output_buf.buf) {
                            fwrite(output_buf.buf, 1, output_buf.sz, out_file);
                        }
                        
                        free(output_buf.buf);
                    }
                    vpx_codec_destroy(&encoder);
                }
            }
            
            vpx_img_free(wrapped_img);
        }
        free(img_data);
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}