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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"vpx_codec_enc_config_set":[[354,12,354,55,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_dec_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_dec_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000021_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is called when a frame is decoded
    (void)user_priv;
    (void)img;
    // We could save frames here, but for fuzzing we just ignore
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use if needed
    
    // 3. Try to determine if this is VP8 or VP9 data
    // We'll try both codecs
    
    // Arrays of decoder interfaces to try
    vpx_codec_iface_t* decoder_interfaces[] = {
        vpx_codec_vp9_dx(),
        vpx_codec_vp8_dx()
    };
    
    // Buffers to read from the file
    uint8_t *buffer = nullptr;
    size_t buffer_size = 0;
    
    // 4. Read the entire input file into buffer
    fseek(in_file, 0, SEEK_END);
    buffer_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (buffer_size == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, buffer_size, in_file);
    if (bytes_read != buffer_size) {
        free(buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Try each decoder interface
    for (int i = 0; i < 2; i++) {
        vpx_codec_iface_t *iface = decoder_interfaces[i];
        
        // Get interface name (required API)
        const char *iface_name = vpx_codec_iface_name(iface);
        (void)iface_name; // Use if needed for debugging
        
        // Create encoder config for testing (required API)
        vpx_codec_enc_cfg_t enc_cfg;
        unsigned int usage = 0; // Good quality
        vpx_codec_err_t enc_config_result = vpx_codec_enc_config_default(iface, &enc_cfg, usage);
        (void)enc_config_result; // Result might fail for decoder iface, that's OK
        
        // Create codec context
        vpx_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));
        
        // Initialize decoder (required API)
        vpx_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 1;
        
        vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&codec, iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (init_result == VPX_CODEC_OK) {
            // Try to set encoder config (might fail, but we call the API)
            vpx_codec_err_t set_result = vpx_codec_enc_config_set(&codec, &enc_cfg);
            (void)set_result; // Expected to fail for decoder
            
            // Register put frame callback (required API)
            vpx_codec_err_t register_result = vpx_codec_register_put_frame_cb(&codec,  id_000021_put_frame_callback, nullptr);
            (void)register_result;
            
            // Try multi-encoder init (required API, will likely fail but we call it)
            vpx_codec_ctx_t multi_ctx;
            vpx_rational_t dsf = {1, 1};
            vpx_codec_err_t multi_result = vpx_codec_enc_init_multi_ver(&multi_ctx, iface, &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
            if (multi_result == VPX_CODEC_OK) {
                vpx_codec_destroy(&multi_ctx);
            }
            
            // Try to peek stream info (not in required list but useful)
            vpx_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            si.sz = sizeof(si);
            vpx_codec_peek_stream_info(iface, buffer, buffer_size, &si);
            
            // Decode the input data (required API)
            vpx_codec_err_t decode_result = vpx_codec_decode(&codec, buffer, buffer_size, nullptr, 0);
            
            // Get error string if needed (required API)
            if (decode_result != VPX_CODEC_OK) {
                const char *error_str = vpx_codec_err_to_string(decode_result);
                const char *ctx_error = vpx_codec_error(&codec);
                (void)error_str;
                (void)ctx_error;
            }
            
            // Get decoded frames (required API)
            vpx_codec_iter_t iter = nullptr;
            const vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
                // Frame available - we could process it here
                // For fuzzing, we just check it exists
                (void)img;
            }
            
            // Destroy codec context (required API)
            vpx_codec_err_t destroy_result = vpx_codec_destroy(&codec);
            (void)destroy_result;
        } else {
            // Get error information (required APIs)
            const char *error_str = vpx_codec_err_to_string(init_result);
            (void)error_str;
        }
    }
    
    // 6. Also try encoding path with VP9 encoder (using different interface)
    {
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx(); // Not in required list but needed for encoding
        
        if (encoder_iface) {
            // Create encoder config (required API)
            vpx_codec_enc_cfg_t cfg;
            unsigned int usage = 0;
            vpx_codec_err_t cfg_result = vpx_codec_enc_config_default(encoder_iface, &cfg, usage);
            
            if (cfg_result == VPX_CODEC_OK) {
                // Set basic config
                cfg.g_w = 640;
                cfg.g_h = 480;
                cfg.rc_target_bitrate = 1000;
                
                // Initialize encoder (multi-version, required API)
                vpx_codec_ctx_t encoder;
                vpx_rational_t dsf = {1, 1};
                vpx_codec_err_t init_result = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
                
                if (init_result == VPX_CODEC_OK) {
                    // Try to set config again (required API)
                    vpx_codec_err_t set_result = vpx_codec_enc_config_set(&encoder, &cfg);
                    (void)set_result;
                    
                    // Create a test image
                    vpx_image_t img;
                    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 640, 480, 1);
                    
                    if (img.planes[0]) {
                        // Fill with some data
                        memset(img.planes[0], 128, img.stride[0] * cfg.g_h);
                        if (img.planes[1]) memset(img.planes[1], 128, img.stride[1] * cfg.g_h / 2);
                        if (img.planes[2]) memset(img.planes[2], 128, img.stride[2] * cfg.g_h / 2);
                        
                        // Try to encode (not in required list but completes the cycle)
                        vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        // Get encoded data
                        vpx_codec_iter_t iter = nullptr;
                        const vpx_codec_cx_pkt_t *pkt;
                        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                            // Packet available
                            (void)pkt;
                        }
                        
                        vpx_img_free(&img);
                    }
                    
                    // Destroy encoder
                    vpx_codec_destroy(&encoder);
                }
            }
        }
    }
    
    // 7. Clean up
    free(buffer);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}