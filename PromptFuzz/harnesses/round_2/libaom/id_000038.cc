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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_remove_metadata","aom_img_set_rect","aom_codec_control","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_remove_metadata","aom_img_set_rect","aom_codec_control","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers (they would be included in the actual build environment)
extern "C" {
}

// Custom frame buffer callbacks to satisfy aom_codec_set_frame_buffer_functions
static int  id_000038_get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Simple implementation that allocates memory
    uint8_t *buf = (uint8_t *)malloc(min_size);
    if (!buf) return -1;
    
    fb->data = buf;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int  id_000038_release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	  // Need minimum data
    
    // 1. ULEB decode/encode operations
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    uint8_t uleb_encoded[16];
    size_t uleb_encoded_size = 0;
    
    // Try to decode a ULEB value from the beginning of input
    int uleb_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    
    // Encode it back with fixed size
    if (uleb_result == 0 && uleb_length > 0) {
        aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_encoded), 
                                   uleb_length + 2, uleb_encoded, &uleb_encoded_size);
    }
    
    // 2. Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // 4. Get file descriptors if needed (though not directly used by libaom APIs)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = out_file ? fuzz_fileno(out_file) : -1;
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // 5. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Get interface name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name;  // Mark as used
    
    // Configure decoder
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder with version 10 (AOM_DECODER_ABI_VERSION)
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 
                                                        0, 10);
    
    if (init_result == AOM_CODEC_OK) {
        // 6. Set decoder options
        aom_codec_set_option(&decoder, "tile-columns", "0");
        
        // 7. Set frame buffer functions
        void *cb_priv = NULL;
        aom_codec_set_frame_buffer_functions(&decoder,  id_000038_get_frame_buffer_cb, 
                                            id_000038_release_frame_buffer_cb, cb_priv);
        
        // 8. Peek stream info
        aom_codec_stream_info_t stream_info = {0};
        aom_codec_peek_stream_info(iface, data, size, &stream_info);
        
        // 9. Decode the input data
        aom_codec_decode(&decoder, data, size, NULL);
        
        // 10. Get stream info after decoding
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // 11. Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
        
        // 12. Create a test image
        aom_image_t *test_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                              stream_info.w ? stream_info.w : 64,
                                              stream_info.h ? stream_info.h : 64,
                                              16);
        
        if (test_img) {
            // 13. Get plane height
            int plane_height = aom_img_plane_height(test_img, 0);
            (void)plane_height;  // Mark as used
            
            // 14. Remove metadata (even if none exists)
            aom_img_remove_metadata(test_img);
            
            // 15. Set a rectangle in the image
            if (test_img->d_w > 10 && test_img->d_h > 10) {
                aom_img_set_rect(test_img, 5, 5, 10, 10, 0);
            }
            
            // 16. Try a decoder control (AV1D_GET_FRAME_SIZE is example control)
            // Note: Actual control IDs are defined in aom/aomdx.h
            // Using a generic control to demonstrate
            aom_codec_control(&decoder, 0, NULL);
            
            // 17. Free the test image
            aom_img_free(test_img);
        }
        
        // 18. Destroy decoder
        aom_codec_destroy(&decoder);
    }
    
    // Clean up files
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}