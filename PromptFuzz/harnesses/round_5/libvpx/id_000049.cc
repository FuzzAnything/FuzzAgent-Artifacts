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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 1
//<Quality> {"density":35,"unique_branches":{"vpx_codec_peek_stream_info":[[71,38,71,41,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*To create a fuzz driver that uses the specified libvpx APIs with a byte stream of libvpx output data, I'll create a program that:

1. Attempts to decode the input data (which should be encoded VP8/VP9 video)
2. Uses the decoded frame data to create images with vpx_img_alloc and vpx_img_wrap
3. Sets rectangles on the wrapped image
4. Initializes multiple encoders to re-encode the images
5. Retrieves global headers from the encoder

Here's the complete implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need at least some data
    
    // First, try to decode the input data (which should be VP8/VP9 encoded video)
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = nullptr;
    
    // Try to determine which codec to use based on the data
    // For simplicity, we'll try VP9 first, then VP8
    if (vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, nullptr) == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
    } else if (vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, nullptr) == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        return 0;  // Not valid VP8/VP9 data
    }
    
    // Initialize decoder
    if (vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Decode the frame
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get the decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // API 1: Allocate a new image using vpx_img_alloc
    vpx_image_t *allocated_img = vpx_img_alloc(nullptr, decoded_img->fmt, 
                                               decoded_img->d_w, decoded_img->d_h, 16);
    if (!allocated_img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy data from decoded image to allocated image
    for (int plane = 0; plane < 3; plane++) {
        if (decoded_img->planes[plane]) {
            size_t plane_size = decoded_img->stride[plane] * 
                               (plane == 0 ? decoded_img->d_h : decoded_img->d_h / 2);
            memcpy(allocated_img->planes[plane], decoded_img->planes[plane], plane_size);
        }
    }
    
    // API 2: Wrap existing data using vpx_img_wrap
    // We'll use the data from the allocated image for wrapping
    vpx_image_t wrapped_img;
    vpx_image_t *wrapped_ptr = vpx_img_wrap(&wrapped_img, decoded_img->fmt,
                                           decoded_img->d_w, decoded_img->d_h, 
                                           16, allocated_img->img_data);
    if (!wrapped_ptr) {
        vpx_img_free(allocated_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // API 3: Set a rectangle on the wrapped image
    // Use a small rectangle to avoid out-of-bounds issues
    unsigned int rect_x = decoded_img->d_w > 10 ? 5 : 0;
    unsigned int rect_y = decoded_img->d_h > 10 ? 5 : 0;
    unsigned int rect_w = decoded_img->d_w > 10 ? (decoded_img->d_w - 10) : decoded_img->d_w;
    unsigned int rect_h = decoded_img->d_h > 10 ? (decoded_img->d_h - 10) : decoded_img->d_h;
    
    if (vpx_img_set_rect(&wrapped_img, rect_x, rect_y, rect_w, rect_h) != 0) {
        // Continue anyway, this might fail for some formats
    }
    
    // API 4: Initialize multiple encoders using vpx_codec_enc_init_multi_ver
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(allocated_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set configuration based on decoded image
    cfg.g_w = decoded_img->d_w;
    cfg.g_h = decoded_img->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    
    // Create downsampling factors for multiple encoders
    vpx_rational_t dsf[2];
    dsf[0].num = 1;
    dsf[0].den = 1;
    dsf[1].num = 2;
    dsf[1].den = 1;
    
    // Initialize with 2 encoders
    if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 2, 0, dsf, 
                                     VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(allocated_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Encode the allocated image
    if (vpx_codec_encode(&encoder, allocated_img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(allocated_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // API 5: Get global headers from the encoder
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    
    // Write output to a file (as per instruction 4)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
        assert_file_closed(&out_file);;
    } else if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Also encode the wrapped image with rectangle applied
    vpx_codec_ctx_t encoder2;
    if (vpx_codec_enc_init_ver(&encoder2, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
        vpx_codec_encode(&encoder2, &wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
        vpx_codec_destroy(&encoder2);
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_img_free(allocated_img);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&out_file);
	return 0;
}