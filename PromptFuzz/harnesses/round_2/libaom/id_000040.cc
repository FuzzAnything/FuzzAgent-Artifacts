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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"aom_uleb_encode_fixed_size":[[83,7,83,30,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers from libaom
extern "C" {
}

// Frame buffer callback functions
static int  id_000040_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Simple implementation that allocates memory
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int  id_000040_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Part 1: ULEB operations
    uint64_t decoded_value = 0;
    size_t uleb_length = 0;
    
    // Decode ULEB value from input data
    if (aom_uleb_decode(data, size, &decoded_value, &uleb_length) == 0) {
        // Calculate available space after decoded ULEB
        size_t available_after_uleb = size - uleb_length;
        
        if (available_after_uleb > 0) {
            // Encode the decoded value back with fixed size
            uint8_t *encoded_buffer = (uint8_t*)malloc(size);
            size_t encoded_size = 0;
            
            if (encoded_buffer) {
                // Pad to 4 bytes for demonstration
                aom_uleb_encode_fixed_size(decoded_value, size, 4, 
                                          encoded_buffer, &encoded_size);
                free(encoded_buffer);
            }
        }
    }
    
    // Part 2: AV1 decoding with the input data
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (!decoder_iface) {
	return 0;
	}
	
    
    // Get interface name (just to use the API)
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t ret = aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &cfg, 
                                                0, AOM_DECODER_ABI_VERSION);
    
    if (ret != AOM_CODEC_OK) {
        // Clean up and return if decoder initialization fails
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Set a decoder option (example: thread count)
    aom_codec_set_option(&codec_ctx, "threads", "1");
    
    // Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&codec_ctx,  id_000040_get_frame_buffer, 
                                          id_000040_release_frame_buffer, NULL);
    
    // Peek stream info before decoding
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // Decode the input data
    ret = aom_codec_decode(&codec_ctx, data, size, NULL);
    
    if (ret == AOM_CODEC_OK) {
        // Get stream info after decoding
        aom_codec_get_stream_info(&codec_ctx, &si);
        
        // Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        
        // Use a control command (example: get last decoded frame's corruption status)
        int corrupted = 0;
        aom_codec_control(&codec_ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);
    }
    
    // Part 3: Image manipulation operations
    // Allocate a dummy image for testing image APIs
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 16);
    
    if (img) {
        // Get plane height
        int plane_height = aom_img_plane_height(img, 0);
        
        // Set a rectangle on the image
        aom_img_set_rect(img, 0, 0, 32, 32, 0);
        
        // Remove metadata (even though this image likely doesn't have any)
        aom_img_remove_metadata(img);
        
        // Free the image
        aom_img_free(img);
    }
    
    // Clean up decoder
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}