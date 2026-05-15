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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"vpx_codec_dec_init_ver":[[30,7,30,37,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    uint8_t *buffer;
    size_t size;
};

// Callback for getting frame buffer
int  id_000061_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or reallocate buffer if needed
    if (fb_data->size < min_size) {
        free(fb_data->buffer);
        fb_data->buffer = static_cast<uint8_t*>(malloc(min_size));
        if (!fb_data->buffer) {
            return -1;
        }
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer;
    fb->size = fb_data->size;
    fb->priv = cb_priv;
    return 0;
}

// Callback for releasing frame buffer
int  id_000061_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    // Buffer is managed by FrameBufferData, don't free here
    return 0;
}

// Put frame callback structure
struct CallbackData {
    vpx_codec_ctx_t encoder1;
    vpx_codec_ctx_t encoder2;
    FILE *out_file1;
    FILE *out_file2;
    bool encoders_initialized;
    int frame_count;
    vpx_codec_enc_cfg_t cfg1;
    vpx_codec_enc_cfg_t cfg2;
};

// Put frame callback function
void  id_000061_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    CallbackData *cb_data = static_cast<CallbackData*>(user_priv);
    
    if (!cb_data->encoders_initialized) {
        // Get encoder interface and its name
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        const char *iface_name = vpx_codec_iface_name(encoder_iface);
        
        // Initialize encoder configurations
        vpx_codec_enc_config_default(encoder_iface, &cb_data->cfg1, 0);
        vpx_codec_enc_config_default(encoder_iface, &cb_data->cfg2, 0);
        
        // Set configuration parameters
        cb_data->cfg1.g_w = img->d_w;
        cb_data->cfg1.g_h = img->d_h;
        cb_data->cfg2.g_w = img->d_w;
        cb_data->cfg2.g_h = img->d_h;
        
        cb_data->cfg1.g_timebase.num = 1;
        cb_data->cfg1.g_timebase.den = 30;
        cb_data->cfg2.g_timebase.num = 1;
        cb_data->cfg2.g_timebase.den = 30;
        
        cb_data->cfg1.rc_target_bitrate = 1000;
        cb_data->cfg2.rc_target_bitrate = 2000;
        
        // Initialize multiple encoders
        vpx_rational_t dsf = {1, 1};
        if (vpx_codec_enc_init_multi_ver(&cb_data->encoder1, encoder_iface, &cb_data->cfg1, 
                                         2, 0, &dsf, 0) != VPX_CODEC_OK) {
            return;
        }
        
        // Get global headers and write to files
        vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&cb_data->encoder1);
        if (global_headers && global_headers->buf && global_headers->sz > 0) {
            fwrite(global_headers->buf, 1, global_headers->sz, cb_data->out_file1);
            fwrite(global_headers->buf, 1, global_headers->sz, cb_data->out_file2);
        }
        
        cb_data->encoders_initialized = true;
    }
    
    // Wrap image data for encoding
    vpx_image_t wrapped_img;
    // Using stride alignment of 1 for simplicity
    if (vpx_img_wrap(&wrapped_img, img->fmt, img->d_w, img->d_h, 1, 
                     const_cast<unsigned char*>(img->planes[0]))) {
        // Copy plane information
        wrapped_img.planes[1] = const_cast<unsigned char*>(img->planes[1]);
        wrapped_img.planes[2] = const_cast<unsigned char*>(img->planes[2]);
        wrapped_img.stride[0] = img->stride[0];
        wrapped_img.stride[1] = img->stride[1];
        wrapped_img.stride[2] = img->stride[2];
        
        // Encode with both encoders
        vpx_codec_encode(&cb_data->encoder1, &wrapped_img, cb_data->frame_count, 
                         1, 0, 0);
        vpx_codec_encode(&cb_data->encoder2, &wrapped_img, cb_data->frame_count, 
                         1, 0, 0);
        
        // Clean up wrapped image
        free(wrapped_img.img_data);
    }
    
    cb_data->frame_count++;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Prepare callback data
    CallbackData cb_data;
    memset(&cb_data, 0, sizeof(cb_data));
    
    cb_data.out_file1 = fopen("output_file", "wb");
    cb_data.out_file2 = fopen("output_file2", "wb");
    cb_data.encoders_initialized = false;
    cb_data.frame_count = 0;
    
    // Set frame buffer functions
    FrameBufferData fb_data = {nullptr, 0};
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000061_get_frame_buffer, 
                                          id_000061_release_frame_buffer, &fb_data);
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000061_put_frame_callback, &cb_data);
    
    // Decode input data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    // Clean up
    vpx_codec_destroy(&decoder);
    
    if (cb_data.encoders_initialized) {
        vpx_codec_destroy(&cb_data.encoder1);
        vpx_codec_destroy(&cb_data.encoder2);
    }
    
    free(fb_data.buffer);
    
    if (cb_data.out_file1) fclose(cb_data.out_file1);
    if (cb_data.out_file2) fclose(cb_data.out_file2);
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}