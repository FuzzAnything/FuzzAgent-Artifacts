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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_cx_data_buf","aom_codec_error","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_get_metadata","aom_img_free","aom_img_free","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_get_metadata","aom_img_free","aom_img_free","aom_codec_error","aom_codec_destroy"],"visited":0}
/*Looking at the provided APIs, I need to create a fuzz driver that uses these specific functions together to handle libaom's output data. The event these APIs could achieve is **decoding an AV1 encoded bitstream and extracting metadata from the decoded frames**.

Here's the fuzz driver implementation:

*/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    aom_codec_err_t res = AOM_CODEC_OK;
    aom_codec_ctx_t decoder;
    aom_image_t *img = nullptr;
    aom_fixed_buf_t encoded_data = {0};
    
    // 1. Create a decoder context
    aom_codec_iface_t *decoder_interface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    res = aom_codec_dec_init_ver(&decoder, decoder_interface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_error(&decoder);
        (void)error_str; // Use error string to satisfy API requirement
        return 0;
    }
    
    // 2. Prepare encoded data buffer
    encoded_data.buf = (void*)data;
    encoded_data.sz = size;
    
    // 3. Set the encoded data buffer (pad_before = 0, pad_after = 0)
    res = aom_codec_set_cx_data_buf(&decoder, &encoded_data, 0, 0);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_error(&decoder);
        (void)error_str;
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 4. Try to decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_error(&decoder);
        (void)error_str;
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 5. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = nullptr;
    
    while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 6. Allocate a new image with the same properties as the decoded frame
        img = aom_img_alloc(nullptr, 
                           static_cast<aom_img_fmt_t>(frame->fmt),
                           frame->d_w, 
                           frame->d_h, 
                           1); // 1 byte alignment
        
        if (!img) {
            continue;
        }
        
        // 7. Try to get metadata from the frame (check at index 0)
        const aom_metadata_t *metadata = aom_img_get_metadata(frame, 0);
        if (metadata) {
            // 8. Create a copy of the metadata to free later
            aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                metadata->type, 
                metadata->payload, 
                metadata->sz, 
                metadata->insert_flag);
            
            if (metadata_copy) {
                // 9. Free the metadata copy
                aom_img_metadata_free(metadata_copy);
            }
        }
        
        // 10. Also try to get metadata from our allocated image (should be null)
        const aom_metadata_t *img_metadata = aom_img_get_metadata(img, 0);
        (void)img_metadata; // Use the result
        
        // 11. Free the allocated image
        aom_img_free(img);
        img = nullptr;
        
        // Break after processing one frame to avoid infinite loops with malformed data
        break;
    }
    
    // 12. Cleanup
    if (img) {
        aom_img_free(img);
    }
    
    // Get error string one more time before destruction
    const char* final_error = aom_codec_error(&decoder);
    (void)final_error;
    
    aom_codec_destroy(&decoder);
    
    return 0;
}