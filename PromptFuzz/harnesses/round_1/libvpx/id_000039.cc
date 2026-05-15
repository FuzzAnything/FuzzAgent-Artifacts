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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000039_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Simple callback that does nothing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // 1. Use vpx_codec_peek_stream_info to peek at stream info
    vpx_codec_err_t err = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
        // Fall back to VP8 if VP9 fails
        err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, &stream_info);
    }
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    memset(&decoder, 0, sizeof(decoder));
    
    // Try to initialize VP9 decoder first
    err = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), NULL, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Fall back to VP8 decoder
        err = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(), NULL, 0, VPX_DECODER_ABI_VERSION);
    }
    
    if (err == VPX_CODEC_OK) {
        // 2. Use vpx_codec_get_stream_info after initialization
        vpx_codec_get_stream_info(&decoder, &stream_info);
        
        // Decode the input data
        vpx_codec_decode(&decoder, data, size, NULL, 0);
        
        // Get decoded frame
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
        
        if (decoded_img) {
            // Create a copy of the image data for manipulation
            size_t img_data_size = decoded_img->bps * decoded_img->d_w * decoded_img->d_h / 8;
            unsigned char *img_data = (unsigned char*)malloc(img_data_size);
            if (img_data) {
                // 3. Use vpx_img_wrap to wrap image data
                vpx_image_t wrapped_img;
                memset(&wrapped_img, 0, sizeof(wrapped_img));
                vpx_image_t *img_ptr = vpx_img_wrap(&wrapped_img, decoded_img->fmt, 
                                                   decoded_img->d_w, decoded_img->d_h, 
                                                   1, img_data);
                
                if (img_ptr) {
                    // Copy data from decoded image to wrapped image
                    for (int plane = 0; plane < 3; plane++) {
                        if (decoded_img->planes[plane]) {
                            size_t plane_size = (plane == 0) ? 
                                decoded_img->d_w * decoded_img->d_h :
                                (decoded_img->d_w >> decoded_img->x_chroma_shift) * 
                                (decoded_img->d_h >> decoded_img->y_chroma_shift);
                            memcpy(img_ptr->planes[plane], decoded_img->planes[plane], plane_size);
                        }
                    }
                    
                    // 4. Use vpx_img_set_rect to set a rectangle
                    unsigned int x = size > 4 ? data[0] % (img_ptr->d_w / 2) : 0;
                    unsigned int y = size > 4 ? data[1] % (img_ptr->d_h / 2) : 0;
                    unsigned int w = size > 8 ? data[2] % (img_ptr->d_w - x) + 1 : img_ptr->d_w / 4;
                    unsigned int h = size > 8 ? data[3] % (img_ptr->d_h - y) + 1 : img_ptr->d_h / 4;
                    
                    vpx_img_set_rect(img_ptr, x, y, w, h);
                    
                    // Initialize encoder configuration
                    vpx_codec_enc_cfg_t cfg;
                    memset(&cfg, 0, sizeof(cfg));
                    
                    // Try to get default config for VP9
                    err = vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0);
                    if (err != VPX_CODEC_OK) {
                        // Fall back to VP8
                        err = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
                    }
                    
                    if (err == VPX_CODEC_OK) {
                        // Set encoder dimensions
                        cfg.g_w = img_ptr->d_w;
                        cfg.g_h = img_ptr->d_h;
                        cfg.g_timebase.num = 1;
                        cfg.g_timebase.den = 30;
                        cfg.rc_target_bitrate = 256;
                        cfg.g_profile = size > 12 ? data[4] % 4 : 0;
                        
                        // Initialize encoder context
                        vpx_codec_ctx_t encoder;
                        memset(&encoder, 0, sizeof(encoder));
                        
                        // 5. Use vpx_codec_enc_init_multi_ver with 1 encoder
                        const vpx_rational_t dsf = {1, 1};
                        err = vpx_codec_enc_init_multi_ver(&encoder, vpx_codec_vp9_cx(), 
                                                          &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
                        
                        if (err != VPX_CODEC_OK) {
                            // Fall back to VP8
                            err = vpx_codec_enc_init_ver(&encoder, vpx_codec_vp8_cx(), 
                                                        &cfg, 0, VPX_ENCODER_ABI_VERSION);
                        }
                        
                        if (err == VPX_CODEC_OK) {
                            // 6. Use vpx_codec_register_put_frame_cb
                            vpx_codec_register_put_frame_cb(&encoder,  id_000039_put_frame_callback, NULL);
                            
                            // 7. Use vpx_codec_enc_config_set to update config
                            cfg.rc_target_bitrate = size > 16 ? (data[5] * 100) + 100 : 300;
                            vpx_codec_enc_config_set(&encoder, &cfg);
                            
                            // Create output buffer
                            vpx_fixed_buf_t output_buf;
                            output_buf.sz = size * 2; // Allocate buffer for encoded data
                            output_buf.buf = malloc(output_buf.sz);
                            
                            if (output_buf.buf) {
                                // 8. Use vpx_codec_set_cx_data_buf
                                unsigned int pad_before = size > 20 ? data[6] % 16 : 0;
                                unsigned int pad_after = size > 24 ? data[7] % 16 : 0;
                                vpx_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);
                                
                                // Encode the image
                                vpx_codec_encode(&encoder, img_ptr, 0, 1, 0, VPX_DL_REALTIME);
                                
                                // Get encoded data
                                vpx_codec_iter_t enc_iter = NULL;
                                const vpx_codec_cx_pkt_t *pkt = vpx_codec_get_cx_data(&encoder, &enc_iter);
                                
                                if (pkt && pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Write encoded data to output file
                                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                }
                                
                                free(output_buf.buf);
                            }
                            
                            vpx_codec_destroy(&encoder);
                        }
                    }
                    
                    // Clean up wrapped image data
                    free(img_data);
                }
            }
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}