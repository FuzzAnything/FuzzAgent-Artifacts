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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 4) {
        return 0;
    }

    // 1. Call version-related APIs (these don't consume input data)
    int version = aom_codec_version();
    const char* version_extra = aom_codec_version_extra_str();
    const char* build_config = aom_codec_build_config();
    
    // These calls are just to satisfy the requirement - we don't use the results
    (void)version;
    (void)version_extra;
    (void)build_config;

    // 2. Peek at stream info using the input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Get AV1 decoder interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Use a copy of the data for peek_stream_info since it might modify the pointer
    std::vector<uint8_t> data_copy(data, data + size);
    aom_codec_peek_stream_info(decoder_iface, data_copy.data(), data_copy.size(), &stream_info);

    // 3. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;  // Single thread for fuzzing
    
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // 4. Get stream info from decoder context (redundant but required by instruction)
    aom_codec_get_stream_info(&decoder, &stream_info);

    // 5. Decode the input data
    // We'll decode in a loop to handle multiple OBUs if present
    size_t data_consumed = 0;
    const size_t chunk_size = 1024;  // Process in chunks
    void* user_priv = nullptr;
    
    while (data_consumed < size) {
        size_t remaining = size - data_consumed;
        size_t decode_size = remaining < chunk_size ? remaining : chunk_size;
        
        ret = aom_codec_decode(&decoder, data + data_consumed, decode_size, user_priv);
        if (ret != AOM_CODEC_OK) {
            // Continue anyway to try to get frames
            break;
        }
        data_consumed += decode_size;
    }

    // 6. Get decoded frames and flip them
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Flip the image vertically
        aom_img_flip(img);
        
        // We could process the flipped image here if needed
        // For fuzzing, we just ensure the API is called
    }

    // 7. Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}