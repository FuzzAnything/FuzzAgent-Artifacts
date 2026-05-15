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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice (required by vpx_codec_register_put_slice_cb)
static void  id_000002_put_slice_callback(void *user_priv,
                              const vpx_image_t *img,
                              const vpx_image_rect_t *valid,
                              const vpx_image_rect_t *update) {
    // This callback can be left empty for fuzzing purposes
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE pointer for the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. First, get the build configuration string
    const char *build_config = vpx_codec_build_config();
    (void)build_config; // May be unused in fuzzing, but API is called

    // 2. Try to peek stream info to determine if this is VP8 or VP9 data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);

    vpx_codec_err_t peek_result = VPX_CODEC_UNSUP_BITSTREAM;
    vpx_codec_iface_t *decoder_iface = nullptr;
    
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    peek_result = vpx_codec_peek_stream_info(decoder_iface, data, 
                                            (unsigned int)(size > 1000 ? 1000 : size), 
                                            &stream_info);
    
    // If not VP9, try VP8
    if (peek_result != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        peek_result = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                (unsigned int)(size > 1000 ? 1000 : size), 
                                                &stream_info);
    }

    // 3. Set up encoder configuration using default settings
    // We'll use VP9 encoder interface since we need to call vpx_codec_enc_config_default
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t encoder_config;
    memset(&encoder_config, 0, sizeof(encoder_config));
    
    // Get default encoder configuration
    vpx_codec_err_t config_result = vpx_codec_enc_config_default(encoder_iface, 
                                                                &encoder_config, 
                                                                0); // Usage: 0 for good quality
    
    if (config_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Initialize encoder context with the default configuration
    vpx_codec_ctx_t encoder_context;
    memset(&encoder_context, 0, sizeof(encoder_context));
    
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&encoder_context, 
                                                         encoder_iface, 
                                                         &encoder_config, 
                                                         0, // flags
                                                         VPX_ENCODER_ABI_VERSION);
    
    // 5. Register put_slice callback with the encoder context
    if (init_result == VPX_CODEC_OK) {
        vpx_codec_err_t cb_result = vpx_codec_register_put_slice_cb(&encoder_context, 
                                                                    id_000002_put_slice_callback, 
                                                                   nullptr);
        (void)cb_result; // Result may be ignored for fuzzing
    }

    // Clean up encoder context if it was initialized
    if (init_result == VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_context);
    }

    // 6. Also try to decode the input data if we successfully peeked stream info
    if (peek_result == VPX_CODEC_OK && decoder_iface) {
        vpx_codec_ctx_t decoder_context;
        memset(&decoder_context, 0, sizeof(decoder_context));
        
        // Initialize decoder
        vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(&decoder_context, 
                                                                decoder_iface, 
                                                                nullptr, // default config
                                                                0, // flags
                                                                VPX_DECODER_ABI_VERSION);
        
        if (dec_init_result == VPX_CODEC_OK) {
            // Try to decode the input data
            vpx_codec_err_t decode_result = vpx_codec_decode(&decoder_context, 
                                                            data, 
                                                            (unsigned int)size, 
                                                            nullptr, // user_priv
                                                            0); // deadline
            
            // If decode was successful, try to get a frame
            if (decode_result == VPX_CODEC_OK) {
                vpx_codec_iter_t iter = nullptr;
                vpx_image_t *img = vpx_codec_get_frame(&decoder_context, &iter);
                (void)img; // May be unused
            }
            
            // Clean up decoder context
            vpx_codec_destroy(&decoder_context);
        }
    }

    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}