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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"vpx_codec_enc_config_default":[[162,25,162,35,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_control_","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_set_rect","vpx_img_flip","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_wrap","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_set_rect","vpx_img_flip","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000076_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback can be empty for fuzzing purposes
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum size for basic operations
    if (size < 1024) {
        return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_image_t *img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Get interface name
    const char *iface_name = vpx_codec_iface_name(iface);
    if (!iface_name) {
        return 0;
    }

    // Initialize encoder configuration with default settings
    // Use VPX_USAGE_REALTIME (1) for usage parameter
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &enc_cfg, 1);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Set encoder configuration from input data
    // Use first few bytes to determine width/height
    uint32_t width = 64 + (data[0] % 192);  // 64-256
    uint32_t height = 64 + (data[1] % 192); // 64-256
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = 1 + (data[2] % 4);  // 1-4 threads

    // Initialize encoder
    err = vpx_codec_enc_init_ver(&codec_ctx, iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&codec_ctx);
        (void)error_detail;
        return 0;
    }

    // Set encoder configuration (redundant but tests the API)
    err = vpx_codec_enc_config_set(&codec_ctx, &enc_cfg);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }

    // Test codec control with some control IDs
    // Use a few different control IDs from input data
    int ctrl_id = VP8E_SET_CPUUSED + (data[3] % 10);
    err = vpx_codec_control_(&codec_ctx, ctrl_id);
    // Ignore error - control might fail with invalid ID
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&codec_ctx,  id_000076_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        // Continue anyway - callback registration might fail
    }

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        // Free the global headers buffer
        free(global_headers->buf);
    }

    // Create image using vpx_img_alloc
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }

    // Also test vpx_img_wrap with the same data
    // We'll use a portion of input data as image data
    size_t img_data_size = width * height * 3 / 2;
    if (img_data_size > size) {
        img_data_size = size;
    }
    
    // Create another image using vpx_img_wrap
    vpx_image_t wrapped_img;
    unsigned char *img_data = (unsigned char *)malloc(img_data_size);
    if (img_data) {
        memcpy(img_data, data, img_data_size);
        vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                           width, height, 1, img_data);
        (void)wrapped;
    }

    // Fill the allocated image with input data for encoding
    size_t copy_size = width * height;
    if (copy_size > size) copy_size = size;
    
    // Fill Y plane
    if (copy_size > 0) {
        memcpy(img->planes[0], data, copy_size);
    }
    
    // Fill U and V planes if we have enough data
    if (size > copy_size) {
        size_t uv_size = (width/2) * (height/2);
        size_t uv_copy = size - copy_size;
        if (uv_copy > uv_size) uv_copy = uv_size;
        
        if (uv_copy > 0) {
            memcpy(img->planes[1], data + copy_size, uv_copy);
        }
        
        if (size > copy_size + uv_copy) {
            size_t remaining = size - (copy_size + uv_copy);
            if (remaining > uv_size) remaining = uv_size;
            memcpy(img->planes[2], data + copy_size + uv_copy, remaining);
        }
    }

    // Encode the image multiple times to test different flags
    for (int i = 0; i < 3 && i * 1024 < (int)size; i++) {
        vpx_enc_frame_flags_t flags = 0;
        if (data[i] & 0x01) flags |= VPX_EFLAG_FORCE_KF;
        
        // Use different PTS values from input data
        vpx_codec_pts_t pts = i * 3000;
        
        // Encode with deadline from input
        vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
        if (data[i] & 0x02) deadline = VPX_DL_GOOD_QUALITY;
        
        err = vpx_codec_encode(&codec_ctx, img, pts, 1, flags, deadline);
        if (err != VPX_CODEC_OK) {
            // Get error details but continue
            const char *error_detail = vpx_codec_error_detail(&codec_ctx);
            (void)error_detail;
            break;
        }

        // Get encoded data
        iter = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
            // Process different packet types
            switch (pkt->kind) {
                case VPX_CODEC_CX_FRAME_PKT:
                    // Frame data available
                    break;
                case VPX_CODEC_STATS_PKT:
                    // Statistics data
                    break;
                case VPX_CODEC_PSNR_PKT:
                    // PSNR data
                    break;
                default:
                    break;
            }
        }
    }

    // Test image manipulation APIs
    // Set a rectangle on the image
    if (width > 10 && height > 10) {
        unsigned int rect_x = data[4] % (width - 10);
        unsigned int rect_y = data[5] % (height - 10);
        unsigned int rect_w = 10;
        unsigned int rect_h = 10;
        vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);
    }

    // Flip the image
    vpx_img_flip(img);

    // Cleanup
    if (img_data) {
        free(img_data);
    }
    vpx_img_free(img);
    
    // Destroy codec context
    vpx_codec_destroy(&codec_ctx);

    return 0;
}