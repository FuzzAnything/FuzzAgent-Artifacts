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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Put frame callback function
static void  id_000009_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback receives decoded frames
    // For fuzzing, we'll just mark that it was called
    if (user_priv) {
        *reinterpret_cast<bool*>(user_priv) = true;
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // 1. Get extra version string (API 1)
    const char *extra_version = vpx_codec_version_extra_str();
    
    // 2. Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // 5. Read some data to determine codec type
    fseek(in_file, 0, SEEK_SET);
    uint8_t header[4];
    size_t read_bytes = fread(header, 1, 4, in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (read_bytes < 4) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Determine which codec interface to use based on input
    vpx_codec_iface_t *decoder_iface = nullptr;
    if (header[0] == 0x9d && header[1] == 0x01 && header[2] == 0x2a) {
        // VP8
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        // Try VP9 as default
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    // 7. Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 0, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 8. Register put frame callback (API 5)
    bool callback_called = false;
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000009_put_frame_callback, &callback_called);
    
    // 9. Peek stream info (API 4)
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    fseek(in_file, 0, SEEK_SET);
    uint8_t *file_data = new uint8_t[size];
    size_t data_read = fread(file_data, 1, size, in_file);
    
    res = vpx_codec_peek_stream_info(decoder_iface, file_data, 
                                     static_cast<unsigned int>(data_read), &si);
    
    // 10. Create encoder context to use other APIs
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (decoder_iface == vpx_codec_vp8_dx()) {
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    vpx_codec_enc_cfg_t enc_cfg;
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    if (res == VPX_CODEC_OK) {
        // Set reasonable values for fuzzing
        enc_cfg.g_w = si.w > 0 ? si.w : 320;
        enc_cfg.g_h = si.h > 0 ? si.h : 240;
        enc_cfg.g_threads = 1;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                     0, VPX_ENCODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            // 11. Get global headers (API 6)
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers && global_headers->buf && global_headers->sz > 0) {
                fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            }
            
            // 12. Try to get preview frame (API 3)
            const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
            
            // 13. Get error message (API 2)
            const char *error_msg = vpx_codec_error(&encoder);
            const char *decoder_error = vpx_codec_error(&decoder);
            
            // 14. Decode the input data
            vpx_codec_iter_t iter = nullptr;
            res = vpx_codec_decode(&decoder, file_data, 
                                  static_cast<unsigned int>(data_read), 
                                  nullptr, 0);
            
            // Get decoded frames
            const vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // We have a decoded frame
                // For fuzzing, just verify we got something
                if (img->w > 0 && img->h > 0) {
                    // Mark that we successfully decoded
                }
            }
            
            // Clean up encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 15. Clean up
    delete[] file_data;
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}