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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_decode","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_control","aom_codec_control","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_size_in_bytes","aom_uleb_encode_fixed_size"],"critical_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_decode","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_control","aom_codec_control","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_obu_type_to_string","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_size_in_bytes","aom_uleb_encode_fixed_size"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get codec version information
    int version = aom_codec_version();
    const char *version_str = aom_codec_version_str();
    
    // Use version info to avoid unused variable warnings
    (void)version;
    (void)version_str;
    
    // 2. Process input data as ULEB encoded values
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    
    if (size > 0) {
        // Try to decode ULEB from input data
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
        
        // Calculate ULEB size
        size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
        
        // Encode the ULEB value back
        size_t encoded_size = 0;
        uint8_t encoded_buffer[32] = {0};
        if (uleb_size <= sizeof(encoded_buffer)) {
            aom_uleb_encode(uleb_value, sizeof(encoded_buffer), 
                           encoded_buffer, &encoded_size);
        }
    }
    
    // 3. Create a decoder context for AV1
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) == AOM_CODEC_OK) {
        // 4. Use codec control API to set/get decoder properties
        // Try to get stream info from input data
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        
        // Try to peek at stream info
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
        
        // Use codec control with various control IDs
        // Note: We use some common control IDs for demonstration
        // In real usage, you'd need to know specific control IDs
        
        // Control ID 0 (dummy) - just to demonstrate usage
        aom_codec_control(&decoder, 0);
        
        // Control ID 1 (another dummy)
        aom_codec_control(&decoder, 1);
        
        // 5. Convert OBU types to strings for common types
        // Try different OBU types
        const char *obu_str1 = aom_obu_type_to_string(OBU_SEQUENCE_HEADER);
        const char *obu_str2 = aom_obu_type_to_string(OBU_TEMPORAL_DELIMITER);
        const char *obu_str3 = aom_obu_type_to_string(OBU_FRAME_HEADER);
        const char *obu_str4 = aom_obu_type_to_string(OBU_TILE_GROUP);
        const char *obu_str5 = aom_obu_type_to_string(OBU_METADATA);
        
        // Use OBU strings to avoid unused variable warnings
        (void)obu_str1;
        (void)obu_str2;
        (void)obu_str3;
        (void)obu_str4;
        (void)obu_str5;
        
        // 6. Try to decode the input data if it looks valid
        if (size > 0) {
            aom_codec_decode(&decoder, data, size, NULL);
            
            // Try to get decoded frames
            aom_codec_iter_t iter = NULL;
            aom_image_t *img;
            while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
                // Process image if needed
                (void)img;
            }
        }
        
        // 7. Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 8. Create an encoder context as well
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Set minimal configuration for fuzzing
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 100;
        
        if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
            // Use codec control on encoder
            aom_codec_control(&encoder, 0);  // Dummy control
            
            // Create a test image for encoding
            aom_image_t test_image;
            if (aom_img_alloc(&test_image, AOM_IMG_FMT_I420, 64, 64, 1)) {
                // Fill with some data
                for (int plane = 0; plane < 3; ++plane) {
                    uint8_t *plane_ptr = test_image.planes[plane];
                    int stride = test_image.stride[plane];
                    int height = aom_img_plane_height(&test_image, plane);
                    
                    for (int y = 0; y < height; ++y) {
                        memset(plane_ptr + y * stride, 128, 
                               aom_img_plane_width(&test_image, plane));
                    }
                }
                
                // Try to encode the image
                aom_codec_encode(&encoder, &test_image, 0, 1, 0);
                
                // Get encoded data
                aom_codec_iter_t iter = NULL;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
                    // Process encoded packet if needed
                    (void)pkt;
                }
                
                // Free the test image
                aom_img_free(&test_image);
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 9. Additional ULEB encoding test with input data
    if (size > 0) {
        // Use the first 8 bytes of input as a 64-bit value if available
        uint64_t test_value = 0;
        if (size >= 8) {
            memcpy(&test_value, data, 8);
        } else {
            // Use size as test value
            test_value = size;
        }
        
        // Calculate size needed
        size_t needed_size = aom_uleb_size_in_bytes(test_value);
        
        // Try to encode with fixed size
        if (needed_size <= 32) {
            uint8_t fixed_buffer[32] = {0};
            size_t fixed_coded_size = 0;
            
            aom_uleb_encode_fixed_size(test_value, sizeof(fixed_buffer),
                                      needed_size, fixed_buffer, &fixed_coded_size);
        }
    }
    
    return 0;
}