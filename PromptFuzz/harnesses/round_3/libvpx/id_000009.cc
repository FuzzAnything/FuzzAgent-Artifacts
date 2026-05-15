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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_global_headers","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback functions
static int  id_000009_dummy_get_frame_buffer(void* cb_priv, size_t min_size, 
                                  vpx_codec_frame_buffer_t* fb) {
    static uint8_t buffer[4096];
    fb->data = buffer;
    fb->size = sizeof(buffer);
    return 0;
}

static int  id_000009_dummy_release_frame_buffer(void* cb_priv, 
                                      vpx_codec_frame_buffer_t* fb) {
    return 0;
}

static void  id_000009_dummy_put_slice_cb(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* valid,
                               const vpx_image_rect_t* update) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
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
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t* global_headers = nullptr;
    
    // Get codec interface (VP8 encoder)
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    
    // Get codec interface name (API 1)
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)iface_name;  // Prevent unused variable warning
    
    // Set default encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder with some parameters from input data
    cfg.g_w = (data[0] % 128 + 64) * 16;  // Width: 1024-3072
    cfg.g_h = (data[1] % 128 + 64) * 16;  // Height: 1024-3072
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (data[2] % 4000);  // 1000-5000 kbps
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = data[3] % 16;
    
    // Initialize encoder (API 3)
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put slice callback (API 6)
    vpx_codec_register_put_slice_cb(&codec,  id_000009_dummy_put_slice_cb, nullptr);
    
    // Set frame buffer functions (API 2)
    vpx_codec_set_frame_buffer_functions(&codec,  id_000009_dummy_get_frame_buffer, 
                                          id_000009_dummy_release_frame_buffer, nullptr);
    
    // Allocate image (API 5)
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image planes with data from input
    size_t bytes_read = 0;
    for (int plane = 0; plane < 3; ++plane) {
        size_t plane_size = img->stride[plane] * (plane == 0 ? cfg.g_h : cfg.g_h / 2);
        size_t to_read = std::min(plane_size, size - bytes_read);
        
        if (to_read > 0) {
            fread(img->planes[plane], 1, to_read, in_file);
            bytes_read += to_read;
        }
        // Fill remaining with pattern if needed
        for (size_t i = to_read; i < plane_size; ++i) {
            img->planes[plane][i] = (plane * 64 + i) % 256;
        }
    }
    
    // Encode frame (API 4)
    vpx_codec_encode(&codec, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // Get global headers (API 7)
    global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}