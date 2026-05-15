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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"decoder_destroy":[[59,7,59,23,0,0,4,1],[63,7,63,23,0,0,4,1]]},"library_calls":["vpx_codec_iface_name","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_iface_name","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for frame buffer allocation
int  id_000014_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    // Simple frame buffer allocation - just allocate requested size
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

// Callback function for frame buffer release
int  id_000014_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
    }
    return 0;
}

// Callback function for put slice
void  id_000014_put_slice_cb(void *user_priv, const vpx_image_t *img, 
                  const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2) {
    // This callback is called when a slice is decoded
    // We don't need to do anything specific for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1024) {
        return 0; // Need minimum data for meaningful fuzzing
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the codec interface names
    const char *decoder_name = vpx_codec_iface_name(vpx_codec_vp9_dx());
    const char *encoder_name = vpx_codec_iface_name(vpx_codec_vp9_cx());
    (void)decoder_name; // Use variables to avoid unused warnings
    (void)encoder_name;

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) 
        != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set frame buffer functions for decoder
    if (vpx_codec_set_frame_buffer_functions(&decoder, 
                                              id_000014_get_frame_buffer_cb,
                                              id_000014_release_frame_buffer_cb,
                                             nullptr) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put slice callback
    if (vpx_codec_register_put_slice_cb(&decoder,  id_000014_put_slice_cb, nullptr) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Wrap the decoded image data for re-use
    vpx_image_t wrapped_img;
    if (!vpx_img_wrap(&wrapped_img, decoded_img->fmt, decoded_img->d_w, 
                     decoded_img->d_h, fuzz_uint32_t_3, decoded_img->planes[0])) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set a rectangle on the image (use a portion of the image)
    unsigned int rect_x = 0;
    unsigned int rect_y = 0;
    unsigned int rect_w = (decoded_img->d_w > 100) ? 100 : decoded_img->d_w;
    unsigned int rect_h = (decoded_img->d_h > 100) ? 100 : decoded_img->d_h;
    
    if (vpx_img_set_rect(&wrapped_img, fuzz_uint32_t_4, fuzz_uint32_t_5, rect_w, rect_h) != 0) {
        vpx_img_free(&wrapped_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize multiple encoders (2 encoders)
    vpx_codec_ctx_t encoder1, encoder2;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, fuzz_uint32_t_6) != VPX_CODEC_OK) {
        vpx_img_free(&wrapped_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoder settings
    enc_cfg.g_w = rect_w;
    enc_cfg.g_h = rect_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 1;

    // Downscaling factors for multiple encoders
    vpx_rational_t dsf[2] = {{1, 1}, {2, 1}}; // Original and half size

    // Initialize multiple encoders
    if (vpx_codec_enc_init_multi_ver(&encoder1, vpx_codec_vp9_cx(), &enc_cfg, 
                                     2, fuzz_int64_t_7, dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(&wrapped_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize second encoder with same config
    if (vpx_codec_enc_init_multi_ver(&encoder2, vpx_codec_vp9_cx(), &enc_cfg, 
                                     2, fuzz_int64_t_8, dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder1);
        vpx_img_free(&wrapped_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get global headers from encoders
    vpx_fixed_buf_t *global_headers1 = vpx_codec_get_global_headers(&encoder1);
    vpx_fixed_buf_t *global_headers2 = vpx_codec_get_global_headers(&encoder2);
    
    // Write global headers to output file if they exist
    if (global_headers1 && global_headers1->buf && global_headers1->sz > 0) {
        fwrite(global_headers1->buf, 1, global_headers1->sz, out_file);
    }
    if (global_headers2 && global_headers2->buf && global_headers2->sz > 0) {
        fwrite(global_headers2->buf, 1, global_headers2->sz, out_file);
    }

    // Clean up
    vpx_codec_destroy(&encoder2);
    vpx_codec_destroy(&encoder1);
    vpx_img_free(&wrapped_img);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}