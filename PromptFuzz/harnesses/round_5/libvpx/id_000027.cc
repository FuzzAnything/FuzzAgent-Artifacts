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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"vpx_codec_enc_config_default":[[162,25,162,35,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function declarations
static int  id_000027_get_frame_buffer(void *user_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    (void)user_priv;
    (void)fb;
    return -1; // Simple stub implementation
}

static int  id_000027_release_frame_buffer(void *user_priv, 
                               vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)fb;
    return 0;
}

static void  id_000027_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize variables
    vpx_codec_ctx_t codec_ctx1, codec_ctx2;
    vpx_codec_enc_cfg_t cfg1, cfg2;
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t global_headers;
    vpx_fixed_buf_t cx_data_buf;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    const vpx_image_t *preview_frame = nullptr;
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
	return 0;
	}
	
    
    // Get interface name
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name;
    
    // Create input file from fuzzer data
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
    (void)in_fd;
    (void)out_fd;
    
    // Initialize encoder configuration with default values
    // Use a small usage value from fuzzer data
    unsigned int usage = data[0] % 3; // 0, 1, or 2
    if (vpx_codec_enc_config_default(iface, &cfg1, usage) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set some configuration parameters from fuzzer data
    cfg1.g_w = 64 + (data[1] % 192);  // Width between 64-256
    cfg1.g_h = 64 + (data[2] % 192);  // Height between 64-256
    cfg1.g_threads = 1 + (data[3] % 4); // 1-4 threads
    cfg1.rc_target_bitrate = 100 + (data[4] % 900); // 100-1000 kbps
    
    // Initialize first encoder (single encoder)
    if (vpx_codec_enc_init_ver(&codec_ctx1, iface, &cfg1, 0, 
                               VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize second encoder (multi-encoder with num_enc=1)
    vpx_rational_t dsf = {1, 1}; // No downsampling
    if (vpx_codec_enc_init_multi_ver(&codec_ctx2, iface, &cfg1, 1, 0, &dsf,
                                     VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx1);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Allocate image buffer
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg1.g_w, cfg1.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec_ctx1);
        vpx_codec_destroy(&codec_ctx2);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Fill image with fuzzer data
    size_t y_size = cfg1.g_w * cfg1.g_h;
    size_t uv_size = (cfg1.g_w / 2) * (cfg1.g_h / 2);
    size_t total_img_size = y_size + 2 * uv_size;
    
    // Read from input file to fill image
    size_t read_size = fread(img->planes[0], 1, y_size, in_file);
    if (read_size < y_size) {
        // Pad with zeros if not enough data
        memset(img->planes[0] + read_size, 0, y_size - read_size);
    }
    
    read_size = fread(img->planes[1], 1, uv_size, in_file);
    if (read_size < uv_size) {
        memset(img->planes[1] + read_size, 0, uv_size - read_size);
    }
    
    read_size = fread(img->planes[2], 1, uv_size, in_file);
    if (read_size < uv_size) {
        memset(img->planes[2] + read_size, 0, uv_size - read_size);
    }
    
    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&codec_ctx1,  id_000027_get_frame_buffer, 
                                         id_000027_release_frame_buffer, nullptr);
    vpx_codec_set_frame_buffer_functions(&codec_ctx2,  id_000027_get_frame_buffer,
                                         id_000027_release_frame_buffer, nullptr);
    
    // Prepare output buffer for encoded data
    std::vector<uint8_t> output_buffer(1024 * 1024); // 1MB buffer
    cx_data_buf.buf = output_buffer.data();
    cx_data_buf.sz = output_buffer.size();
    
    // Set cx data buffer with padding from fuzzer data
    unsigned int pad_before = data[5] % 16;
    unsigned int pad_after = data[6] % 16;
    vpx_codec_set_cx_data_buf(&codec_ctx1, &cx_data_buf, pad_before, pad_after);
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&codec_ctx1,  id_000027_put_frame_callback, nullptr);
    
    // Encode the image with different parameters from fuzzer data
    vpx_codec_pts_t pts = data[7] * 1000;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = (data[8] & 1) ? VPX_EFLAG_FORCE_KF : 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    // Encode with first encoder
    if (vpx_codec_encode(&codec_ctx1, img, pts, duration, flags, 
                        deadline) != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    // Get global headers
    global_headers = *vpx_codec_get_global_headers(&codec_ctx1);
    
    // Get encoded data packets
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx1, &iter)) != nullptr) {
        // Write to output file if it's a frame packet
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Get preview frame
    preview_frame = vpx_codec_get_preview_frame(&codec_ctx1);
    (void)preview_frame;
    
    // Encode with second encoder as well
    vpx_codec_encode(&codec_ctx2, img, pts + 1000, duration, 0, deadline);
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx1);
    vpx_codec_destroy(&codec_ctx2);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}