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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_peek_stream_info","vpx_codec_error","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_error","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_get_stream_info","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_error","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_get_stream_info","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read the input data into a buffer
    std::vector<uint8_t> input_buffer(size);
    size_t bytes_read = fread(input_buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != size) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream information
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    err = vpx_codec_peek_stream_info(decoder_iface, input_buffer.data(), input_buffer.size(), &stream_info);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the frame
    err = vpx_codec_decode(&decoder, input_buffer.data(), input_buffer.size(), nullptr, 0);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Set a rectangle on the image (using safe coordinates)
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int w = img->d_w > 10 ? 10 : img->d_w;
        unsigned int h = img->d_h > 10 ? 10 : img->d_h;
        
        int rect_result = vpx_img_set_rect(img, x, y, w, h);
        // Note: rect_result may fail for some images, but we still continue
    }

    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t enc_cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 640;
    enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 480;
    enc_cfg.rc_target_bitrate = 1000;
    
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(4096); // Allocate initial buffer
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        // Set output buffer for encoder
        err = vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
        if (err != VPX_CODEC_OK) {
            const char *error_msg = vpx_codec_error(&encoder);
        }
        
        // Try to encode if we have an image
        if (img) {
            err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
            if (err != VPX_CODEC_OK) {
                const char *error_msg = vpx_codec_error(&encoder);
            }
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                // Packet received, but we don't need to process it for fuzzing
            }
        }
        
        free(output_buf.buf);
    }

    // Get stream info from decoder context (alternative method)
    vpx_codec_stream_info_t stream_info2;
    stream_info2.sz = sizeof(stream_info2);
    err = vpx_codec_get_stream_info(&decoder, &stream_info2);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
    }

    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}