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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_control","aom_uleb_encode_fixed_size","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_control","aom_uleb_encode_fixed_size","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve the event of decoding an AV1 bitstream, extracting metadata, and performing ULEB encoding:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Use aom_codec_error to get error string
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str; // Prevent unused variable warning
        return 0;
    }
    
    // Try to decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get error string using aom_codec_error
    const char *error_str = aom_codec_error(&decoder);
    (void)error_str; // Prevent unused variable warning
    
    // Try to get a preview frame (if available)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    if (preview_frame) {
        // Get metadata from the image if it exists
        size_t num_metadata = aom_img_num_metadata(preview_frame);
        
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, i);
            if (metadata) {
                // Free the metadata using aom_img_metadata_free
                // Note: We need to cast away const since aom_img_metadata_free expects non-const
                aom_img_metadata_free((aom_metadata_t *)metadata);
            }
        }
    }
    
    // Use aom_codec_control to set/get decoder properties
    // Try a few different control IDs that might be relevant
    int ctrl_id = 0;
    
    // Try different control IDs based on input data
    if (size > 0) {
        ctrl_id = data[0] % 100; // Use first byte to choose a control ID
    }
    
    // Note: aom_codec_control returns aom_codec_err_t, but we ignore the result
    // for fuzzing purposes since we're testing various inputs
    aom_codec_err_t ctrl_res = aom_codec_control(&decoder, ctrl_id);
    (void)ctrl_res; // Prevent unused variable warning
    
    // Use aom_uleb_encode_fixed_size to encode a value
    // Use the input size as the value to encode
    uint64_t value_to_encode = size;
    size_t available_buffer_size = 16; // Enough for ULEB encoding
    uint8_t encoded_buffer[16];
    size_t coded_size = 0;
    
    // Try different pad sizes
    size_t pad_to_size = (size > 0) ? (data[0] % 8) + 1 : 4;
    
    int encode_res = aom_uleb_encode_fixed_size(
        value_to_encode, 
        available_buffer_size, 
        pad_to_size, 
        encoded_buffer, 
        &coded_size
    );
    (void)encode_res; // Prevent unused variable warning
    
    // Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}