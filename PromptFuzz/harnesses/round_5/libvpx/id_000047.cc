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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 19
//<Quality> {"density":19,"unique_branches":{"encoder_set_config":[[841,7,841,31,0,0,4,0],[841,35,841,59,0,0,4,0],[842,9,842,33,0,0,4,0],[842,9,842,33,0,0,4,1],[842,37,842,67,0,0,4,0],[842,37,842,67,0,0,4,1],[853,10,853,36,0,0,4,0],[853,40,853,67,0,0,4,0],[854,11,856,42,0,0,4,0],[854,11,856,42,0,0,4,1],[857,11,857,51,0,0,4,0],[858,10,858,33,0,0,4,0],[858,37,858,76,0,0,4,0],[858,37,858,76,0,0,4,1],[859,10,859,34,0,0,4,0],[860,10,860,50,0,0,4,0],[860,10,860,50,0,0,4,1],[869,7,869,54,0,0,4,0],[873,7,873,26,0,0,4,0]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Define the custom types and callbacks
struct FrameBufferContext {
    std::vector<uint8_t> buffer;
    size_t allocated_size;
};

// Callback for getting frame buffer
int  id_000047_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    // Ensure we have enough space
    if (ctx->buffer.size() < min_size) {
        ctx->buffer.resize(min_size);
        ctx->allocated_size = min_size;
    }
    
    fb->data = ctx->buffer.data();
    fb->size = ctx->allocated_size;
    return 0;
}

// Callback for releasing frame buffer
int  id_000047_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // Nothing to do here - we manage memory in FrameBufferContext
    (void)cb_priv;
    (void)fb;
    return 0;
}

// Callback for put frame
void  id_000047_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // This is called when a frame is decoded
    (void)user_priv;
    (void)img;
    // Could save frame to file here, but for fuzzing we just consume it
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Need minimum data for configuration
    }
    
    // Create input file from fuzzer data
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
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();  // Use VP9 encoder
    
    // Initialize encoder configuration with default settings
    unsigned int usage = 0;  // Good quality
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, usage);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str;  // Use error string in debug
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use fuzzer data to set some configuration parameters
    // We'll use first few bytes to modify width/height
    cfg.g_w = 64 + (data[0] % 192);  // 64-256 width
    cfg.g_h = 64 + (data[1] % 192);  // 64-256 height
    cfg.rc_target_bitrate = 100 + (data[2] % 900);  // 100-1000 kbps
    cfg.g_threads = 1 + (data[3] % 4);  // 1-4 threads
    
    // Initialize encoder
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up frame buffer context and register callbacks
    FrameBufferContext fb_ctx;
    err = vpx_codec_set_frame_buffer_functions(&codec,  id_000047_get_frame_buffer_cb, 
                                               id_000047_release_frame_buffer_cb, &fb_ctx);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&codec,  id_000047_put_frame_cb, nullptr);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // Get global headers
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Create a test image to encode
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with data from fuzzer input
    size_t y_plane_size = cfg.g_w * cfg.g_h;
    size_t uv_plane_size = (cfg.g_w * cfg.g_h) / 4;
    
    // Read Y plane
    if (y_plane_size > 0 && size > 100) {
        size_t read_size = std::min(y_plane_size, size - 100);
        fread(img->planes[VPX_PLANE_Y], 1, read_size, in_file);
    }
    
    // Read U and V planes
    if (uv_plane_size > 0 && size > 100 + y_plane_size) {
        size_t offset = 100 + y_plane_size;
        size_t read_size = std::min(uv_plane_size, size - offset);
        fread(img->planes[VPX_PLANE_U], 1, read_size, in_file);
    }
    
    if (uv_plane_size > 0 && size > 100 + y_plane_size + uv_plane_size) {
        size_t offset = 100 + y_plane_size + uv_plane_size;
        size_t read_size = std::min(uv_plane_size, size - offset);
        fread(img->planes[VPX_PLANE_V], 1, read_size, in_file);
    }
    
    // Encode the image
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Try setting a new configuration with remaining fuzzer data
    if (size > sizeof(vpx_codec_enc_cfg_t) + 200) {
        // Use part of fuzzer data as a configuration structure
        vpx_codec_enc_cfg_t new_cfg;
        memcpy(&new_cfg, data + 100, sizeof(vpx_codec_enc_cfg_t));
        
        // Set reasonable bounds on configuration
        new_cfg.g_w = 64 + (new_cfg.g_w % 192);
        new_cfg.g_h = 64 + (new_cfg.g_h % 192);
        new_cfg.rc_target_bitrate = 100 + (new_cfg.rc_target_bitrate % 900);
        new_cfg.g_threads = 1 + (new_cfg.g_threads % 4);
        
        err = vpx_codec_enc_config_set(&codec, &new_cfg);
        if (err != VPX_CODEC_OK) {
            const char* err_str = vpx_codec_err_to_string(err);
            (void)err_str;
        }
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}