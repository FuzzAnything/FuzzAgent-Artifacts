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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.333333, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"estimate_keyframe_frequency":[[1323,31,1323,64,0,0,4,0]]},"library_calls":["vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_img_free","vpx_codec_destroy","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_img_free","vpx_codec_destroy","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":2}
/**/


extern "C" {
}

// Custom callback structures to satisfy API requirements
struct CallbackData {
    std::vector<uint8_t> buffer;
    int frame_count = 0;
};

extern "C" void put_slice_callback(void* user_priv, const vpx_image_t* img,
                                   const vpx_image_rect_t* rect, 
                                   const vpx_image_rect_t* rect2) {
    // Minimal implementation
}

extern "C" int get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                        vpx_codec_frame_buffer_t* fb) {
    // Minimal implementation
    fb->data = static_cast<uint8_t*>(malloc(min_size));
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

extern "C" int release_frame_buffer_callback(void* cb_priv,
                                            vpx_codec_frame_buffer_t* fb) {
    // Minimal implementation
    if (fb->data) free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read a small config from the beginning of the data
    unsigned int width = 64, height = 64;
    if (size >= 8) {
        width = (data[0] % 128) + 32;  // 32-159
        height = (data[1] % 128) + 32; // 32-159
        width = width & ~1;  // Make even
        height = height & ~1; // Make even
    }
    
    // Create image using vpx_img_alloc (required API)
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 16);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with some data from input
    size_t y_plane_size = width * height;
    size_t uv_plane_size = y_plane_size / 4;
    
    // Read image data from input if available
    if (size > 8) {
        size_t offset = 8;
        size_t to_read = std::min(y_plane_size, size - offset);
        if (to_read > 0) {
            fseek(in_file, offset, SEEK_SET);
            fread(img->planes[0], 1, to_read, in_file);
        }
        
        if (size > offset + y_plane_size + uv_plane_size) {
            offset += y_plane_size;
            to_read = std::min(uv_plane_size * 2, size - offset);
            if (to_read > 0) {
                fseek(in_file, offset, SEEK_SET);
                fread(img->planes[1], 1, to_read, in_file);
            }
        }
    }
    
    assert_file_closed(&in_file);;
    
    // Initialize encoder
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t* enc_iface = vpx_codec_vp8_cx();
    
    // Get interface name (required API)
    const char* iface_name = vpx_codec_iface_name(enc_iface);
    
    // Setup encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(enc_iface, &cfg, 0);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.rc_end_usage = VPX_VBR;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 30;
    
    // Initialize encoder (required API)
    if (vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION)) {
        vpx_img_free(img);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to set config again (required API)
    vpx_codec_enc_config_set(&enc_ctx, &cfg);
    
    // Try a control operation (required API)
    vpx_codec_control_(&enc_ctx, VP8E_SET_CPUUSED, 8);
    
    // Setup callbacks (required APIs)
    CallbackData cb_data;
    vpx_codec_register_put_slice_cb(&enc_ctx, put_slice_callback, NULL);
    vpx_codec_set_frame_buffer_functions(&enc_ctx, get_frame_buffer_callback,
                                       release_frame_buffer_callback, &cb_data);
    
    // Get global headers (required API)
    vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&enc_ctx);
    if (headers) {
        free(headers->buf);
    }
    
    // Encode the image (required API)
    if (vpx_codec_encode(&enc_ctx, img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        
        // Get encoded data (required API)
        while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Store some data in callback buffer
                size_t copy_size = std::min((size_t)pkt->data.frame.sz, (size_t)1024);
                cb_data.buffer.resize(copy_size);
                memcpy(cb_data.buffer.data(), pkt->data.frame.buf, copy_size);
                cb_data.frame_count++;
            }
        }
    }
    
    // Try to get preview frame (required API)
    const vpx_image_t* preview = vpx_codec_get_preview_frame(&enc_ctx);
    
    // Create another image using vpx_img_wrap (required API)
    std::vector<uint8_t> img_data(width * height * 3 / 2);
    vpx_image_t* wrapped_img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, width, height, 
                                            16, img_data.data());
    if (wrapped_img) {
        // Try to set a rectangle (optional but useful)
        vpx_img_set_rect(wrapped_img, 0, 0, width/2, height/2);
        
        // Flip the image (optional)
        vpx_img_flip(wrapped_img);
        
        // Free wrapped image
        vpx_img_free(wrapped_img);
    }
    
    // Cleanup encoder
    vpx_codec_destroy(&enc_ctx);
    
    // Free the original image
    vpx_img_free(img);
    
    // Now try to decode the encoded data if we have any
    if (!cb_data.buffer.empty()) {
        vpx_codec_ctx_t dec_ctx;
        vpx_codec_iface_t* dec_iface = vpx_codec_vp8_dx();
        
        // Initialize decoder
        if (vpx_codec_dec_init_ver(&dec_ctx, dec_iface, NULL, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Decode the data (use callback buffer or original data)
            const uint8_t* decode_data;
            size_t decode_size;
            
            if (cb_data.frame_count > 0 && !cb_data.buffer.empty()) {
                decode_data = cb_data.buffer.data();
                decode_size = cb_data.buffer.size();
            } else {
                decode_data = data;
                decode_size = std::min(size, (size_t)1024);
            }
            
            vpx_codec_decode(&dec_ctx, decode_data, decode_size, NULL, 0);
            
            // Try to get decoded frame (required API)
            vpx_codec_iter_t dec_iter = NULL;
            vpx_image_t* dec_img = vpx_codec_get_frame(&dec_ctx, &dec_iter);
            
            // Cleanup decoder
            vpx_codec_destroy(&dec_ctx);
        }
    }
    
    assert_file_closed(&in_file);
	return 0;
}