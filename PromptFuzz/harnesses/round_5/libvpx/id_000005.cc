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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000005_get_frame_buffer(void *cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    uint8_t **buf = (uint8_t**)cb_priv;
    static uint8_t dummy_buffer[1];
    
    // Return a dummy buffer
    fb->data = dummy_buffer;
    fb->size = 1;
    return 0;
}

static int  id_000005_release_frame_buffer(void *cb_priv, 
                               vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Use the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize variables
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_image_t *img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    vpx_fixed_buf_t output_buf;
    uint8_t *frame_buffer = nullptr;
    
    // 1. Get VP8 encoder interface
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    
    // 2. Get codec interface name
    const char *iface_name = vpx_codec_iface_name(codec_iface);
    (void)iface_name; // Mark as used
    
    // 3. Get default encoder configuration
    vpx_codec_err_t err = vpx_codec_enc_config_default(codec_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Configure encoder with some values from input data
    enc_cfg.g_w = (data[0] % 64 + 1) * 16;
    enc_cfg.g_h = (data[size > 1 ? 1 : 0] % 64 + 1) * 16;
    enc_cfg.rc_target_bitrate = 1000;
    
    // 4. Initialize encoder
    err = vpx_codec_enc_init_ver(&codec_ctx, codec_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Create an image
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                       enc_cfg.g_w, enc_cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Fill image with some data from input
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        uint8_t *plane_ptr = img->planes[plane];
        int stride = img->stride[plane];
        int height = (plane == 0) ? enc_cfg.g_h : enc_cfg.g_h / 2;
        int width = (plane == 0) ? enc_cfg.g_w : enc_cfg.g_w / 2;
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (data_offset < size) {
                    plane_ptr[y * stride + x] = data[data_offset++];
                } else {
                    plane_ptr[y * stride + x] = 128;
                }
            }
        }
    }
    
    // 6. Set frame buffer functions (for decoder simulation)
    uint8_t *cb_priv = nullptr;
    vpx_codec_set_frame_buffer_functions(&codec_ctx, 
                                         id_000005_get_frame_buffer,
                                         id_000005_release_frame_buffer,
                                        cb_priv);
    
    // 7. Prepare output buffer
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        // 8. Set output buffer for encoder
        vpx_codec_set_cx_data_buf(&codec_ctx, &output_buf, 0, 0);
        
        // Encode the image
        err = vpx_codec_encode(&codec_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
        (void)err; // Mark as used
        
        // 9. Get encoded data
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Try to get a preview frame (simulating vpx_codec_get_frame usage)
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec_ctx);
        (void)preview; // Mark as used
    }
    
    // Cleanup
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    vpx_img_free(img);
    
    // 10. Destroy codec context
    vpx_codec_destroy(&codec_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}