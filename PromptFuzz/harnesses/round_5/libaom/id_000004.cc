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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_enc_config_set","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve an event (encoding a frame with custom buffer management):

*/


extern "C" {
}

// Custom frame buffer callback structure
struct FrameBufferCallbackContext {
    uint8_t* buffer;
    size_t buffer_size;
    int buffers_allocated;
};

// Get frame buffer callback
int  id_000004_get_frame_buffer_cb(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferCallbackContext* ctx = static_cast<FrameBufferCallbackContext*>(cb_priv);
    
    if (ctx->buffers_allocated >= 10) {
        return -1; // Limit number of buffers
    }
    
    // Allocate buffer
    uint8_t* buf = static_cast<uint8_t*>(malloc(min_size));
    if (!buf) {
        return -1;
    }
    
    fb->data = buf;
    fb->size = min_size;
    
    // Store for later cleanup
    ctx->buffer = buf;
    ctx->buffer_size = min_size;
    ctx->buffers_allocated++;
    
    return 0;
}

// Release frame buffer callback
int  id_000004_release_frame_buffer_cb(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    if (fb->data) {
        free(fb->data);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Need minimum data for basic operations
    if (size < 1024) {
        return 0;
    }
    
    // Use first part of data for image data
    size_t image_data_size = size / 2;
    uint8_t* image_data = static_cast<uint8_t*>(malloc(image_data_size));
    if (!image_data) {
        return 0;
    }
    memcpy(image_data, data, image_data_size);
    
    // Use second part for compressed data buffer
    size_t compressed_size = size - image_data_size;
    const uint8_t* compressed_data = data + image_data_size;
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    
    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(image_data);
        return 0;
    }
    
    // Set small dimensions for fuzzing
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    
    // Initialize encoder
    if (aom_codec_enc_init(&encoder, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(image_data);
        return 0;
    }
    
    // Create custom frame buffer callback context
    FrameBufferCallbackContext callback_ctx = {nullptr, 0, 0};
    
    // Set frame buffer functions (API #4)
    if (aom_codec_set_frame_buffer_functions(&encoder,  id_000004_get_frame_buffer_cb, 
                                            id_000004_release_frame_buffer_cb, &callback_ctx) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        free(image_data);
        return 0;
    }
    
    // Create image wrapper (API #3)
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int width = 32;
    unsigned int height = 32;
    unsigned int stride_align = 1;
    
    // Calculate required stride for I420 format
    unsigned int y_stride = (width + stride_align - 1) & ~(stride_align - 1);
    unsigned int uv_stride = ((width / 2) + stride_align - 1) & ~(stride_align - 1);
    
    // Ensure we have enough image data
    size_t required_size = y_stride * height + 2 * (uv_stride * (height / 2));
    if (image_data_size < required_size) {
        // Adjust image data size
        uint8_t* new_image_data = static_cast<uint8_t*>(realloc(image_data, required_size));
        if (!new_image_data) {
            aom_codec_destroy(&encoder);
            free(image_data);
            return 0;
        }
        image_data = new_image_data;
        
        // Fill with pattern if needed
        for (size_t i = 0; i < required_size; i++) {
            image_data[i] = static_cast<uint8_t>(i % 256);
        }
    }
    
    // Wrap the image
    aom_image_t* wrapped_img = aom_img_wrap(&img, fmt, width, height, 
                                          stride_align, image_data);
    if (!wrapped_img) {
        aom_codec_destroy(&encoder);
        free(image_data);
        return 0;
    }
    
    // Set encoder configuration (API #2)
    if (aom_codec_enc_config_set(&encoder, &cfg) != AOM_CODEC_OK) {
        aom_img_free(wrapped_img);
        aom_codec_destroy(&encoder);
        free(image_data);
        return 0;
    }
    
    // Set compressed data buffer (API #5)
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = const_cast<void*>(static_cast<const void*>(compressed_data));
    cx_buf.sz = compressed_size;
    
    if (aom_codec_set_cx_data_buf(&encoder, &cx_buf, 0, 0) != AOM_CODEC_OK) {
        aom_img_free(wrapped_img);
        aom_codec_destroy(&encoder);
        free(image_data);
        return 0;
    }
    
    // Try to encode a frame
    aom_codec_encode(&encoder, wrapped_img, 0, 1, 0);
    
    // Get compressed data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame packet received
            break;
        }
    }
    
    // Cleanup (API #1)
    aom_img_free(wrapped_img);
    aom_codec_destroy(&encoder);
    
    // Cleanup image data
    free(image_data);
    
    // Cleanup any allocated frame buffers
    if (callback_ctx.buffer) {
        free(callback_ctx.buffer);
    }
    
    return 0;
}