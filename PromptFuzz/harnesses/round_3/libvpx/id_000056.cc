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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 294.5, nr_unique_branch: 158
//<Quality> {"density":19,"unique_branches":{"pick_quickcompress_mode":[[818,7,818,15,0,0,4,1],[841,7,841,34,0,0,4,1],[843,14,843,50,0,0,4,1],[845,14,845,49,0,0,4,1]],"vp8_rdcost_mby":[[460,15,460,21,0,0,4,0],[460,15,460,21,0,0,4,1]],"vp8_encode_frame":[[684,7,684,33,0,0,4,1]],"check_reset_2nd_coeffs":[[364,7,364,27,0,0,4,1],[366,15,366,29,0,0,4,0],[366,15,366,29,0,0,4,1],[368,12,368,23,0,0,4,1],[369,9,369,18,0,0,4,0],[379,7,379,15,0,0,4,0],[380,17,380,31,0,0,4,1]],"vp8cx_encode_intra_macroblock":[[1098,7,1098,17,0,0,4,0],[1098,21,1098,47,0,0,4,0]],"vp8_get_compressed_data":[[5030,7,5030,33,0,0,4,1],[5103,7,5103,33,0,0,4,1]],"vp8_set_speed_features":[[1062,7,1062,52,0,0,4,1]],"cost_coeffs":[[428,10,428,17,0,0,4,0],[428,10,428,17,0,0,4,1],[436,7,436,13,0,0,4,0],[436,7,436,13,0,0,4,1]],"recode_loop_test":[[2830,7,2830,33,0,0,4,0],[2835,10,2835,50,0,0,4,1],[2836,10,2836,49,0,0,4,0],[2836,53,2836,63,0,0,4,0],[2836,53,2836,63,0,0,4,1],[2840,14,2840,62,0,0,4,1]],"get_image_bps":[[466,5,466,26,0,0,4,0],[468,5,468,26,0,0,4,1]],"encoder_encode":[[1423,9,1423,58,0,0,4,1],[1424,9,1424,58,0,0,4,1],[1425,9,1425,58,0,0,4,0]],"is_valid_img_fmt":[[22,5,22,26,0,0,4,0],[23,5,23,26,0,0,4,1],[24,5,24,26,0,0,4,0]],"macro_block_yrd":[[484,27,484,49,0,0,4,0],[484,27,484,49,0,0,4,1],[494,15,494,21,0,0,4,0],[494,15,494,21,0,0,4,1]],"vp8_regular_quantize_b_sse4_1":[[111,9,111,14,0,0,4,0],[111,9,111,14,0,0,4,1]],"vp8_encode_intra16x16mby":[[95,7,95,18,0,0,4,0]],"vpx_img_set_rect":[[224,18,224,51,0,0,4,1]],"vp8_optimize_mby":[[436,7,436,30,0,0,4,1],[438,7,438,29,0,0,4,1],[446,20,446,67,0,0,4,0],[447,20,447,68,0,0,4,0],[448,10,448,23,0,0,4,0],[450,15,450,21,0,0,4,0],[450,15,450,21,0,0,4,1],[454,7,454,20,0,0,4,0]],"vp8_update_rate_correction_factors":[[1105,5,1105,11,0,0,4,0],[1107,5,1107,11,0,0,4,1]],"vp8_encode_intra16x16mbuv":[[115,7,115,18,0,0,4,0]],"vp8_rd_pick_intra_mode":[[2397,7,2397,28,0,0,4,0],[2397,7,2397,28,0,0,4,1]],"rd_pick_intra16x16mby_mode":[[657,24,657,39,0,0,4,0],[657,24,657,39,0,0,4,1],[670,9,670,26,0,0,4,0],[670,9,670,26,0,0,4,1]],"encode_frame_to_data_rate":[[4099,14,4101,44,0,0,4,0],[4110,11,4110,61,0,0,4,1],[4164,13,4164,41,0,0,4,0],[4166,20,4166,31,0,0,4,0],[4174,13,4174,27,0,0,4,1],[4196,15,4196,37,0,0,4,0],[4207,15,4207,65,0,0,4,1],[4212,19,4212,31,0,0,4,1],[4212,35,4212,75,0,0,4,1],[4224,11,4224,21,0,0,4,1],[4226,18,4226,27,0,0,4,1],[4232,11,4232,50,0,0,4,1],[4233,13,4233,53,0,0,4,1],[4244,9,4244,18,0,0,4,0],[4250,12,4250,21,0,0,4,0]],"rd_pick_intra4x4block":[[547,26,547,43,0,0,4,0],[547,26,547,43,0,0,4,1],[568,9,568,26,0,0,4,0],[568,9,568,26,0,0,4,1]],"vp8_optimize_mbuv":[[469,7,469,30,0,0,4,1],[471,7,471,29,0,0,4,1],[479,16,479,22,0,0,4,0],[479,16,479,22,0,0,4,1]],"rd_pick_intra4x4mby_modes":[[610,15,610,21,0,0,4,0],[610,15,610,21,0,0,4,1],[616,9,616,42,0,0,4,0],[634,9,634,37,0,0,4,0],[634,9,634,37,0,0,4,1],[637,7,637,35,0,0,4,0],[637,7,637,35,0,0,4,1]],"rd_cost_mbuv":[[698,16,698,22,0,0,4,0],[698,16,698,22,0,0,4,1]],"rd_pick_intra_mbuv_mode":[[753,24,753,39,0,0,4,0],[753,24,753,39,0,0,4,1],[781,9,781,26,0,0,4,0],[781,9,781,26,0,0,4,1]],"img_alloc_helper":[[72,5,72,26,0,0,4,1],[73,5,73,26,0,0,4,0],[75,5,75,26,0,0,4,0],[89,5,89,26,0,0,4,1],[90,5,90,26,0,0,4,0],[91,5,91,26,0,0,4,0],[98,5,98,26,0,0,4,1],[101,5,101,26,0,0,4,0],[104,5,104,12,0,0,4,0]],"optimize_b":[[188,7,188,65,0,0,4,0],[202,17,202,25,0,0,4,0],[202,17,202,25,0,0,4,1],[210,9,210,10,0,0,4,0],[210,9,210,10,0,0,4,1],[219,11,219,20,0,0,4,0],[219,11,219,20,0,0,4,1],[227,11,227,31,0,0,4,0],[227,11,227,31,0,0,4,1],[236,40,236,44,0,0,4,0],[236,40,236,44,0,0,4,1],[237,34,237,38,0,0,4,0],[237,34,237,38,0,0,4,1],[246,11,246,58,0,0,4,0],[246,11,246,58,0,0,4,1],[247,11,247,76,0,0,4,0],[253,11,253,19,0,0,4,0],[253,11,253,19,0,0,4,1],[259,11,259,13,0,0,4,0],[259,11,259,13,0,0,4,1],[264,13,264,51,0,0,4,0],[264,13,264,51,0,0,4,1],[266,13,266,51,0,0,4,0],[266,13,266,51,0,0,4,1],[270,11,270,20,0,0,4,0],[270,11,270,20,0,0,4,1],[272,13,272,32,0,0,4,0],[272,13,272,32,0,0,4,1],[276,13,276,32,0,0,4,0],[276,13,276,32,0,0,4,1],[284,11,284,31,0,0,4,0],[284,11,284,31,0,0,4,1],[292,11,292,19,0,0,4,0],[292,11,292,19,0,0,4,1],[296,40,296,44,0,0,4,0],[296,40,296,44,0,0,4,1],[297,34,297,38,0,0,4,0],[297,34,297,38,0,0,4,1],[299,28,299,32,0,0,4,0],[299,28,299,32,0,0,4,1],[313,11,313,30,0,0,4,0],[317,11,317,30,0,0,4,0],[317,11,317,30,0,0,4,1],[338,7,338,27,0,0,4,0],[338,7,338,27,0,0,4,1],[344,18,344,25,0,0,4,0],[344,18,344,25,0,0,4,1],[346,9,346,10,0,0,4,0]],"validate_img":[[292,5,292,26,0,0,4,0],[293,5,293,26,0,0,4,1],[295,5,295,12,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


