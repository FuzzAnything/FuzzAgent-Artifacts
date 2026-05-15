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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 41.8, nr_unique_branch: 27
//<Quality> {"density":19,"unique_branches":{"block_rd_txfm":[[736,7,736,26,0,0,4,1],[752,11,752,25,0,0,4,1],[772,9,772,50,0,0,4,1],[777,9,777,45,0,0,4,1],[782,9,782,41,0,0,4,0],[796,16,796,51,0,0,4,1],[837,10,837,27,0,0,4,1],[837,44,837,57,0,0,4,1]],"vpx_write":[[87,9,87,19,0,0,4,1]],"set_vt_partitioning":[[492,9,492,32,0,0,4,1],[494,9,494,48,0,0,4,1]],"vpx_stop_encode":[[42,21,42,61,0,0,4,0],[43,9,43,27,0,0,4,0]],"vp9_encode_block_intra":[[1049,11,1049,26,0,0,4,1],[1057,11,1057,26,0,0,4,1],[1062,11,1062,26,0,0,4,1],[1073,11,1073,26,0,0,4,1]],"vp9_block_error_avx2":[[29,7,29,23,0,0,4,1]],"choose_largest_tx_size":[[24,23,24,34,1,0,4,1]],"pack_mb_tokens":[[156,11,156,20,0,0,4,0],[156,24,156,46,0,0,4,0]],"pixel_sse":[[539,8,539,39,0,0,4,1],[539,43,539,75,0,0,4,1],[549,17,549,26,0,0,4,1],[24,23,24,34,3,0,4,1],[24,23,24,34,4,0,4,1]],"rd_pick_intra_sub_8x8_y_mode":[[1325,11,1325,46,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":4}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode encoded video data and re-encode it:

*/


// Include libvpx headers

// Put slice callback function
static void  id_000044_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect,
                               const vpx_image_rect_t* rect2) {
    // This callback is called during decoding when a slice is available
    // For fuzzing, we can simply ignore or do minimal processing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
	return 0;
	}
	
    
    // Create a FILE pointer for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for writing encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused variable warning
    (void)out_fd; // Suppress unused variable warning
    
    // Variables for decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx(); // Using VP9 decoder
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put slice callback
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000044_put_slice_callback, NULL);
    if (res != VPX_CODEC_OK) {
        // Get error message
        const char* error_msg = vpx_codec_error(&decoder);
        (void)error_msg; // Suppress unused variable warning
    }
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_2);
    
    // Check for decoder errors
    if (res != VPX_CODEC_OK) {
        const char* error_msg = vpx_codec_error(&decoder);
        (void)error_msg; // Suppress unused variable warning
    }
    
    // Variables for encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx(); // Using VP9 encoder
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set some basic parameters
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000; // 1000 kbps
    enc_cfg.g_error_resilient = 0;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set encoder configuration again (for testing the API)
    res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    if (res != VPX_CODEC_OK) {
        const char* error_msg = vpx_codec_error(&encoder);
        (void)error_msg; // Suppress unused variable warning
    }
    
    // Get decoded frames from decoder
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // If we have a decoded frame, try to encode it
    if (decoded_frame) {
        // Encode the decoded frame
        res = vpx_codec_encode(&encoder, decoded_frame, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);
        
        if (res == VPX_CODEC_OK) {
            // Get encoded data packets
            vpx_codec_iter_t enc_iter = NULL;
            const vpx_codec_cx_pkt_t* pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write encoded data to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    } else {
        // If no frame was decoded, try to encode a simple frame
        
        // Create a simple image
        vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 640, 480, fuzz_uint32_t_9);
        if (img) {
            // Fill with some data
            for (int plane = 0; plane < 3; plane++) {
                uint8_t* buf = img->planes[plane];
                int stride = img->stride[plane];
                int height = (plane == 0) ? 480 : 240;
                int width = (plane == 0) ? 640 : 320;
                
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        buf[y * stride + x] = (uint8_t)((x + y) % 256);
                    }
                }
            }
            
            // Encode the image
            res = vpx_codec_encode(&encoder, img, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = NULL;
            const vpx_codec_cx_pkt_t* pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            vpx_img_free(img);
        }
    }
    
    // Check for encoder errors
    if (res != VPX_CODEC_OK) {
        const char* error_msg = vpx_codec_error(&encoder);
        (void)error_msg; // Suppress unused variable warning
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}