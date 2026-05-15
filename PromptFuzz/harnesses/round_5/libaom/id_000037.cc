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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 133, nr_unique_branch: 24
//<Quality> {"density":19,"unique_branches":{"write_sgrproj_filter":[[1898,7,1898,24,0,0,4,0]],"encode_frame_to_data_rate":[[4345,7,4345,48,0,0,4,0],[4348,9,4348,35,0,0,4,0],[4351,11,4351,54,0,0,4,1],[4366,11,4366,40,0,0,4,1],[4435,7,4435,48,0,0,4,0],[4436,21,4436,35,0,0,4,0],[4436,21,4436,35,0,0,4,1],[4437,11,4437,56,0,0,4,0]],"write_uncompressed_header_obu":[[2980,9,2980,50,0,0,4,0]],"setup_processing_stripe_boundary":[[306,9,306,19,0,0,4,0],[319,9,319,19,0,0,4,0],[205,27,205,32,13,0,4,1],[205,27,205,32,14,0,4,1],[205,27,205,32,15,0,4,1],[205,27,205,32,16,0,4,1],[205,27,205,32,17,0,4,1],[205,27,205,32,18,0,4,1]],"restore_processing_stripe_boundary":[[368,9,368,19,0,0,4,0],[376,9,376,19,0,0,4,0],[381,11,381,65,0,0,4,0],[205,27,205,32,10,0,4,1],[205,27,205,32,12,0,4,1]],"write_sequence_header":[[2655,9,2655,50,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_codec_encode","aom_codec_get_global_headers","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_codec_encode","aom_codec_get_global_headers","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

// Frame buffer get callback
int  id_000037_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Resize buffer if needed
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->size;
    return 0;
}

// Frame buffer release callback
int  id_000037_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    // Nothing to do here - we manage the buffer in FrameBufferData
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. ULEB decode from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    const uint8_t *uleb_data = data;
    size_t uleb_available = size > 50 ? 50 : size;
    
    int decode_result = aom_uleb_decode(uleb_data, uleb_available, &uleb_value, &uleb_length);
    
    // 2. ULEB encode with fixed size
    uint8_t encoded_uleb[10];
    size_t encoded_size = 0;
    size_t pad_to_size = 5;  // Fixed padding size
    
    if (decode_result == 0 && uleb_available >= uleb_length + pad_to_size) {
        aom_uleb_encode_fixed_size(uleb_value, uleb_available - uleb_length, 
                                   pad_to_size, encoded_uleb, &encoded_size);
    }
    
    // 3. Create codec context for encoding
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set small dimensions for fuzzing
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;
    
    // Initialize encoder
    if (aom_codec_enc_init(&encoder, encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 4. Create a simple image for encoding
    aom_image_t img;
    unsigned int align = 32;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, align);
    
    // Fill image planes with fuzz data (simplified)
    size_t data_offset = uleb_length;
    size_t remaining = size - data_offset;
    
    if (remaining > 0) {
        // Fill Y plane (simplified - in real usage would need proper YUV layout)
        size_t y_size = 64 * 64;
        if (y_size > 0 && data_offset < size) {
            size_t copy_size = y_size < remaining ? y_size : remaining;
            memcpy(img.planes[0], data + data_offset, copy_size);
            data_offset += copy_size;
            remaining = size - data_offset;
        }
    }
    
    // 5. Set frame buffer functions for encoder
    FrameBufferData fb_data;
    aom_codec_set_frame_buffer_functions(&encoder,  id_000037_get_frame_buffer, 
                                         id_000037_release_frame_buffer, &fb_data);
    
    // 6. Encode the image
    aom_codec_encode(&encoder, &img, 0, 1, 0);
    
    // 7. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    
    // 8. Try to get metadata from image (if any)
    size_t num_metadata = aom_img_num_metadata(&img);
    if (num_metadata > 0) {
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(&img, i);
            // metadata could be used here
        }
    }
    
    // 9. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (aom_codec_dec_init(&decoder, decoder_iface, NULL, 0) == AOM_CODEC_OK) {
        // Try to decode some data if we have encoded output
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Try to decode the encoded frame
                aom_codec_decode(&decoder, 
                                static_cast<const uint8_t*>(pkt->data.frame.buf),
                                pkt->data.frame.sz, 
                                NULL);
                
                // Get decoded frame
                aom_codec_iter_t dec_iter = NULL;
                aom_image_t *decoded_img;
                while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != NULL) {
                    // Try to get metadata from decoded image
                    size_t dec_num_metadata = aom_img_num_metadata(decoded_img);
                    if (dec_num_metadata > 0) {
                        for (size_t i = 0; i < dec_num_metadata; i++) {
                            const aom_metadata_t *dec_metadata = aom_img_get_metadata(decoded_img, i);
                            // dec_metadata could be used here
                        }
                    }
                }
                break;  // Just process first frame
            }
        }
        
        // Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // Clean up
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    aom_img_free(&img);
    aom_codec_destroy(&encoder);
    
    return 0;
}