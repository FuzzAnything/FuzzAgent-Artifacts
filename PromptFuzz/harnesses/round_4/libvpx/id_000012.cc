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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_img_free","vpx_codec_control_","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_img_free","vpx_codec_control_","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for initialization
    
    // Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize encoder context
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Set up encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use fuzzing data to set some configuration parameters
    cfg.g_w = 64 + (data[0] % 192);  // Width between 64-256
    cfg.g_h = 64 + (data[1] % 192);  // Height between 64-256
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256 + (data[2] % 768);  // Bitrate between 256-1024
    
    // Create multiple encoder instances
    const int num_enc = 2;
    vpx_codec_ctx_t codec_ctx_multi;
    vpx_rational_t dsf[2] = {{1, 1}, {2, 2}};
    
    // Initialize multi-encoder
    if (vpx_codec_enc_init_multi_ver(&codec_ctx_multi, iface, &cfg, num_enc, 
                                     fuzz_int64_t_2, dsf, 
                                     VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate image for input
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec_ctx_multi);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with some data from input
    size_t bytes_to_read = img->stride[0] * cfg.g_h;
    if (bytes_to_read > 0) {
        size_t read_size = fread(img->planes[0], 1, 
                                bytes_to_read < size ? bytes_to_read : size, in_file);
        if (read_size > 0) {
            // Fill UV planes with pattern
            for (int i = 1; i < 3; i++) {
                if (img->stride[i] > 0) {
                    memset(img->planes[i], 128, img->stride[i] * (cfg.g_h >> 1));
                }
            }
        }
    }
    
    // Create buffer for encoded data
    vpx_fixed_buf_t output_buffer;
    output_buffer.sz = 1024 * 1024;  // 1MB buffer
    output_buffer.buf = malloc(output_buffer.sz);
    if (!output_buffer.buf) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec_ctx_multi);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set buffer for encoded data
    unsigned int pad_before = data[3] % 64;
    unsigned int pad_after = data[4] % 64;
    if (vpx_codec_set_cx_data_buf(&codec_ctx_multi, &output_buffer, 
                                  pad_before, pad_after) != VPX_CODEC_OK) {
        free(output_buffer.buf);
        vpx_img_free(img);
        vpx_codec_destroy(&codec_ctx_multi);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx_multi);
    if (global_headers) {
        // Write global headers to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // Try to encode the image
    if (vpx_codec_encode(&codec_ctx_multi, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7) == VPX_CODEC_OK) {
        // Try to get preview frame
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec_ctx_multi);
        if (preview_frame) {
            // Allocate new image for the preview frame
            vpx_image_t *preview_copy = vpx_img_alloc(NULL, preview_frame->fmt,
                                                     preview_frame->d_w, 
                                                     preview_frame->d_h, fuzz_uint32_t_8);
            if (preview_copy) {
                // Copy preview frame data
                for (int plane = 0; plane < 3; plane++) {
                    if (preview_frame->planes[plane] && preview_copy->planes[plane]) {
                        size_t plane_size = preview_frame->stride[plane] * 
                                          (preview_frame->d_h >> (plane > 0 ? 1 : 0));
                        memcpy(preview_copy->planes[plane], preview_frame->planes[plane],
                               plane_size);
                    }
                }
                vpx_img_free(preview_copy);
            }
        }
        
        // Try various control operations
        int ctrl_ids[] = {VP8E_SET_CPUUSED, VP8E_SET_NOISE_SENSITIVITY, 
                         VP8E_SET_SHARPNESS, VP8E_SET_STATIC_THRESHOLD};
        
        for (int i = 0; i < 4 && i < (int)(size - 5); i++) {
            int ctrl_value = data[5 + i] % 10;
            vpx_codec_control_(&codec_ctx_multi, ctrl_ids[i], ctrl_value);
        }
    }
    
    // Clean up
    free(output_buffer.buf);
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx_multi);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}