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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Custom frame buffer callback structures
typedef struct FrameBufferContext {
    uint8_t* buffer;
    size_t size;
    int used;
} FrameBufferContext;

// Frame buffer allocation callback
int  id_000008_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = (FrameBufferContext*)cb_priv;
    
    if (ctx->buffer == nullptr || ctx->size < min_size) {
        free(ctx->buffer);
        ctx->buffer = (uint8_t*)malloc(min_size);
        ctx->size = min_size;
        if (!ctx->buffer) return -1;
    }
    
    fb->data = ctx->buffer;
    fb->size = min_size;
    ctx->used = 1;
    
    return 0;
}

// Frame buffer release callback
int  id_000008_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = (FrameBufferContext*)cb_priv;
    ctx->used = 0;
    return 0;
}

// Put slice callback
void  id_000008_put_slice_cb(void* user_priv, const vpx_image_t* img,
                  const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // This callback is called during encoding when slices are available
    // We can process or ignore the slice data as needed
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for meaningful processing
    
    // Create a FILE pointer for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (as per instruction 5)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx(); // Use VP8 encoder interface
    
    // Get codec interface name (API 1)
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Mark as used
    
    // Configure encoder
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set reasonable configuration based on input size
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder (API 3)
    res = vpx_codec_enc_init_ver(&encoder, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set frame buffer functions (API 2)
    FrameBufferContext fb_ctx = {nullptr, 0, 0};
    res = vpx_codec_set_frame_buffer_functions(&encoder,  id_000008_get_frame_buffer_cb, 
                                                id_000008_release_frame_buffer_cb, &fb_ctx);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put slice callback (API 6)
    res = vpx_codec_register_put_slice_cb(&encoder,  id_000008_put_slice_cb, nullptr);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Allocate image for input frames (API 5)
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Fill image with data from input
    size_t bytes_to_read = cfg.g_w * cfg.g_h * 3 / 2; // YUV420 size
    bytes_to_read = bytes_to_read < size ? bytes_to_read : size;
    
    // Read Y plane
    size_t y_size = cfg.g_w * cfg.g_h;
    fread(img->planes[VPX_PLANE_Y], 1, y_size < size ? y_size : size, in_file);
    
    // Read U and V planes if enough data
    size_t uv_size = cfg.g_w * cfg.g_h / 4;
    if (size > y_size + uv_size) {
        fread(img->planes[VPX_PLANE_U], 1, uv_size, in_file);
    }
    if (size > y_size + uv_size * 2) {
        fread(img->planes[VPX_PLANE_V], 1, uv_size, in_file);
    }
    
    // Get global headers (API 7)
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Encode the image (API 4)
    // Use input data to determine PTS and duration for variability
    uint32_t pts_seed = 0;
    if (size >= 4) {
        memcpy(&pts_seed, data, 4);
    }
    
    res = vpx_codec_encode(&encoder, img, pts_seed % 1000, 
                           (size % 30) + 1, fuzz_int64_t_4, fuzz_uint64_t_5);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    free(fb_ctx.buffer);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file to avoid accumulating data
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}