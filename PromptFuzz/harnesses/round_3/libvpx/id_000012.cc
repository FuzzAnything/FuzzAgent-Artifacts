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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"vpx_memalign":[[60,7,60,53,0,0,4,0]],"vpx_img_free":[[270,9,270,22,0,0,4,1]],"img_alloc_helper":[[152,7,152,21,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_error_detail","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_error_detail","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000012_get_frame_buffer(void *cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000012_release_frame_buffer(void *cb_priv, 
                               vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

// Custom buffer for compressed data
static uint8_t*  id_000012_create_padded_buffer(const uint8_t* data, size_t size, 
                                     unsigned int pad_before, 
                                     unsigned int pad_after, 
                                     size_t* total_size) {
    *total_size = size + pad_before + pad_after;
    uint8_t* padded = (uint8_t*)malloc(*total_size);
    if (!padded) return nullptr;
    
    memset(padded, 0, pad_before);
    memcpy(padded + pad_before, data, size);
    memset(padded + pad_before + size, 0, pad_after);
    
    return padded;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Create a FILE* from input data for potential future use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. First use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 codecs
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp8_dx(), data, size, &stream_info);
    
    if (peek_result != VPX_CODEC_OK) {
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp9_dx(), data, size, &stream_info);
    }
    
    // 2. Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, vpx_codec_vp8_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        init_result = vpx_codec_dec_init_ver(
            &decoder, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    }
    
    if (init_result == VPX_CODEC_OK) {
        // 3. Set frame buffer functions
        vpx_codec_err_t fb_result = vpx_codec_set_frame_buffer_functions(
            &decoder,  id_000012_get_frame_buffer,  id_000012_release_frame_buffer, nullptr);
        
        // 4. Create padded buffer and set it as compressed data buffer
        unsigned int pad_before = size > 100 ? 50 : size / 2;
        unsigned int pad_after = size > 100 ? 50 : size / 2;
        size_t padded_size;
        
        uint8_t* padded_data =  id_000012_create_padded_buffer(data, size, 
                                                   pad_before, pad_after, 
                                                   &padded_size);
        
        if (padded_data) {
            vpx_fixed_buf_t cx_buf;
            cx_buf.buf = padded_data;
            cx_buf.sz = padded_size;
            
            // Note: vpx_codec_set_cx_data_buf is typically for encoder,
            // but we call it anyway for fuzzing
            vpx_codec_err_t set_buf_result = vpx_codec_set_cx_data_buf(
                &decoder, &cx_buf, pad_before, pad_after);
            
            // 5. Try to decode the data
            vpx_codec_err_t decode_result = vpx_codec_decode(
                &decoder, data, size, nullptr, fuzz_int64_t_2);
            
            // 6. Get error details if decode failed
            if (decode_result != VPX_CODEC_OK) {
                const char* error_detail = vpx_codec_error_detail(&decoder);
                // We don't use error_detail, but we've called the API
            }
            
            // 7. Create an image and set rectangle
            vpx_image_t* image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                              stream_info.w > 0 ? stream_info.w : 64,
                                              stream_info.h > 0 ? stream_info.h : 64,
                                              fuzz_uint32_t_3);
            
            if (image) {
                // Set a rectangle on the image
                unsigned int rect_x = stream_info.w > 10 ? 5 : 0;
                unsigned int rect_y = stream_info.h > 10 ? 5 : 0;
                unsigned int rect_w = stream_info.w > 20 ? 10 : stream_info.w;
                unsigned int rect_h = stream_info.h > 20 ? 10 : stream_info.h;
                
                if (rect_w > 0 && rect_h > 0) {
                    vpx_img_set_rect(image, rect_x, rect_y, rect_w, rect_h);
                }
                
                vpx_img_free(image);
            }
            
            free(padded_data);
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    // Create output file for potential use
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // We could write something here, but for fuzzing we'll just close
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}