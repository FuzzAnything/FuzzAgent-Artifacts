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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_img_free","aom_img_metadata_free"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_img_free","aom_img_metadata_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables to track resources
    aom_codec_ctx_t codec_ctx;
    aom_image_t *allocated_img = nullptr;
    aom_image_t wrapped_img;
    aom_metadata_t *metadata = nullptr;
    uint8_t *img_buffer = nullptr;
    
    // Get codec capabilities
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec_ctx, iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    
    if (err == AOM_CODEC_OK) {
        // Decode the input data
        err = aom_codec_decode(&codec_ctx, data, size, nullptr);
        
        if (err == AOM_CODEC_OK) {
            // Try to get a frame
            aom_codec_iter_t iter = nullptr;
            aom_image_t *img = aom_codec_get_frame(&codec_ctx, &iter);
            
            if (img) {
                // Get number of metadata in the image
                size_t num_metadata = aom_img_num_metadata(img);
                (void)num_metadata; // Use to avoid unused variable warning
            }
        }
        
        // Allocate an image
        allocated_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
        
        // Allocate buffer for wrapped image
        size_t buffer_size = 64 * 64 * 3 / 2; // For I420 format
        img_buffer = (uint8_t *)malloc(buffer_size);
        if (img_buffer) {
            // Wrap the buffer as an image
            aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 64, 64, 1, img_buffer);
            
            // Try to get global headers (though this is typically for encoder)
        }
        
        // Create a dummy metadata object
        uint8_t dummy_data[] = {0x01, 0x02, 0x03};
        metadata = aom_img_metadata_alloc(1, dummy_data, 3, 
                                         AOM_MIF_ANY_FRAME);
        
        // Encode a ULEB value
        uint8_t coded_value[10];
        size_t coded_size = 0;
        uint64_t value_to_encode = size; // Use input size as value
        aom_uleb_encode_fixed_size(value_to_encode, 10, 5, 
                                   coded_value, &coded_size);
        
        // Write coded value to output file
        if (coded_size > 0) {
            fwrite(coded_value, 1, coded_size, out_file);
        }
        
        // Clean up codec
        aom_codec_destroy(&codec_ctx);
    }
    
    // Free allocated image
    if (allocated_img) {
        aom_img_free(allocated_img);
    }
    
    // Free wrapped image buffer
    if (img_buffer) {
        free(img_buffer);
    }
    
    // Free metadata
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}