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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 115.5, nr_unique_branch: 80
//<Quality> {"density":11,"unique_branches":{"macro_block_yrd":[[484,27,484,49,0,0,4,0],[484,27,484,49,0,0,4,1],[494,15,494,21,0,0,4,0],[494,15,494,21,0,0,4,1]],"pick_quickcompress_mode":[[818,7,818,15,0,0,4,1],[841,7,841,34,0,0,4,1],[843,14,843,50,0,0,4,1],[845,14,845,49,0,0,4,1],[850,7,850,31,0,0,4,1]],"vpx_codec_register_put_frame_cb":[[142,12,142,57,0,0,4,1]],"vp8cx_encode_intra_macroblock":[[1098,7,1098,17,0,0,4,0],[1098,21,1098,47,0,0,4,0]],"cost_coeffs":[[428,10,428,17,0,0,4,0],[428,10,428,17,0,0,4,1],[436,7,436,13,0,0,4,0],[436,7,436,13,0,0,4,1]],"rd_pick_intra4x4mby_modes":[[610,15,610,21,0,0,4,0],[610,15,610,21,0,0,4,1],[616,9,616,42,0,0,4,0],[634,9,634,37,0,0,4,1],[637,7,637,35,0,0,4,1]],"rd_pick_intra_mbuv_mode":[[753,24,753,39,0,0,4,0],[753,24,753,39,0,0,4,1],[781,9,781,26,0,0,4,0],[781,9,781,26,0,0,4,1]],"optimize_b":[[188,7,188,65,0,0,4,0],[202,17,202,25,0,0,4,0],[202,17,202,25,0,0,4,1],[210,9,210,10,0,0,4,0],[210,9,210,10,0,0,4,1],[219,11,219,20,0,0,4,0],[227,11,227,31,0,0,4,0],[236,40,236,44,0,0,4,1],[237,34,237,38,0,0,4,1],[246,11,246,58,0,0,4,1],[253,11,253,19,0,0,4,1],[259,11,259,13,0,0,4,1],[270,11,270,20,0,0,4,0],[272,13,272,32,0,0,4,0],[276,13,276,32,0,0,4,0],[284,11,284,31,0,0,4,0],[292,11,292,19,0,0,4,1],[296,40,296,44,0,0,4,1],[297,34,297,38,0,0,4,1],[299,28,299,32,0,0,4,1],[313,11,313,30,0,0,4,0],[317,11,317,30,0,0,4,0],[338,7,338,27,0,0,4,0],[344,18,344,25,0,0,4,0],[344,18,344,25,0,0,4,1],[346,9,346,10,0,0,4,0]],"vp8_optimize_mbuv":[[469,7,469,30,0,0,4,1],[471,7,471,29,0,0,4,1],[479,16,479,22,0,0,4,0],[479,16,479,22,0,0,4,1]],"vp8_set_speed_features":[[1062,7,1062,52,0,0,4,1]],"vp8_rdcost_mby":[[460,15,460,21,0,0,4,0],[460,15,460,21,0,0,4,1]],"vp8_encode_frame":[[684,7,684,33,0,0,4,1]],"vp8_regular_quantize_b_sse4_1":[[111,9,111,14,0,0,4,0],[111,9,111,14,0,0,4,1]],"vp8_encode_intra16x16mbuv":[[115,7,115,18,0,0,4,0]],"vp8_rd_pick_intra_mode":[[2397,7,2397,28,0,0,4,0]],"vp8_get_compressed_data":[[5030,7,5030,33,0,0,4,1],[5103,7,5103,33,0,0,4,1]],"rd_cost_mbuv":[[698,16,698,22,0,0,4,0],[698,16,698,22,0,0,4,1]],"recode_loop_test":[[2830,7,2830,33,0,0,4,0],[2835,10,2835,50,0,0,4,1],[2836,10,2836,49,0,0,4,0],[2836,53,2836,63,0,0,4,1],[2840,14,2840,62,0,0,4,1]],"rd_pick_intra4x4block":[[547,26,547,43,0,0,4,0],[547,26,547,43,0,0,4,1],[568,9,568,26,0,0,4,0],[568,9,568,26,0,0,4,1]],"rd_pick_intra16x16mby_mode":[[657,24,657,39,0,0,4,0],[657,24,657,39,0,0,4,1],[670,9,670,26,0,0,4,0],[670,9,670,26,0,0,4,1]]},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000021_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Empty callback - just to satisfy API requirement
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read input data into a buffer
    uint8_t* encoded_data = (uint8_t*)malloc(size);
    if (!encoded_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    size_t bytes_read = fread(encoded_data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read == 0) {
        free(encoded_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Try VP8 first, then VP9 if VP8 fails
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, vpx_codec_vp8_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        dec_init_result = vpx_codec_dec_init_ver(
            &decoder, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
    }

    if (dec_init_result == VPX_CODEC_OK) {
        // Get stream information
        vpx_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        
        vpx_codec_err_t stream_info_result = vpx_codec_get_stream_info(
            &decoder, &stream_info);
        
        // Decode the frame
        vpx_codec_decode(&decoder, encoded_data, bytes_read, NULL, fuzz_int64_t_3);
        
        // Get decoded frame
        vpx_codec_iter_t iter = NULL;
        vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
        
        if (decoded_frame) {
            // Create output image for encoding
            vpx_image_t output_image;
            
            // Allocate image data buffer
            unsigned int width = decoded_frame->d_w;
            unsigned int height = decoded_frame->d_h;
            
            // Calculate required buffer size
            size_t img_data_size = width * height * 3 / 2; // For I420 format
            
            if (img_data_size > 0 && img_data_size < 100 * 1024 * 1024) { // Sanity check
                unsigned char* img_buffer = (unsigned char*)malloc(img_data_size);
                if (img_buffer) {
                    // Wrap the image buffer
                    vpx_img_wrap(&output_image, VPX_IMG_FMT_I420, 
                                width, height, 1, img_buffer);
                    
                    // Initialize encoder context for multi-encoder scenario
                    vpx_codec_ctx_t encoder;
                    vpx_codec_enc_cfg_t enc_cfg;
                    
                    // Try to initialize encoder with default config
                    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
                    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4) == VPX_CODEC_OK) {
                        enc_cfg.g_w = width;
                        enc_cfg.g_h = height;
                        enc_cfg.g_timebase.num = 1;
                        enc_cfg.g_timebase.den = 30;
                        
                        // Initialize multi-encoder (even though we only use 1)
                        vpx_rational_t dsf = {1, 1};
                        vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                                    1, fuzz_int64_t_5, &dsf, VPX_ENCODER_ABI_VERSION);
                        
                        // Register put_frame callback
                        vpx_codec_register_put_frame_cb(&encoder,  id_000021_put_frame_callback, NULL);
                        
                        // Try to encode a frame (use dummy data)
                        if (width > 0 && height > 0) {
                            // Fill image with some data
                            unsigned char* y_plane = output_image.planes[0];
                            unsigned char* u_plane = output_image.planes[1];
                            unsigned char* v_plane = output_image.planes[2];
                            
                            int y_stride = output_image.stride[0];
                            int uv_stride = output_image.stride[1];
                            
                            // Simple pattern fill
                            for (unsigned int y = 0; y < height; y++) {
                                for (unsigned int x = 0; x < width; x++) {
                                    y_plane[y * y_stride + x] = (x + y) % 256;
                                }
                            }
                            
                            for (unsigned int y = 0; y < height / 2; y++) {
                                for (unsigned int x = 0; x < width / 2; x++) {
                                    u_plane[y * uv_stride + x] = x % 256;
                                    v_plane[y * uv_stride + x] = y % 256;
                                }
                            }
                            
                            // Encode the frame
                            vpx_codec_encode(&encoder, &output_image, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
                        }
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder);
                    }
                    
                    free(img_buffer);
                }
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    free(encoded_data);
    assert_file_closed(&in_file);
	return 0;
}