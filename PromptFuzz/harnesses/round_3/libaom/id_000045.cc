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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_error_detail","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	

    // Open input as file stream for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get decoder capabilities first
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use variable to avoid unused warning
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Get error detail if initialization failed
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use variable
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get error detail if decoding failed
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use variable
    }
    
    // Get compressed data packets
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&decoder, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Got a frame packet
        }
    }
    
    // Get preview frame if available
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // Create a test image for additional operations
    aom_image_t *test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
    if (test_img) {
        // Set rectangle on the image (using some values from input data)
        unsigned int x = data[0] % 32;
        unsigned int y = data[1] % 32;
        unsigned int w = 8 + (data[2] % 16);
        unsigned int h = 8 + (data[3] % 16);
        aom_img_set_rect(test_img, x, y, w, h, 0);
        
        // Remove metadata if any
        aom_img_remove_metadata(test_img);
        
        aom_img_free(test_img);
    }
    
    // ULEB operations
    uint64_t uleb_value = 0;
    size_t decoded_length = 0;
    
    // Try to decode ULEB from the beginning of input
    if (size >= 5) {
        int decode_res = aom_uleb_decode(data, size, &uleb_value, &decoded_length);
        if (decode_res == 0) { // Success
            // Encode with fixed size
            uint8_t encoded_buf[10];
            size_t encoded_size = 0;
            size_t pad_to = 5; // Pad to 5 bytes
            
            int encode_res = aom_uleb_encode_fixed_size(uleb_value, sizeof(encoded_buf), 
                                                       pad_to, encoded_buf, &encoded_size);
            (void)encode_res; // Use variable
        }
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}