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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_destroy"],"visited":0}
/*Looking at the provided APIs and the requirement to process libvpx output data, I'll create a fuzzer that simulates a video decoding pipeline with frame processing. The event would be: **Decoding encoded video data and processing decoded frames with cropping and callbacks**.

Here's the complete fuzzer implementation:

*/


// Include libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000017_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Simple callback that does nothing for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_fixed_buf_t global_headers_buf = {nullptr, 0};
    vpx_fixed_buf_t cx_data_buf = {nullptr, 0};
    vpx_image_t *preview_frame = nullptr;
    vpx_image_t img;
    
    // Choose decoder based on input size (alternating between VP8 and VP9 for fuzzing)
    if (size % 2 == 0) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    // Initialize decoder configuration
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000017_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        // Continue anyway - some codecs might not support this
    }
    
    // Allocate buffer for compressed data
    cx_data_buf.buf = malloc(size);
    if (!cx_data_buf.buf) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    cx_data_buf.sz = size;
    
    // Read data into buffer
    fread(cx_data_buf.buf, 1, size, in_file);
    
    // Set compressed data buffer with padding
    err = vpx_codec_set_cx_data_buf(&decoder, &cx_data_buf, fuzz_uint32_t_2, fuzz_uint32_t_3);
    if (err != VPX_CODEC_OK) {
        // Continue anyway - this might fail for some codecs
    }
    
    // Try to get global headers (might fail for some codecs)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&decoder);
    if (global_headers) {
        // Write global headers to output file
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Decode the data
    err = vpx_codec_decode(&decoder, (const uint8_t*)cx_data_buf.buf, size, 
                          nullptr, fuzz_int64_t_4);
    
    // Try to get preview frame
    preview_frame = (vpx_image_t*)vpx_codec_get_preview_frame(&decoder);
    if (preview_frame) {
        // Create a copy of the image to apply rectangle operations
        memset(&img, 0, sizeof(img));
        img.fmt = preview_frame->fmt;
        img.w = preview_frame->w;
        img.h = preview_frame->h;
        img.d_w = preview_frame->d_w;
        img.d_h = preview_frame->d_h;
        
        // Allocate image data
        size_t img_size = img.w * img.h * 3; // Rough estimate
        unsigned char *img_data = (unsigned char*)malloc(img_size);
        if (img_data) {
            vpx_image_t *wrapped_img = vpx_img_wrap(&img, img.fmt, img.w, img.h, 
                                                   fuzz_uint32_t_5, img_data);
            if (wrapped_img) {
                // Set a rectangle on the image (crop operation)
                // Use fuzzed data to determine rectangle parameters
                unsigned int x = data[0] % (img.w > 1 ? img.w - 1 : 1);
                unsigned int y = data[1] % (img.h > 1 ? img.h - 1 : 1);
                unsigned int w = data[2] % (img.w - x);
                unsigned int h = data[3] % (img.h - y);
                
                if (w > 0 && h > 0) {
                    vpx_img_set_rect(wrapped_img, x, y, w, h);
                }
                
                // Free the wrapped image
                vpx_img_free(wrapped_img);
            }
            free(img_data);
        }
    }
    
    // Iterate through decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *frame;
    while ((frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Write frame dimensions to output file
        fwrite(&frame->w, sizeof(frame->w), 1, out_file);
        fwrite(&frame->h, sizeof(frame->h), 1, out_file);
        
        // Process frame with rectangle operations if we have enough data
        if (size >= 8) {
            vpx_image_t temp_img;
            memset(&temp_img, 0, sizeof(temp_img));
            temp_img.fmt = frame->fmt;
            temp_img.w = frame->w;
            temp_img.h = frame->h;
            
            // Use different parts of fuzzed data for rectangle parameters
            unsigned int rect_x = data[4] % (temp_img.w > 1 ? temp_img.w - 1 : 1);
            unsigned int rect_y = data[5] % (temp_img.h > 1 ? temp_img.h - 1 : 1);
            unsigned int rect_w = data[6] % (temp_img.w - rect_x);
            unsigned int rect_h = data[7] % (temp_img.h - rect_y);
            
            if (rect_w > 0 && rect_h > 0) {
                // Note: vpx_img_set_rect requires an image with allocated data
                // We're just testing the API call here
                int result = vpx_img_set_rect(&temp_img, rect_x, rect_y, 
                                             rect_w, rect_h);
                (void)result; // Result ignored for fuzzing
            }
        }
    }
    
    // Cleanup
    free(cx_data_buf.buf);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}