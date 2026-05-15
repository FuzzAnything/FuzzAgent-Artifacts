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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_set_rect","aom_img_alloc","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_set_rect","aom_img_alloc","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 4) {
	return 0;
	}
	
    
    // Use fmemopen to create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize variables
    aom_codec_ctx_t decoder = {0}, encoder = {0};
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_enc_cfg_t enc_cfg = {0};
    aom_image_t *decoded_image = nullptr, *allocated_image = nullptr, *bordered_image = nullptr;
    aom_metadata_t *metadata = nullptr;
    
    // 1. Peek stream info to get basic stream properties
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 2. Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get stream info from decoder context
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // 4. Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 5. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = aom_codec_get_frame(&decoder, &iter);
    
    if (frame) {
        // 6. Allocate image with border
        const unsigned int border = 16;
        bordered_image = aom_img_alloc_with_border(nullptr, frame->fmt, 
                                                   frame->d_w, frame->d_h, 
                                                   32, 32, border);
        
        // 7. Set rectangle in bordered image
        if (bordered_image) {
            unsigned int x = size % bordered_image->d_w;
            unsigned int y = (size / 256) % bordered_image->d_h;
            unsigned int w = 64;
            unsigned int h = 64;
            aom_img_set_rect(bordered_image, x, y, w, h, border);
        }
        
        // 8. Allocate regular image
        allocated_image = aom_img_alloc(nullptr, frame->fmt, frame->d_w, frame->d_h, 32);
        
        // 9. Create dummy metadata to free later
        uint8_t dummy_data[] = {1, 2, 3, 4};
        metadata = aom_img_metadata_alloc(1, dummy_data, 4, AOM_MIF_ANY_FRAME);
        
        // 10. Initialize encoder
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, AOM_USAGE_GOOD_QUALITY) == AOM_CODEC_OK) {
            // Set basic encoder configuration
            enc_cfg.g_w = frame->d_w;
            enc_cfg.g_h = frame->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                // 11. Encode the frame
                aom_codec_encode(&encoder, frame, 0, 1, 0);
                
                // Get encoded data
                const aom_codec_cx_pkt_t *pkt = nullptr;
                aom_codec_iter_t enc_iter = nullptr;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Process encoded packet if needed
                        break;
                    }
                }
            }
        }
    }
    
    // 12. Free metadata
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // Cleanup
    if (allocated_image) {
        aom_img_free(allocated_image);
    }
    
    if (bordered_image) {
        aom_img_free(bordered_image);
    }
    
    if (aom_codec_destroy(&decoder) != AOM_CODEC_OK) {
        // Handle cleanup error if needed
    }
    
    if (encoder.iface) {
        aom_codec_destroy(&encoder);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}