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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. ULEB decode/encode operations
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    uint8_t uleb_buffer[10] = {0};
    size_t coded_size = 0;
    
    // Try to decode a ULEB value from input
    aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    
    // Encode the decoded value back
    aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);

    // 2. Set up decoder for the input bitstream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Peek at stream info from input data
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Get error string
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Use error string to avoid unused variable warning
    }
    
    // Decode the input data
    aom_codec_decode(&decoder, data, size, NULL);
    
    // 3. Set up encoder to re-encode decoded frames
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    
    // Get default encoder config
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    
    // Initialize encoder
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 4. Create and use an image
    aom_image_t image;
    memset(&image, 0, sizeof(image));
    
    // Allocate image with reasonable dimensions
    aom_image_t *img_ptr = aom_img_alloc(&image, AOM_IMG_FMT_I420, 64, 64, 1);
    if (img_ptr) {
        // Get plane dimensions
        int plane_width = aom_img_plane_width(img_ptr, 0);
        int plane_height = aom_img_plane_height(img_ptr, 0);
        (void)plane_width; (void)plane_height; // Use to avoid warnings
        
        // Encode the image
        aom_codec_encode(&encoder, img_ptr, 0, 1, 0);
        
        // Get encoded data
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
            // Process packet if needed
        }
        
        // Create and free metadata
        aom_metadata_t *metadata = aom_img_metadata_alloc(1, data, size > 100 ? 100 : size, AOM_MIF_ANY_FRAME);
        if (metadata) {
            aom_img_metadata_free(metadata);
        }
        
        // Free the image
        aom_img_free(img_ptr);
    }
    
    // 5. Clean up
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    return 0;
}