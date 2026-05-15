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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Use version APIs
    int version = vpx_codec_version();
    const char *version_str = vpx_codec_version_str();
    const char *version_extra_str = vpx_codec_version_extra_str();
    
    (void)version;  // Mark as used to avoid warnings
    (void)version_str;
    (void)version_extra_str;

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();  // Use VP9 decoder
    
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    // Use error string API
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str;
    
    if (err != VPX_CODEC_OK) {
        // Get error detail
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
        
        assert_file_closed(&in_file);;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Peek stream info first
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Try to peek stream info from input data
    vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    
    // Decode the frame
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, NULL, 0);
    
    if (err != VPX_CODEC_OK) {
        err_str = vpx_codec_err_to_string(err);
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
    } else {
        // Get stream info after decoding
        err = vpx_codec_get_stream_info(&decoder, &si);
        if (err != VPX_CODEC_OK) {
            err_str = vpx_codec_err_to_string(err);
        }
    }

    // Try to get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Allocate buffer for image data
        unsigned int align = 32;
        unsigned char *img_data = (unsigned char*)malloc(img->d_w * img->d_h * 3 / 2);
        if (img_data) {
            // Create image using vpx_img_wrap
            vpx_image_t wrapped_img;
            vpx_image_t *result = vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                               img->d_w, img->d_h, align, img_data);
            
            if (result) {
                // Use vpx_img_set_rect with safe values
                unsigned int x = 0;
                unsigned int y = 0;
                unsigned int w = img->d_w > 10 ? 10 : img->d_w;
                unsigned int h = img->d_h > 10 ? 10 : img->d_h;
                
                int rect_result = vpx_img_set_rect(result, x, y, w, h);
                (void)rect_result;  // Mark as used
            }
            
            free(img_data);
        }
    }

    // Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}