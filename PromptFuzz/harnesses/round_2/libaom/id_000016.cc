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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error_detail","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error_detail","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t allocated_size;
};

static int  id_000016_get_frame_buffer_cb(void* cb_priv, size_t min_size,
                               aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or reallocate buffer if needed
    if (fb_data->allocated_size < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->allocated_size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->allocated_size;
    
    // Set the private data that will be passed to release callback
    fb->priv = cb_priv;
    
    return 0;
}

static int  id_000016_release_frame_buffer_cb(void* cb_priv,
                                   aom_codec_frame_buffer_t* fb) {
    // No special cleanup needed since we're using vector
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data
    
    // 1. First use aom_uleb_encode APIs
    uint64_t uleb_value = 0;
    if (size >= sizeof(uint64_t)) {
        memcpy(&uleb_value, data, sizeof(uint64_t));
    }
    
    uint8_t uleb_buffer[16];
    size_t coded_size = 0;
    
    // Use aom_uleb_encode
    int uleb_result = aom_uleb_encode(uleb_value, sizeof(uleb_buffer),
                                      uleb_buffer, &coded_size);
    (void)uleb_result; // Result may be error, but we continue
    
    // Use aom_uleb_encode_fixed_size
    size_t fixed_coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 8,
                               uleb_buffer, &fixed_coded_size);
    
    // 2. Create encoder configuration and set it up
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder with AV1 interface
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
	return 0;
	}
	
    
    // Get default configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Adjust config based on input data
    cfg.g_w = 64 + (data[0] % 128);
    cfg.g_h = 64 + (data[1] % 128);
    cfg.rc_target_bitrate = 256 + (data[2] % 1000);
    
    // 3. Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 4. Set encoder configuration (can be same or modified)
    cfg.rc_min_quantizer = 10 + (data[3] % 40);
    cfg.rc_max_quantizer = 20 + (data[4] % 40);
    aom_codec_enc_config_set(&encoder, &cfg);
    
    // 5. Create and set output buffer for encoded data
    const size_t output_buf_size = 1024 * 1024;
    std::vector<uint8_t> output_buffer(output_buf_size);
    aom_fixed_buf_t cx_data_buf;
    cx_data_buf.buf = output_buffer.data();
    cx_data_buf.sz = output_buf_size;
    
    // Set the buffer with padding
    unsigned int pad_before = data[5] % 16;
    unsigned int pad_after = data[6] % 16;
    aom_codec_set_cx_data_buf(&encoder, &cx_data_buf, pad_before, pad_after);
    
    // 6. Create decoder and set frame buffer functions
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0,
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Set up frame buffer callbacks
    FrameBufferData fb_data;
    aom_codec_set_frame_buffer_functions(&decoder,  id_000016_get_frame_buffer_cb,
                                          id_000016_release_frame_buffer_cb, &fb_data);
    
    // 7. Try to decode input data as AV1 stream
    size_t decode_size = size > 8192 ? 8192 : size;
    aom_codec_decode(&decoder, data, decode_size, nullptr);
    
    // Get error detail if available
    const char* error_detail = aom_codec_error_detail(&decoder);
    (void)error_detail; // May be null
    
    // 8. Try to encode a simple image
    aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                     cfg.g_w, cfg.g_h, 16);
    if (img) {
        // Fill image with some data from input
        for (int plane = 0; plane < 3; plane++) {
            uint8_t* plane_ptr = img->planes[plane];
            int stride = img->stride[plane];
            int height = aom_img_plane_height(img, plane);
            int width = aom_img_plane_width(img, plane);
            
            for (int y = 0; y < height; y++) {
                size_t data_offset = (y * 7 + plane * 3) % size;
                size_t copy_size = width < (int)(size - data_offset) ? 
                                  width : size - data_offset;
                if (copy_size > 0) {
                    memcpy(plane_ptr + y * stride, 
                           data + data_offset, copy_size);
                }
            }
        }
        
        // Encode the image
        aom_codec_encode(&encoder, img, 0, 1, 0);
        
        // Get encoded data
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t* pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Process packets if needed
            (void)pkt;
        }
        
        aom_img_free(img);
    }
    
    // 9. Try to get frames from decoder
    aom_codec_iter_t dec_iter = nullptr;
    aom_image_t* decoded_img;
    while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
        // Just to ensure we process the frame
        (void)decoded_img;
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    return 0;
}