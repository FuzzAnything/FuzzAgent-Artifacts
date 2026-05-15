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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"ctrl_get_frame_corrupted":[[1258,9,1258,26,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Define a simple frame buffer callback structure
struct FrameBufferCallback {
    void* cb_priv;
};

// Callback functions for frame buffer handling
int  id_000041_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    // Simple allocation
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = nullptr;
    return 0;
}

int  id_000041_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    if (fb->data) {
        free(fb->data);
        fb->data = nullptr;
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (!data || size < 4) {
	return 0;
	}
	
    
    // 1. Use aom_uleb_decode to decode a ULEB value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    const uint8_t* uleb_start = data;
    size_t uleb_available = size;
    
    // Try to decode a ULEB value from the beginning of input
    int uleb_result = aom_uleb_decode(uleb_start, uleb_available, &uleb_value, &uleb_length);
    
    // 2. Create an image with border using aom_img_alloc_with_border
    aom_image_t* img = nullptr;
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 16;
    unsigned int size_align = 1;
    unsigned int border = 4;
    
    // Use the ULEB value to influence border size (modulo to keep it reasonable)
    if (uleb_result == 0 && uleb_available > uleb_length) {
        border = (uleb_value % 16) + 1;  // Border between 1-16
    }
    
    img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                    width, height, align, size_align, border);
    if (!img) {
        return 0;
    }
    
    // 3. Use aom_img_set_rect to set a rectangle in the image
    if (size > uleb_length + 4) {
        // Use input data to determine rectangle parameters
        unsigned int x = data[uleb_length] % (width - 8);
        unsigned int y = data[uleb_length + 1] % (height - 8);
        unsigned int w = (data[uleb_length + 2] % (width - x - 1)) + 1;
        unsigned int h = (data[uleb_length + 3] % (height - y - 1)) + 1;
        
        int set_rect_result = aom_img_set_rect(img, x, y, w, h, border);
        (void)set_rect_result; // Result intentionally unused for fuzzing
    }
    
    // 4. Create a decoder context and use aom_codec_set_frame_buffer_functions
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    FrameBufferCallback fb_callback;
    fb_callback.cb_priv = nullptr;
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, decoder_iface, 
                                                        &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result == AOM_CODEC_OK) {
        // Set frame buffer callbacks
        aom_codec_err_t fb_result = aom_codec_set_frame_buffer_functions(
            &decoder,  id_000041_get_frame_buffer,  id_000041_release_frame_buffer, &fb_callback);
        (void)fb_result; // Result intentionally unused for fuzzing
        
        // 5. Use aom_codec_control with some control IDs
        // Try some common decoder controls
        int ctrl_id = 0;
        
        // Use input byte to select control ID (modulo to safe range)
        if (size > uleb_length) {
            ctrl_id = data[uleb_length] % 100; // Arbitrary safe range
        }
        
        // Try to call control - note this may fail for invalid ctrl_id
        aom_codec_err_t ctrl_result = aom_codec_control(&decoder, ctrl_id);
        (void)ctrl_result; // Result intentionally unused for fuzzing
        
        // Try another control that might be valid
        ctrl_result = aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED);
        (void)ctrl_result;
        
        // Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 6. Create and free metadata using aom_img_metadata_free
    if (img && size > uleb_length + 8) {
        // Create some dummy metadata
        uint32_t metadata_type = 1; // Arbitrary type
        size_t metadata_size = (size - uleb_length) % 256; // Limit size
        const uint8_t* metadata_data = data + uleb_length;
        
        // Allocate metadata
        aom_metadata_t* metadata = aom_img_metadata_alloc(
            metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
        
        if (metadata) {
            // Free the metadata using the required API
            aom_img_metadata_free(metadata);
        }
    }
    
    // 7. Clean up allocated image
    if (img) {
        aom_img_free(img);
    }
    
    return 0;
}