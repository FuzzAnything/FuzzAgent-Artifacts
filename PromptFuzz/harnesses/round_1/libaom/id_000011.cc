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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_wrap","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_control","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_error_detail","aom_codec_destroy","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_wrap","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_control","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_error_detail","aom_codec_destroy","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small to be meaningful
    if (size < 100) {
        return 0;
    }

    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    const aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    const aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    // 1. Get build configuration string (for logging/info)
    const char* build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning
    
    // 2. Try to decode the input data (assuming it's a compressed AV1 stream)
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        // Failed to initialize decoder, but continue with other operations
        const char* error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Prevent unused variable warning
    } else {
        // Try to decode the input data
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // Get decoded frame if available
        aom_codec_iter_t iter = nullptr;
        aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &iter);
        
        if (decoded_img) {
            // 3. Get plane height using decoded image
            int plane_height = aom_img_plane_height(decoded_img, 0);
            (void)plane_height; // Prevent unused variable warning
            
            // 4. Create a new image by wrapping existing data
            // Use a portion of input data as image data (simulated)
            size_t img_data_size = decoded_img->w * decoded_img->h * 3 / 2;
            if (img_data_size > 0 && img_data_size <= size) {
                unsigned char* img_data = (unsigned char*)data;
                
                // Wrap the input data as an image (simulating YUV I420 format)
                aom_img_wrap(&img, AOM_IMG_FMT_I420, 
                            decoded_img->w, decoded_img->h, 
                            1, img_data);
                
                // 5. Initialize encoder with default configuration
                if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) == AOM_CODEC_OK) {
                    cfg.g_w = decoded_img->w;
                    cfg.g_h = decoded_img->h;
                    cfg.g_threads = 1;
                    cfg.rc_target_bitrate = 1000;
                    cfg.g_error_resilient = 0;
                    
                    // Initialize encoder
                    if (aom_codec_enc_init(&encoder, encoder_iface, &cfg, 0) == AOM_CODEC_OK) {
                        
                        // 6. Set encoder configuration
                        aom_codec_enc_config_set(&encoder, &cfg);
                        
                        // 7. Set encoder controls using various control IDs
                        // Use some common control IDs
                        int controls[] = {
                            AOME_SET_CPUUSED,  // 1
                            AOME_SET_ENABLEAUTOALTREF,  // 2
                            AOME_SET_ARNR_MAXFRAMES,  // 3
                            AOME_SET_ARNR_STRENGTH,  // 4
                        };
                        
                        for (int i = 0; i < 4 && i * sizeof(uint32_t) < size; i++) {
                            int value = (data[i * sizeof(uint32_t)] % 10) + 1;
                            aom_codec_control(&encoder, controls[i], value);
                        }
                        
                        // Try to encode the wrapped image
                        aom_codec_encode(&encoder, &img, 0, 1, 0);
                        
                        // Clean up encoder
                        aom_codec_destroy(&encoder);
                    }
                }
                
                // Free the wrapped image (doesn't free the data pointer)
                aom_img_free(&img);
            }
            
            // 8. Get error detail from decoder (even if no error)
            const char* decoder_error_detail = aom_codec_error_detail(&decoder);
            (void)decoder_error_detail; // Prevent unused variable warning
        }
        
        // Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 9. Additional test: Try to decode a smaller chunk of data
    // Use the first portion of data to simulate a valid stream
    if (size > 50) {
        aom_codec_ctx_t test_decoder;
        if (aom_codec_dec_init(&test_decoder, decoder_iface, &dec_cfg, 0) == AOM_CODEC_OK) {
            size_t test_size = size > 1000 ? 1000 : size;
            aom_codec_decode(&test_decoder, data, test_size, nullptr);
            
            // Get error detail
            const char* test_error_detail = aom_codec_error_detail(&test_decoder);
            (void)test_error_detail; // Prevent unused variable warning
            
            aom_codec_destroy(&test_decoder);
        }
    }
    
    // 10. Test file operations as specified in instructions
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Prevent unused variable warning
        
        // Read a small amount to test
        uint8_t buffer[100];
        if (size > 100) {
            fread(buffer, 1, 100, in_file);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Create output file (as per instructions)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Prevent unused variable warning
        
        // Write something small
        const char* test_output = "test";
        fwrite(test_output, 1, strlen(test_output), out_file);
        
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}