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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 7
//<Quality> {"density":13,"unique_branches":{"aom_read_obu_header_and_size":[[101,7,101,16,0,0,4,0],[106,9,106,31,0,0,4,0],[106,9,106,31,0,0,4,1],[115,7,115,34,0,0,4,0],[118,9,118,36,0,0,4,0],[118,9,118,36,0,0,4,1]],"read_obu_header":[[51,34,51,44,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_flip","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get codec interface and capabilities
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    
    // 2. Peek stream information from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.is_annexb = 1; // Assume Annex-B format
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        iface, data, size, &stream_info);
    
    // 3. Initialize decoder
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Cleanup and return if initialization fails
        return 0;
    }
    
    // 4. Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec_ctx, data, size, nullptr);
    
    // 5. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 6. Get image plane dimensions
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            
            // Use the values to prevent optimization
            if (plane_width > 0 && plane_height > 0) {
                // Do nothing, just using the values
            }
        }
        
        // 7. Handle metadata operations
        size_t num_metadata = aom_img_num_metadata(img);
        
        // Access metadata if present
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            if (metadata) {
                // Use metadata to prevent optimization
                volatile uint32_t type = metadata->type;
                volatile size_t sz = metadata->sz;
                (void)type;
                (void)sz;
            }
        }
        
        // Remove metadata if present
        if (num_metadata > 0) {
            aom_img_remove_metadata(img);
        }
        
        // 8. Flip the image (in-place operation)
        aom_img_flip(img);
    }
    
    // 9. Cleanup decoder
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}