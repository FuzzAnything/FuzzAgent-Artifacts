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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_get_global_headers","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_get_global_headers","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libvpx APIs to decode an input video stream and process it:

*/


extern "C" {
}

// Frame buffer callback functions
static int  id_000011_get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000011_release_frame_buffer_cb(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    if (fb->data) free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Create file-like interface for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    
    // Initialize decoder context
    vpx_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Determine codec interface
    vpx_codec_iface_t *codec_iface = nullptr;
    
    // Try VP9 first
    codec_iface = vpx_codec_vp9_dx();
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        codec_iface, data, size, &stream_info);
    
    // If VP9 fails, try VP8
    if (peek_result != VPX_CODEC_OK) {
        codec_iface = vpx_codec_vp8_dx();
        peek_result = vpx_codec_peek_stream_info(
            codec_iface, data, size, &stream_info);
    }
    
    if (peek_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(codec_iface);
    
    // Initialize decoder
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &codec_ctx, codec_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Get error message
        const char *error_msg = vpx_codec_error(&codec_ctx);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions
    void *cb_priv = nullptr;
    vpx_codec_err_t set_fb_result = vpx_codec_set_frame_buffer_functions(
        &codec_ctx,  id_000011_get_frame_buffer_cb,  id_000011_release_frame_buffer_cb, cb_priv);
    
    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &codec_ctx, data, size, nullptr, 0);
    
    if (decode_result == VPX_CODEC_OK) {
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *img;
        
        while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
            // Create a copy of the image to manipulate
            vpx_image_t *img_copy = vpx_img_alloc(nullptr, img->fmt, 
                                                  img->d_w, img->d_h, 1);
            if (img_copy) {
                // Copy image data
                for (int plane = 0; plane < 3; ++plane) {
                    if (img->planes[plane]) {
                        size_t plane_size = img->stride[plane] * 
                            ((plane == 0) ? img->d_h : (img->d_h + 1) >> 1);
                        memcpy(img_copy->planes[plane], img->planes[plane], 
                               plane_size);
                    }
                }
                
                // Use vpx_img_set_rect - set a rectangle in the image
                unsigned int rect_x = size % (img->d_w > 10 ? img->d_w - 10 : 1);
                unsigned int rect_y = (size / 256) % (img->d_h > 10 ? img->d_h - 10 : 1);
                unsigned int rect_w = 10;
                unsigned int rect_h = 10;
                
                vpx_img_set_rect(img_copy, rect_x, rect_y, rect_w, rect_h);
                
                // Write some data to output file
                if (out_file) {
                    fwrite(&img_copy->d_w, sizeof(img_copy->d_w), 1, out_file);
                    fwrite(&img_copy->d_h, sizeof(img_copy->d_h), 1, out_file);
                }
                
                vpx_img_free(img_copy);
            }
        }
    }
    
    // Get global headers (though this is more for encoding)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx);
    if (global_headers && global_headers->buf && out_file) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Clean up
    vpx_codec_destroy(&codec_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove output file
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}