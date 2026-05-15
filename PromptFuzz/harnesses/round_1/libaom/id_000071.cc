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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_img_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_img_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Print version string
    const char *version = aom_codec_version_str();
    
    // 2. Decode ULEB value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 3. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // 4. Decode input data (skip ULEB portion)
    size_t decode_offset = (uleb_length < size) ? uleb_length : 0;
    size_t decode_size = (size > decode_offset) ? size - decode_offset : 0;
    
    if (decode_size > 0) {
        aom_codec_decode(&decoder, data + decode_offset, decode_size, nullptr);
        
        // 5. Get stream info
        aom_codec_stream_info_t stream_info;
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // 6. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = nullptr;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 7. Get image plane dimensions
            for (int plane = 0; plane < 3; plane++) {
                int width = aom_img_plane_width(img, plane);
                int height = aom_img_plane_height(img, plane);
            }
            
            // 8. Get metadata count
            size_t metadata_count = aom_img_num_metadata(img);
            
            // 9. Create encoder context for re-encoding
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            // 10. Allocate dummy image (to use aom_img_alloc)
            aom_image_t dummy_img;
            unsigned int align = 32;
            unsigned int width = 64;
            unsigned int height = 64;
            aom_img_alloc(&dummy_img, AOM_IMG_FMT_I420, width, height, align);
            
            // 11. Set encoder configuration
            aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            enc_cfg.g_w = width;
            enc_cfg.g_h = height;
            enc_cfg.rc_target_bitrate = 1000;
            
            aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
            
            // 12. Set encoder option
            aom_codec_set_option(&encoder, "cpu-used", "5");
            
            // 13. Prepare buffer for encoded data
            size_t buffer_size = 1024 * 1024;
            uint8_t *enc_buffer = (uint8_t*)malloc(buffer_size);
            aom_fixed_buf_t enc_buf = {enc_buffer, buffer_size};
            aom_codec_set_cx_data_buf(&encoder, &enc_buf, 0, 0);
            
            // 14. Encode the dummy image
            aom_codec_encode(&encoder, &dummy_img, 0, 1, 0);
            
            // 15. Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                // Process encoded packet if needed
            }
            
            // 16. Encode ULEB value
            uint8_t coded_value[10];
            size_t coded_size = 0;
            if (size > 0) {
                uint64_t value_to_encode = (uleb_value % 1000) + 1;
                size_t available = sizeof(coded_value);
                aom_uleb_encode(value_to_encode, available, coded_value, &coded_size);
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
            free(enc_buffer);
            aom_img_free(&dummy_img);
            
            // Break after processing first frame to limit execution time
            break;
        }
    }
    
    // 17. Clean up decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}