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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use version APIs
    int version = aom_codec_version();
    const char* version_extra = aom_codec_version_extra_str();
    const char* build_config = aom_codec_build_config();
    
    // Create AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
	return 0;
	}
	
    
    // First peek at stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Peek at stream info without decoder context
    aom_codec_peek_stream_info(iface, data, size, &stream_info);
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Decode the input data
    aom_codec_decode(&decoder, data, size, NULL);
    
    // Get stream info from decoder context
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Get decoded frames
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        // Flip the image
        aom_img_flip(img);
    }
    
    // Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}