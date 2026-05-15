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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_get_frame","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_get_frame","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve a video transcoding flow with frame processing:

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Need minimum data for headers

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors (not strictly needed but following instructions)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused warning
    (void)out_fd; // Suppress unused warning

    // Read data from file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *input_buffer = (uint8_t*)malloc(file_size);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    fread(input_buffer, 1, file_size, in_file);

    // 3. Use vpx_codec_get_caps to check codec capabilities
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Check caps but may not use all features

    // 4. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        free(input_buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 5. Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(&decoder, input_buffer, file_size, NULL, 0);
    
    // 6. Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get stream info to configure encoder properly
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    if (vpx_codec_peek_stream_info(decoder_iface, input_buffer, file_size, &si) == VPX_CODEC_OK) {
        // 7. Setup encoder configuration
        vpx_codec_enc_cfg_t enc_cfg;
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        enc_cfg.g_w = si.w;
        enc_cfg.g_h = si.h;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // 8. Set encoder configuration (required API)
            vpx_codec_enc_config_set(&encoder, &enc_cfg);
            
            // 9. Register put slice callback (required API)
            auto put_slice_cb = [](void *user_priv, const vpx_image_t *img,
                                  const vpx_image_rect_t *rect, 
                                  const vpx_image_rect_t *rect2) {
                // Callback implementation
                (void)user_priv;
                (void)img;
                (void)rect;
                (void)rect2;
            };
            vpx_codec_register_put_slice_cb(&encoder, put_slice_cb, NULL);
            
            // Get decoded frames
            vpx_codec_iter_t iter = NULL;
            const vpx_image_t *decoded_img;
            while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
                // 10. Create a copy of the image to flip
                vpx_image_t *img_copy = vpx_img_alloc(NULL, decoded_img->fmt, 
                                                     decoded_img->d_w, decoded_img->d_h, 16);
                if (img_copy) {
                    // Copy image data
                    for (int plane = 0; plane < 3; ++plane) {
                        uint8_t *src = decoded_img->planes[plane];
                        uint8_t *dst = img_copy->planes[plane];
                        int stride = decoded_img->stride[plane];
                        int height = (plane == 0) ? decoded_img->d_h : (decoded_img->d_h + 1) >> 1;
                        
                        for (int y = 0; y < height; ++y) {
                            memcpy(dst, src, stride);
                            src += stride;
                            dst += stride;
                        }
                    }
                    
                    // 11. Flip the image (required API)
                    vpx_img_flip(img_copy);
                    
                    // 12. Encode the flipped image (required API)
                    vpx_codec_encode(&encoder, img_copy, 0, 1, 0, VPX_DL_REALTIME);
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = NULL;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    vpx_img_free(img_copy);
                }
            }
            
            // Flush encoder
            vpx_codec_encode(&encoder, NULL, 0, 1, 0, VPX_DL_REALTIME);
            
            vpx_codec_destroy(&encoder);
        }
    }

    // 13. Cleanup
    vpx_codec_destroy(&decoder);
    free(input_buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}