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
//<score> 46, nr_unique_branch: 3
//<Quality> {"density":23,"unique_branches":{"aom_img_add_metadata":[[401,7,401,45,0,0,4,0],[402,8,402,45,0,0,4,1],[403,8,403,42,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy","aom_img_set_rect","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to process encoded AV1 video data, extract metadata, and manipulate image properties:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing

    // 1. Create encoder context and set options
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder with default config
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use aom_codec_set_option at least once
    aom_codec_set_option(&encoder_ctx, "cpu-used", "1");
    
    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder_ctx);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 2. Create decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // 3. Decode input data
    aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    // 4. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    if (!img) {
        aom_codec_destroy(&decoder_ctx);
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // 5. Use aom_img_set_rect to set a rectangle on the image
    // Use fuzzed data to determine rectangle parameters
    unsigned int x = data[0] % (img->d_w / 2);
    unsigned int y = data[1] % (img->d_h / 2);
    unsigned int w = 16 + (data[2] % 32);
    unsigned int h = 16 + (data[3] % 32);
    unsigned int border = data[4] % 8;
    
    aom_img_set_rect(img, x, y, w, h, border);
    
    // 6. Use aom_uleb_encode_fixed_size
    uint64_t uleb_value = 0;
    for (int i = 0; i < 8 && i < (int)size; i++) {
        uleb_value |= (uint64_t)data[i] << (i * 8);
    }
    
    uint8_t uleb_buffer[16];
    size_t coded_size = 0;
    size_t pad_to_size = 8 + (data[5] % 8);
    
    aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), pad_to_size, 
                               uleb_buffer, &coded_size);
    
    // 7. Add metadata to image using encoded ULEB data
    uint32_t metadata_type = 1 + (data[6] % 10);
    aom_metadata_insert_flags_t insert_flag = static_cast<aom_metadata_insert_flags_t>(
        data[7] % (AOM_MIF_ANY_FRAME_LAYER_SPECIFIC + 1));
    
    aom_img_add_metadata(img, metadata_type, uleb_buffer, 
                         coded_size, insert_flag);
    
    // 8. Get the metadata back
    size_t num_metadata = aom_img_num_metadata(img);
    if (num_metadata > 0) {
        const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
        // metadata can be used here if needed
    }
    
    // 9. Create a new image for encoding
    aom_image_t *new_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                         cfg.g_w, cfg.g_h, 1);
    if (new_img) {
        // Copy some data from original image if possible
        if (img->planes[0] && new_img->planes[0]) {
            size_t copy_size = std::min((size_t)img->stride[0] * img->d_h,
                                       (size_t)new_img->stride[0] * new_img->d_h);
            if (copy_size > 0 && copy_size <= size) {
                memcpy(new_img->planes[0], data, std::min(copy_size, size));
            }
        }
        
        // Encode the new image
        aom_codec_encode(&encoder_ctx, new_img, 0, 1, 0);
        
        // Get encoded data
        const aom_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
            // Process packet if needed
        }
        
        aom_img_free(new_img);
    }
    
    // 10. Clean up
    aom_codec_destroy(&decoder_ctx);
    aom_codec_destroy(&encoder_ctx);
    
    return 0;
}