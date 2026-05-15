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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 7
//<Quality> {"density":22,"unique_branches":{"validate_config":[[125,33,125,49,3,0,4,1],[125,54,125,71,3,0,4,1],[125,12,125,29,4,0,4,1],[125,33,125,49,4,0,4,0],[125,33,125,49,4,0,4,1],[125,54,125,71,4,0,4,1],[131,9,131,29,5,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function prototypes
static void  id_000063_put_slice_cb(void* user_priv, const vpx_image_t* img,
                         const vpx_image_rect_t* slice, const vpx_image_rect_t* modified) {
    // Callback implementation
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)modified;
}

static void  id_000063_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // Callback implementation
    (void)user_priv;
    (void)img;
}

static int  id_000063_get_frame_buffer_cb(void* cb_priv, size_t min_size,
                               vpx_codec_frame_buffer_t* fb) {
    // Callback implementation
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1; // Indicate failure
}

static int  id_000063_release_frame_buffer_cb(void* cb_priv,
                                   vpx_codec_frame_buffer_t* fb) {
    // Callback implementation
    (void)cb_priv;
    (void)fb;
    return -1; // Indicate failure
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get version and build info
    const char* version_str = vpx_codec_version_str();
    const char* build_config = vpx_codec_build_config();
    (void)version_str;
    (void)build_config;

    // 2. Create VP8 encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // 3. Get codec capabilities and name
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)caps;
    (void)iface_name;

    // 4. Set up encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to modify configuration
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        // Ensure valid values
        cfg.g_w = cfg.g_w % 640 + 1;
        cfg.g_h = cfg.g_h % 480 + 1;
        cfg.rc_target_bitrate = cfg.rc_target_bitrate % 1000000 + 1;
        cfg.g_threads = cfg.g_threads % 8 + 1;
    }

    // 5. Create codec context
    vpx_codec_ctx_t ctx;
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&ctx);
        const char* error_detail = vpx_codec_error_detail(&ctx);
        (void)error;
        (void)error_detail;
        return 0;
    }

    // 6. Set encoder configuration
    err = vpx_codec_enc_config_set(&ctx, &cfg);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 7. Register callbacks
    vpx_codec_register_put_slice_cb(&ctx,  id_000063_put_slice_cb, NULL);
    vpx_codec_register_put_frame_cb(&ctx,  id_000063_put_frame_cb, NULL);

    // 8. Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&ctx,  id_000063_get_frame_buffer_cb,
                                          id_000063_release_frame_buffer_cb, NULL);

    // 9. Create input image
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420,
                                     cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 10. Fill image with fuzzed data
    size_t bytes_to_copy = size;
    if (bytes_to_copy > 0) {
        size_t y_plane_size = img->stride[0] * cfg.g_h;
        size_t uv_plane_size = img->stride[1] * (cfg.g_h / 2);
        
        // Copy to Y plane
        size_t copy_size = bytes_to_copy < y_plane_size ? bytes_to_copy : y_plane_size;
        if (copy_size > 0) {
            memcpy(img->planes[0], data, copy_size);
            bytes_to_copy -= copy_size;
        }
        
        // Copy to U plane
        if (bytes_to_copy > 0) {
            copy_size = bytes_to_copy < uv_plane_size ? bytes_to_copy : uv_plane_size;
            if (copy_size > 0) {
                memcpy(img->planes[1], data + (size - bytes_to_copy), copy_size);
                bytes_to_copy -= copy_size;
            }
        }
        
        // Copy to V plane
        if (bytes_to_copy > 0) {
            copy_size = bytes_to_copy < uv_plane_size ? bytes_to_copy : uv_plane_size;
            if (copy_size > 0) {
                memcpy(img->planes[2], data + (size - bytes_to_copy), copy_size);
            }
        }
    }

    // 11. Set output buffer
    vpx_fixed_buf_t output_buf;
    const size_t output_size = 1024 * 1024; // 1MB buffer
    output_buf.buf = malloc(output_size);
    output_buf.sz = output_size;
    
    if (output_buf.buf) {
        err = vpx_codec_set_cx_data_buf(&ctx, &output_buf, 0, 0);
        (void)err; // Continue even if this fails
    }

    // 12. Encode the image
    err = vpx_codec_encode(&ctx, img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&ctx);
        const char* error_detail = vpx_codec_error_detail(&ctx);
        (void)error;
        (void)error_detail;
    }

    // 13. Get encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process packet if needed
        (void)pkt;
    }

    // 14. Get preview frame
    const vpx_image_t* preview = vpx_codec_get_preview_frame(&ctx);
    (void)preview;

    // 15. Clean up
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    vpx_img_free(img);
    vpx_codec_destroy(&ctx);

    return 0;
}