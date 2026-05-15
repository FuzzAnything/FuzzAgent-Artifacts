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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_error_detail","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_error_detail","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode an AV1 bitstream:

*/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000036_get_frame_buffer(void *user_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000036_release_frame_buffer(void *user_priv, 
                               aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
	return 0;
	}
	
    
    // 2. Get interface name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name;  // Prevent unused warning
    
    // 3. Get build configuration
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    // Initialize decoder
    aom_codec_ctx_t codec;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    // 4. Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // 7. Get error string
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
        return 0;
    }
    
    // 5. Set frame buffer functions
    err = aom_codec_set_frame_buffer_functions(&codec,  id_000036_get_frame_buffer, 
                                               id_000036_release_frame_buffer, NULL);
    if (err != AOM_CODEC_OK) {
        // 13. Get error detail
        const char *err_detail = aom_codec_error_detail(&codec);
        (void)err_detail;
    }
    
    // 6. Try to get global headers (may return NULL for some streams)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
    (void)global_headers;
    
    // 8. Set CX data buffer (for encoder context, but we call it anyway)
    aom_fixed_buf_t dummy_buf;
    dummy_buf.buf = nullptr;
    dummy_buf.sz = 0;
    err = aom_codec_set_cx_data_buf(&codec, &dummy_buf, 0, 0);
    (void)err;
    
    // 9. Decode the input data
    err = aom_codec_decode(&codec, data, size, NULL);
    if (err != AOM_CODEC_OK) {
        // Get error string again
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
    }
    
    aom_codec_iter_t iter = nullptr;
    
    // 11. Get decoded frames
    while (true) {
        aom_image_t *img = aom_codec_get_frame(&codec, &iter);
        if (!img) break;
        
        // 12. Get metadata if exists
        size_t metadata_count = aom_img_num_metadata(img);
        for (size_t i = 0; i < metadata_count; ++i) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            (void)metadata;
        }
        
        // 14. Get plane dimensions
        for (int plane = 0; plane < 3; ++plane) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            (void)plane_width;
            (void)plane_height;
            
            // 10. Set a rectangle on the image (safe parameters)
            if (plane_width > 0 && plane_height > 0) {
                unsigned int x = 0;
                unsigned int y = 0;
                unsigned int w = (unsigned int)plane_width;
                unsigned int h = (unsigned int)plane_height;
                unsigned int border = 0;
                
                // Ensure rectangle is within bounds
                if (w > 0 && h > 0) {
                    int result = aom_img_set_rect(img, x, y, w, h, border);
                    (void)result;
                }
            }
        }
    }
    
    // 16. Clean up
    aom_codec_destroy(&codec);
    
    return 0;
}