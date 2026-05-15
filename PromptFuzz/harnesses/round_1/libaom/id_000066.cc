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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"iadst16x16_low1_sse4_1":[[35,23,35,34,1,0,4,0]]},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_caps","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_caps","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_caps","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_caps","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Print codec version
    const char *version = aom_codec_version_str();
    
    // 2. Try to decode ULEB value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size >= 1) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 3. Initialize AV1 decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    
    // 4. Decode input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 5. Get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&decoder, &si);
    
    // 6. Get decoder capabilities
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    
    // 7. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    int frame_count = 0;
    
    while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr && frame_count < 3) {
        // Use image plane functions
        int plane_width = aom_img_plane_width(decoded_img, 0);
        int plane_height = aom_img_plane_height(decoded_img, 0);
        size_t metadata_count = aom_img_num_metadata(decoded_img);
        frame_count++;
        
        // 8. Initialize AV1 encoder with same dimensions
        aom_codec_ctx_t encoder;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t enc_cfg;
        
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err == AOM_CODEC_OK) {
            enc_cfg.g_w = decoded_img->d_w;
            enc_cfg.g_h = decoded_img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.rc_end_usage = AOM_Q;
            
            // 9. Set encoder option
            err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
            if (err == AOM_CODEC_OK) {
                aom_codec_set_option(&encoder, "cpu-used", "8");
                
                // Get encoder capabilities
                aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
                
                // 10. Allocate an image for encoding (demonstrating aom_img_alloc)
                aom_image_t *img_to_encode = aom_img_alloc(nullptr, decoded_img->fmt, 
                                                          decoded_img->d_w, decoded_img->d_h, 1);
                if (img_to_encode) {
                    // Copy plane data from decoded image
                    for (int plane = 0; plane < 3; plane++) {
                        int width = aom_img_plane_width(img_to_encode, plane);
                        int height = aom_img_plane_height(img_to_encode, plane);
                        if (plane < 3 && decoded_img->planes[plane] && img_to_encode->planes[plane]) {
                            size_t copy_size = width * height;
                            if (copy_size > 0 && decoded_img->stride[plane] >= width) {
                                for (int y = 0; y < height; y++) {
                                    memcpy(img_to_encode->planes[plane] + y * img_to_encode->stride[plane],
                                           decoded_img->planes[plane] + y * decoded_img->stride[plane],
                                           width);
                                }
                            }
                        }
                    }
                    
                    // 11. Encode the frame
                    aom_codec_encode(&encoder, img_to_encode, 0, 1, 0);
                    
                    // 12. Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    FILE *out_file = fopen("output_file", "wb");
                    
                    if (out_file) {
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                        assert_file_closed(&out_file);;
                    }
                    
                    // 13. Free allocated image
                    aom_img_free(img_to_encode);
                }
                
                // 14. Destroy encoder
                aom_codec_destroy(&encoder);
            }
        }
        
        // Break after processing first frame for efficiency
        break;
    }
    
    // 15. Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}