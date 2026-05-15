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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"read_bitdepth":[[3976,14,3976,46,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_enc_init_ver","aom_img_free","aom_codec_get_global_headers","aom_codec_set_frame_buffer_functions","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_img_free","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_frame_buffer_functions","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback context
struct FrameBufferContext {
    std::vector<uint8_t> buffer;
    size_t allocated_size;
};

// Frame buffer get callback
int  id_000038_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    if (!ctx) return -1;
    
    // Allocate or resize buffer
    if (ctx->buffer.size() < min_size) {
        ctx->buffer.resize(min_size);
        ctx->allocated_size = min_size;
    }
    
    fb->data = ctx->buffer.data();
    fb->size = ctx->allocated_size;
    fb->priv = ctx;
    return 0;
}

// Frame buffer release callback
int  id_000038_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // Nothing to do here - we keep the buffer allocated
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Use aom_uleb_decode to decode a value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t decode_offset = 0;
    
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
        decode_offset = uleb_length;
    }
    
    // 2. Use aom_uleb_encode_fixed_size to encode a value
    uint8_t encoded_uleb[10];
    size_t encoded_size = 0;
    uint64_t value_to_encode = uleb_value + 1;  // Use decoded value + 1
    aom_uleb_encode_fixed_size(value_to_encode, sizeof(encoded_uleb), 
                               sizeof(encoded_uleb), encoded_uleb, &encoded_size);
    
    // 3. Create a simple image for encoding
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, align);
    
    // Add some metadata to the image
    uint8_t metadata_data[] = {1, 2, 3, 4, 5};
    aom_img_add_metadata(&img, 1, metadata_data, sizeof(metadata_data), 
                        AOM_MIF_ANY_FRAME);
    
    // 4. Get metadata from image using aom_img_get_metadata
    const aom_metadata_t* metadata = aom_img_get_metadata(&img, 0);
    
    // 5. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }
    
    // 6. Get global headers using aom_codec_get_global_headers
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Can use global_headers->buf and global_headers->sz if needed
    }
    
    // 7. Set frame buffer functions for encoder (though typically for decoder)
    FrameBufferContext fb_ctx;
    aom_codec_set_frame_buffer_functions(&encoder,  id_000038_get_frame_buffer, 
                                         id_000038_release_frame_buffer, &fb_ctx);
    
    // 8. Encode the image using aom_codec_encode
    // Use the decoded ULEB value as PTS if it's reasonable
    aom_codec_pts_t pts = (uleb_value < 1000000) ? uleb_value : 0;
    aom_codec_encode(&encoder, &img, pts, 1, 0);
    
    // Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Got an encoded frame
            break;
        }
    }
    
    // 9. Initialize decoder with the remaining input data
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0,
                              AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        aom_img_free(&img);
        return 0;
    }
    
    // Use remaining input data as compressed bitstream
    size_t remaining_size = size - decode_offset;
    const uint8_t* compressed_data = data + decode_offset;
    
    if (remaining_size > 0) {
        // Try to decode the data
        aom_codec_decode(&decoder, compressed_data, remaining_size, nullptr);
        
        // Try to get decoded frame
        aom_codec_iter_t dec_iter = nullptr;
        aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &dec_iter);
        
        if (decoded_img) {
            // Got a decoded image
            // Could process it further if needed
        }
    }
    
    // 10. Clean up
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    aom_img_free(&img);
    
    // Free global headers if allocated
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    return 0;
}