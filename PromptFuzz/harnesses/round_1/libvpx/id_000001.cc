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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000001_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This is just a fuzzing callback - we don't need to do anything meaningful
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Get decoder interface for VP9
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get encoder interface for VP8 (for encoding later if needed)
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get capabilities of the decoder
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Mark as used

    // Step 4: Peek stream info to get basic information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)(size > 1000 ? 1000 : size), &stream_info);
    
    // Step 5: Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Register put frame callback
    vpx_codec_err_t register_result = vpx_codec_register_put_frame_cb(
        &decoder,  id_000001_put_frame_callback, NULL);
    
    // Step 7: Allocate an image using vpx_img_alloc
    // Use dimensions from stream info if available, otherwise use defaults
    unsigned int width = stream_info.w > 0 ? stream_info.w : 320;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 240;
    
    vpx_image_t* image = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 16);
    if (!image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 8: Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, data, (unsigned int)size, NULL, 0);

    // Step 9: Get stream info from decoder context
    vpx_codec_stream_info_t decoder_stream_info;
    memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
    decoder_stream_info.sz = sizeof(decoder_stream_info);
    
    vpx_codec_err_t get_info_result = vpx_codec_get_stream_info(&decoder, &decoder_stream_info);

    // Step 10: Set up output buffer for encoding
    // We'll allocate a small buffer for demonstration
    size_t output_buffer_size = 1024 * 1024; // 1MB
    uint8_t* output_buffer = (uint8_t*)malloc(output_buffer_size);
    if (output_buffer) {
        vpx_fixed_buf_t output_buf;
        output_buf.buf = output_buffer;
        output_buf.sz = output_buffer_size;
        
        // Note: vpx_codec_set_cx_data_buf is typically used with encoder context,
        // but we're demonstrating its usage here
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_init_ver(&encoder, encoder_iface, NULL, 0, VPX_ENCODER_ABI_VERSION);
        
        if (encoder.err == VPX_CODEC_OK) {
            vpx_codec_err_t set_buf_result = vpx_codec_set_cx_data_buf(
                &encoder, &output_buf, 0, 0);
            (void)set_buf_result; // Mark as used
            
            vpx_codec_destroy(&encoder);
        }
        
        free(output_buffer);
    }

    // Step 11: Try to get decoded frames
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t* decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // We have a decoded frame - for fuzzing, we just want to exercise the API
        (void)decoded_img;
    }

    // Clean up
    vpx_img_free(image);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}