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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_set_frame_buffer_functions","vpx_codec_error","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callback structures
typedef struct FrameBufferData {
    uint8_t* buffer;
    size_t size;
} FrameBufferData;

static int  id_000010_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fbd = static_cast<FrameBufferData*>(cb_priv);
    
    if (min_size > fbd->size) {
        uint8_t* new_buffer = static_cast<uint8_t*>(realloc(fbd->buffer, min_size));
        if (!new_buffer) return -1;
        fbd->buffer = new_buffer;
        fbd->size = min_size;
    }
    
    fb->data = fbd->buffer;
    fb->size = min_size;
    return 0;
}

static int  id_000010_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // For this fuzzing context, we don't actually release the buffer
    // as we manage it separately
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    
    // Initialize decoder for VP9
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    const char* iface_name = vpx_codec_iface_name(decoder_iface);
    
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&decoder);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup custom frame buffer functions
    FrameBufferData frame_buffer_data = {nullptr, 0};
    res = vpx_codec_set_frame_buffer_functions(&decoder,  id_000010_get_frame_buffer_cb, 
                                               id_000010_release_frame_buffer_cb, &frame_buffer_data);
    if (res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&decoder);
        // Continue anyway
    }
    
    // Read and decode frames from input
    uint8_t* buffer = static_cast<uint8_t*>(malloc(size));
    if (!buffer) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Decode the data
    res = vpx_codec_decode(&decoder, buffer, bytes_read, nullptr, 0);
    if (res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&decoder);
        // Continue to test error handling
    }
    
    // Initialize encoder for VP8
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(buffer);
        vpx_codec_destroy(&decoder);
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some basic encoder configuration
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&encoder);
        free(buffer);
        vpx_codec_destroy(&decoder);
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get global headers from encoder
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && out_file) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Create an image and modify it using vpx_img_set_rect
    vpx_image_t img;
    const int align = 16;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 640, 480, align);
    
    if (img.fmt == VPX_IMG_FMT_I420) {
        // Set a rectangle on the image (use data from input for coordinates)
        unsigned int x = data[0] % (img.d_w - 10);
        unsigned int y = data[1] % (img.d_h - 10);
        unsigned int w = 10 + (data[2] % 50);
        unsigned int h = 10 + (data[3] % 50);
        
        vpx_img_set_rect(&img, x, y, w, h);
        
        // Encode the modified image
        vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT && out_file) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Clean up
    vpx_img_free(&img);
    free(buffer);
    if (frame_buffer_data.buffer) {
        free(frame_buffer_data.buffer);
    }
    
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}