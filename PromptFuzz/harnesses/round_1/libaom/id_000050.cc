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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 22.5, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"realloc_frame_buffer_aligned":[[111,31,111,61,0,0,4,0]],"read_uncompressed_header":[[4893,15,4899,21,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_destroy","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_num_metadata","aom_codec_get_frame","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_num_metadata","aom_codec_get_frame","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Frame buffer callbacks
extern "C" {
int get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = (uint8_t*)calloc(1, 4096); // Simple allocation
    fb->size = 4096;
    fb->priv = nullptr;
    return 0;
}

int release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
	return 0;
	}
	
    
    // 2. Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
        return 0;
    }
    
    // 3. Set frame buffer functions
    err = aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, release_frame_buffer, nullptr);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // 4. Decode input data
    err = aom_codec_decode(&ctx, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 5. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    while (frame) {
        // 6. Try to get preview frame (may be nullptr)
        const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
        (void)preview;
        
        // 7. Check metadata count
        size_t metadata_count = aom_img_num_metadata(frame);
        (void)metadata_count;
        
        frame = aom_codec_get_frame(&ctx, &iter);
    }
    
    // 8. Demonstrate aom_img_alloc and aom_img_wrap
    aom_image_t dummy_img;
    aom_img_alloc(&dummy_img, AOM_IMG_FMT_I420, 64, 64, 16);
    
    unsigned char img_data[64 * 64 * 3 / 2];
    aom_image_t wrapped_img;
    aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 64, 64, 16, img_data);
    
    // 9. Create and free metadata
    aom_metadata_t *metadata = aom_img_metadata_alloc(1, data, size % 100, AOM_MIF_ANY_FRAME);
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // 10. Clean up
    aom_img_free(&dummy_img);
    // Note: aom_img_wrap doesn't own the data, so we don't call aom_img_free on wrapped_img
    aom_codec_destroy(&ctx);
    
    return 0;
}