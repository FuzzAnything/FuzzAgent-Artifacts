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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_codec_err_to_string"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_codec_err_to_string"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if insufficient data
    if (size < 4) {
	return 0;
	}
	
    
    // Step 1: Decode ULEB value from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    
    // Step 2: Encode the ULEB value back
    uint8_t encoded_buffer[16];
    size_t encoded_size = 0;
    aom_uleb_encode(uleb_value, sizeof(encoded_buffer), encoded_buffer, &encoded_size);
    
    // Step 3: Setup encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    
    // Get default encoder configuration
    aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    // Initialize encoder
    aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // Step 4: Create test image
    aom_image_t img;
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 16;
    
    // Allocate image
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, align);
    
    // Fill image with some data (simplified)
    for (int plane = 0; plane < 3; plane++) {
        int plane_w = aom_img_plane_width(&img, plane);
        int plane_h = aom_img_plane_height(&img, plane);
        size_t plane_size = plane_w * plane_h;
        
        if (plane_size > 0) {
            // Use some input data for image content
            size_t offset = (plane * size) / 3;
            size_t copy_size = std::min(plane_size, size - offset);
            if (copy_size > 0 && offset < size) {
                memcpy(img.planes[plane], data + offset, copy_size);
            }
        }
    }
    
    // Step 5: Encode the image
    aom_codec_encode(&encoder, &img, 0, 1, 0);
    
    // Step 6: Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
    }
    
    // Step 7: Setup decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder;
    
    // Initialize decoder
    aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, AOM_DECODER_ABI_VERSION);
    
    // Step 8: Peek stream info from input data
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // Step 9: Decode input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // Step 10: Create and free metadata
    aom_metadata_t *metadata = aom_img_metadata_alloc(1, data, std::min(size, (size_t)16), AOM_MIF_ANY_FRAME);
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // Step 11: Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    // Step 12: Use error string function (for demonstration)
    const char *err_str = aom_codec_err_to_string(AOM_CODEC_OK);
    (void)err_str; // Prevent unused variable warning
    
    return 0;
}