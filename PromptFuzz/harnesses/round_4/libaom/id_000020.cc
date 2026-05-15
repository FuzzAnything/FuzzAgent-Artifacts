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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_uleb_decode","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_uleb_decode","aom_codec_error_detail","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Decode an AV1 stream and extract information from it
    
    // 1. Get library version (using aom_codec_version_str)
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Use variable to avoid unused warning
    
    // 2. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *decoder = aom_codec_av1_dx();
    
    // Initialize with default configuration
    aom_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init(&codec_ctx, decoder, &cfg, 0);
    if (init_result != AOM_CODEC_OK) {
        // If initialization fails, try to get error detail
        const char *error_detail = aom_codec_error_detail(&codec_ctx);
        (void)error_detail;
        return 0;
    }
    
    // 3. Get stream information (using aom_codec_get_stream_info)
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // First decode the data to populate stream info
    aom_codec_decode(&codec_ctx, data, size, nullptr);
    aom_codec_get_stream_info(&codec_ctx, &stream_info);
    
    // 4. Try to get decoded frames and check metadata
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *img;
    while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // Get number of metadata in the image (using aom_img_num_metadata)
        size_t metadata_count = aom_img_num_metadata(img);
        (void)metadata_count;
    }
    
    // 5. Decode ULEB value from input data (using aom_uleb_decode)
    if (size > 0) {
        uint64_t uleb_value = 0;
        size_t uleb_length = 0;
        // Try to decode ULEB from the beginning of input
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 6. Check for any errors and get detail if available
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    (void)error_detail;
    
    // 7. Clean up
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}