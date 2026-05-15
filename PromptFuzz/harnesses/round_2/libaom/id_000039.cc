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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"ctrl_get_frame_corrupted":[[1257,7,1257,16,0,0,4,1]],"ctrl_get_last_ref_updates":[[1046,7,1046,18,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_control","aom_codec_control","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_control","aom_codec_control","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Custom frame buffer callbacks
static int  id_000039_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
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
	 // Too small for meaningful processing

    // 1. ULEB decode from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t uleb_offset = size > 10 ? 10 : 0;
    if (aom_uleb_decode(data + uleb_offset, 
                       size - uleb_offset, 
                       &uleb_value, 
                       &uleb_length) == 0) {
        // 2. ULEB encode with fixed size
        uint8_t coded_value[16];
        size_t coded_size = 0;
        aom_uleb_encode_fixed_size(uleb_value, 
                                   sizeof(coded_value), 
                                   8, 
                                   coded_value, 
                                   &coded_size);
    }

    // 3. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.allow_lowbitdepth = 1;
    cfg.threads = 1;
    
    // 4. Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // 5. Get interface name
    const char *iface_name = aom_codec_iface_name(iface);
    
    // 6. Initialize decoder with version
    if (aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION)) {
        return 0; // Decoder initialization failed
    }

    // 7. Set decoder options
    aom_codec_set_option(&decoder, "tile-rows", "1");
    
    // 8. Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder, 
                                         id_000039_get_frame_buffer, 
                                         id_000039_release_frame_buffer, 
                                        nullptr);

    // 9. Peek stream info
    aom_codec_stream_info_t stream_info = {0};
    if (aom_codec_peek_stream_info(iface, data, size, &stream_info) == 0) {
        // 10. Get stream info after decoding
        aom_codec_get_stream_info(&decoder, &stream_info);
    }

    // 11. Decode input data
    aom_codec_decode(&decoder, data, size, nullptr);

    // 12. Get preview frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&decoder);

    // 13. Create an image
    aom_image_t *img = aom_img_alloc(nullptr, 
                                    AOM_IMG_FMT_I420, 
                                    64, 64, 16);
    if (img) {
        // 14. Get plane height
        int plane_height = aom_img_plane_height(img, 0);
        
        // 15. Set rectangle on image
        aom_img_set_rect(img, 0, 0, 32, 32, 0);
        
        // 16. Remove metadata
        aom_img_remove_metadata(img);
        
        // 17. Free the image
        aom_img_free(img);
    }

    // 18. Codec control operations
    aom_codec_control(&decoder, AOMD_GET_LAST_REF_UPDATES, nullptr);
    aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, nullptr);

    // 19. Clean up decoder
    aom_codec_destroy(&decoder);

    return 0;
}