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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for configuration
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize variables
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t *headers = nullptr;
    const vpx_image_t *preview_frame = nullptr;
    
    // Get codec interface (VP8 encoder)
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get interface name (using required API)
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use variable to avoid unused warning
    
    // Set up default encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some values from input data to modify configuration
    fseek(in_file, 0, SEEK_SET);
    int width = 0, height = 0;
    if (size >= 8) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        if (width < 16) width = 16;
        if (height < 16) height = 16;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    } else {
        width = 320;
        height = 240;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 256; // kbps
    
    // Initialize encoder (using required API)
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate image (using required API)
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image planes with data from input
    size_t bytes_consumed = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = (plane == 0) ? width : width / 2;
        int plane_height = (plane == 0) ? height : height / 2;
        
        for (int y = 0; y < plane_height; y++) {
            size_t bytes_to_read = plane_width;
            if (bytes_consumed + bytes_to_read > size) {
                bytes_to_read = size - bytes_consumed;
            }
            if (bytes_to_read > 0) {
                fread(img->planes[plane] + y * img->stride[plane], 1, bytes_to_read, in_file);
                bytes_consumed += bytes_to_read;
            }
            // If we run out of data, fill remaining with pattern
            for (size_t x = bytes_to_read; x < (size_t)plane_width; x++) {
                img->planes[plane][y * img->stride[plane] + x] = (plane * 64) + (y + x) % 192;
            }
        }
    }
    
    // Encode the image
    if (vpx_codec_encode(&codec, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7) == VPX_CODEC_OK) {
        // Get global headers (using required API)
        headers = vpx_codec_get_global_headers(&codec);
        
        // Try to get preview frame (using required API)
        preview_frame = vpx_codec_get_preview_frame(&codec);
        (void)preview_frame; // Use variable to avoid unused warning
    }
    
    // Clean up
    if (headers) {
        // Note: vpx_codec_get_global_headers returns pointer to internal buffer
        // Don't free it directly
    }
    
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}