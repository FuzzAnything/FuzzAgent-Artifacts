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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"vp8_change_config":[[1619,7,1619,60,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*We can create a fuzzer that simulates a video encoding/decoding pipeline. Here's a step-by-step implementation:

*/


// libvpx headers

// Global callback data structure
struct CallbackData {
    uint8_t* buffer;
    size_t size;
    int callback_called;
};

// Frame buffer callback functions
int  id_000032_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    CallbackData* cb_data = static_cast<CallbackData*>(cb_priv);
    fb->data = cb_data->buffer;
    fb->size = min_size;
    return 0;
}

int  id_000032_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // We don't need to free anything since the buffer is managed elsewhere
    return 0;
}

// Put frame callback function
void  id_000032_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    int* callback_called = static_cast<int*>(user_priv);
    *callback_called = 1;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
	return 0;
	}
	
    
    // Initialize variables
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t cx_data_buf;
    vpx_image_t img_wrapped;
    CallbackData callback_data;
    int put_frame_cb_called = 0;
    
    // 1. First, check codec capabilities
    vpx_codec_iface_t* codec_iface = vpx_codec_vp8_cx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(codec_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // 2. Initialize encoder configuration with default values
    unsigned int usage = 0; // Good quality
    if (vpx_codec_enc_config_default(codec_iface, &cfg, usage) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set some parameters from the input data
    cfg.g_w = (data[0] % 128) + 64;  // Width between 64-192
    cfg.g_h = (data[1] % 128) + 64;  // Height between 64-192
    cfg.rc_target_bitrate = (data[2] % 1000) + 100;  // Bitrate 100-1100 kbps
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0; // No lookahead for simplicity
    cfg.rc_end_usage = VPX_VBR;
    
    // 3. Initialize encoder context
    if (vpx_codec_enc_init_ver(&codec_ctx, codec_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // 4. Set frame buffer functions (for memory allocation callbacks)
    callback_data.buffer = (uint8_t*)malloc(size);
    if (!callback_data.buffer) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    callback_data.size = size;
    callback_data.callback_called = 0;
    
    vpx_codec_set_frame_buffer_functions(&codec_ctx, 
                                          id_000032_get_frame_buffer_cb,
                                          id_000032_release_frame_buffer_cb,
                                         &callback_data);
    
    // 5. Register put frame callback
    vpx_codec_register_put_frame_cb(&codec_ctx,  id_000032_put_frame_cb, &put_frame_cb_called);
    
    // 6. Prepare output buffer for encoded data
    cx_data_buf.buf = malloc(size * 2); // Allocate twice input size for safety
    if (!cx_data_buf.buf) {
        free(callback_data.buffer);
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    cx_data_buf.sz = size * 2;
    
    // 7. Set cx data buffer with padding
    unsigned int pad_before = (data[3] % 16);
    unsigned int pad_after = (data[3] >> 4) % 16;
    vpx_codec_set_cx_data_buf(&codec_ctx, &cx_data_buf, pad_before, pad_after);
    
    // 8. Create input image using vpx_img_wrap
    // We'll use a portion of input data as image data
    size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2; // I420 format
    uint8_t* img_data = (uint8_t*)malloc(img_data_size);
    if (!img_data) {
        free(cx_data_buf.buf);
        free(callback_data.buffer);
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Fill image data with input (cyclic if needed)
    for (size_t i = 0; i < img_data_size; i++) {
        img_data[i] = data[i % size];
    }
    
    // Wrap the image
    vpx_image_t* wrapped_img = vpx_img_wrap(&img_wrapped, 
                                            VPX_IMG_FMT_I420,
                                            cfg.g_w, 
                                            cfg.g_h, 
                                            1,  // stride alignment
                                            img_data);
    
    if (!wrapped_img) {
        free(img_data);
        free(cx_data_buf.buf);
        free(callback_data.buffer);
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // 9. Set a rectangle on the image (using safe coordinates)
    unsigned int rect_x = (data[0] % (cfg.g_w / 2));
    unsigned int rect_y = (data[1] % (cfg.g_h / 2));
    unsigned int rect_w = (data[2] % (cfg.g_w - rect_x)) + 1;
    unsigned int rect_h = (data[3] % (cfg.g_h - rect_y)) + 1;
    
    vpx_img_set_rect(wrapped_img, rect_x, rect_y, rect_w, rect_h);
    
    // 10. Try to set encoder configuration again (this may fail, but that's OK for fuzzing)
    // We modify one parameter first
    cfg.rc_max_quantizer = 56;
    vpx_codec_enc_config_set(&codec_ctx, &cfg);
    
    // 11. Encode the image
    vpx_codec_encode(&codec_ctx, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
    
    // 12. Try to get preview frame (may or may not exist)
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    (void)preview_frame; // Use to avoid unused variable warning
    
    // 13. Free the wrapped image
    vpx_img_free(&img_wrapped);
    
    // Clean up allocated resources
    free(img_data);
    free(cx_data_buf.buf);
    free(callback_data.buffer);
    
    // 14. Destroy codec context
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}