// Include necessary headers for libvpx

// Callback functions for put_frame and put_slice
static void  id_000056_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This callback receives decoded frames
    // We don't need to do anything for fuzzing
    (void)user_priv;
    (void)img;
}

static void  id_000056_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect,
                               const vpx_image_rect_t* rect2) {
    // This callback receives decoded slices
    // We don't need to do anything for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need at least some data
    
    // Create a FILE* to read the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read all input data into a buffer
    uint8_t* input_buffer = static_cast<uint8_t*>(malloc(size));
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Try both VP8 and VP9 decoders
    const vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Try both VP8 and VP9 encoders
    const vpx_codec_iface_t* encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    // Image formats to try
    vpx_img_fmt_t formats[] = {
        VPX_IMG_FMT_I420,
        VPX_IMG_FMT_YV12,
        VPX_IMG_FMT_I422
    };
    
    // For each decoder type
    for (int decoder_idx = 0; decoder_idx < 2; decoder_idx++) {
        vpx_codec_ctx_t decoder_ctx;
        vpx_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 1;
        
        // Initialize decoder
        vpx_codec_err_t res = vpx_codec_dec_init_ver(
            &decoder_ctx, 
            const_cast<vpx_codec_iface_t*>(decoder_ifaces[decoder_idx]),
            &dec_cfg, 
            fuzz_int64_t_1, 
            VPX_DECODER_ABI_VERSION
        );
        
        if (res != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_error(&decoder_ctx);
            continue;
        }
        
        // Register callbacks
        vpx_codec_register_put_frame_cb(&decoder_ctx,  id_000056_put_frame_callback, nullptr);
        vpx_codec_register_put_slice_cb(&decoder_ctx,  id_000056_put_slice_callback, nullptr);
        
        // Decode the input data
        res = vpx_codec_decode(&decoder_ctx, input_buffer, size, nullptr, fuzz_int64_t_2);
        
        if (res == VPX_CODEC_OK) {
            // Get decoded frames
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t* decoded_frame;
            
            while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
                // For each image format
                for (size_t fmt_idx = 0; fmt_idx < sizeof(formats)/sizeof(formats[0]); fmt_idx++) {
                    // Allocate image for encoding
                    vpx_image_t* img_to_encode = vpx_img_alloc(
                        nullptr,
                        formats[fmt_idx],
                        decoded_frame->d_w,
                        decoded_frame->d_h,
                        fuzz_uint32_t_3
                    );
                    
                    if (!img_to_encode) continue;
                    
                    // Set a rectangle in the image (using random values from input)
                    if (size >= 8) {
                        unsigned int x = (data[0] * decoded_frame->d_w) / 256;
                        unsigned int y = (data[1] * decoded_frame->d_h) / 256;
                        unsigned int w = (data[2] * decoded_frame->d_w) / 256;
                        unsigned int h = (data[3] * decoded_frame->d_h) / 256;
                        vpx_img_set_rect(img_to_encode, x, y, w, h);
                    }
                    
                    // For each encoder type
                    for (int encoder_idx = 0; encoder_idx < 2; encoder_idx++) {
                        vpx_codec_ctx_t encoder_ctx;
                        vpx_codec_enc_cfg_t enc_cfg;
                        
                        // Get default encoder configuration
                        res = vpx_codec_enc_config_default(
                            const_cast<vpx_codec_iface_t*>(encoder_ifaces[encoder_idx]),
                            &enc_cfg,
                            fuzz_uint32_t_4
                        );
                        
                        if (res != VPX_CODEC_OK) continue;
                        
                        // Set encoder configuration
                        enc_cfg.g_w = decoded_frame->d_w;
                        enc_cfg.g_h = decoded_frame->d_h;
                        enc_cfg.rc_target_bitrate = 256; // Reasonable default
                        
                        // Initialize encoder
                        res = vpx_codec_enc_init_ver(
                            &encoder_ctx,
                            const_cast<vpx_codec_iface_t*>(encoder_ifaces[encoder_idx]),
                            &enc_cfg,
                            fuzz_int64_t_5,
                            VPX_ENCODER_ABI_VERSION
                        );
                        
                        if (res != VPX_CODEC_OK) {
                            const char* error_str = vpx_codec_error(&encoder_ctx);
                            continue;
                        }
                        
                        // Register callbacks for encoder too
                        vpx_codec_register_put_frame_cb(&encoder_ctx,  id_000056_put_frame_callback, nullptr);
                        vpx_codec_register_put_slice_cb(&encoder_ctx,  id_000056_put_slice_callback, nullptr);
                        
                        // Encode the image
                        res = vpx_codec_encode(
                            &encoder_ctx,
                            img_to_encode,
                            fuzz_int64_t_6, // pts
                            fuzz_uint64_t_7, // duration
                            fuzz_int64_t_8, // flags
                            fuzz_uint64_t_9  // deadline
                        );
                        
                        if (res == VPX_CODEC_OK) {
                            // Get preview frame
                            const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder_ctx);
                            (void)preview; // Use it to avoid unused variable warning
                            
                            // Get encoded data
                            vpx_codec_iter_t enc_iter = nullptr;
                            const vpx_codec_cx_pkt_t* pkt;
                            
                            while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                                // Process encoded packets if needed
                                // For fuzzing, we just need to call the API
                            }
                        }
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder_ctx);
                    }
                    
                    // Free the allocated image
                    vpx_img_free(img_to_encode);
                }
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder_ctx);
    }
    
    // Clean up
    free(input_buffer);
    
    assert_file_closed(&in_file);
	return 0;
}