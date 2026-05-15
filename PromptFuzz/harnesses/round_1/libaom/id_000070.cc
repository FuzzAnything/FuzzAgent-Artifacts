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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_img_free","aom_uleb_encode","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_img_free","aom_uleb_encode","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get version string
    const char *version = aom_codec_version_str();
    (void)version; // Prevent unused variable warning

    // 2. Decode a ULEB value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }

    // 3. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // 4. Decode the input data
    if (dec_init_result == AOM_CODEC_OK && size > 0) {
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // 5. Get stream information
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        aom_codec_get_stream_info(&decoder, &si);
        
        // 6. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_image = nullptr;
        
        while ((decoded_image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 7. Get image plane information
            for (int plane = 0; plane < 3; plane++) {
                int plane_w = aom_img_plane_width(decoded_image, plane);
                int plane_h = aom_img_plane_height(decoded_image, plane);
                (void)plane_w; // Prevent unused variable warning
                (void)plane_h; // Prevent unused variable warning
            }
            
            // 8. Get metadata count
            size_t metadata_count = aom_img_num_metadata(decoded_image);
            (void)metadata_count; // Prevent unused variable warning
        }
    }

    // 9. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 128;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        
        aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        // 10. Set encoder option
        aom_codec_set_option(&encoder, "cpu-used", "5");
        
        // 11. Set encoder configuration
        aom_codec_enc_config_set(&encoder, &enc_cfg);
        
        // 12. Allocate image for encoding
        aom_image_t *input_image = aom_img_alloc(
            nullptr, AOM_IMG_FMT_I420, 128, 128, 1);
        
        if (input_image) {
            // Fill image with some data (simplified - in real fuzzing would use input data)
            for (int plane = 0; plane < 3; plane++) {
                int stride = input_image->stride[plane];
                int height = aom_img_plane_height(input_image, plane);
                int width = aom_img_plane_width(input_image, plane);
                
                for (int y = 0; y < height; y++) {
                    memset(input_image->planes[plane] + y * stride, 
                           (plane == 0) ? 128 : 0, width);
                }
            }
            
            // 13. Encode the image
            aom_codec_encode(&encoder, input_image, 0, 1, 0);
            
            // 14. Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // 15. Set buffer for encoded data
                    aom_fixed_buf_t buf;
                    buf.buf = (void*)pkt->data.frame.buf;
                    buf.sz = pkt->data.frame.sz;
                    
                    // Pad 16 bytes before and after
                    aom_codec_set_cx_data_buf(&encoder, &buf, 16, 16);
                    break; // Just process first frame packet
                }
            }
            
            // Free input image
            aom_img_free(input_image);
        }
        
        // 16. Encode a ULEB value
        uint8_t coded_value[10];
        size_t coded_size = 0;
        uint64_t value_to_encode = uleb_value % 1000; // Use a reasonable value
        if (size >= 10) {
            aom_uleb_encode(value_to_encode, 10, coded_value, &coded_size);
        }
        
        // 17. Destroy encoder
        aom_codec_destroy(&encoder);
    }
    
    // 18. Destroy decoder
    if (dec_init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
    }
    
    return 0;
}