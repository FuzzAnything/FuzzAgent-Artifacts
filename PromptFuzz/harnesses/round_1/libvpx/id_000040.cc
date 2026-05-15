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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a fuzz driver that uses all the specified APIs to achieve video transcoding/re-encoding with libvpx:

*/


// Callback function for put_frame
static void  id_000040_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Just acknowledge frame is available
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    
    // Skip if we can't get valid stream info
    if (peek_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // 2. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // 3. Register put_frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(
        &decoder,  id_000040_put_frame_callback, nullptr);
    
    // 4. Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(
        &decoder, data, (unsigned int)size, nullptr, 0);
    
    // 5. Get stream info again using decoder context
    vpx_codec_stream_info_t stream_info2;
    memset(&stream_info2, 0, sizeof(stream_info2));
    stream_info2.sz = sizeof(stream_info2);
    
    vpx_codec_err_t get_info_err = vpx_codec_get_stream_info(
        &decoder, &stream_info2);
    
    // 6. Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 7. Create an encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default encoder configuration
    vpx_codec_err_t cfg_default_err = vpx_codec_enc_config_default(
        encoder_iface, &enc_cfg, 0);
    
    if (cfg_default_err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder parameters based on decoded image
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    // 8. Initialize encoder using multi-encoder version
    vpx_rational_t dsf = {1, 1};
    vpx_codec_err_t enc_init_err = vpx_codec_enc_init_multi_ver(
        &encoder, encoder_iface, &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 9. Re-configure encoder (just to demonstrate vpx_codec_enc_config_set)
    enc_cfg.rc_target_bitrate = 500; // Change bitrate
    vpx_codec_err_t enc_cfg_set_err = vpx_codec_enc_config_set(
        &encoder, &enc_cfg);
    
    // 10. Prepare output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(size * 2); // Allocate enough space
    output_buf.sz = size * 2;
    
    if (output_buf.buf) {
        // 11. Set output buffer for compressed data
        vpx_codec_err_t set_buf_err = vpx_codec_set_cx_data_buf(
            &encoder, &output_buf, 0, 0);
        
        // 12. Create a copy of the image to modify
        unsigned char *img_data = (unsigned char *)malloc(
            decoded_img->stride[0] * decoded_img->d_h * 3 / 2);
        
        if (img_data) {
            // 13. Wrap the image data
            vpx_image_t wrapped_img;
            vpx_img_wrap(&wrapped_img, decoded_img->fmt,
                        decoded_img->d_w, decoded_img->d_h,
                        1, img_data);
            
            // Copy data from decoded image
            for (int plane = 0; plane < 3; plane++) {
                if (decoded_img->planes[plane]) {
                    unsigned char *src = decoded_img->planes[plane];
                    unsigned char *dst = wrapped_img.planes[plane];
                    int stride = decoded_img->stride[plane];
                    int height = (plane == 0) ? decoded_img->d_h : 
                                 (decoded_img->d_h + 1) >> 1;
                    
                    for (int y = 0; y < height; y++) {
                        memcpy(dst, src, stride);
                        src += stride;
                        dst += wrapped_img.stride[plane];
                    }
                }
            }
            
            // 14. Set a rectangle on the image
            unsigned int rect_x = 0;
            unsigned int rect_y = 0;
            unsigned int rect_w = (decoded_img->d_w > 16) ? 
                                  decoded_img->d_w / 4 : decoded_img->d_w;
            unsigned int rect_h = (decoded_img->d_h > 16) ? 
                                  decoded_img->d_h / 4 : decoded_img->d_h;
            
            vpx_img_set_rect(&wrapped_img, rect_x, rect_y, rect_w, rect_h);
            
            // 15. Encode the modified image
            vpx_codec_encode(&encoder, &wrapped_img, 0, 1, 0, 0);
            
            // 16. Get encoded data
            const vpx_codec_cx_pkt_t *pkt;
            vpx_codec_iter_t enc_iter = nullptr;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Encoded data available in pkt->data.frame.buf
                    break;
                }
            }
            
            free(img_data);
        }
        
        free(output_buf.buf);
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}