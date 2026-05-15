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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"decoder_set_fb_fn":[[438,14,438,30,0,0,4,1]],"encoder_set_config":[[841,7,841,31,0,0,4,0],[842,9,842,33,0,0,4,0]],"vpx_codec_register_put_slice_cb":[[162,12,162,57,0,0,4,1]]},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_decode","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_enc_config_set","vpx_img_set_rect","vpx_codec_encode","vpx_codec_control_","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_alloc","vpx_codec_decode","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_enc_config_set","vpx_img_set_rect","vpx_codec_encode","vpx_codec_control_","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static int  id_000031_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000031_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

static void  id_000031_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *valid,
                         const vpx_image_rect_t *update) {
    // Callback implementation
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
	return 0;
	}
	

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, fuzz_uint32_t_2);
    
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.rc_target_bitrate = 1000;
    
    res = vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &enc_cfg, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 640, 480, fuzz_uint32_t_4);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Process input as encoded bitstream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        vpx_img_free(img);
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input as compressed data
    uint8_t *compressed_data = (uint8_t*)malloc(size);
    if (!compressed_data) {
        assert_file_closed(&in_file);;
        vpx_img_free(img);
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(compressed_data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Decode the input
    res = vpx_codec_decode(&decoder, compressed_data, size, NULL, fuzz_int64_t_5);
    
    // Get stream info
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), compressed_data, size, &si);
    
    // Set frame buffer functions for decoder
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000031_get_frame_buffer_cb, 
                                         id_000031_release_frame_buffer_cb, NULL);

    // Register put slice callback
    vpx_codec_register_put_slice_cb(&encoder,  id_000031_put_slice_cb, NULL);

    // Set encoder configuration
    enc_cfg.g_w = si.w > 0 ? si.w : 640;
    enc_cfg.g_h = si.h > 0 ? si.h : 480;
    vpx_codec_enc_config_set(&encoder, &enc_cfg);

    // Set image rectangle
    unsigned int x = data[0] % (img->d_w - 10);
    unsigned int y = data[1] % (img->d_h - 10);
    unsigned int w = 10 + (data[2] % 50);
    unsigned int h = 10 + (data[3] % 50);
    
    // Clamp values
    if (x + w > img->d_w) w = img->d_w - x;
    if (y + h > img->d_h) h = img->d_h - y;
    
    vpx_img_set_rect(img, x, y, w, h);

    // Encode frames
    vpx_codec_encode(&encoder, img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);

    // Use codec control
    int ctrl_id = VP8E_SET_CPUUSED;
    int value = data[4] % 8;
    vpx_codec_control_(&encoder, ctrl_id, value);

    // Clean up
    free(compressed_data);
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}