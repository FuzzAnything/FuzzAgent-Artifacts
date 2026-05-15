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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"realloc_frame_buffer_aligned":[[111,31,111,61,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000040_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    // Use aom_img_alloc_with_border to allocate image with border
    aom_image_t *img = aom_img_alloc_with_border(nullptr, 
                                                 AOM_IMG_FMT_I420,
                                                 128, 128,  // dimensions
                                                 1,         // align
                                                 1,         // size_align
                                                 32);       // border
    
    if (!img) return -1;
    
    fb->data = img->img_data;
    fb->size = img->sz;
    fb->priv = img;
    return 0;
}

static int  id_000040_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    aom_image_t *img = (aom_image_t *)fb->priv;
    aom_img_free(img);
    return 0;
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
	  // Need minimum data
    
    // Create file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Decode ULEB value from input
    uint64_t uleb_value;
    size_t uleb_length;
    int uleb_result = aom_uleb_decode(data, 
                                      size > 100 ? 100 : size,  // Use first 100 bytes
                                      &uleb_value, 
                                      &uleb_length);
    
    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Get codec interface name
    const char *iface_name = aom_codec_iface_name(iface);
    
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, iface, &cfg, 
                                                0, AOM_DECODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set custom frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder, 
                                         id_000040_get_frame_buffer,
                                         id_000040_release_frame_buffer,
                                        nullptr);
    
    // 4. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 5. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 6. Get metadata from image (if any)
        for (size_t i = 0; i < aom_img_num_metadata(img); i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            // Use metadata if available
        }
        
        // 7. Allocate new metadata using aom_img_metadata_alloc
        // Use the ULEB decoded value as metadata type
        uint32_t metadata_type = (uint32_t)(uleb_value & 0xFFFFFFFF);
        size_t metadata_size = size > 50 ? 50 : size;
        
        aom_metadata_t *new_metadata = aom_img_metadata_alloc(
            metadata_type,
            data,  // Use input data as metadata payload
            metadata_size,
            AOM_MIF_ANY_FRAME
        );
        
        if (new_metadata) {
            // Clean up allocated metadata
            aom_img_metadata_free(new_metadata);
        }
    }
    
    // 8. Try to write something to output file
    if (size > 0 && out_file) {
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}