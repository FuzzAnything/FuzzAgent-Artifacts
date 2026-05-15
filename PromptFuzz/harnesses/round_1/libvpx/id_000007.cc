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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_err_to_string","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_error_detail","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_err_to_string"],"critical_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_err_to_string","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_err_to_string"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode video data and manipulate the decoded frames:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Step 1: Use version APIs
    int version = vpx_codec_version();
    const char* version_str = vpx_codec_version_str();
    const char* version_extra_str = vpx_codec_version_extra_str();
    
    // Step 2: Initialize decoder context for VP9
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    // Default decoder configuration
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    // Initialize decoder
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&codec_ctx, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Use error APIs
        const char* error_detail = vpx_codec_error_detail(&codec_ctx);
        const char* error_string = vpx_codec_err_to_string(init_result);
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Step 3: Try to get stream info from input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t stream_info_result = vpx_codec_peek_stream_info(
        decoder_iface, data, static_cast<unsigned int>(size), &stream_info);
    
    if (stream_info_result == VPX_CODEC_OK) {
        // Use error string API
        const char* error_string = vpx_codec_err_to_string(VPX_CODEC_OK);
    }
    
    // Step 4: Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &codec_ctx, data, static_cast<unsigned int>(size), nullptr, 0);
    
    if (decode_result == VPX_CODEC_OK) {
        // Step 5: Get decoded frame
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t* decoded_img = vpx_codec_get_frame(&codec_ctx, &iter);
        
        if (decoded_img && decoded_img->d_w > 0 && decoded_img->d_h > 0) {
            // Step 6: Create a new image using vpx_img_wrap
            // Calculate buffer size for I420 format (YUV 4:2:0)
            unsigned int width = decoded_img->d_w;
            unsigned int height = decoded_img->d_h;
            
            // Allocate buffer for image data
            unsigned int buffer_size = width * height * 3 / 2; // I420 format
            unsigned char* img_data = (unsigned char*)malloc(buffer_size);
            
            if (img_data) {
                // Wrap the buffer into a vpx_image_t structure
                vpx_image_t* wrapped_img = vpx_img_wrap(
                    nullptr,                     // Allocate new image struct
                    VPX_IMG_FMT_I420,           // I420 format
                    width,                      // Display width
                    height,                     // Display height
                    1,                          // Stride alignment
                    img_data                    // Image data buffer
                );
                
                if (wrapped_img) {
                    // Step 7: Set a rectangle on the wrapped image
                    // Use a safe rectangle within image bounds
                    unsigned int rect_x = 0;
                    unsigned int rect_y = 0;
                    unsigned int rect_w = (width > 10) ? width - 10 : width;
                    unsigned int rect_h = (height > 10) ? height - 10 : height;
                    
                    if (rect_w > 0 && rect_h > 0) {
                        int set_rect_result = vpx_img_set_rect(
                            wrapped_img, rect_x, rect_y, rect_w, rect_h);
                        
                        // Use error API for demonstration
                        const char* error_string = vpx_codec_err_to_string(
                            (set_rect_result == 0) ? VPX_CODEC_OK : VPX_CODEC_ERROR);
                    }
                    
                    // Clean up wrapped image (doesn't free img_data)
                    vpx_img_free(wrapped_img);
                }
                
                free(img_data);
            }
            
            // Step 8: Get stream info using vpx_codec_get_stream_info
            vpx_codec_stream_info_t stream_info2;
            memset(&stream_info2, 0, sizeof(stream_info2));
            stream_info2.sz = sizeof(stream_info2);
            
            vpx_codec_err_t get_stream_info_result = vpx_codec_get_stream_info(
                &codec_ctx, &stream_info2);
            
            if (get_stream_info_result != VPX_CODEC_OK) {
                // Use error detail API
                const char* error_detail = vpx_codec_error_detail(&codec_ctx);
            }
        }
    } else {
        // Use error APIs on decode failure
        const char* error_detail = vpx_codec_error_detail(&codec_ctx);
        const char* error_string = vpx_codec_err_to_string(decode_result);
    }
    
    // Step 9: Clean up decoder
    vpx_codec_err_t destroy_result = vpx_codec_destroy(&codec_ctx);
    
    // Final use of error string API
    const char* final_error_string = vpx_codec_err_to_string(destroy_result);
    
    return 0;
}