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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_codec_get_stream_info","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_get_stream_info","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
	return 0;
	}
	
    
    // 2. Check decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Prevent unused variable warning
    
    // 3. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Use first part of data for stream info
    size_t peek_size = size > 100 ? 100 : size;
    aom_codec_err_t err = aom_codec_peek_stream_info(iface, data, peek_size, &stream_info);
    if (err != AOM_CODEC_OK) {
        // Convert error to string for debugging
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
    }
    
    // 4. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init_ver(&decoder, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
        return 0;
    }
    
    // 5. Set decoder option
    aom_codec_set_option(&decoder, "postproc", "0");
    
    // 6. Get stream info again using decoder context
    err = aom_codec_get_stream_info(&decoder, &stream_info);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
    }
    
    // 7. Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
    }
    
    // 8. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    int frame_count = 0;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_count++;
        
        // 9. Create a copy of the image using aom_img_alloc
        aom_image_t *allocated_img = aom_img_alloc(nullptr, 
                                                    img->fmt, 
                                                    img->d_w, 
                                                    img->d_h, 
                                                    16);
        if (allocated_img) {
            // Copy image data (simplified - just copy first plane for demo)
            if (allocated_img->planes[0] && img->planes[0]) {
                size_t copy_size = allocated_img->stride[0] * allocated_img->d_h;
                if (copy_size > 0) {
                    memcpy(allocated_img->planes[0], img->planes[0], 
                          copy_size < (size_t)(img->stride[0] * img->d_h) ? 
                          copy_size : (size_t)(img->stride[0] * img->d_h));
                }
            }
            
            // 10. Create another image with border
            aom_image_t *bordered_img = aom_img_alloc_with_border(nullptr,
                                                                   img->fmt,
                                                                   img->d_w,
                                                                   img->d_h,
                                                                   16,
                                                                   16,
                                                                   32);
            if (bordered_img) {
                // Copy image data to bordered image
                if (bordered_img->planes[0] && img->planes[0]) {
                    size_t copy_size = bordered_img->stride[0] * bordered_img->d_h;
                    if (copy_size > 0) {
                        memcpy(bordered_img->planes[0], img->planes[0],
                              copy_size < (size_t)(img->stride[0] * img->d_h) ?
                              copy_size : (size_t)(img->stride[0] * img->d_h));
                    }
                }
            }
            
            // 11. Test ULEB encoding/decoding with image dimensions
            uint64_t width = img->d_w;
            uint8_t encoded_value[10];
            size_t encoded_size = 0;
            
            // Encode width as ULEB with fixed size
            if (aom_uleb_encode_fixed_size(width, sizeof(encoded_value), 
                                           5, encoded_value, &encoded_size) == 0) {
                // Decode the ULEB value
                uint64_t decoded_value;
                size_t decoded_length = 0;
                if (aom_uleb_decode(encoded_value, encoded_size, 
                                   &decoded_value, &decoded_length) == 0) {
                    // Add metadata with encoded ULEB value
                    aom_img_add_metadata(allocated_img, 
                                         1, // Custom type
                                         encoded_value, 
                                         encoded_size,
                                         AOM_MIF_ANY_FRAME);
                }
            }
            
            // 12. Flip the allocated image
            aom_img_flip(allocated_img);
            
            // Clean up allocated images
            if (bordered_img) {
                aom_img_free(bordered_img);
            }
            aom_img_free(allocated_img);
        }
        
        // Break after processing a few frames to limit execution time
        if (frame_count >= 3) break;
    }
    
    // 13. Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}