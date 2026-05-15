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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_error_detail","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_control","aom_img_wrap","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_control","aom_img_wrap","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Get build config string (API usage 1)
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning
    
    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init(&decoder, aom_codec_av1_dx(), &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        // Get error detail (API usage 4)
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        // Decode the data (API usage 7)
        aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        
        // Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        while (aom_image_t *img = aom_codec_get_frame(&decoder, &iter)) {
            // Get plane height (API usage 3)
            int plane_height = aom_img_plane_height(img, 0);
            (void)plane_height;
            
            // Create output file for encoder
            FILE *out_file = fopen("output_file", "wb");
            if (!out_file) continue;
            
            // Initialize encoder
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
            
            // Set image dimensions
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_error_resilient = 0;
            
            // Initialize encoder
            if (aom_codec_enc_init(&encoder, aom_codec_av1_cx(), &enc_cfg, 0) == AOM_CODEC_OK) {
                // Set encoder configuration (API usage 5)
                aom_codec_enc_config_set(&encoder, &enc_cfg);
                
                // Use control API (API usage 6)
                // Set CPU usage (lower = better quality but slower)
                aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
                
                // Create a wrapped image for encoding (API usage 1)
                // We'll create a new image with the same dimensions
                const size_t img_size = img->stride[0] * img->d_h * 3 / 2;
                std::vector<unsigned char> img_data(img_size);
                aom_image_t wrapped_img;
                aom_img_wrap(&wrapped_img, img->fmt, img->d_w, img->d_h, 
                           1, img_data.data());
                
                // Copy image data
                for (int plane = 0; plane < 3; ++plane) {
                    int plane_w = aom_img_plane_width(img, plane);
                    int plane_h = aom_img_plane_height(img, plane);
                    for (int y = 0; y < plane_h; ++y) {
                        memcpy(wrapped_img.planes[plane] + y * wrapped_img.stride[plane],
                               img->planes[plane] + y * img->stride[plane],
                               plane_w);
                    }
                }
                
                // Encode the image
                aom_codec_encode(&encoder, &wrapped_img, 0, 1, 0);
                
                // Get compressed data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up encoder
                aom_codec_destroy(&encoder);
            }
            
            assert_file_closed(&out_file);;
            break; // Process only first frame for fuzzing
        }
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}