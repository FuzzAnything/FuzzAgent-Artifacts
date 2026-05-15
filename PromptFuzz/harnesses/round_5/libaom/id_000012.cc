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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"aom_read_obu_header_and_size":[[101,7,101,16,0,0,4,0],[106,9,106,31,0,0,4,0],[106,9,106,31,0,0,4,1],[115,7,115,34,0,0,4,0],[118,9,118,36,0,0,4,0],[118,9,118,36,0,0,4,1]],"read_obu_header":[[51,34,51,44,0,0,4,1]]},"library_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Decode an AV1 video stream from input data, get frames, and process them
    // while also querying codec version information
    
    // 1. Get codec version information
    int version = aom_codec_version();
    const char *extra_str = aom_codec_version_extra_str();
    const char *build_config = aom_codec_build_config();
    
    // 2. Create decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Failed to initialize decoder, but we can still try peek_stream_info
        // Don't return early as we need to call other APIs
    }
    
    // 3. Peek stream info without full decode context
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.is_annexb = 1; // Assume Annex-B format
    
    if (size > 0) {
        aom_codec_peek_stream_info(iface, data, size, &stream_info);
    }
    
    // 4. Decode the input data if decoder initialized successfully
    if (err == AOM_CODEC_OK && size > 0) {
        // Get stream info using decoder context
        aom_codec_stream_info_t stream_info2;
        memset(&stream_info2, 0, sizeof(stream_info2));
        stream_info2.is_annexb = 1;
        aom_codec_get_stream_info(&codec_ctx, &stream_info2);
        
        // Decode the frame
        aom_codec_decode(&codec_ctx, data, size, nullptr);
        
        // 5. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = nullptr;
        
        while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
            // 6. Flip the image as an example operation
            aom_img_flip(img);
            
            // We could process the image further here, but for fuzzing
            // we just ensure the API is called
        }
        
        // 7. Clean up decoder
        aom_codec_destroy(&codec_ctx);
    }
    
    // Note: The version info strings (extra_str, build_config) are only
    // used to ensure the functions are called - we don't need to do anything
    // with them for fuzzing purposes
    
    return 0;
}