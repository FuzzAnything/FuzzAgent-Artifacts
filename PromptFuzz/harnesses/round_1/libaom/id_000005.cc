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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_error","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_destroy","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_error","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_cx_data_buf","aom_img_alloc","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_error","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers for libaom

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 100) {
	return 0;
	}
	
    
    // Step 1: Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;  // Initial width
    dec_cfg.h = 128;  // Initial height
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(
        &decoder, 
        aom_codec_av1_dx(), 
        &dec_cfg, 
        0, 
        AOM_DECODER_ABI_VERSION
    );
    
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        return 0;  // Early exit if decoder init failed
    }
    
    // Step 2: Prepare compressed data buffer
    aom_fixed_buf_t compressed_data;
    compressed_data.buf = (void *)data;
    compressed_data.sz = size;
    
    // Set the compressed data buffer (pad with 0 bytes before and after)
    err = aom_codec_set_cx_data_buf(&decoder, &compressed_data, 0, 0);
    
    // Step 3: Allocate an image for potential metadata extraction
    aom_image_t img;
    aom_image_t *image_ptr = aom_img_alloc(
        &img, 
        AOM_IMG_FMT_I420, 
        128,  // width
        128,  // height
        16    // alignment
    );
    
    if (!image_ptr) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Step 4: Try to decode the input data
    // Note: Normally we'd call aom_codec_decode() here, but it's not in the provided API list
    // We'll simulate metadata access on the allocated image instead
    
    // Step 5: Extract and process metadata from the image
    size_t metadata_count = aom_img_num_metadata(image_ptr);
    
    for (size_t i = 0; i < metadata_count; i++) {
        const aom_metadata_t *metadata = aom_img_get_metadata(image_ptr, i);
        if (metadata) {
            // Process metadata if needed
            // For fuzzing purposes, just ensure we can access it
            
            // Free the metadata using the dedicated function
            // Note: The API expects a non-const pointer, but we have const
            // We need to create a copy to free it properly
            aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                metadata->type,
                metadata->payload,
                metadata->sz,
                metadata->insert_flag
            );
            
            if (metadata_copy) {
                aom_img_metadata_free(metadata_copy);
            }
        }
    }
    
    // Step 6: Check for decoder errors
    const char *decoder_error = aom_codec_error(&decoder);
    
    // Cleanup
    aom_img_free(image_ptr);
    aom_codec_destroy(&decoder);
    
    return 0;
}