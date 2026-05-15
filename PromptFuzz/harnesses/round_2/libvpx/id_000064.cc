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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"validate_config":[[125,33,125,49,1,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_img_free","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_error","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_wrap","vpx_img_free","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_error","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that decodes and re-encodes VP8 video data using the specified libvpx APIs:

*/


// Include necessary headers for libvpx

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for any meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t encoder_config;
    vpx_image_t *img = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    // Open input data as a file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoder and encoder interfaces for VP8
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Get interface names
    const char *decoder_name = vpx_codec_iface_name(decoder_iface);
    const char *encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)decoder_name; (void)encoder_name; // Prevent unused warning
    
    // Get codec capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    (void)decoder_caps; (void)encoder_caps; // Prevent unused warning
    
    // Peek at stream information
    stream_info.sz = sizeof(stream_info);
    vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    
    // Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = 2;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error = vpx_codec_error(&decoder);
        (void)error; // Prevent unused warning
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder with default config
    if (vpx_codec_enc_config_default(encoder_iface, &encoder_config, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder config based on stream info
    encoder_config.g_w = stream_info.w;
    encoder_config.g_h = stream_info.h;
    encoder_config.g_timebase.num = 1;
    encoder_config.g_timebase.den = 30;
    encoder_config.rc_target_bitrate = 256; // 256 kbps
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_config, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error = vpx_codec_error(&encoder);
        (void)error; // Prevent unused warning
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate image for decoded frames
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, stream_info.w, stream_info.h, 1);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Also test vpx_img_wrap
    unsigned char *img_data = (unsigned char *)malloc(img->w * img->h * 3 / 2);
    vpx_image_t *wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, img->w, img->h, 1, img_data);
    if (wrapped_img) {
        vpx_img_free(wrapped_img);
    }
    free(img_data);
    
    // Set a rectangle in the image
    vpx_img_set_rect(img, 0, 0, stream_info.w, stream_info.h);
    
    // Create a callback for put_slice (simulated)
    auto put_slice_cb = [](void *user_priv, const vpx_image_t *img, 
                          const vpx_image_rect_t *valid, const vpx_image_rect_t *update) {
        // Empty callback for demonstration
    };
    
    // Register callback (though we may not use it in practice)
    vpx_codec_register_put_slice_cb(&encoder, put_slice_cb, nullptr);
    
    // Create buffer for encoded data
    vpx_fixed_buf_t output_buffer;
    output_buffer.buf = malloc(1024 * 1024); // 1MB buffer
    output_buffer.sz = 1024 * 1024;
    
    // Set output buffer for encoder
    vpx_codec_set_cx_data_buf(&encoder, &output_buffer, 0, 0);
    
    // Read data from input file and decode
    uint8_t *input_buffer = (uint8_t *)malloc(size);
    fread(input_buffer, 1, size, in_file);
    
    // Decode the frame
    if (vpx_codec_decode(&decoder, input_buffer, (unsigned int)size, nullptr, 0) == VPX_CODEC_OK) {
        // Get the decoded frame
        vpx_codec_iter_t dec_iter = nullptr;
        vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &dec_iter);
        
        if (decoded_img) {
            // Copy decoded image to our image buffer
            for (int plane = 0; plane < 3; ++plane) {
                int plane_width = (plane > 0) ? (img->w + 1) / 2 : img->w;
                int plane_height = (plane > 0) ? (img->h + 1) / 2 : img->h;
                
                for (int y = 0; y < plane_height; ++y) {
                    memcpy(img->planes[plane] + y * img->stride[plane],
                           decoded_img->planes[plane] + y * decoded_img->stride[plane],
                           plane_width);
                }
            }
            
            // Flip image for testing
            vpx_img_flip(img);
            
            // Encode the frame
            if (vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
                // Get encoded data packets
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Test codec control (get preview frame)
                vpx_codec_control_(&encoder, VP8_COPY_REFERENCE, nullptr);
                
                // Get encoder error
                const char *encoder_error = vpx_codec_error(&encoder);
                (void)encoder_error; // Prevent unused warning
            }
        }
    }
    
    // Get decoder error
    const char *decoder_error = vpx_codec_error(&decoder);
    (void)decoder_error; // Prevent unused warning
    
    // Cleanup
    free(input_buffer);
    free(output_buffer.buf);
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}