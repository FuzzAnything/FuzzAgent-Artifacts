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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_set_rect","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First decode ULEB encoded values from the input stream
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    
    // Try to decode a ULEB value from the beginning of the data
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 2. Create an image with border using decoded ULEB value for dimensions
    const unsigned int border = (uleb_value % 16) + 1; // Ensure border is at least 1
    const unsigned int width = (uleb_value % 128) + 16; // Ensure reasonable width
    const unsigned int height = (uleb_value % 128) + 16; // Ensure reasonable height
    
    aom_image_t img;
    aom_image_t *allocated_img = aom_img_alloc_with_border(
        &img, 
        AOM_IMG_FMT_I420, 
        width, 
        height, 
        1,  // align
        1,  // size_align  
        border
    );
    
    if (!allocated_img) {
        // If allocation fails, try with simpler parameters
        allocated_img = aom_img_alloc_with_border(
            &img,
            AOM_IMG_FMT_I420,
            16,  // default width
            16,  // default height
            1,
            1,
            1   // minimal border
        );
        
        if (!allocated_img) {
            return 0; // Early return if we can't allocate
        }
    }
    
    // 3. Set a rectangular region in the image using ULEB decoded values
    // Use modulo to ensure parameters are within bounds
    unsigned int rect_x = (uleb_value * 7) % width;
    unsigned int rect_y = (uleb_value * 11) % height;
    unsigned int rect_w = (uleb_value * 13) % (width - rect_x) + 1;
    unsigned int rect_h = (uleb_value * 17) % (height - rect_y) + 1;
    
    aom_img_set_rect(allocated_img, rect_x, rect_y, rect_w, rect_h, border);
    
    // 4. Get codec interface name (AV1 decoder)
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(iface);
    
    // 5. Create a codec context and get global headers
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(
        &codec_ctx, 
        iface, 
        &cfg, 
        0,  // flags
        AOM_DECODER_ABI_VERSION
    );
    
    if (res == AOM_CODEC_OK) {
        // Try to decode some data to get global headers
        if (size > 0) {
            // Use a small portion of data for decoding attempt
            size_t decode_size = size > 1024 ? 1024 : size;
            aom_codec_decode(&codec_ctx, data, decode_size, nullptr);
            
            // Get global headers (may be NULL for decoder)
            aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec_ctx);
            
            // If we got headers, we could process them, but for fuzzing we just check
            if (global_headers) {
                // Headers exist, but we don't need to do anything with them for fuzzing
            }
        }
        
        // Clean up codec context
        aom_codec_destroy(&codec_ctx);
    }
    
    // 6. Clean up allocated image
    aom_img_free(allocated_img);
    
    return 0;
}