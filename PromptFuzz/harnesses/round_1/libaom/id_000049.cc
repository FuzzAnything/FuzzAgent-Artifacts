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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_get_global_headers","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_wrap","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_wrap","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimal size check

    // 1. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Get codec capabilities first
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use variable to avoid unused warning

    // 3. Initialize decoder with default config
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Decode the input data
    // Read data from file in chunks
    const size_t chunk_size = 4096;
    uint8_t* buffer = (uint8_t*)malloc(chunk_size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    while (!feof(in_file)) {
        size_t bytes_read = fread(buffer, 1, chunk_size, in_file);
        if (bytes_read > 0) {
            aom_codec_decode(&decoder, buffer, bytes_read, NULL);
        }
    }

    // 5. Get global headers from decoder
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&decoder);
    if (global_headers) {
        // Use the global headers data if available
        uint64_t dummy_value = global_headers->sz;
        uint8_t coded_buffer[16];
        size_t coded_size;
        
        // 6. Use uleb encoding with global headers size
        int encode_result = aom_uleb_encode_fixed_size(
            dummy_value, 
            sizeof(coded_buffer), 
            8,  // pad to 8 bytes
            coded_buffer, 
            &coded_size
        );
        (void)encode_result; // Use variable to avoid warning
        
        // Free global headers if needed
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // 7. Create test images using different allocation methods
    const unsigned int width = 128;
    const unsigned int height = 128;
    const unsigned int align = 32;
    
    // Method 1: Using aom_img_alloc
    aom_image_t* img1 = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, align);
    
    // Method 2: Using aom_img_wrap with pre-allocated buffer
    aom_image_t img2;
    size_t img_size = width * height * 3 / 2; // I420 format
    unsigned char* img_data = (unsigned char*)malloc(img_size);
    if (img_data) {
        memset(img_data, 0, img_size);
        aom_image_t* wrapped_img = aom_img_wrap(&img2, AOM_IMG_FMT_I420, width, height, align, img_data);
        (void)wrapped_img; // Use variable
    }

    // 8. Get metadata from allocated image if it exists
    if (img1) {
        size_t metadata_count = aom_img_num_metadata(img1);
        
        // Process each metadata entry
        for (size_t i = 0; i < metadata_count; i++) {
            const aom_metadata_t* metadata = aom_img_get_metadata(img1, i);
            if (metadata) {
                // 9. Free metadata when done
                aom_img_metadata_free((aom_metadata_t*)metadata);
            }
        }
        
        // Remove all metadata from image
        aom_img_remove_metadata(img1);
    }

    // 10. Try to get decoded frames
    aom_codec_iter_t iter = NULL;
    const aom_image_t* decoded_frame;
    while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        // Use frame if needed
        (void)decoded_frame;
    }

    // 11. Cleanup
    if (img1) {
        aom_img_free(img1);
    }
    
    if (img_data) {
        free(img_data);
    }
    
    free(buffer);
    assert_file_closed(&in_file);;
    aom_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}