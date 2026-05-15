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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"ctrl_get_frame_size":[[1280,7,1280,17,0,0,4,1]],"ctrl_get_frame_corrupted":[[1257,7,1257,16,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_free","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_control","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_control","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data

    // 1. Use aom_uleb_decode to read a value from the input
    uint64_t uleb_value;
    size_t uleb_length;
    int uleb_result = aom_uleb_decode(data, 
                                      size, 
                                      &uleb_value, 
                                      &uleb_length);
    // Continue even if uleb decode fails - fuzzing different code paths

    // 2. Allocate an image with border
    aom_image_t *img = aom_img_alloc_with_border(
        nullptr,                    // img
        AOM_IMG_FMT_I420,           // fmt
        64,                         // d_w
        64,                         // d_h
        16,                         // align
        16,                         // size_align
        8                           // border
    );
    if (!img) {
	return 0;
	}
	

    // 3. Set a rectangle on the image using uleb-decoded values
    // Use modulo to ensure valid coordinates
    unsigned int x = uleb_value % 32;
    unsigned int y = (uleb_value >> 8) % 32;
    unsigned int w = 16 + (uleb_value >> 16) % 16;
    unsigned int h = 16 + (uleb_value >> 24) % 16;
    
    aom_img_set_rect(img, x, y, w, h, 4);

    // 4. Create a decoder context
    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0, 0, 0};
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec,
        iface,
        &cfg,
        0,
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_result != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0;
    }

    // 5. Set frame buffer functions
    // Create dummy callbacks for fuzzing
    auto dummy_get_buffer = [](void *cb_priv, size_t min_size, 
                              aom_codec_frame_buffer_t *fb) -> int {
        // Return error to test error path
        return -1;
    };
    
    auto dummy_release_buffer = [](void *cb_priv, 
                                  aom_codec_frame_buffer_t *fb) -> int {
        return 0;
    };
    
    aom_codec_set_frame_buffer_functions(
        &codec,
        dummy_get_buffer,
        dummy_release_buffer,
        nullptr
    );

    // 6. Use aom_codec_control with various control IDs
    // Try different control IDs that might be relevant
    aom_codec_control(&codec, AOMD_GET_LAST_REF_UPDATES, nullptr);
    aom_codec_control(&codec, AOMD_GET_FRAME_CORRUPTED, nullptr);
    aom_codec_control(&codec, AV1D_GET_FRAME_SIZE, nullptr);

    // 7. Decode the input data (excluding what was used for uleb)
    size_t decode_offset = uleb_length < size ? uleb_length : 0;
    size_t decode_size = size - decode_offset;
    
    if (decode_size > 0) {
        aom_codec_decode(&codec, 
                        data + decode_offset, 
                        decode_size, 
                        nullptr);
        
        // Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        while (aom_codec_get_frame(&codec, &iter)) {
            // Just iterate through available frames
        }
    }

    // 8. Create and free metadata using uleb-decoded value
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        static_cast<uint32_t>(uleb_value & 0xFFFFFFFF),  // type
        data,                                            // data
        size < 100 ? size : 100,                         // sz (limited)
        AOM_MIF_ANY_FRAME                                // insert_flag
    );
    
    if (metadata) {
        aom_img_metadata_free(metadata);
    }

    // 9. Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}