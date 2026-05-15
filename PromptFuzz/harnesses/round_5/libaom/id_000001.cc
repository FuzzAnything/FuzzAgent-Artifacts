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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_img_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_img_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get and use libaom version string
    const char *version_str = aom_codec_version_str();
    if (version_str) {
        // Could use version_str for logging or conditional logic
    }

    // 2. Use ULEB size calculation with a value derived from input
    uint64_t uleb_value = 0;
    size_t bytes_to_read = size < 8 ? size : 8;
    memcpy(&uleb_value, data, bytes_to_read);
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);

    // 3. Get codec interface and its capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
	return 0;
	}
	

    const char *iface_name = aom_codec_iface_name(decoder_iface);
    if (iface_name) {
        // Could use iface_name for logging
    }

    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps if needed

    // 4. Create a minimal image to add metadata to
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Use a small fixed size for the image
    const unsigned int width = 16;
    const unsigned int height = 16;
    const unsigned int align = 16;
    
    // Allocate image with a simple format
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, align);
    
    // 5. Add metadata to the image using input data
    if (size > 4) {
        uint32_t metadata_type = 0;
        memcpy(&metadata_type, data, 4);
        
        // Use remaining data (if any) as metadata payload
        size_t metadata_size = size - 4;
        const uint8_t *metadata_data = data + 4;
        
        // Add metadata with appropriate flag
        aom_img_add_metadata(&img, metadata_type, metadata_data, 
                           metadata_size < 1024 ? metadata_size : 1024, 
                           AOM_MIF_KEY_FRAME);
    }

    // 6. Free any metadata that might have been allocated internally
    // (Note: The image owns the metadata, but we demonstrate the API usage)
    size_t num_metadata = aom_img_num_metadata(&img);
    if (num_metadata > 0) {
        // Get the first metadata and create a copy to free
        const aom_metadata_t *metadata = aom_img_get_metadata(&img, 0);
        if (metadata) {
            // Create a copy of the metadata to demonstrate freeing
            aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                metadata->type, 
                metadata->payload, 
                metadata->sz, 
                metadata->insert_flag);
            
            if (metadata_copy) {
                aom_img_metadata_free(metadata_copy);
            }
        }
    }

    // 7. Clean up all allocated resources
    aom_img_free(&img);

    return 0;
}