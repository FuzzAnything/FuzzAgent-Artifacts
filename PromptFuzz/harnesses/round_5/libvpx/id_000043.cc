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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"vp8_post_proc_frame":[[155,7,155,26,0,0,4,0]],"vp8e_get_preview":[[1212,7,1212,60,0,0,4,1]]},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;  // Default width
    dec_cfg.h = 240;  // Default height
    
    // Try both VP8 and VP9 decoders
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    }
    
    if (res == VPX_CODEC_OK) {
        // Decode the input data
        vpx_codec_decode(&decoder, data, size, nullptr, 0);
        
        // Try to get stream information
        vpx_codec_stream_info_t si = {0};
        vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, &si);
        
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *img = nullptr;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create output image using vpx_img_wrap
            vpx_image_t out_img;
            unsigned int stride_align = 1;
            unsigned char *img_data = (unsigned char *)malloc(img->d_w * img->d_h * 3 / 2);
            if (img_data) {
                vpx_img_wrap(&out_img, VPX_IMG_FMT_I420, img->d_w, img->d_h, stride_align, img_data);
                
                // Copy data from decoded image
                for (int plane = 0; plane < 3; plane++) {
                    unsigned char *src = img->planes[plane];
                    unsigned char *dst = out_img.planes[plane];
                    int src_stride = img->stride[plane];
                    int dst_stride = out_img.stride[plane];
                    int height = (plane == 0) ? img->d_h : img->d_h / 2;
                    
                    for (int y = 0; y < height; y++) {
                        memcpy(dst, src, img->d_w / (plane == 0 ? 1 : 2));
                        src += src_stride;
                        dst += dst_stride;
                    }
                }
                
                // Use vpx_img_set_rect to set a rectangle region
                unsigned int x = img->d_w / 4;
                unsigned int y = img->d_h / 4;
                unsigned int w = img->d_w / 2;
                unsigned int h = img->d_h / 2;
                vpx_img_set_rect(&out_img, x, y, w, h);
                
                free(img_data);
            }
        }
        
        // Initialize encoder context
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &enc_cfg, 0) == VPX_CODEC_OK) {
            enc_cfg.g_w = 320;
            enc_cfg.g_h = 240;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            if (vpx_codec_enc_init_ver(&encoder, vpx_codec_vp8_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                // Use vpx_codec_control_ for encoder settings
                int ctrl_id = VP8E_SET_CPUUSED;
                int value = 8;
                vpx_codec_control_(&encoder, ctrl_id, value);
                
                // Create output buffer for encoded data
                vpx_fixed_buf_t output_buf;
                output_buf.buf = malloc(size * 2);  // Allocate buffer for encoded data
                output_buf.sz = size * 2;
                
                if (output_buf.buf) {
                    // Set output buffer for encoder
                    unsigned int pad_before = 0;
                    unsigned int pad_after = 0;
                    vpx_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);
                    
                    // Get global headers
                    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
                    
                    // Try to get preview frame
                    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
                    
                    free(output_buf.buf);
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
}