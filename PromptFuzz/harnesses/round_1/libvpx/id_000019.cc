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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"vpx_img_free":[[269,7,269,10,0,0,4,1]],"img_alloc_helper":[[48,7,48,29,0,0,4,0]],"is_valid_img_fmt":[[32,5,32,12,0,0,4,0]]},"library_calls":["vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free"],"visited":1}
/**/


extern "C" {
}

// Custom callback function for put_slice
static void  id_000019_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* slice, 
                               const vpx_image_rect_t* clip) {
    // Callback implementation - minimal for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clip;
}

// Custom frame buffer callbacks
static int  id_000019_get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1;  // Indicate failure for fuzzing
}

static int  id_000019_release_frame_buffer_callback(void* cb_priv, 
                                         vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    (void)fb;
    return -1;  // Indicate failure for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read configuration parameters from input data
    unsigned int width = 64 + (data[0] % 256);  // 64-319
    unsigned int height = 64 + (data[1] % 256); // 64-319
    unsigned int align = 1 + (data[2] % 32);    // 1-32
    vpx_img_fmt_t fmt = static_cast<vpx_img_fmt_t>((data[3] % 5) + 1);  // Valid formats
    
    // Allocate image using vpx_img_alloc
    vpx_image_t* img = vpx_img_alloc(nullptr, fmt, width, height, align);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder configuration
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();  // Use VP8 encoder
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_ctx_t ctx;
    
    // Get interface name using vpx_codec_iface_name
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)iface_name;  // Use variable to avoid unused warning
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder with parameters from input
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 100 + (data[4] % 1000);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1 + (data[5] % 4);
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = data[6] % 16;
    
    // Initialize encoder using vpx_codec_enc_init_ver
    if (vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(img);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration using vpx_codec_enc_config_set
    // Modify some parameters first
    cfg.rc_min_quantizer = data[7] % 64;
    cfg.rc_max_quantizer = 63;
    vpx_codec_enc_config_set(&ctx, &cfg);
    
    // Register put_slice callback using vpx_codec_register_put_slice_cb
    vpx_codec_register_put_slice_cb(&ctx,  id_000019_put_slice_callback, nullptr);
    
    // Set frame buffer functions using vpx_codec_set_frame_buffer_functions
    vpx_codec_set_frame_buffer_functions(&ctx,  id_000019_get_frame_buffer_callback, 
                                         id_000019_release_frame_buffer_callback, nullptr);
    
    // Use vpx_codec_control_ with various control IDs
    int ctrl_ids[] = {
        VP8E_SET_CPUUSED,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_SHARPNESS,
        VP8E_SET_STATIC_THRESHOLD
    };
    
    for (int i = 0; i < 5 && (size_t)(8 + i) < size; i++) {
        int ctrl_value = data[8 + i] % 256;
        vpx_codec_control_(&ctx, ctrl_ids[i % 5], ctrl_value);
    }
    
    // Get global headers using vpx_codec_get_global_headers
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&ctx);
    if (global_headers) {
        // Use the headers in some way
        (void)global_headers->buf;
        (void)global_headers->sz;
        // Note: We don't free this as it's managed by the codec
    }
    
    // Encode a few frames with data from input
    for (int frame = 0; frame < 3 && (size_t)(frame * 100) < size - 100; frame++) {
        // Fill image with some data from input
        size_t offset = 13 + frame * 100;
        if (offset < size && img->planes[0]) {
            size_t copy_size = std::min(size - offset, (size_t)(img->stride[0] * height));
            memcpy(img->planes[0], data + offset, copy_size);
        }
        
        // Encode the image
        vpx_codec_encode(&ctx, img, frame, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != nullptr) {
            // Process packet if needed
            (void)pkt;
        }
    }
    
    // Get error detail using vpx_codec_error_detail
    const char* error_detail = vpx_codec_error_detail(&ctx);
    (void)error_detail;  // Use variable to avoid unused warning
    
    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(img);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}