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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Dummy frame buffer callback functions
static int  id_000023_dummy_get_frame_buffer(void *priv, size_t min_size, 
                                 aom_codec_frame_buffer_t *fb) {
    return -1; // Indicate failure
}

static int  id_000023_dummy_release_frame_buffer(void *priv, 
                                     aom_codec_frame_buffer_t *fb) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Event: Process encoded AV1 stream to extract preview frame with metadata
    
    // 1. Get codec capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning

    // 2. Initialize decoder
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &cfg, 0, 
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&codec_ctx,  id_000023_dummy_get_frame_buffer, 
                                         id_000023_dummy_release_frame_buffer, nullptr);

    // 4. Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Try to peek stream info first
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info) != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // 5. Decode the input data
    aom_codec_decode(&codec_ctx, data, size, nullptr);
    
    // 6. Get error detail if any
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    (void)error_detail; // Use error_detail to avoid unused variable warning

    // 7. Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
    
    if (preview_frame) {
        // 8. Create metadata
        uint32_t metadata_type = 1; // Example type
        // Use small portion of data for metadata, ensure we don't overflow
        size_t metadata_size = (size > 100) ? 100 : size;
        
        // Allocate and copy metadata data
        uint8_t *metadata_data = (uint8_t*)malloc(metadata_size);
        if (metadata_data) {
            memcpy(metadata_data, data, metadata_size);
            
            // 9. Add metadata to image (need non-const image)
            aom_image_t *writable_img = (aom_image_t*)preview_frame;
            aom_img_add_metadata(writable_img, metadata_type, metadata_data, 
                                metadata_size, AOM_MIF_ANY_FRAME);
            
            free(metadata_data);
            
            // 10. Get metadata back
            size_t num_metadata = aom_img_num_metadata(preview_frame);
            if (num_metadata > 0) {
                const aom_metadata_t *retrieved_metadata = 
                    aom_img_get_metadata(preview_frame, 0);
                
                // 11. Free metadata (create a copy to free)
                if (retrieved_metadata) {
                    // Allocate new metadata structure to free
                    aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                        retrieved_metadata->type,
                        retrieved_metadata->payload,
                        retrieved_metadata->sz,
                        retrieved_metadata->insert_flag);
                    
                    if (metadata_copy) {
                        aom_img_metadata_free(metadata_copy);
                    }
                }
            }
        }
    }

    // 12. Set cx data buffer (for encoder side, but we'll call it anyway)
    aom_fixed_buf_t buf;
    buf.buf = (void*)data; // Cast away const for this API
    buf.sz = size;
    
    // Initialize encoder to use set_cx_data_buf
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        if (aom_codec_enc_init_ver(&enc_ctx, encoder_iface, &enc_cfg, 0, 
                                   AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            // Call set_cx_data_buf
            aom_codec_set_cx_data_buf(&enc_ctx, &buf, 0, 0);
            aom_codec_destroy(&enc_ctx);
        }
    }

    // 13. Clean up decoder
    aom_codec_destroy(&codec_ctx);

    return 0;
}