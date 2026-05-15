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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_control","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_control","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t codec;
    aom_codec_iface_t *decoder = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    if (aom_codec_dec_init(&codec, decoder, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode input data
    const size_t buffer_size = 4096;
    uint8_t buffer[buffer_size];
    
    while (!feof(in_file)) {
        size_t bytes_read = fread(buffer, 1, buffer_size, in_file);
        if (bytes_read > 0) {
            aom_codec_decode(&codec, buffer, bytes_read, NULL);
        }
    }

    // Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec);
    if (preview_frame) {
        // Create some metadata to add
        uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
        uint32_t metadata_type = 1234;
        
        // We need a non-const pointer to add metadata
        aom_image_t *mutable_frame = const_cast<aom_image_t*>(preview_frame);
        
        // Add metadata to the image
        int add_result = aom_img_add_metadata(
            mutable_frame,
            metadata_type,
            metadata_data,
            sizeof(metadata_data),
            AOM_MIF_ANY_FRAME
        );
        
        if (add_result == 0) {
            // Get number of metadata
            size_t metadata_count = aom_img_num_metadata(preview_frame);
            
            // Get the metadata we just added
            if (metadata_count > 0) {
                const aom_metadata_t *retrieved_metadata = 
                    aom_img_get_metadata(preview_frame, 0);
                
                if (retrieved_metadata) {
                    // Use aom_codec_control at least once
                    // Try to get some control information from decoder
                    int ctrl_value = 0;
                    aom_codec_control(&codec, AOMD_GET_FRAME_CORRUPTED, &ctrl_value);
                    
                    // Free metadata (although typically this is handled by image cleanup)
                    // We create a copy to free since the original is owned by the image
                    aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                        retrieved_metadata->type,
                        retrieved_metadata->payload,
                        retrieved_metadata->sz,
                        retrieved_metadata->insert_flag
                    );
                    
                    if (metadata_copy) {
                        aom_img_metadata_free(metadata_copy);
                    }
                }
            }
        }
    }

    // Cleanup
    aom_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}