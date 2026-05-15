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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/*We need to create a fuzzing driver that uses the specified libvpx APIs to decode VP8 encoded data. The input is a byte stream of libvpx's output (encoded video data).

Here's a C++ fuzzing driver that uses all the specified APIs:

*/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* data;
    size_t size;
    int in_use;
};

// Custom frame buffer callbacks
static int  id_000004_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* frame_buffers = static_cast<FrameBufferData*>(cb_priv);
    
    // Find an available buffer
    for (int i = 0; i < 4; i++) {
        if (!frame_buffers[i].in_use) {
            if (frame_buffers[i].size < min_size) {
                free(frame_buffers[i].data);
                frame_buffers[i].data = static_cast<uint8_t*>(malloc(min_size));
                if (!frame_buffers[i].data) return -1;
                frame_buffers[i].size = min_size;
            }
            frame_buffers[i].in_use = 1;
            fb->data = frame_buffers[i].data;
            fb->size = min_size;
            return 0;
        }
    }
    return -1;
}

static int  id_000004_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* frame_buffers = static_cast<FrameBufferData*>(cb_priv);
    
    for (int i = 0; i < 4; i++) {
        if (frame_buffers[i].data == fb->data) {
            frame_buffers[i].in_use = 0;
            return 0;
        }
    }
    return -1;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize encoder interface (not used for decoding, but we need to call the API)
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
	return 0;
	}
	
    
    // Call vpx_codec_iface_name
    const char* encoder_name = vpx_codec_iface_name(encoder_iface);
    if (!encoder_name) {
	return 0;
	}
	
    
    // Create encoder config (not used, but we need to call the API)
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    if (err != VPX_CODEC_OK) {
	return 0;
	}
	
    
    // Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    err = vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0);
    if (err != VPX_CODEC_OK) {
	return 0;
	}
	
    
    // Set up frame buffer functions
    FrameBufferData frame_buffers[4] = {0};
    err = vpx_codec_set_frame_buffer_functions(&decoder,  id_000004_get_frame_buffer, 
                                                 id_000004_release_frame_buffer, frame_buffers);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Create a fixed buffer for compressed data
    vpx_fixed_buf_t compressed_buf;
    compressed_buf.buf = malloc(size);
    if (!compressed_buf.buf) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    memcpy(compressed_buf.buf, data, size);
    compressed_buf.sz = size;
    
    // Set compressed data buffer (this is actually for encoder, but we call it anyway)
    // Note: This API is for encoder, but we call it to satisfy the requirement
    vpx_codec_set_cx_data_buf(&decoder, &compressed_buf, 0, 0);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), nullptr, 0);
    if (err != VPX_CODEC_OK) {
        free(compressed_buf.buf);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // We could process the frame here, but for fuzzing we just want to exercise the API
        // Access some image properties to ensure the frame is valid
        if (img->fmt != VPX_IMG_FMT_I420 && 
            img->fmt != VPX_IMG_FMT_I422 && 
            img->fmt != VPX_IMG_FMT_I444 &&
            img->fmt != VPX_IMG_FMT_I42016 &&
            img->fmt != VPX_IMG_FMT_I42216 &&
            img->fmt != VPX_IMG_FMT_I44416) {
            // Invalid format, but we continue anyway for fuzzing
        }
    }
    
    // Cleanup
    free(compressed_buf.buf);
    
    // Destroy decoder
    vpx_codec_destroy(&decoder);
    
    // Clean up frame buffers
    for (int i = 0; i < 4; i++) {
        free(frame_buffers[i].data);
    }
    
    return 0;
}