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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"estimate_keyframe_frequency":[[1323,31,1323,64,0,0,4,0]],"init_config":[[1335,7,1335,27,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_img_alloc","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // 1. First, consume input data as encoded stream header information
    // We'll parse the first bytes to get configuration parameters
    if (size < 8) {
	return 0;
	}
	  // Need minimum data for configuration
    
    // Open input as file stream to simulate reading encoded data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get codec interface name
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    const char *iface_name = vpx_codec_iface_name(codec_iface);
    // Use iface_name to avoid unused variable warning
    (void)iface_name;
    
    // 3. Allocate image buffer for input frames
    // Use some data to determine image dimensions
    unsigned int width = (data[0] % 64 + 1) * 16;  // 16-1024 pixels
    unsigned int height = (data[1] % 64 + 1) * 16; // 16-1024 pixels
    
    vpx_image_t *input_image = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                           width, height, fuzz_uint32_t_1);
    if (!input_image) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Fill image planes with input data
    // Calculate required bytes for I420 format
    size_t y_plane_size = width * height;
    size_t uv_plane_size = y_plane_size / 4;
    size_t total_image_size = y_plane_size + 2 * uv_plane_size;
    
    size_t data_offset = 2;  // Skip width/height bytes
    size_t copy_size = (size - data_offset < total_image_size) ? 
                      size - data_offset : total_image_size;
    
    if (copy_size > 0) {
        // Fill Y plane
        size_t y_copy = (copy_size > y_plane_size) ? y_plane_size : copy_size;
        memcpy(input_image->planes[VPX_PLANE_Y], data + data_offset, y_copy);
        
        // Fill U plane
        if (copy_size > y_plane_size) {
            size_t u_copy = (copy_size - y_plane_size > uv_plane_size) ? 
                           uv_plane_size : copy_size - y_plane_size;
            memcpy(input_image->planes[VPX_PLANE_U], 
                   data + data_offset + y_plane_size, u_copy);
        }
        
        // Fill V plane
        if (copy_size > y_plane_size + uv_plane_size) {
            size_t v_copy = (copy_size - y_plane_size - uv_plane_size > uv_plane_size) ?
                           uv_plane_size : copy_size - y_plane_size - uv_plane_size;
            memcpy(input_image->planes[VPX_PLANE_V],
                   data + data_offset + y_plane_size + uv_plane_size, v_copy);
        }
    }
    
    // 5. Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(codec_iface, &cfg, fuzz_uint32_t_2);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(input_image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set configuration from input data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = (data[2] % 1000 + 100);  // 100-1100 kbps
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[3] % 30 + 1) * 1000;  // 1000-31000 fps
    
    // 6. Create encoder context
    vpx_codec_ctx_t codec;
    int flags = 0;
    int ver = VPX_ENCODER_ABI_VERSION;
    
    res = vpx_codec_enc_init_ver(&codec, codec_iface, &cfg, fuzz_int64_t_3, ver);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(input_image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get global headers (codec configuration)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        // Write global headers to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // 8. Encode a frame
    long deadline = VPX_DL_REALTIME;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t enc_flags = 0;
    
    res = vpx_codec_encode(&codec, input_image, fuzz_int64_t_4, fuzz_uint64_t_5, 
                          fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // 9. Try to get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec);
    if (preview_frame) {
        // Write preview frame to file if available
        FILE *preview_file = fopen("preview_frame", "wb");
        if (preview_file) {
            // Write basic preview info
            fwrite(&preview_frame->w, sizeof(preview_frame->w), 1, preview_file);
            fwrite(&preview_frame->h, sizeof(preview_frame->h), 1, preview_file);
            fwrite(&preview_frame->fmt, sizeof(preview_frame->fmt), 1, preview_file);
            assert_file_closed(&preview_file);;
        }
    }
    
    // 10. Clean up
    vpx_codec_destroy(&codec);
    vpx_img_free(input_image);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}