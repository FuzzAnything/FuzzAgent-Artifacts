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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 13
//<Quality> {"density":6,"unique_branches":{"read_metadata_itut_t35":[[643,7,643,36,0,0,4,0],[651,7,651,30,0,0,4,1]],"ctrl_get_last_ref_updates":[[1046,7,1046,18,0,0,4,0],[1047,9,1047,26,0,0,4,1]],"read_uncompressed_header":[[4805,13,4805,75,0,0,4,0],[4806,15,4806,59,0,0,4,0],[4806,15,4806,59,0,0,4,1],[4807,16,4809,21,0,0,4,0],[4807,16,4809,21,0,0,4,1],[4810,16,4812,21,0,0,4,0],[4810,16,4812,21,0,0,4,1]],"read_film_grain_params":[[4067,44,4067,76,0,0,4,1],[4102,45,4102,77,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to process a byte stream of libaom's output data:

*/


// Include necessary libaom headers
extern "C" {
}

// Callback functions for frame buffer management
static int  id_000042_get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Allocate frame buffer with requested size
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = nullptr;
    return 0;
}

static int  id_000042_release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb && fb->data) {
        free(fb->data);
        fb->data = nullptr;
        fb->size = 0;
        fb->priv = nullptr;
    }
    return 0;
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 20) {
        return 0; // Not enough data to process
    }

    // Variables for cleanup
    aom_codec_ctx_t codec = {};
    aom_image_t *img = nullptr;
    aom_metadata_t *metadata = nullptr;
    
    // 1. Decode a ULEB-encoded value from the input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (aom_uleb_decode(data, size, &uleb_value, &uleb_length) == 0) {
        // Successfully decoded ULEB value
        // Use the decoded value for image dimensions and border
        unsigned int width = (uleb_value >> 32) & 0xFFFF;
        unsigned int height = (uleb_value >> 16) & 0xFFFF;
        unsigned int border = (uleb_value & 0xFF);
        
        // Ensure minimum dimensions
        if (width < 16) width = 16;
        if (height < 16) height = 16;
        if (border > 100) border = 10;
        
        // 2. Allocate image with border using the decoded parameters
        img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                       width, height, 1, 1, border);
        if (img) {
            // 3. Set a rectangular region on the image
            // Use part of the ULEB value for rectangle coordinates
            unsigned int x = (uleb_value >> 24) & 0xFF;
            unsigned int y = (uleb_value >> 16) & 0xFF;
            unsigned int rect_w = (uleb_value >> 8) & 0xFF;
            unsigned int rect_h = uleb_value & 0xFF;
            
            // Ensure rectangle fits within image
            if (rect_w == 0) rect_w = 16;
            if (rect_h == 0) rect_h = 16;
            if (x + rect_w > width) x = 0;
            if (y + rect_h > height) y = 0;
            
            aom_img_set_rect(img, x, y, rect_w, rect_h, border);
            
            // 4. Create a dummy metadata object
            uint8_t dummy_data[] = {1, 2, 3, 4, 5};
            metadata = aom_img_metadata_alloc(1, dummy_data, sizeof(dummy_data), 
                                             AOM_MIF_ANY_FRAME);
        }
    }
    
    // 5. Initialize AV1 decoder
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&codec, iface, &cfg, 0, AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
        // 6. Set frame buffer functions
        aom_codec_set_frame_buffer_functions(&codec,  id_000042_get_frame_buffer_cb, 
                                             id_000042_release_frame_buffer_cb, nullptr);
        
        // 7. Use aom_codec_control with a safe control ID
        // AOMD_GET_LAST_REF_UPDATES is a read-only control that should be safe
        int last_ref_updates = 0;
        aom_codec_control(&codec, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);
        
        // 8. Try to decode the input data (or a portion of it)
        // Use the remaining data after ULEB decode
        size_t decode_offset = (uleb_length < size) ? uleb_length : 0;
        size_t decode_size = size - decode_offset;
        const uint8_t *decode_data = data + decode_offset;
        
        if (decode_size > 0) {
            aom_codec_decode(&codec, decode_data, decode_size, nullptr);
            
            // Try to get decoded frames
            aom_codec_iter_t iter = nullptr;
            aom_image_t *decoded_frame;
            while ((decoded_frame = aom_codec_get_frame(&codec, &iter)) != nullptr) {
                // We could process the decoded frame here, but for fuzzing
                // we just want to exercise the code paths
            }
        }
    }
    
    // Cleanup in reverse order of creation
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    if (img) {
        aom_img_free(img);
    }
    
    if (codec.iface) {
        aom_codec_destroy(&codec);
    }
    
    return 0;
}