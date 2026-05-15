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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"set_lpf_parameters":[[298,30,298,36,0,0,4,0],[315,21,315,33,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000039_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Allocate buffer for frame data
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000039_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for meaningful operations
    
    // 1. First decode a ULEB value from the input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    
    // 2. Get codec interface name
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    // 3. Allocate an image with border (using ULEB value for dimensions)
    aom_image_t image;
    unsigned int width = (uleb_value % 640) + 1; // Ensure reasonable size
    unsigned int height = (uleb_value % 480) + 1;
    
    // Use AOM_IMG_FMT_I420 format (common YUV format)
    aom_image_t *img_ptr = aom_img_alloc_with_border(
        &image, 
        AOM_IMG_FMT_I420, 
        width, 
        height, 
        32, // align
        32, // size_align
        16  // border
    );
    
    if (!img_ptr) {
        // If allocation fails, try with smaller dimensions
        img_ptr = aom_img_alloc_with_border(
            &image, 
            AOM_IMG_FMT_I420, 
            64, // fallback width
            64, // fallback height
            32, 
            32, 
            8
        );
        if (!img_ptr) return 0;
    }
    
    // 4. Create metadata and add to image
    uint32_t metadata_type = 1; // Example type
    size_t metadata_size = size > 1024 ? 1024 : size; // Limit metadata size
    uint8_t *metadata_data = (uint8_t *)malloc(metadata_size);
    if (metadata_data) {
        memcpy(metadata_data, data, metadata_size);
        
        // Create metadata with KEY_FRAME flag
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            metadata_type,
            metadata_data,
            metadata_size,
            AOM_MIF_KEY_FRAME
        );
        
        if (metadata) {
            // Add metadata to image
            aom_img_add_metadata(img_ptr, metadata_type, metadata_data, metadata_size, AOM_MIF_KEY_FRAME);
            
            // 5. Retrieve metadata from image
            size_t metadata_count = aom_img_num_metadata(img_ptr);
            if (metadata_count > 0) {
                const aom_metadata_t *retrieved_metadata = aom_img_get_metadata(img_ptr, 0);
            }
            
            // Clean up metadata
            aom_img_metadata_free(metadata);
        }
        free(metadata_data);
    }
    
    // 6. Set up decoder context with frame buffer functions
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    cfg.w = width;
    cfg.h = height;
    
    // Initialize decoder
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx,
        decoder_iface,
        &cfg,
        0,
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_result == AOM_CODEC_OK) {
        // Set custom frame buffer functions
        aom_codec_set_frame_buffer_functions(
            &codec_ctx,
             id_000039_get_frame_buffer,
             id_000039_release_frame_buffer,
            NULL
        );
        
        // Try to decode the input data (assuming it's a compressed bitstream)
        aom_codec_decode(&codec_ctx, data, size, NULL);
        
        // Try to get stream info
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
        
        // Clean up decoder
        aom_codec_destroy(&codec_ctx);
    }
    
    // 7. Clean up image
    aom_img_free(img_ptr);
    
    return 0;
}