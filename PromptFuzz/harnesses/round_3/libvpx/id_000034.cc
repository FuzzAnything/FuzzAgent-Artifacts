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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static void  id_000034_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is invoked when a frame is decoded
    (void)user_priv;
    (void)img;
}

static int  id_000034_get_frame_buffer_callback(void *cb_priv, size_t min_size, 
                                     vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1; // Indicate failure to allocate
}

static int  id_000034_release_frame_buffer_callback(void *cb_priv, 
                                         vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_err_t res;
    
    // Try to determine codec type from data
    // Simple heuristic: Check for VP8/VP9 start codes
    bool try_vp9 = false;
    if (size > 4) {
        // Check for VP9 superframe marker or keyframe header
        if ((data[0] & 0xE0) == 0xC0) { // VP9 superframe marker
            try_vp9 = true;
        }
    }
    
    // Select decoder interface
    decoder_iface = try_vp9 ? vpx_codec_vp9_dx() : vpx_codec_vp8_dx();
    
    // Use vpx_codec_iface_name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name;
    
    // Use vpx_codec_get_caps
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;
    
    // Initialize decoder
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                 fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Use vpx_codec_error
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_error(&decoder);
        (void)error_str;
        return 0;
    }
    
    // Use vpx_codec_register_put_frame_cb
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000034_put_frame_callback, nullptr);
    (void)res;
    
    // Use vpx_codec_set_frame_buffer_functions
    res = vpx_codec_set_frame_buffer_functions(&decoder, 
                                                id_000034_get_frame_buffer_callback,
                                                id_000034_release_frame_buffer_callback,
                                               nullptr);
    (void)res;
    
    // Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    
    // Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    while (vpx_codec_get_frame(&decoder, &iter)) {
        // Frames are handled in  id_000034_put_frame_callback
    }
    
    // Now try encoding with the same data
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *encoder_iface = try_vp9 ? vpx_codec_vp9_cx() : vpx_codec_vp8_cx();
    
    // Get default encoder configuration
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder configuration based on input size
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                 fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    
    // Use vpx_codec_enc_config_set
    if (res == VPX_CODEC_OK) {
        // Modify and set configuration again
        enc_cfg.rc_target_bitrate = 500;
        vpx_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Create output buffer for encoded data
        vpx_fixed_buf_t output_buf;
        output_buf.buf = malloc(4096);
        output_buf.sz = 4096;
        
        if (output_buf.buf) {
            // Use vpx_codec_set_cx_data_buf
            vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_5, fuzz_uint32_t_6);
            
            // Create a dummy image for encoding
            vpx_image_t img;
            vpx_img_alloc(&img, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_7);
            
            if (img.planes[0]) {
                // Fill with some data from input
                size_t copy_size = std::min(size, (size_t)(img.w * img.h * 3 / 2));
                uint8_t *img_data = reinterpret_cast<uint8_t*>(img.planes[0]);
                for (size_t i = 0; i < copy_size && i < 100; ++i) {
                    img_data[i] = data[i % size];
                }
                
                // Encode the image
                vpx_codec_encode(&encoder, &img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter))) {
                    // Process encoded packet
                    (void)pkt;
                }
                
                vpx_img_free(&img);
            }
            
            free(output_buf.buf);
        }
        
        vpx_codec_destroy(&encoder);
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}