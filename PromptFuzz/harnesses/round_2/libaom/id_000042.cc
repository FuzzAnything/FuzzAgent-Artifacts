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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create a FILE* from the input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Use aom_codec_err_to_string
        const char* err_str = aom_codec_err_to_string(err);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Read encoded data from input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    std::vector<uint8_t> encoded_data(file_size);
    fread(encoded_data.data(), 1, file_size, in_file);

    // Step 3: Decode the data
    err = aom_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), nullptr);
    if (err != AOM_CODEC_OK) {
        // Use aom_codec_error_detail
        const char* error_detail = aom_codec_error_detail(&decoder);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img = nullptr;
    int frame_count = 0;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_count++;
        
        // Step 5: Allocate new image with border using aom_img_alloc_with_border
        aom_image_t* img_with_border = aom_img_alloc_with_border(nullptr, img->fmt, 
                                                                img->d_w, img->d_h,
                                                                1, 1, 16);
        if (!img_with_border) {
            continue;
        }

        // Step 6: Create metadata using aom_img_metadata_alloc
        uint32_t metadata_type = 1;
        std::vector<uint8_t> metadata_data = {0x01, 0x02, 0x03, 0x04};
        aom_metadata_t* metadata = aom_img_metadata_alloc(metadata_type, 
                                                         metadata_data.data(),
                                                         metadata_data.size(),
                                                         AOM_MIF_ANY_FRAME);
        
        if (metadata) {
            // Step 7: Add metadata to image using aom_img_add_metadata
            int add_result = aom_img_add_metadata(img_with_border, 
                                                 metadata->type,
                                                 metadata->payload,
                                                 metadata->sz,
                                                 metadata->insert_flag);
            
            // Step 8: Get number of metadata using aom_img_num_metadata
            size_t num_metadata = aom_img_num_metadata(img_with_border);
            
            // Step 9: Free metadata using aom_img_metadata_free
            aom_img_metadata_free(metadata);
        }

        // Step 10: Set up output buffer for encoder using aom_codec_set_cx_data_buf
        size_t output_buffer_size = 1024 * 1024; // 1MB
        std::vector<uint8_t> output_buffer(output_buffer_size);
        aom_fixed_buf_t fixed_buf;
        fixed_buf.buf = output_buffer.data();
        fixed_buf.sz = output_buffer_size;
        
        // Initialize encoder for re-encoding
        aom_codec_ctx_t encoder;
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t enc_cfg;
        
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err == AOM_CODEC_OK) {
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            
            err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                         AOM_ENCODER_ABI_VERSION);
            
            if (err == AOM_CODEC_OK) {
                // Set output buffer
                err = aom_codec_set_cx_data_buf(&encoder, &fixed_buf, 0, 0);
                
                // Encode the image
                aom_codec_encode(&encoder, img, frame_count, 1, 0);
                
                // Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t* pkt = nullptr;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                aom_codec_destroy(&encoder);
            }
        }

        // Clean up
        aom_img_free(img_with_border);
        
        if (frame_count >= 10) { // Limit to 10 frames for performance
            break;
        }
    }

    // Clean up all resources
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}