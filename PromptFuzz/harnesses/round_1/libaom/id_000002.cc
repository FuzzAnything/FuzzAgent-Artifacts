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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_decode","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_error","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_error","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Need minimum data for basic operations
    
    // Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Set decoder configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 640;  // Default width
    cfg.h = 480;  // Default height
    cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder (API 1)
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (init_result != AOM_CODEC_OK) {
        // Get error message (API 6)
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Prevent unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read data from file to simulate bitstream reading
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, size, in_file);
    
    // Create a fixed buffer for encoded data
    aom_fixed_buf_t enc_buf = {0};
    enc_buf.buf = buffer;
    enc_buf.sz = bytes_read;
    
    // Set compressed data buffer (API 5)
    // Note: This is typically used for encoder output, but we're fuzzing the API
    aom_codec_err_t set_buf_result = aom_codec_set_cx_data_buf(&decoder, &enc_buf, 0, 0);
    (void)set_buf_result; // Result may fail for decoder, but we're testing API
    
    // Try to decode the data
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
    
    // Allocate an image using aom_img_alloc (API 1)
    aom_image_t img;
    aom_image_t *img_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, 640, 480, 16);
    
    if (img_ptr) {
        // Try to get metadata from the allocated image (API 2)
        // First, we need to add some metadata to test the get function
        uint8_t test_metadata[] = {0x01, 0x02, 0x03};
        int add_result = aom_img_add_metadata(&img, 1, test_metadata, 3, 
                                             AOM_MIF_ANY_FRAME);
        (void)add_result; // Ignore result
        
        // Now try to get the metadata
        const aom_metadata_t *metadata = aom_img_get_metadata(&img, 0);
        if (metadata) {
            // Create a copy to free separately
            aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                metadata->type, metadata->payload, metadata->sz, metadata->insert_flag);
            
            // Free the metadata copy (API 3)
            if (metadata_copy) {
                aom_img_metadata_free(metadata_copy);
            }
        }
        
        // Try to get frame from decoder if decode was successful
        if (decode_result == AOM_CODEC_OK) {
            aom_codec_iter_t iter = NULL;
            aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
            if (decoded_img) {
                // Try to get metadata from decoded image if any
                size_t num_metadata = aom_img_num_metadata(decoded_img);
                for (size_t i = 0; i < num_metadata; i++) {
                    const aom_metadata_t *decoded_metadata = aom_img_get_metadata(decoded_img, i);
                    if (decoded_metadata) {
                        aom_metadata_t *decoded_metadata_copy = aom_img_metadata_alloc(
                            decoded_metadata->type, decoded_metadata->payload, 
                            decoded_metadata->sz, decoded_metadata->insert_flag);
                        
                        if (decoded_metadata_copy) {
                            aom_img_metadata_free(decoded_metadata_copy);
                        }
                    }
                }
            }
        }
        
        // Get error message again to ensure API usage (API 6)
        const char *final_error = aom_codec_error(&decoder);
        (void)final_error;
        
        // Free the allocated image
        aom_img_free(&img);
    }
    
    // Clean up all resources
    free(buffer);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}