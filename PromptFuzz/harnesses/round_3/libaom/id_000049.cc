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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_control","aom_uleb_encode_fixed_size","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_control","aom_uleb_encode_fixed_size","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        // Decode the input data
        res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        
        // Get error message if decoding failed
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Use variable to avoid unused warning
        
        // Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
        
        if (preview_frame) {
            // Get metadata from the preview frame
            size_t metadata_count = aom_img_num_metadata(preview_frame);
            for (size_t i = 0; i < metadata_count; ++i) {
                const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, i);
                if (metadata) {
                    // Create a copy of metadata to free later
                    aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                        metadata->type, 
                        metadata->payload, 
                        metadata->sz, 
                        metadata->insert_flag
                    );
                    
                    // Free the copied metadata
                    if (metadata_copy) {
                        aom_img_metadata_free(metadata_copy);
                    }
                }
            }
        }
        
        // Use aom_codec_control with a control ID derived from input data
        int ctrl_id = data[0] % 100; // Use first byte to determine control ID
        aom_codec_control(&decoder, ctrl_id, nullptr);
    }
    
    // Use aom_uleb_encode_fixed_size with some data from input
    if (size >= sizeof(uint64_t)) {
        uint64_t value;
        memcpy(&value, data, sizeof(uint64_t));
        
        uint8_t coded_value[16];
        size_t coded_size = 0;
        size_t available = sizeof(coded_value);
        size_t pad_to_size = 8;
        
        aom_uleb_encode_fixed_size(value, available, pad_to_size, coded_value, &coded_size);
    }
    
    // Clean up
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}