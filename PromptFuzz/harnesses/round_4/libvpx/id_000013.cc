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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 171, nr_unique_branch: 19
//<Quality> {"density":19,"unique_branches":{"vp8_get_preview_raw_frame":[[5302,7,5302,40,0,0,4,1]],"vp8e_get_preview":[[1206,7,1206,40,0,0,4,1],[1212,7,1212,60,0,0,4,0]],"vp8_pack_tokens":[[167,28,167,48,0,0,4,0]],"vpx_codec_enc_init_multi_ver":[[95,9,95,42,0,0,4,1],[97,9,97,58,0,0,4,0],[98,19,98,30,0,0,4,0],[98,19,98,30,0,0,4,1],[102,13,102,25,0,0,4,1],[102,29,102,44,0,0,4,1],[102,48,102,60,0,0,4,1],[103,13,103,32,0,0,4,1],[122,13,122,16,0,0,4,1]],"vpx_codec_get_preview_frame":[[320,14,320,57,0,0,4,1],[322,14,322,42,0,0,4,1]],"update_extracfg":[[531,7,531,26,0,0,4,0]],"vp8e_init":[[730,9,730,15,0,0,4,0]],"set_cpu_used":[[24,23,24,34,2,0,4,1],[25,23,25,34,3,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder context
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Default configuration for VP8 encoder
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some basic configuration
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    // Initialize multi-encoder (using 1 encoder instance)
    vpx_rational_t dsf = {1, 1};
    if (vpx_codec_enc_init_multi_ver(&codec_ctx, iface, &cfg, 1, fuzz_int64_t_2, &dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate image buffer for raw frames
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Prepare a buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.sz = 1024 * 1024; // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    if (!output_buf.buf) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the output buffer for compressed data
    if (vpx_codec_set_cx_data_buf(&codec_ctx, &output_buf, fuzz_uint32_t_4, fuzz_uint32_t_5) != VPX_CODEC_OK) {
        free(output_buf.buf);
        vpx_img_free(img);
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        // Write global headers to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // Read input data and use it as raw frame data
    size_t bytes_read = fread(img->planes[0], 1, 
                             (cfg.g_w * cfg.g_h * 3) / 2 > size ? size : (cfg.g_w * cfg.g_h * 3) / 2,
                             in_file);
    
    // If we have enough data, encode a frame
    if (bytes_read >= (cfg.g_w * cfg.g_h * 3) / 2) {
        // Set some control parameters
        vpx_codec_control_(&codec_ctx, VP8E_SET_CPUUSED, 8);
        vpx_codec_control_(&codec_ctx, VP8E_SET_NOISE_SENSITIVITY, 0);
        
        // Encode the frame
        if (vpx_codec_encode(&codec_ctx, img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9) == VPX_CODEC_OK) {
            // Try to get preview frame
            const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
            if (preview_frame) {
                // Allocate new image to potentially process preview frame
                vpx_image_t *processed_img = vpx_img_alloc(NULL, preview_frame->fmt, 
                                                          preview_frame->d_w, preview_frame->d_h, fuzz_uint32_t_10);
                if (processed_img) {
                    // In a real scenario, we would process the preview frame here
                    vpx_img_free(processed_img);
                }
            }
        }
    }
    
    // Cleanup
    free(output_buf.buf);
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}