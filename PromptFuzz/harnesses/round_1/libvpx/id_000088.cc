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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"encoder_set_config":[[875,7,875,26,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000088_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Do nothing in fuzzing - just consume the callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();  // Try VP9 first
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Register frame callback for decoder
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000088_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Consume error string
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Consume error string
    }
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder config
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder dimensions based on input size (capped for safety)
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        encoder_iface = vpx_codec_vp8_cx();
        err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            return 0;
        }
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            return 0;
        }
    }
    
    // Modify encoder configuration with data from input
    if (size >= 4) {
        // Use first 4 bytes to modify some encoder settings
        uint32_t mod_val = *(uint32_t*)data;
        enc_cfg.rc_target_bitrate = 500 + (mod_val % 1500);  // Range: 500-2000
        enc_cfg.rc_min_quantizer = mod_val % 52;
        enc_cfg.rc_max_quantizer = 20 + (mod_val % 32);
    }
    
    // Set the modified encoder configuration
    err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Consume error string
    }
    
    // Create a dummy image for encoding
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, 1);
    if (img) {
        // Fill image with some data
        size_t y_plane_size = 320 * 240;
        size_t uv_plane_size = 160 * 120;
        
        if (size >= y_plane_size + 2 * uv_plane_size) {
            // Use input data for image planes
            memcpy(img->planes[0], data, std::min(y_plane_size, size));
            if (size >= y_plane_size + uv_plane_size) {
                memcpy(img->planes[1], data + y_plane_size, std::min(uv_plane_size, size - y_plane_size));
            }
            if (size >= y_plane_size + 2 * uv_plane_size) {
                memcpy(img->planes[2], data + y_plane_size + uv_plane_size, 
                      std::min(uv_plane_size, size - y_plane_size - uv_plane_size));
            }
            
            // Try to encode the image
            err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
            if (err != VPX_CODEC_OK) {
                const char* err_str = vpx_codec_err_to_string(err);
                (void)err_str; // Consume error string
            }
            
            // Try to get preview frame from encoder
            const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder);
            (void)preview; // Consume preview frame
        }
        
        vpx_img_free(img);
    }
    
    // Create a FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Consume file descriptor
        
        // Try to read a small amount
        uint8_t buf[16];
        fread(buf, 1, sizeof(buf), in_file);
        assert_file_closed(&in_file);;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Consume file descriptor
        
        // Write something small
        const char test[] = "test";
        fwrite(test, 1, sizeof(test) - 1, out_file);
        assert_file_closed(&out_file);;
        
        // Remove the output file
        remove("output_file");
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}