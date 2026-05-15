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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"validate_img":[[301,7,301,33,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* data;
    size_t size;
    int used;
};

// Get frame buffer callback
int  id_000013_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fbd = static_cast<FrameBufferData*>(cb_priv);
    
    if (fbd->used) {
        return -1; // Buffer already in use
    }
    
    if (fbd->size < min_size) {
        free(fbd->data);
        fbd->data = static_cast<uint8_t*>(malloc(min_size));
        if (!fbd->data) return -1;
        fbd->size = min_size;
    }
    
    fb->data = fbd->data;
    fb->size = fbd->size;
    fbd->used = 1;
    
    return 0;
}

// Release frame buffer callback
int  id_000013_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fbd = static_cast<FrameBufferData*>(cb_priv);
    fbd->used = 0;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // 1. First, peek stream information from the encoded data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 codecs
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), 
                                                             data, 
                                                             (unsigned int)size, 
                                                             &stream_info);
    
    if (peek_result != VPX_CODEC_OK) {
        // Try VP9 if VP8 fails
        peek_result = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), 
                                                 data, 
                                                 (unsigned int)size, 
                                                 &stream_info);
    }
    
    // 2. Set up a decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_err_t init_result = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    
    if (init_result != VPX_CODEC_OK) {
        // Try VP9 decoder
        decoder_iface = vpx_codec_vp9_dx();
        init_result = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
        if (init_result != VPX_CODEC_OK) {
            return 0; // Cannot initialize decoder
        }
    }
    
    // 3. Set frame buffer functions for the decoder
    FrameBufferData frame_buffer_data = {nullptr, 0, 0};
    vpx_codec_err_t set_fb_result = vpx_codec_set_frame_buffer_functions(
        &decoder, 
         id_000013_get_frame_buffer_cb, 
         id_000013_release_frame_buffer_cb, 
        &frame_buffer_data);
    
    // 4. Try to decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(&decoder, 
                                                     data, 
                                                     (unsigned int)size, 
                                                     nullptr, 
                                                     fuzz_int64_t_1);
    
    // 5. Get error detail if decoding failed
    const char* error_detail = vpx_codec_error_detail(&decoder);
    if (error_detail) {
        // Just to use the error detail variable
        (void)error_detail;
    }
    
    // 6. Set up an encoder to use set_cx_data_buf
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2) == VPX_CODEC_OK) {
        // Initialize encoder with default config
        if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
            // Create a buffer for compressed data
            vpx_fixed_buf_t output_buffer;
            output_buffer.buf = malloc(4096);
            output_buffer.sz = 4096;
            
            if (output_buffer.buf) {
                // 7. Set compressed data buffer for encoder
                vpx_codec_err_t set_buf_result = vpx_codec_set_cx_data_buf(
                    &encoder, 
                    &output_buffer, 
                    fuzz_uint32_t_3,  // pad_before
                    fuzz_uint32_t_4); // pad_after
                
                // 8. Create an image and set a rectangle on it
                vpx_image_t* img = vpx_img_alloc(nullptr, 
                                                 VPX_IMG_FMT_I420, 
                                                 128,  // width
                                                 128,  // height
                                                 fuzz_uint32_t_5);   // align
                
                if (img) {
                    // Set a rectangle on the image (crop region)
                    // Using safe values within image bounds
                    unsigned int x = 0;
                    unsigned int y = 0;
                    unsigned int w = 64;
                    unsigned int h = 64;
                    
                    if (w <= img->d_w && h <= img->d_h) {
                        vpx_img_set_rect(img, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8, fuzz_uint32_t_9);
                    }
                    
                    // Try to encode the image
                    vpx_codec_encode(&encoder, 
                                     img, 
                                     fuzz_int64_t_10,  // pts
                                     fuzz_uint64_t_11,  // duration
                                     fuzz_int64_t_12,  // flags
                                     fuzz_uint64_t_13); // deadline
                    
                    vpx_img_free(img);
                }
                
                free(output_buffer.buf);
            }
            
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 9. Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create a copy of the image to manipulate
        vpx_image_t* img_copy = vpx_img_alloc(nullptr, 
                                              VPX_IMG_FMT_I420, 
                                              decoded_img->d_w, 
                                              decoded_img->d_h, 
                                              fuzz_uint32_t_14);
        
        if (img_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; ++plane) {
                if (decoded_img->planes[plane]) {
                    size_t plane_size = decoded_img->stride[plane] * 
                                       (plane == 0 ? decoded_img->d_h : decoded_img->d_h / 2);
                    if (plane_size <= img_copy->stride[plane] * 
                        (plane == 0 ? img_copy->d_h : img_copy->d_h / 2)) {
                        memcpy(img_copy->planes[plane], 
                               decoded_img->planes[plane], 
                               plane_size);
                    }
                }
            }
            
            // Set a rectangle on the copied image
            // Use safe rectangle dimensions
            unsigned int rect_w = decoded_img->d_w > 32 ? 32 : decoded_img->d_w;
            unsigned int rect_h = decoded_img->d_h > 32 ? 32 : decoded_img->d_h;
            
            if (rect_w > 0 && rect_h > 0) {
                vpx_img_set_rect(img_copy, fuzz_uint32_t_15, fuzz_uint32_t_16, rect_w, rect_h);
            }
            
            vpx_img_free(img_copy);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    
    // Free frame buffer data if allocated
    if (frame_buffer_data.data) {
        free(frame_buffer_data.data);
    }
    
    return 0;
}