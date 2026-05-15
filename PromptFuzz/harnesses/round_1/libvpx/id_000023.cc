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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_img_wrap","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_codec_decode","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_img_wrap","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_codec_decode","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static void  id_000023_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Callback implementation
}

static void  id_000023_put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *recon) {
    // Callback implementation
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder;
    vpx_codec_stream_info_t stream_info;
    vpx_fixed_buf_t output_buf;
    vpx_image_t *preview_frame = nullptr;
    vpx_image_t wrapped_img;
    unsigned char *img_data = nullptr;
    
    // Initialize output buffer
    output_buf.buf = malloc(size * 2);  // Allocate extra space
    if (!output_buf.buf) {
	return 0;
	}
	
    output_buf.sz = size * 2;
    
    // Create input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(output_buf.buf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder context
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    if (vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        free(output_buf.buf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Register callbacks
    vpx_codec_register_put_frame_cb(&decoder,  id_000023_put_frame_callback, NULL);
    vpx_codec_register_put_slice_cb(&decoder,  id_000023_put_slice_callback, NULL);
    
    // Set output buffer for encoded data
    if (vpx_codec_set_cx_data_buf(&decoder, &output_buf, 16, 16) != VPX_CODEC_OK) {
        // Get error detail if failed
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use error detail
    }
    
    // Get stream information
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    if (vpx_codec_get_stream_info(&decoder, &stream_info) == VPX_CODEC_OK) {
        // Use stream info
        unsigned int width = stream_info.w;
        unsigned int height = stream_info.h;
        
        // Allocate image data
        size_t img_data_size = width * height * 3 / 2; // For I420 format
        img_data = (unsigned char *)malloc(img_data_size);
        if (img_data) {
            // Wrap image data
            vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                                width, height, 1, img_data);
            if (wrapped) {
                // Set rectangle on the image
                vpx_img_set_rect(&wrapped_img, 0, 0, width/2, height/2);
            }
        }
    }
    
    // Try to get preview frame (for encoder, but we try anyway)
    preview_frame = (vpx_image_t *)vpx_codec_get_preview_frame(&decoder);
    
    // Try to decode the input data
    size_t bytes_read = fread(output_buf.buf, 1, size, in_file);
    if (bytes_read > 0) {
        vpx_codec_decode(&decoder, (const uint8_t *)output_buf.buf, 
                         bytes_read, NULL, 0);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    if (img_data) free(img_data);
    free(output_buf.buf);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}