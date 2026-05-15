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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_uleb_decode","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_codec_error_detail","aom_codec_destroy","aom_uleb_decode"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_uleb_decode","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_codec_error_detail","aom_codec_destroy","aom_uleb_decode"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Call aom_codec_version_str() at least once
    const char *version = aom_codec_version_str();
    (void)version; // Avoid unused variable warning
    
    // 2. Create a decoder context to use with other APIs
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t* decoder = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init(&codec_ctx, decoder, &cfg, 0);
    
    // 3. Use aom_uleb_decode() with input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        // Try to decode a ULEB value from the beginning of input
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 4. Try to get stream info from the input data (if it looks like AV1 stream)
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    if (init_result == AOM_CODEC_OK) {
        // Try to decode the input data
        aom_codec_err_t decode_result = aom_codec_decode(&codec_ctx, data, size, nullptr);
        
        if (decode_result == AOM_CODEC_OK) {
            // Get stream information
            aom_codec_err_t stream_info_result = aom_codec_get_stream_info(&codec_ctx, &stream_info);
            (void)stream_info_result; // Result may be OK or error
            
            // Try to get a frame to have an image for metadata functions
            aom_codec_iter_t iter = nullptr;
            aom_image_t* img = aom_codec_get_frame(&codec_ctx, &iter);
            
            // 5. Call aom_img_num_metadata() if we got an image
            if (img != nullptr) {
                size_t metadata_count = aom_img_num_metadata(img);
                (void)metadata_count; // Avoid unused variable warning
            }
        }
        
        // 6. Call aom_codec_error_detail() to check for any errors
        const char* error_detail = aom_codec_error_detail(&codec_ctx);
        (void)error_detail; // Avoid unused variable warning
    }
    
    // 7. Clean up all allocated resources
    if (init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
    }
    
    // 8. Additional ULEB decoding test with different offsets
    // Try to decode ULEB from various positions in the input
    for (size_t offset = 0; offset < size && offset < 10; offset++) {
        uint64_t test_value = 0;
        size_t test_length = 0;
        if (size - offset > 0) {
            aom_uleb_decode(data + offset, size - offset, &test_value, &test_length);
        }
    }
    
    return 0;
}