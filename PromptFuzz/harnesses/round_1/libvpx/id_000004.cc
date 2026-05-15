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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 11
//<Quality> {"density":9,"unique_branches":{"encode_mb_row":[[412,11,412,38,0,0,4,1],[416,21,416,44,0,0,4,1]],"vp8_regulate_q":[[1203,9,1203,62,0,0,4,0],[1205,11,1205,47,0,0,4,1]],"encode_frame_to_data_rate":[[4526,7,4526,59,0,0,4,0]],"rescale":[[272,7,272,24,0,0,4,1]],"thread_encoding_proc":[[112,15,112,42,0,0,4,1],[116,15,116,21,0,0,4,1],[116,25,116,48,0,0,4,1],[201,17,201,40,0,0,4,1],[227,17,227,42,0,0,4,1]]},"library_calls":["vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Define callback function for put_slice
static void  id_000004_put_slice_callback(void* user_priv, 
                               const vpx_image_t* img,
                               const vpx_image_rect_t* rect1,
                               const vpx_image_rect_t* rect2) {
    // Callback implementation - minimal for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect1;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 10) {
        return 0;
    }
    
    // API 1: Get build configuration string
    const char* build_config = vpx_codec_build_config();
    if (build_config) {
        // Just to ensure it's called, but we don't use the result
        (void)build_config;
    }
    
    // Create FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create output file (not actually used, but required by instructions)
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    if (fd_out < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize codec interface (using VP8 encoder)
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    
    // API 2: Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);  // usage = 0 for good quality
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set basic configuration parameters from input data
    // Use first few bytes for deterministic but varied configurations
    cfg.g_w = (data[0] % 64 + 1) * 16;      // Width: 16-1024 pixels
    cfg.g_h = (data[1] % 64 + 1) * 16;      // Height: 16-1024 pixels
    cfg.rc_target_bitrate = (data[2] % 1000 + 100) * 1000;  // Bitrate: 100k-1.1Mbps
    cfg.g_threads = (data[3] % 4) + 1;      // Threads: 1-4
    
    // Create codec context
    vpx_codec_ctx_t ctx;
    
    // API 3: Initialize encoder
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // API 4: Peek stream info (try both VP8 and VP9 interfaces)
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Use remaining data for stream info peeking
    size_t peek_size = size > 1000 ? 1000 : size;  // Limit peek size
    vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, peek_size, &si);
    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, peek_size, &si);
    
    // API 5: Register put slice callback
    err = vpx_codec_register_put_slice_cb(&ctx,  id_000004_put_slice_callback, NULL);
    if (err != VPX_CODEC_OK) {
        // Callback registration may fail for some configurations, continue anyway
    }
    
    // Create a dummy image for encoding
    const int align = 32;
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, align);
    if (img) {
        // Fill image with some data from input
        size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2;
        size_t fill_size = size > img_data_size ? img_data_size : size;
        
        if (fill_size > 0) {
            // Fill Y plane
            size_t y_size = cfg.g_w * cfg.g_h;
            size_t y_fill = fill_size > y_size ? y_size : fill_size;
            if (y_fill > 0) {
                memcpy(img->planes[0], data, y_fill);
            }
            
            // Fill UV planes if enough data
            if (fill_size > y_size) {
                size_t uv_size = y_size / 2;
                size_t uv_fill = (fill_size - y_size) > uv_size ? uv_size : (fill_size - y_size);
                if (uv_fill > 0) {
                    memcpy(img->planes[1], data + y_size, uv_fill);
                    memcpy(img->planes[2], data + y_size + uv_fill/2, uv_fill/2);
                }
            }
        }
        
        // Try to encode the dummy frame
        vpx_codec_encode(&ctx, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write some output (just to use the file descriptor)
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                break;
            }
        }
        
        vpx_img_free(img);
    }
    
    // Cleanup
    vpx_codec_destroy(&ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}