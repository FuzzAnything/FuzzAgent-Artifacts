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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_set_rect","aom_img_remove_metadata","aom_uleb_encode_fixed_size","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_set_rect","aom_img_remove_metadata","aom_uleb_encode_fixed_size","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Initialize decoder with default config
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Check error using aom_codec_error
        const char *error_str = aom_codec_error(&codec_ctx);
        (void)error_str; // Prevent unused variable warning
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // 2. Get stream information from input data
    aom_codec_stream_info_t stream_info = {0};
    if (size > 0) {
        err = aom_codec_get_stream_info(&codec_ctx, &stream_info);
        (void)err; // We'll check errors later with aom_codec_error
    }
    
    // 3. Decode the input data
    if (size > 0) {
        err = aom_codec_decode(&codec_ctx, data, size, NULL);
        
        // Check error using aom_codec_error
        const char *error_detail = aom_codec_error(&codec_ctx);
        (void)error_detail; // Prevent unused variable warning
    }
    
    // 4. Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = aom_codec_get_frame(&codec_ctx, &iter);
    
    if (img != NULL) {
        // 5. Use aom_img_set_rect to modify part of the image
        // We'll set a small rectangle at the top-left corner
        unsigned int rect_w = img->d_w > 10 ? 10 : img->d_w;
        unsigned int rect_h = img->d_h > 10 ? 10 : img->d_h;
        
        if (rect_w > 0 && rect_h > 0) {
            // Create a simple pattern for the rectangle
            // We'll use part of the input data for the rectangle content
            size_t pattern_size = rect_w * rect_h;
            if (pattern_size > 0) {
                uint8_t *pattern = (uint8_t*)malloc(pattern_size);
                if (pattern != NULL) {
                    // Fill pattern with data from input (or zeros if insufficient)
                    size_t copy_size = pattern_size < size ? pattern_size : size;
                    if (copy_size > 0) {
                        memcpy(pattern, data, copy_size);
                    }
                    if (copy_size < pattern_size) {
                        memset(pattern + copy_size, 0, pattern_size - copy_size);
                    }
                    
                    // Set rectangle in Y plane (plane 0)
                    if (img->planes[0] != NULL && img->stride[0] >= rect_w) {
                        for (unsigned int y = 0; y < rect_h; y++) {
                            memcpy(img->planes[0] + y * img->stride[0], 
                                   pattern + y * rect_w, rect_w);
                        }
                    }
                    
                    free(pattern);
                }
                
                // Apply the rectangle to the image
                aom_img_set_rect(img, 0, 0, rect_w, rect_h, 0);
            }
        }
        
        // 6. Remove metadata from the image
        aom_img_remove_metadata(img);
    }
    
    // 7. Use aom_uleb_encode_fixed_size
    // We'll encode a value derived from the input size
    if (size > 0) {
        uint64_t value = 0;
        // Use first 8 bytes of input as value, or size if input is small
        if (size >= sizeof(uint64_t)) {
            memcpy(&value, data, sizeof(uint64_t));
        } else {
            value = size;
        }
        
        size_t available = 16; // Buffer for encoded value
        uint8_t coded_buffer[16];
        size_t coded_size = 0;
        
        // Try to encode with fixed size
        aom_uleb_encode_fixed_size(value, available, 8, coded_buffer, &coded_size);
    }
    
    // 8. Clean up
    aom_codec_destroy(&codec_ctx);
    
    // 9. Additional cleanup if image was allocated separately
    if (img != NULL) {
        // Note: We don't call aom_img_free because the image is owned by the decoder
        // and will be freed when the decoder is destroyed
    }
    
    return 0;
}