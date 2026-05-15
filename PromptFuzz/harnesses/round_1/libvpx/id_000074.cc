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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_wrap","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for meaningful fuzzing

    // Create input file from fuzzing data
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

    // Get encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get interface name
    const char* iface_name = vpx_codec_iface_name(iface);

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set reasonable dimensions based on input size
    unsigned int width = 64;
    unsigned int height = 64;
    cfg.g_w = width;
    cfg.g_h = height;

    // Initialize encoder context
    vpx_codec_ctx_t ctx;
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration (even though it's same)
    res = vpx_codec_enc_config_set(&ctx, &cfg);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Try various control operations
    vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, 8);
    vpx_codec_control_(&ctx, VP8E_SET_NOISE_SENSITIVITY, 0);
    vpx_codec_control_(&ctx, VP8E_SET_STATIC_THRESHOLD, 0);

    // Get global headers
    vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&ctx);
    if (headers && headers->buf && headers->sz > 0) {
        fwrite(headers->buf, 1, headers->sz, out_file);
    }

    // Allocate image using vpx_img_alloc
    vpx_image_t img_alloc;
    vpx_image_t* img1 = vpx_img_alloc(&img_alloc, VPX_IMG_FMT_I420, width, height, 1);
    if (!img1) {
        vpx_codec_destroy(&ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Wrap existing data using vpx_img_wrap
    std::vector<unsigned char> img_data(width * height * 3 / 2);  // I420 format
    vpx_image_t img_wrap;
    size_t bytes_read = fread(img_data.data(), 1, std::min(img_data.size(), size), in_file);
    
    vpx_image_t* img2 = vpx_img_wrap(&img_wrap, VPX_IMG_FMT_I420, width, height, 1, img_data.data());
    
    // Register a put frame callback (dummy)
    auto dummy_callback = [](void* user_priv, const vpx_image_t* img) {};
    vpx_codec_register_put_frame_cb(&ctx, dummy_callback, nullptr);

    // Encode using allocated image
    res = vpx_codec_encode(&ctx, img1, 0, 1, 0, VPX_DL_REALTIME);
    if (res != VPX_CODEC_OK) {
        const char* error_detail = vpx_codec_error_detail(&ctx);
        vpx_img_free(img1);
        vpx_codec_destroy(&ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Encode using wrapped image
    if (img2 && bytes_read > 0) {
        vpx_codec_encode(&ctx, img2, 1, 1, 0, VPX_DL_REALTIME);
    }

    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Check for errors
    const char* error_detail = vpx_codec_error_detail(&ctx);

    // Cleanup
    vpx_img_free(img1);
    // Note: img2 doesn't need freeing as it wraps existing data
    
    vpx_codec_destroy(&ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}