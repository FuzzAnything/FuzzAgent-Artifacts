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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"aom_codec_enc_init_ver":[[63,30,63,34,0,0,4,0]]},"library_calls":["aom_uleb_encode_fixed_size","aom_img_wrap","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_img_remove_metadata","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_destroy"],"critical_calls":["aom_uleb_encode_fixed_size","aom_img_wrap","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_img_remove_metadata","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing

    // Create a FILE* for input data
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

    // Initialize variables
    aom_codec_ctx_t codec_ctx;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    const aom_image_t *preview_frame = nullptr;
    aom_image_t wrapped_img;
    uint8_t *img_data = nullptr;
    uint8_t coded_value[32];
    size_t coded_size = 0;
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int ret = 0;

    // 1. First use aom_uleb_encode_fixed_size with some data
    if (size >= 8) {
        memcpy(&uleb_value, data, sizeof(uint64_t));
        ret = aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_value), 10, 
                                         coded_value, &coded_size);
        if (ret == 0) {
            fwrite(coded_value, 1, coded_size, out_file);
        }
    }

    // 2. Use aom_img_wrap to create an image
    // Allocate image data buffer
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int stride_align = 32;
    size_t img_data_size = width * height * 3;  // Rough estimate for YUV
    img_data = (unsigned char *)malloc(img_data_size);
    if (img_data) {
        // Fill with some data from input
        size_t copy_size = img_data_size < size ? img_data_size : size;
        memcpy(img_data, data, copy_size);
        
        aom_image_t *img = aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 
                                        width, height, stride_align, img_data);
        if (img) {
            // 3. Use aom_codec_set_option
            aom_codec_iface_t *iface = aom_codec_av1_cx();
            if (aom_codec_enc_init(&codec_ctx, iface, nullptr, 0) == AOM_CODEC_OK) {
                // Set some options using data from input
                char option_name[32];
                char option_value[32];
                
                // Use some bytes for option name and value
                size_t name_len = size > 20 ? 20 : size;
                memcpy(option_name, data, name_len);
                option_name[name_len] = '\0';
                
                memcpy(option_value, data + name_len, 
                       size - name_len > 31 ? 31 : size - name_len);
                option_value[31] = '\0';
                
                aom_codec_set_option(&codec_ctx, option_name, option_value);
                
                // 4. Try to encode and get cx data
                if (aom_codec_encode(&codec_ctx, img, 0, 1, 0) == AOM_CODEC_OK) {
                    while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
                        // Process the packet
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                }
                
                // 5. Try to get preview frame (for encoder context)
                preview_frame = aom_codec_get_preview_frame(&codec_ctx);
                
                // 6. Use aom_obu_type_to_string
                // Try different OBU types based on input data
                for (int i = 0; i < 8 && i < (int)size; i++) {
                    OBU_TYPE obu_type = static_cast<OBU_TYPE>(data[i] % 10);
                    const char *obu_str = aom_obu_type_to_string(obu_type);
                    if (obu_str) {
                        // Do something with the string
                        fwrite(obu_str, 1, strlen(obu_str), out_file);
                    }
                }
                
                // 7. Remove metadata from image
                aom_img_remove_metadata(&wrapped_img);
                
                aom_codec_destroy(&codec_ctx);
            }
        }
    }

    // 8. Also try decoding path
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (aom_codec_dec_init(&dec_ctx, dec_iface, nullptr, 0) == AOM_CODEC_OK) {
        // Try to decode the input data
        aom_codec_decode(&dec_ctx, data, size, nullptr);
        
        // Try to get preview frame (for decoder context)
        preview_frame = aom_codec_get_preview_frame(&dec_ctx);
        
        aom_codec_destroy(&dec_ctx);
    }

    // Clean up
    if (img_data) {
        free(img_data);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}