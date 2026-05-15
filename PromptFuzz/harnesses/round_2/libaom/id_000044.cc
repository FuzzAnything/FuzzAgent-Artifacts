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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_error","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_get_metadata","aom_img_metadata_free","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_get_metadata","aom_img_metadata_free","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a comprehensive fuzz driver that uses the specified libaom APIs to decode and re-encode AV1 video data while handling metadata:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a file-like buffer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Check decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Configure decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        
        if (res != AOM_CODEC_OK) {
            // Check error details
            const char *error_detail = aom_codec_error_detail(&decoder);
            const char *error_msg = aom_codec_error(&decoder);
            (void)error_detail;
            (void)error_msg;
        } else {
            // Get decoded frames
            aom_codec_iter_t iter = nullptr;
            aom_image_t *img = nullptr;
            
            while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Get plane height (Y plane)
                int plane_height = aom_img_plane_height(img, 0);
                (void)plane_height;
                
                // Handle metadata
                size_t metadata_count = aom_img_num_metadata(img);
                
                // Create test metadata
                uint8_t test_metadata_data[] = {0x01, 0x02, 0x03, 0x04};
                aom_metadata_t *test_metadata = aom_img_metadata_alloc(
                    0x01, test_metadata_data, sizeof(test_metadata_data), 
                    AOM_MIF_ANY_FRAME);
                
                if (test_metadata) {
                    // Get metadata at index 0 if exists
                    if (metadata_count > 0) {
                        const aom_metadata_t *existing_metadata = 
                            aom_img_get_metadata(img, 0);
                        (void)existing_metadata;
                    }
                    
                    // Free test metadata
                    aom_img_metadata_free(test_metadata);
                }
                
                // Create image with border for encoding
                aom_image_t bordered_img;
                aom_image_t *allocated_img = aom_img_alloc_with_border(
                    &bordered_img, img->fmt, img->d_w, img->d_h, 
                    32, 32, 16);
                
                if (allocated_img) {
                    // Copy image data
                    for (int plane = 0; plane < 3; plane++) {
                        int width = aom_img_plane_width(img, plane);
                        int height = aom_img_plane_height(img, plane);
                        
                        if (width > 0 && height > 0) {
                            for (int y = 0; y < height; y++) {
                                memcpy(allocated_img->planes[plane] + y * allocated_img->stride[plane],
                                       img->planes[plane] + y * img->stride[plane],
                                       width);
                            }
                        }
                    }
                    
                    // Initialize encoder
                    aom_codec_ctx_t encoder;
                    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                    
                    // Get default encoder config
                    aom_codec_enc_cfg_t enc_cfg;
                    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
                    
                    if (res == AOM_CODEC_OK) {
                        // Configure encoder
                        enc_cfg.g_w = allocated_img->d_w;
                        enc_cfg.g_h = allocated_img->d_h;
                        enc_cfg.g_timebase.num = 1;
                        enc_cfg.g_timebase.den = 30;
                        enc_cfg.rc_target_bitrate = 1000;
                        enc_cfg.g_error_resilient = 0;
                        
                        // Initialize encoder
                        res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                                     0, AOM_ENCODER_ABI_VERSION);
                        
                        if (res == AOM_CODEC_OK) {
                            // Encode the image
                            res = aom_codec_encode(&encoder, allocated_img, 
                                                   0, 1, 0);
                            
                            if (res == AOM_CODEC_OK) {
                                // Get encoded data
                                aom_codec_iter_t enc_iter = nullptr;
                                const aom_codec_cx_pkt_t *pkt = nullptr;
                                
                                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                    // Process encoded packets
                                    (void)pkt;
                                }
                            }
                            
                            // Clean up encoder
                            aom_codec_destroy(&encoder);
                        }
                    }
                    
                    // Free allocated image
                    aom_img_free(allocated_img);
                }
            }
        }
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}