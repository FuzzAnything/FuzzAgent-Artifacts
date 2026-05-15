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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode stream information from VP9 encoded data:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small to be meaningful
    }

    // Step 1: Get libvpx version information
    int version = vpx_codec_version();
    const char *version_str = vpx_codec_version_str();
    const char *version_extra_str = vpx_codec_version_extra_str();
    
    // These calls are for API usage only - we don't need the results for fuzzing
    (void)version;
    (void)version_str;
    (void)version_extra_str;

    // Step 2: Create decoder context for VP9
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t *decoder = vpx_codec_vp9_dx();
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    cfg.h = 0;
    cfg.w = 0;
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&codec_ctx, decoder, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Convert error to string
        const char *err_str = vpx_codec_err_to_string(init_result);
        (void)err_str;
        return 0;
    }

    // Step 3: Get stream information from the input data
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t stream_info_result = vpx_codec_get_stream_info(&codec_ctx, &stream_info);
    
    if (stream_info_result != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(stream_info_result);
        (void)err_str;
    }
    
    // Get error detail if available
    const char *error_detail = vpx_codec_error_detail(&codec_ctx);
    (void)error_detail;

    // Step 4: Create a dummy image using vpx_img_wrap
    // We'll use dimensions from stream info if available, otherwise default
    unsigned int width = (stream_info.w > 0) ? stream_info.w : 320;
    unsigned int height = (stream_info.h > 0) ? stream_info.h : 240;
    
    // Allocate image buffer for I420 format (YUV 4:2:0)
    // I420 requires width*height + (width/2)*(height/2)*2 bytes
    size_t img_size = width * height * 3 / 2;
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Initialize with some pattern
    memset(img_data, 0x80, img_size); // Gray background
    
    vpx_image_t image;
    vpx_image_t *img_result = vpx_img_wrap(&image, VPX_IMG_FMT_I420, width, height, 1, img_data);
    
    if (!img_result) {
        free(img_data);
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }

    // Step 5: Set a rectangle on the image
    // Use safe rectangle dimensions within image bounds
    unsigned int rect_x = (width > 10) ? 10 : 0;
    unsigned int rect_y = (height > 10) ? 10 : 0;
    unsigned int rect_w = (width > rect_x + 20) ? 20 : (width - rect_x);
    unsigned int rect_h = (height > rect_y + 20) ? 20 : (height - rect_y);
    
    if (rect_w > 0 && rect_h > 0) {
        int set_rect_result = vpx_img_set_rect(&image, rect_x, rect_y, rect_w, rect_h);
        (void)set_rect_result; // Result unused in fuzzing context
    }

    // Step 6: Try to decode the input data
    // Only decode if we have enough data
    if (size > 0) {
        vpx_codec_decode(&codec_ctx, data, size, NULL, 0);
        
        // Try to get a decoded frame
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *decoded_img = vpx_codec_get_frame(&codec_ctx, &iter);
        
        // If we got a decoded image, we could process it further
        (void)decoded_img;
    }

    // Step 7: Clean up
    // Note: vpx_img_wrap doesn't allocate img_data, so we must free it ourselves
    free(img_data);
    
    // Destroy codec context
    vpx_codec_destroy(&codec_ctx);

    return 0;
}