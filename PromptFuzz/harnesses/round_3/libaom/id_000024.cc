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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_set_cx_data_buf","aom_codec_error_detail","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_set_cx_data_buf","aom_codec_error_detail","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000024_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000024_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Get decoder capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    
    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 3. Set frame buffer functions
    void *cb_priv = nullptr;
    if (aom_codec_set_frame_buffer_functions(&decoder,  id_000024_get_frame_buffer,  id_000024_release_frame_buffer, cb_priv) != AOM_CODEC_OK) {
        // Continue anyway - not all decoders support this
    }
    
    // 4. Get stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info) == AOM_CODEC_OK) {
        aom_codec_get_stream_info(&decoder, &stream_info);
    }
    
    // 5. Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 6. Try to get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // 7. Create a test image for metadata operations
    aom_image_t test_img;
    memset(&test_img, 0, sizeof(test_img));
    
    // Allocate a small test image
    if (aom_img_alloc(&test_img, AOM_IMG_FMT_I420, 64, 64, 1)) {
        // 8. Add metadata to the test image
        uint32_t metadata_type = 1;
        uint8_t metadata_data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        aom_img_add_metadata(&test_img, metadata_type, metadata_data, 10, AOM_MIF_ANY_FRAME);
        
        // 9. Get metadata back
        const aom_metadata_t *retrieved_metadata = aom_img_get_metadata(&test_img, 0);
        
        // 10. Create a separate metadata object and free it
        if (retrieved_metadata) {
            aom_metadata_t *copied_metadata = aom_img_metadata_alloc(
                retrieved_metadata->type,
                retrieved_metadata->payload,
                retrieved_metadata->sz,
                AOM_MIF_ANY_FRAME
            );
            
            if (copied_metadata) {
                aom_img_metadata_free(copied_metadata);
            }
        }
        
        // Clean up test image
        aom_img_free(&test_img);
    }
    
    // 11. Try to set cx data buffer (encoder function - may fail with decoder)
    aom_fixed_buf_t buf;
    buf.buf = (void *)data;
    buf.sz = size;
    aom_codec_set_cx_data_buf(&decoder, &buf, 0, 0); // This will likely fail for decoder
    
    // 12. Get error detail if any
    const char *error_detail = aom_codec_error_detail(&decoder);
    if (error_detail) {
        // Just check it's not null - we don't need to use it
    }
    
    // 13. Iterate through decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *img;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Got a decoded frame - we could process it here
        // For fuzzing, we just want to exercise the API
    }
    
    // 14. Clean up decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}