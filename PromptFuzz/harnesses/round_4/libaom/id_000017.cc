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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_destroy","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_uleb_decode","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_uleb_decode","aom_codec_error_detail","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // 1. Use aom_codec_version_str() - get library version
    const char *version_str = aom_codec_version_str();
    if (!version_str) {
	return 0;
	}
	
    
    // 2. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *decoder = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    // Initialize decoder
    aom_codec_err_t init_res = aom_codec_dec_init_ver(&codec_ctx, decoder, &cfg, fuzz_int64_t_1, 
                                                      AOM_DECODER_ABI_VERSION);
    
    // 3. Use aom_codec_error_detail() if initialization fails
    if (init_res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&codec_ctx);
        (void)error_detail; // Use variable to avoid unused warning
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // 4. Try to decode the input data
    aom_codec_decode(&codec_ctx, data, size, nullptr);
    
    // 5. Use aom_codec_get_stream_info() to get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_get_stream_info(&codec_ctx, &stream_info);
    
    // 6. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 7. Use aom_img_num_metadata() to check for metadata in the image
        size_t metadata_count = aom_img_num_metadata(img);
        (void)metadata_count; // Use variable to avoid unused warning
    }
    
    // 8. Use aom_uleb_decode() on a portion of the input data
    // We'll try to decode a ULEB value from the beginning of the buffer
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    
    // Use first min(10, size) bytes for ULEB decoding
    size_t uleb_available = (size < 10) ? size : 10;
    aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    
    // 9. Use aom_codec_error_detail() again to check for any decoding errors
    const char *final_error_detail = aom_codec_error_detail(&codec_ctx);
    (void)final_error_detail; // Use variable to avoid unused warning
    
    // 10. Clean up
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}