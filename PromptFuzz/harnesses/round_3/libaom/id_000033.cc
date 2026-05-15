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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_destroy","aom_img_alloc","aom_codec_destroy","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_add_metadata","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_img_alloc","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_add_metadata","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"visited":0}
/*Looking at the APIs and the requirements, I'll create a fuzzer that:
1. Decodes an AV1 stream from the input data
2. Extracts frames and adds metadata to them
3. Uses all the specified APIs

Here's the implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
        return 0;
    }

    // Initialize variables
    aom_codec_ctx_t decoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_stream_info_t stream_info;
    aom_image_t *image = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *decoded_frame = nullptr;
    
    // Use fmemopen to create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output file for writing (not used for fuzzing but needed per requirements)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. First, use aom_codec_peek_stream_info to get stream information
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_err_t err = aom_codec_peek_stream_info(aom_codec_av1_dx(), 
                                                      data, 
                                                      size, 
                                                      &stream_info);
    
    // 2. Get default encoder configuration (even though we're mainly decoding)
    err = aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);

    // 3. Initialize decoder
    err = aom_codec_dec_init_ver(&decoder_ctx, 
                                  aom_codec_av1_dx(), 
                                  nullptr, 
                                  0, 
                                  AOM_DECODER_ABI_VERSION);
    
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Decode the input data
    err = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Allocate an image buffer using aom_img_alloc
    // Use stream info or default dimensions if available
    unsigned int width = stream_info.w > 0 ? stream_info.w : 64;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 64;
    
    image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, 1);
    if (!image) {
        aom_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Try to get decoded frames
    while ((decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // 7. Create metadata from input data
        // Use first 4 bytes as type, rest as payload
        uint32_t metadata_type = 0;
        size_t metadata_size = size > 4 ? size - 4 : 0;
        const uint8_t* metadata_data = size > 4 ? data + 4 : data;
        
        if (metadata_size > 0) {
            // 8. Allocate metadata using aom_img_metadata_alloc
            metadata = aom_img_metadata_alloc(metadata_type, 
                                              metadata_data, 
                                              metadata_size > 1024 ? 1024 : metadata_size,
                                              AOM_MIF_ANY_FRAME);
            
            if (metadata) {
                // 9. Add metadata to the image using aom_img_add_metadata
                aom_img_add_metadata(image, 
                                     metadata->type, 
                                     metadata->payload, 
                                     metadata->sz, 
                                     metadata->insert_flag);
                
                // 10. Free the metadata using aom_img_metadata_free
                aom_img_metadata_free(metadata);
                metadata = nullptr;
            }
        }
        
        // Also try to add metadata directly to the decoded frame if possible
        if (metadata_size > 0) {
            aom_img_add_metadata((aom_image_t*)decoded_frame,
                                 metadata_type,
                                 metadata_data,
                                 metadata_size > 512 ? 512 : metadata_size,
                                 AOM_MIF_KEY_FRAME);
        }
    }

    // 11. Try to get more frames (just in case)
    iter = nullptr;
    decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // 12. Clean up resources
    if (image) {
        aom_img_free(image);
    }
    
    aom_codec_destroy(&decoder_ctx);
    
    // 13. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}