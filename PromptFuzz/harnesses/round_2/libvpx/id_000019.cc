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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_img_wrap","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_img_wrap","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Custom callback functions
static int  id_000019_get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    fb->size = min_size;
    return fb->data != nullptr;
}

static int  id_000019_release_frame_buffer_cb(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
        fb->data = nullptr;
    }
    return 0;
}

// Slice callback function
static void  id_000019_put_slice_cb(void *user_priv, const vpx_image_t *img,
                        const vpx_image_rect_t *valid,
                        const vpx_image_rect_t *update) {
    // Do nothing - just required for registration
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read some data to use for image
    uint8_t *img_data = (uint8_t*)malloc(640 * 480 * 3 / 2);  // I420 format
    if (!img_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_bytes = fread(img_data, 1, 640 * 480 * 3 / 2, in_file);
    rewind(in_file);

    // 1. Create image using vpx_img_wrap
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, 640, 480, 1, img_data);
    
    // 2. Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                               VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        free(img_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Register put slice callback
    vpx_codec_register_put_slice_cb(&encoder,  id_000019_put_slice_cb, nullptr);
    
    // 4. Set frame buffer functions for encoder
    vpx_codec_set_frame_buffer_functions(&encoder,  id_000019_get_frame_buffer_cb, 
                                         id_000019_release_frame_buffer_cb, nullptr);
    
    // 5. Create output buffer and set it
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
    
    // 6. Encode the image
    vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);
    
    // 7. Get error detail (even if no error)
    const char *error_detail = vpx_codec_error_detail(&encoder);
    
    // 8. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, 
                              VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        free(img_data);
        free(output_buf.buf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    // 10. Get stream info
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&decoder, &si);
    
    // 11. Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    free(img_data);
    free(output_buf.buf);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}