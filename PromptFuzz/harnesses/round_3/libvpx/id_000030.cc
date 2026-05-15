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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"vp8_pack_tokens":[[217,32,217,52,0,0,4,0]]},"library_calls":["vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy frame buffer callbacks
static int  id_000030_dummy_get_frame_buffer(void *user_priv, size_t min_size, 
                                  vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)min_size;
    (void)fb;
    return -1; // Always fail to force internal allocation
}

static int  id_000030_dummy_release_frame_buffer(void *user_priv, 
                                      vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<100) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get build config
    const char *build_config = vpx_codec_build_config();
    (void)build_config; // Use to avoid unused warning
    
    // Initialize VP8 encoder interface
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    const char *vp8_name = vpx_codec_iface_name(vp8_iface);
    (void)vp8_name;
    
    // Initialize VP9 encoder interface
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_cx();
    const char *vp9_name = vpx_codec_iface_name(vp9_iface);
    (void)vp9_name;
    
    // Create encoder configurations
    vpx_codec_enc_cfg_t cfg_vp8, cfg_vp9;
    vpx_codec_ctx_t codec_vp8, codec_vp9_multi;
    
    // Initialize VP8 encoder with default config
    vpx_codec_enc_config_default(vp8_iface, &cfg_vp8, fuzz_uint32_t_1);
    cfg_vp8.g_w = 64;
    cfg_vp8.g_h = 64;
    cfg_vp8.g_timebase.num = 1;
    cfg_vp8.g_timebase.den = 30;
    cfg_vp8.rc_target_bitrate = 256;
    cfg_vp8.g_threads = 1;
    
    vpx_codec_err_t res_vp8 = vpx_codec_enc_init_ver(&codec_vp8, vp8_iface, 
                                                      &cfg_vp8, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    
    // Initialize VP9 encoder with multi-encoder config
    vpx_codec_enc_config_default(vp9_iface, &cfg_vp9, fuzz_uint32_t_3);
    cfg_vp9.g_w = 64;
    cfg_vp9.g_h = 64;
    cfg_vp9.g_timebase.num = 1;
    cfg_vp9.g_timebase.den = 30;
    cfg_vp9.rc_target_bitrate = 256;
    cfg_vp9.g_threads = 1;
    cfg_vp9.ss_number_layers = 2;
    
    // Set up downsampling factors for multi-encoder
    vpx_rational_t dsf[2];
    dsf[0].num = 1;
    dsf[0].den = 1;
    dsf[1].num = 2;
    dsf[1].den = 1;
    
    vpx_codec_err_t res_vp9 = vpx_codec_enc_init_multi_ver(&codec_vp9_multi, vp9_iface,
                                                          &cfg_vp9, 2, fuzz_int64_t_4, dsf, VPX_ENCODER_ABI_VERSION);
    
    // Set frame buffer functions for VP9 encoder
    if (res_vp9 == VPX_CODEC_OK) {
        vpx_codec_set_frame_buffer_functions(&codec_vp9_multi, 
                                            id_000030_dummy_get_frame_buffer,
                                            id_000030_dummy_release_frame_buffer,
                                           nullptr);
    }
    
    // Get global headers from both encoders
    if (res_vp8 == VPX_CODEC_OK) {
        vpx_fixed_buf_t *headers_vp8 = vpx_codec_get_global_headers(&codec_vp8);
        if (headers_vp8 && headers_vp8->buf && headers_vp8->sz > 0) {
            fwrite(headers_vp8->buf, 1, headers_vp8->sz, out_file);
        }
    }
    
    if (res_vp9 == VPX_CODEC_OK) {
        vpx_fixed_buf_t *headers_vp9 = vpx_codec_get_global_headers(&codec_vp9_multi);
        if (headers_vp9 && headers_vp9->buf && headers_vp9->sz > 0) {
            fwrite(headers_vp9->buf, 1, headers_vp9->sz, out_file);
        }
    }
    
    // Create image from fuzzer data
    vpx_image_t img;
    unsigned int img_width = 64;
    unsigned int img_height = 64;
    
    // Calculate required image buffer size for I420 format
    size_t y_plane = img_width * img_height;
    size_t uv_plane = (img_width * img_height) / 4;
    size_t total_size = y_plane + 2 * uv_plane;
    
    // Use fuzzer data as image buffer if we have enough
    if (size >= total_size) {
        // Wrap the fuzzer data as an image
        unsigned char *img_data = (unsigned char *)malloc(total_size);
        if (img_data) {
            // Copy fuzzer data to image buffer
            size_t copy_size = (size < total_size) ? size : total_size;
            memcpy(img_data, data, copy_size);
            
            vpx_image_t *wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420,
                                                   fuzz_uint32_t_5, fuzz_uint32_t_6,
                                                   fuzz_uint32_t_7, img_data);
            (void)wrapped_img; // Use to avoid unused warning
            
            // Try to encode with VP8 if initialized successfully
            if (res_vp8 == VPX_CODEC_OK) {
                vpx_codec_encode(&codec_vp8, &img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
            }
            
            // Try to encode with VP9 multi-encoder if initialized successfully
            if (res_vp9 == VPX_CODEC_OK) {
                vpx_codec_encode(&codec_vp9_multi, &img, fuzz_int64_t_12, fuzz_uint64_t_13, fuzz_int64_t_14, fuzz_uint64_t_15);
            }
            
            free(img_data);
        }
    }
    
    // Cleanup
    if (res_vp8 == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_vp8);
    }
    
    if (res_vp9 == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_vp9_multi);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}