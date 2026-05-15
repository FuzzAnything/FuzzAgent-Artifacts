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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Define callback function for put_slice
static void  id_000014_put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data
    
    // Create file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Use to avoid warnings
    
    // 1. Get codec interface name
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name;
    
    // 2. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                  &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Register put slice callback (for decoder)
    vpx_codec_register_put_slice_cb(&decoder,  id_000014_put_slice_callback, NULL);
    
    // 4. Decode input data
    vpx_codec_decode(&decoder, data, size, NULL, 0);
    
    // 5. Get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 6. Get preview frame (from decoder)
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
        (void)preview_frame;
        
        // 7. Create image wrapper for encoding
        vpx_image_t img;
        memset(&img, 0, sizeof(img));
        
        // Use decoded image dimensions
        unsigned int width = decoded_img->d_w;
        unsigned int height = decoded_img->d_h;
        
        // Allocate image data buffer
        size_t img_data_size = width * height * 3 / 2; // I420 format
        unsigned char *img_data = (unsigned char *)malloc(img_data_size);
        if (img_data) {
            // 8. Wrap image data
            vpx_image_t *wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, 
                                                    width, height, 1, img_data);
            
            if (wrapped_img) {
                // 9. Set rectangle on image
                unsigned int rect_x = width > 16 ? 8 : 0;
                unsigned int rect_y = height > 16 ? 8 : 0;
                unsigned int rect_w = (width - rect_x) > 16 ? 16 : (width - rect_x);
                unsigned int rect_h = (height - rect_y) > 16 ? 16 : (height - rect_y);
                
                vpx_img_set_rect(wrapped_img, rect_x, rect_y, rect_w, rect_h);
                
                // 10. Initialize multiple encoders
                vpx_codec_ctx_t encoder1, encoder2;
                vpx_codec_enc_cfg_t cfg1, cfg2;
                vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
                
                // Get default config
                vpx_codec_enc_config_default(encoder_iface, &cfg1, 0);
                cfg1.g_w = width;
                cfg1.g_h = height;
                cfg1.g_timebase.num = 1;
                cfg1.g_timebase.den = 30;
                
                // Copy config for second encoder
                memcpy(&cfg2, &cfg1, sizeof(cfg1));
                cfg2.rc_target_bitrate = cfg1.rc_target_bitrate * 2;
                
                // Initialize two encoders with different configurations
                vpx_rational_t dsf[2] = {{1,1}, {1,1}};
                
                res = vpx_codec_enc_init_multi_ver(&encoder1, encoder_iface, 
                                                   &cfg1, 2, 0, dsf, VPX_ENCODER_ABI_VERSION);
                if (res == VPX_CODEC_OK) {
                    // 11. Set up output buffer for encoder
                    vpx_fixed_buf_t output_buf;
                    output_buf.buf = malloc(100000);
                    output_buf.sz = 100000;
                    
                    if (output_buf.buf) {
                        vpx_codec_set_cx_data_buf(&encoder1, &output_buf, 0, 0);
                        
                        // 12. Encode the image
                        vpx_codec_encode(&encoder1, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        // 13. Get encoded data
                        vpx_codec_iter_t enc_iter = NULL;
                        const vpx_codec_cx_pkt_t *pkt;
                        while ((pkt = vpx_codec_get_cx_data(&encoder1, &enc_iter)) != NULL) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                // Write to output file
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                        
                        free(output_buf.buf);
                    }
                    
                    // 14. Free image
                    vpx_img_free(wrapped_img);
                    
                    // Clean up encoder
                    vpx_codec_destroy(&encoder1);
                }
            }
            
            free(img_data);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}