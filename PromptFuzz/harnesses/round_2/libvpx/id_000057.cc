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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Dummy put_slice callback function
static void  id_000057_dummy_put_slice_cb(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice_rect,
                               const vpx_image_rect_t *tile_rect) {
    // This is just a dummy callback for testing
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)tile_rect;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;  // Need at least some data
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_err_t res;
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    vpx_image_t *img = nullptr;
    vpx_image_t wrapped_img;
    vpx_fixed_buf_t *global_headers = nullptr;
    
    // Determine which codec to use based on input data
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    encoder_iface = vpx_codec_vp9_cx();
    
    // Check codec interface names
    const char *decoder_name = vpx_codec_iface_name(decoder_iface);
    const char *encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)decoder_name;
    (void)encoder_name;

    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    res = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    if (res != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
        res = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
        if (res != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(res);
            (void)error_str;
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Register put slice callback
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000057_dummy_put_slice_cb, nullptr);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        (void)error_str;
        // Continue even if callback registration fails
    }

    // Read and decode the input data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    (void)bytes_read;
    
    res = vpx_codec_decode(&decoder, buffer.data(), size, nullptr, 0);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        (void)error_str;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    img = vpx_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Initialize encoder configuration
        vpx_codec_enc_cfg_t enc_cfg;
        encoder_iface = vpx_codec_vp8_cx();  // Use VP8 for encoding
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        if (res == VPX_CODEC_OK) {
            // Set encoder configuration based on decoded frame
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.rc_end_usage = VPX_VBR;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.kf_mode = VPX_KF_AUTO;
            
            // Initialize encoder
            res = vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0);
            
            if (res == VPX_CODEC_OK) {
                // Get global headers
                global_headers = vpx_codec_get_global_headers(&encoder);
                (void)global_headers;  // Use the variable
                
                // Create a buffer for wrapped image data
                size_t img_size = img->d_w * img->d_h * 3;  // Rough estimate
                std::vector<unsigned char> img_data(img_size);
                
                // Wrap the image data using vpx_img_wrap
                vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img, 
                                                    VPX_IMG_FMT_I420,
                                                    img->d_w,
                                                    img->h,
                                                    1,  // stride_align
                                                    img_data.data());
                
                if (wrapped) {
                    // Set a rectangle on the image
                    unsigned int rect_x = 0;
                    unsigned int rect_y = 0;
                    unsigned int rect_w = img->d_w / 2;
                    unsigned int rect_h = img->h / 2;
                    
                    if (rect_w > 0 && rect_h > 0) {
                        int set_rect_res = vpx_img_set_rect(wrapped, rect_x, rect_y, rect_w, rect_h);
                        (void)set_rect_res;
                        
                        // Try to encode the wrapped image
                        res = vpx_codec_encode(&encoder, wrapped, 0, 1, 0, VPX_DL_REALTIME);
                        if (res != VPX_CODEC_OK) {
                            const char *error_str = vpx_codec_err_to_string(res);
                            (void)error_str;
                        }
                    }
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Clean up resources
    if (global_headers) {
        free(global_headers->buf);
    }
    
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}