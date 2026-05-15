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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers for libaom

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Use informational APIs
    int version = aom_codec_version();
    const char *extra_str = aom_codec_version_extra_str();
    const char *build_config = aom_codec_build_config();
    
    // Avoid unused variable warnings
    (void)version;
    (void)extra_str;
    (void)build_config;

    // Step 2: Get stream info using peek
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t err = aom_codec_peek_stream_info(codec_iface, data, size, &stream_info);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Step 3: Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init_ver(&decoder, codec_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Step 4: Get stream info again (after decoder init)
    err = aom_codec_get_stream_info(&decoder, &stream_info);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Step 5: Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Step 6: Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Step 7: Flip the image
        aom_img_flip(img);
        
        // Note: We don't need to free the image here as it's managed by the decoder
    }

    // Step 8: Cleanup
    aom_codec_destroy(&decoder);
    
    return 0;
}