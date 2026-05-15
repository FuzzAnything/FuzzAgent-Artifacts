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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"vp8cx_pick_filter_level":[[365,34,365,57,0,0,4,1]]},"library_calls":["vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_img_free","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_img_free","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free"],"visited":1}
/**/


extern "C" {
}

// Frame buffer callbacks
static int  id_000081_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = (uint8_t*)malloc(1024);
    fb->size = 1024;
    *(void**)cb_priv = fb->data;
    return 0;
}

static int  id_000081_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

// Callback functions
static void  id_000081_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

static void  id_000081_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice, const vpx_image_rect_t *recon) {
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)recon;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read basic parameters from input
    uint32_t width, height;
    if (fread(&width, sizeof(width), 1, in_file) != 1 ||
        fread(&height, sizeof(height), 1, in_file) != 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Clamp dimensions for safety
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width < 16) width = 16;
    if (height < 16) height = 16;
    
    // Align dimensions
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    // Allocate and initialize image
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set image rectangle (using data from input)
    uint32_t rect_x, rect_y, rect_w, rect_h;
    if (fread(&rect_x, sizeof(rect_x), 1, in_file) != 1 ||
        fread(&rect_y, sizeof(rect_y), 1, in_file) != 1 ||
        fread(&rect_w, sizeof(rect_w), 1, in_file) != 1 ||
        fread(&rect_h, sizeof(rect_h), 1, in_file) != 1) {
        rect_x = rect_y = 0;
        rect_w = width;
        rect_h = height;
    }
    
    // Clamp rectangle to image bounds
    if (rect_x >= width) rect_x = 0;
    if (rect_y >= height) rect_y = 0;
    if (rect_w > width - rect_x) rect_w = width - rect_x;
    if (rect_h > height - rect_y) rect_h = height - rect_y;
    if (rect_w < 4) rect_w = 4;
    if (rect_h < 4) rect_h = 4;
    
    // Set rectangle
    vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);

    // Try to fill image data from remaining input
    size_t remaining = size - ftell(in_file);
    size_t to_copy = remaining;
    if (to_copy > (size_t)(img->stride[0] * height * 3 / 2)) {
        to_copy = img->stride[0] * height * 3 / 2;
    }
    
    if (to_copy > 0) {
        // Read Y plane
        size_t y_plane_size = img->stride[0] * height;
        size_t y_to_copy = to_copy > y_plane_size ? y_plane_size : to_copy;
        fread(img->planes[0], 1, y_to_copy, in_file);
        
        // Read U plane if we have more data
        if (to_copy > y_plane_size) {
            size_t u_plane_size = img->stride[1] * height / 2;
            size_t u_to_copy = (to_copy - y_plane_size) > u_plane_size ? 
                             u_plane_size : (to_copy - y_plane_size);
            fread(img->planes[1], 1, u_to_copy, in_file);
        }
        
        // Read V plane if we have even more data
        if (to_copy > y_plane_size + (img->stride[1] * height / 2)) {
            size_t v_plane_size = img->stride[2] * height / 2;
            size_t remaining_for_v = to_copy - y_plane_size - (img->stride[1] * height / 2);
            size_t v_to_copy = remaining_for_v > v_plane_size ? v_plane_size : remaining_for_v;
            fread(img->planes[2], 1, v_to_copy, in_file);
        }
    }
    
    assert_file_closed(&in_file);;

    // Get encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        vpx_img_free(img);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning

    // Create encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256; // 256 kbps
    
    // Initialize multi-encoder
    vpx_rational_t dsf = {1, 1};
    if (vpx_codec_enc_init_multi_ver(&codec, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(img);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration
    if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        vpx_img_free(img);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set frame buffer functions
    void *cb_priv = NULL;
    vpx_codec_set_frame_buffer_functions(&codec,  id_000081_get_frame_buffer, 
                                          id_000081_release_frame_buffer, &cb_priv);

    // Register callbacks
    vpx_codec_register_put_frame_cb(&codec,  id_000081_put_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&codec,  id_000081_put_slice_cb, NULL);

    // Set output buffer
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(1024 * 1024); // 1MB buffer
    output_buf.sz = 1024 * 1024;
    
    if (output_buf.buf) {
        vpx_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    }

    // Encode the image
    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);

    // Get encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet if needed
        (void)pkt;
    }

    // Try to get preview frame
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
    (void)preview; // Use preview to avoid unused variable warning

    // Cleanup
    vpx_codec_destroy(&codec);
    vpx_img_free(img);
    free(output_buf.buf);
    
    // Free callback buffer if allocated
    if (cb_priv) {
        free(*(void**)&cb_priv);
    }

    assert_file_closed(&in_file);
	return 0;
}