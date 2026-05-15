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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_control_","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_str","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_control_","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_img_wrap","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode VP9 video streams from fuzzer input:

*/


extern "C" {
}

// Define control IDs needed for vpx_codec_control_
#define VP9D_GET_FRAME_CORRUPTED 10
#define VP8_COPY_REFERENCE 1

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        // Too small to be valid VP9 data
        return 0;
    }

    // 1. Get codec version information
    int version = vpx_codec_version();
    const char* version_str = vpx_codec_version_str();
    const char* version_extra = vpx_codec_version_extra_str();
    
    (void)version; (void)version_str; (void)version_extra; // Prevent unused warnings

    // 2. Get VP9 decoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp9_dx();
    if (!iface) {
        return 0;
    }

    // 3. Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Prevent unused warning

    // 4. Peek at stream info before initialization
    vpx_codec_stream_info_t si_peek;
    memset(&si_peek, 0, sizeof(si_peek));
    si_peek.sz = sizeof(si_peek);
    
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(iface, data, 
                                                          size < 10000 ? (unsigned int)size : 10000, 
                                                          &si_peek);
    
    // Convert error code to string
    const char* err_str = vpx_codec_err_to_string(peek_err);
    (void)err_str; // Prevent unused warning

    // 5. Initialize decoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 2;
    
    if (si_peek.w > 0 && si_peek.h > 0) {
        cfg.w = si_peek.w;
        cfg.h = si_peek.h;
    } else {
        cfg.w = 640;
        cfg.h = 480;
    }

    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&codec, iface, &cfg, 0, 
                                                      VPX_DECODER_ABI_VERSION);
    
    if (init_err != VPX_CODEC_OK) {
        // Try with default config
        init_err = vpx_codec_dec_init_ver(&codec, iface, NULL, 0, 
                                          VPX_DECODER_ABI_VERSION);
        if (init_err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // 6. Get stream info after initialization
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t info_err = vpx_codec_get_stream_info(&codec, &si);
    (void)info_err; // Prevent unused warning

    // 7. Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(&codec, data, (unsigned int)size, 
                                                  NULL, 0);
    
    // Get error detail if decode failed
    if (decode_err != VPX_CODEC_OK) {
        const char* detail = vpx_codec_error_detail(&codec);
        (void)detail; // Prevent unused warning
    }

    // 8. Try to use control API
    int frame_corrupted = 0;
    vpx_codec_err_t ctrl_err = vpx_codec_control_(&codec, VP9D_GET_FRAME_CORRUPTED, 
                                                  &frame_corrupted);
    (void)ctrl_err; // Prevent unused warning

    // 9. Get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* img;
    
    while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
        // Create a dummy image using vpx_img_wrap for demonstration
        if (img->w > 0 && img->h > 0 && img->fmt < 11) {
            // Allocate buffer for dummy image
            size_t buffer_size = img->w * img->h * 3;
            if (buffer_size > 0 && buffer_size < 1000000) { // Sanity check
                unsigned char* dummy_data = (unsigned char*)malloc(buffer_size);
                if (dummy_data) {
                    // Fill with some pattern
                    memset(dummy_data, 0x80, buffer_size);
                    
                    vpx_image_t dummy_img;
                    memset(&dummy_img, 0, sizeof(dummy_img));
                    
                    // Wrap the buffer as an image
                    vpx_image_t* wrapped = vpx_img_wrap(&dummy_img, 
                                                        (vpx_img_fmt_t)img->fmt,
                                                        img->w, img->h, 
                                                        1, dummy_data);
                    
                    if (wrapped) {
                        // Try to set a rectangle on the image
                        unsigned int rect_w = img->w > 10 ? 10 : img->w;
                        unsigned int rect_h = img->h > 10 ? 10 : img->h;
                        
                        int set_rect_result = vpx_img_set_rect(wrapped, 0, 0, 
                                                               rect_w, rect_h);
                        (void)set_rect_result; // Prevent unused warning
                        
                        // Clean up the dummy image
                        vpx_img_free(wrapped);
                    }
                    
                    free(dummy_data);
                }
            }
        }
    }

    // 10. Create another dummy image for additional API coverage
    if (size > 100) {
        unsigned char* img_data = (unsigned char*)malloc(640 * 480 * 3);
        if (img_data) {
            // Use part of fuzzer data as image data
            size_t copy_size = size < 640 * 480 * 3 ? size : 640 * 480 * 3;
            memcpy(img_data, data, copy_size);
            
            vpx_image_t test_img;
            memset(&test_img, 0, sizeof(test_img));
            
            vpx_image_t* wrapped_img = vpx_img_wrap(&test_img, VPX_IMG_FMT_I420,
                                                    640, 480, 1, img_data);
            
            if (wrapped_img) {
                // Try setting various rectangles
                for (int i = 0; i < 3 && i * 100 < (int)size; i++) {
                    unsigned int x = data[i] % 600;
                    unsigned int y = data[i + 1] % 440;
                    unsigned int w = 10 + (data[i + 2] % 50);
                    unsigned int h = 10 + (data[i + 3] % 50);
                    
                    if (x + w <= 640 && y + h <= 480) {
                        vpx_img_set_rect(wrapped_img, x, y, w, h);
                    }
                }
                
                vpx_img_free(wrapped_img);
            }
            
            free(img_data);
        }
    }

    // 11. Clean up
    vpx_codec_err_t destroy_err = vpx_codec_destroy(&codec);
    (void)destroy_err; // Prevent unused warning

    return 0;
}