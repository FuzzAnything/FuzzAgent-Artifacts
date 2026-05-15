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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Print codec version
    const char *version_str = aom_codec_version_str();

    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Try to peek stream info from input data
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);

    // 4. Try to decode the input data
    if (aom_codec_decode(&decoder, data, size, nullptr) == AOM_CODEC_OK) {
        // 5. Get stream info after decoding
        aom_codec_get_stream_info(&decoder, &stream_info);

        // 6. Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_image = nullptr;
        
        while ((decoded_image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 7. Create an image for encoding
            aom_image_t *image_to_encode = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                                          decoded_image->d_w, 
                                                          decoded_image->d_h, 16);
            if (!image_to_encode) {
                break;
            }

            // 8. Copy decoded image data if sizes match
            if (image_to_encode->d_w == decoded_image->d_w && 
                image_to_encode->d_h == decoded_image->d_h) {
                for (int plane = 0; plane < 3; ++plane) {
                    int src_width = aom_img_plane_width(decoded_image, plane);
                    int src_height = aom_img_plane_height(decoded_image, plane);
                    int dst_width = aom_img_plane_width(image_to_encode, plane);
                    int dst_height = aom_img_plane_height(image_to_encode, plane);
                    
                    int copy_width = src_width < dst_width ? src_width : dst_width;
                    int copy_height = src_height < dst_height ? src_height : dst_height;
                    
                    for (int y = 0; y < copy_height; ++y) {
                        memcpy(image_to_encode->planes[plane] + y * image_to_encode->stride[plane],
                               decoded_image->planes[plane] + y * decoded_image->stride[plane],
                               copy_width);
                    }
                }
            }

            // 9. Add metadata to the image
            uint32_t metadata_type = 1;
            const uint8_t metadata_data[] = {0x01, 0x02, 0x03};
            aom_img_add_metadata(image_to_encode, metadata_type, metadata_data, 
                                sizeof(metadata_data), AOM_MIF_ANY_FRAME);

            // 10. Check number of metadata
            size_t num_metadata = aom_img_num_metadata(image_to_encode);

            // 11. Remove metadata
            aom_img_remove_metadata(image_to_encode);

            // 12. Initialize encoder
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                enc_cfg.g_w = image_to_encode->d_w;
                enc_cfg.g_h = image_to_encode->d_h;
                enc_cfg.g_threads = 1;
                enc_cfg.rc_target_bitrate = 1000;
                
                if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                          AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                    // 13. Encode the image
                    if (aom_codec_encode(&encoder, image_to_encode, 0, 1, 0) == AOM_CODEC_OK) {
                        // 14. Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t *pkt = nullptr;
                        
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            // Process encoded packets if needed
                        }
                    }
                    
                    // 15. Clean up encoder
                    aom_codec_destroy(&encoder);
                }
            }

            // 16. Clean up image
            aom_img_free(image_to_encode);
            
            // Break after processing first frame to avoid infinite loop
            break;
        }
    }

    // 17. Clean up decoder
    aom_codec_destroy(&decoder);

    return 0;
}