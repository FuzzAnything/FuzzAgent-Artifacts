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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


// Include necessary headers for libaom
extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

extern "C" int get_frame_buffer_callback(void *cb_priv, size_t min_size, 
                                         aom_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    if (!fb_data) return -1;
    
    // Resize buffer if needed
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer.size();
    fb->priv = nullptr;
    return 0;
}

extern "C" int release_frame_buffer_callback(void *cb_priv, 
                                             aom_codec_frame_buffer_t *fb) {
    // We don't need to do anything special since we manage our own buffer
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0;  // Not enough data for meaningful fuzzing
    }
    
    // 1. First, try to decode a ULEB value from the beginning of the data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_result = aom_uleb_decode(data, 
                                      (size < 10) ? size : 10,  // Use first 10 bytes max
                                      &uleb_value, 
                                      &uleb_length);
    
    // 2. Allocate an image with border using the ULEB value for dimensions
    //    Use modulo to ensure reasonable dimensions
    unsigned int width = 16 + (uleb_value % 256);
    unsigned int height = 16 + ((uleb_value >> 8) % 256);
    unsigned int border = 16;
    
    aom_image_t image;
    aom_image_t *img_ptr = aom_img_alloc_with_border(&image, 
                                                     AOM_IMG_FMT_I420,
                                                     width,
                                                     height,
                                                     1,  // align
                                                     1,  // size_align
                                                     border);
    
    if (!img_ptr) {
        return 0;  // Allocation failed
    }
    
    // 3. Set a rectangle in the image using part of the fuzzing data
    //    Use modulo to ensure coordinates are within bounds
    if (size >= 20) {
        unsigned int x = data[10] % (width + 2 * border);
        unsigned int y = data[11] % (height + 2 * border);
        unsigned int w = 8 + (data[12] % 16);
        unsigned int h = 8 + (data[13] % 16);
        
        // Ensure rectangle stays within image bounds
        if (x + w <= width + 2 * border && y + h <= height + 2 * border) {
            aom_img_set_rect(img_ptr, x, y, w, h, border);
        }
    }
    
    // 4. Initialize a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init(&decoder, 
                                                     iface, 
                                                     &cfg, 
                                                     0);
    
    if (init_result == AOM_CODEC_OK) {
        // 5. Set frame buffer functions
        FrameBufferData fb_data;
        aom_codec_set_frame_buffer_functions(&decoder,
                                            get_frame_buffer_callback,
                                            release_frame_buffer_callback,
                                            &fb_data);
        
        // 6. Try to use codec control with various control IDs
        //    Use modulo to select different control IDs safely
        if (size >= 30) {
            int control_id = 1 + (data[14] % 100);  // Use a safe range
            aom_codec_control(&decoder, control_id);
        }
        
        // Try to decode the remaining data as AV1 stream
        // Skip the part we already used
        size_t data_used = (uleb_length < 15) ? 15 : uleb_length;
        if (data_used < size) {
            const uint8_t *av1_data = data + data_used;
            size_t av1_size = size - data_used;
            
            aom_codec_decode(&decoder, av1_data, av1_size, nullptr);
            
            // Try to get a frame
            aom_codec_iter_t iter = nullptr;
            aom_image_t *decoded_frame = aom_codec_get_frame(&decoder, &iter);
            
            // If we got a frame, check for metadata
            if (decoded_frame) {
                size_t num_metadata = aom_img_num_metadata(decoded_frame);
                for (size_t i = 0; i < num_metadata; i++) {
                    const aom_metadata_t *metadata = aom_img_get_metadata(decoded_frame, i);
                    if (metadata) {
                        // Create a copy to free
                        aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                            metadata->type,
                            metadata->payload,
                            metadata->sz,
                            metadata->insert_flag);
                        
                        if (metadata_copy) {
                            // 7. Free the metadata
                            aom_img_metadata_free(metadata_copy);
                        }
                    }
                }
            }
        }
        
        // Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 8. Clean up the allocated image
    aom_img_free(img_ptr);
    
    return 0;
}