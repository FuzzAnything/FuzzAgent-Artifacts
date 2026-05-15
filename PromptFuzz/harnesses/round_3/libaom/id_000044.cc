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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_get_cx_data","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_destroy","aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_get_cx_data","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_destroy","aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Get codec capabilities first
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    
    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // 3. Decode input data
    if (err == AOM_CODEC_OK) {
        err = aom_codec_decode(&decoder, data, size, nullptr);
        
        // Check for error details
        const char *error_detail = aom_codec_error_detail(&decoder);
        
        // 4. Get preview frame
        aom_codec_iter_t iter = nullptr;
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
        
        if (preview_frame) {
            // Create a mutable copy of the image to work with
            aom_image_t img_copy;
            if (aom_img_alloc(&img_copy, preview_frame->fmt, 
                             preview_frame->d_w, preview_frame->d_h, 1)) {
                
                // 5. Copy image data
                for (int plane = 0; plane < 3; plane++) {
                    int plane_width = aom_img_plane_width(preview_frame, plane);
                    int plane_height = aom_img_plane_height(preview_frame, plane);
                    
                    for (int y = 0; y < plane_height; y++) {
                        memcpy(img_copy.planes[plane] + y * img_copy.stride[plane],
                               preview_frame->planes[plane] + y * preview_frame->stride[plane],
                               plane_width);
                    }
                }
                
                // 6. Set rectangle on image (within bounds)
                unsigned int rect_w = img_copy.d_w > 10 ? 10 : img_copy.d_w / 2;
                unsigned int rect_h = img_copy.d_h > 10 ? 10 : img_copy.d_h / 2;
                aom_img_set_rect(&img_copy, 0, 0, rect_w, rect_h, 1);
                
                // 7. Remove metadata
                aom_img_remove_metadata(&img_copy);
                
                aom_img_free(&img_copy);
            }
        }
        
        // 8. Get encoded data packets
        iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&decoder, &iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Write packet data to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                
                // 9. Test ULEB encoding/decoding with packet size
                uint64_t packet_size = pkt->data.frame.sz;
                uint8_t encoded[10];
                size_t encoded_size = 0;
                
                // Encode with fixed size
                if (aom_uleb_encode_fixed_size(packet_size, sizeof(encoded), 
                                               5, encoded, &encoded_size) == 0) {
                    
                    // Decode back
                    uint64_t decoded_value;
                    size_t decoded_length;
                    if (aom_uleb_decode(encoded, encoded_size, 
                                        &decoded_value, &decoded_length) == 0) {
                        // ULEB round-trip successful
                    }
                }
            }
        }
        
        // 10. Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // Test more ULEB operations with fuzzing data
    if (size > 20) {
        uint64_t test_value = 0;
        size_t decoded_len = 0;
        
        // Decode a ULEB value from input
        if (aom_uleb_decode(data, size > 100 ? 100 : size, 
                           &test_value, &decoded_len) == 0) {
            
            uint8_t encoded_buffer[20];
            size_t encoded_size = 0;
            
            // Encode it back
            aom_uleb_encode(test_value, sizeof(encoded_buffer), 
                           encoded_buffer, &encoded_size);
            
            // Try fixed size encoding
            if (encoded_size > 0) {
                uint8_t fixed_encoded[20];
                size_t fixed_size = 0;
                aom_uleb_encode_fixed_size(test_value, sizeof(fixed_encoded),
                                           encoded_size + 2, fixed_encoded, &fixed_size);
            }
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}