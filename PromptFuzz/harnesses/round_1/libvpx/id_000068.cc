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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"vpx_codec_get_stream_info":[[89,22,89,62,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000068_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing, just to satisfy the API requirement
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t *allocated_img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_frame = nullptr;
    vpx_fixed_buf_t output_buf = {nullptr, 0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_caps_t caps = 0;
    
    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use to avoid unused variable warning

    // 1. Get codec interfaces
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // 2. Get interface names
    const char *dec_name = vpx_codec_iface_name(decoder_iface);
    const char *enc_name = vpx_codec_iface_name(encoder_iface);
    (void)dec_name;
    (void)enc_name;
    
    // 3. Get capabilities
    caps = vpx_codec_get_caps(decoder_iface);
    caps = vpx_codec_get_caps(encoder_iface);
    
    // 4. Peek stream info from input data
    // Use first 1KB or entire data, whichever is smaller
    size_t peek_size = size > 1024 ? 1024 : size;
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                         (unsigned int)peek_size, 
                                                         &stream_info);
    
    // 5. Initialize decoder
    if (peek_err == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
        dec_cfg.w = stream_info.w;
        dec_cfg.h = stream_info.h;
        dec_cfg.threads = 1;
    }
    
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                         &dec_cfg, 0, 
                                                         VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        // Try with NULL config
        dec_init_err = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                             nullptr, 0, 
                                             VPX_DECODER_ABI_VERSION);
    }
    
    if (dec_init_err == VPX_CODEC_OK) {
        // 6. Register put frame callback
        vpx_codec_register_put_frame_cb(&decoder,  id_000068_put_frame_callback, nullptr);
        
        // 7. Get stream info after initialization
        vpx_codec_get_stream_info(&decoder, &stream_info);
        
        // 8. Decode the input data
        vpx_codec_err_t decode_err = vpx_codec_decode(&decoder, data, 
                                                     (unsigned int)size, 
                                                     nullptr, 0);
        
        if (decode_err == VPX_CODEC_OK) {
            // 9. Get decoded frames
            while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // 10. Allocate image
                if (decoded_frame->w > 0 && decoded_frame->h > 0) {
                    allocated_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                                                 decoded_frame->w, 
                                                 decoded_frame->h, 
                                                 1);
                }
                
                // 11. Initialize encoder if we have a valid frame
                if (decoded_frame->w > 0 && decoded_frame->h > 0) {
                    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
                    enc_cfg.g_w = decoded_frame->w;
                    enc_cfg.g_h = decoded_frame->h;
                    enc_cfg.rc_target_bitrate = 256;
                    
                    vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(&encoder, 
                                                                         encoder_iface, 
                                                                         &enc_cfg, 
                                                                         0,
                                                                         VPX_ENCODER_ABI_VERSION);
                    
                    if (enc_init_err == VPX_CODEC_OK) {
                        // 12. Set output buffer
                        output_buf.sz = 1024 * 1024; // 1MB buffer
                        output_buf.buf = malloc(output_buf.sz);
                        if (output_buf.buf) {
                            vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                        }
                        
                        // 13. Encode the frame (simulate encoding)
                        vpx_codec_encode(&encoder, decoded_frame, 0, 1, 0, 0);
                        
                        // 14. Get preview frame
                        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                        (void)preview; // Use to avoid warning
                        
                        // 15. Clean up encoder
                        vpx_codec_destroy(&encoder);
                        
                        // Free output buffer
                        free(output_buf.buf);
                        output_buf.buf = nullptr;
                    }
                }
                
                // Free allocated image if created
                if (allocated_img) {
                    vpx_img_free(allocated_img);
                    allocated_img = nullptr;
                }
            }
        }
        
        // 16. Destroy decoder
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}