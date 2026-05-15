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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 32
//<Quality> {"density":26,"unique_branches":{"update_golden_frame_stats":[[2654,9,2654,37,0,0,4,0]],"encode_frame_to_data_rate":[[3881,15,3881,49,0,0,4,0]],"update_rd_ref_frame_probs":[[2682,14,2682,36,0,0,4,1],[2697,16,2697,45,0,0,4,1],[2699,16,2699,45,0,0,4,0],[2699,16,2699,45,0,0,4,1],[2702,16,2702,42,0,0,4,1]],"vp8e_encode":[[934,7,934,13,0,0,4,1],[987,17,987,45,0,0,4,0]],"vpx_codec_decode":[[112,46,112,54,0,0,4,0]],"calculate_zeromv_rd_adjustment":[[522,7,522,30,0,0,4,0],[527,9,527,43,0,0,4,0],[527,9,527,43,0,0,4,1],[528,11,528,34,0,0,4,0],[528,38,528,61,0,0,4,0],[537,9,537,43,0,0,4,0],[537,9,537,43,0,0,4,1],[538,11,538,35,0,0,4,0],[538,39,538,63,0,0,4,0],[547,9,547,43,0,0,4,0],[547,9,547,43,0,0,4,1],[548,11,548,34,0,0,4,0],[548,38,548,61,0,0,4,0],[553,11,553,35,0,0,4,0],[553,11,553,35,0,0,4,1],[553,39,553,64,0,0,4,0],[553,39,553,64,0,0,4,1],[554,10,554,32,0,0,4,0],[554,10,554,32,0,0,4,1],[555,9,555,31,0,0,4,0],[555,9,555,31,0,0,4,1],[557,16,557,38,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_img_wrap","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Forward declarations for callback functions
static int  id_000045_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb);
static int  id_000045_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb);
static void  id_000045_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2);

// Simple frame buffer structure for callback
struct FrameBufferData {
    uint8_t *data;
    size_t size;
};

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* for input data
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
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Get interface name (API 3)
    const char *iface_name = vpx_codec_iface_name(encoder_iface);
    
    // Create encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    // Get default encoder configuration (API 5)
    vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    // Set some configuration values
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    
    // Initialize encoder (API 6)
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration again (API 7)
    vpx_codec_enc_config_set(&encoder, &cfg);
    
    // Call control API (API 8)
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 1);
    vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);
    
    // Register put slice callback (API 9)
    vpx_codec_register_put_slice_cb(&encoder,  id_000045_put_slice_cb, nullptr);
    
    // Set frame buffer functions (API 10)
    FrameBufferData fb_data = {nullptr, 0};
    vpx_codec_set_frame_buffer_functions(&encoder,  id_000045_get_frame_buffer_cb, 
                                          id_000045_release_frame_buffer_cb, &fb_data);
    
    // Create input image using vpx_img_alloc (API 11)
    vpx_image_t *input_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                           cfg.g_w, cfg.g_h, 16);
    
    // Also create an image using vpx_img_wrap (API 4)
    unsigned char *wrapped_data = new unsigned char[cfg.g_w * cfg.g_h * 3 / 2];
    vpx_image_t *wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420,
                                           cfg.g_w, cfg.g_h, 1, wrapped_data);
    
    if (!input_img || !wrapped_img) {
        if (input_img) vpx_img_free(input_img);
        delete[] wrapped_data;
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill input images with some data
    for (int i = 0; i < 3; i++) {
        int stride = input_img->stride[i];
        int height = (i == 0) ? cfg.g_h : cfg.g_h / 2;
        int width = (i == 0) ? cfg.g_w : cfg.g_w / 2;
        
        for (int y = 0; y < height; y++) {
            memset(input_img->planes[i] + y * stride, y % 256, width);
            memset(wrapped_img->planes[i] + y * stride, (y + 128) % 256, width);
        }
    }
    
    // Get global headers (API 13)
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&encoder);
    if (headers && headers->buf) {
        fwrite(headers->buf, 1, headers->sz, out_file);
    }
    
    // Encode frames (API 12)
    for (int frame_cnt = 0; frame_cnt < 5; frame_cnt++) {
        // Use alternating images
        vpx_image_t *img_to_encode = (frame_cnt % 2 == 0) ? input_img : wrapped_img;
        
        // Encode the frame
        vpx_codec_encode(&encoder, img_to_encode, frame_cnt, 1, 
                         (frame_cnt == 0) ? VPX_EFLAG_FORCE_KF : 0, VPX_DL_REALTIME);
        
        // Get encoded data (API 14)
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Get preview frame (API 2)
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
    }
    
    // Flush encoder
    vpx_codec_encode(&encoder, nullptr, -1, 1, 0, VPX_DL_REALTIME);
    
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Now decode the output back
    fflush(out_file);
    rewind(out_file);
    
    // Get decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_ctx_t decoder;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(input_img);
        vpx_img_free(wrapped_img);
        delete[] wrapped_data;
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode the encoded data
    fseek(out_file, 0, SEEK_END);
    size_t out_size = ftell(out_file);
    rewind(out_file);
    
    uint8_t *encoded_data = new uint8_t[out_size];
    fread(encoded_data, 1, out_size, out_file);
    
    // Decode the data
    if (vpx_codec_decode(&decoder, encoded_data, out_size, nullptr, 0) == VPX_CODEC_OK) {
        // Get decoded frames (API 15)
        vpx_codec_iter_t dec_iter = nullptr;
        vpx_image_t *decoded_img;
        while ((decoded_img = vpx_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
            // Frame is available but we don't need to process it for fuzzing
        }
    }
    
    // Cleanup
    delete[] encoded_data;
    vpx_codec_destroy(&decoder);
    
    // Free images (API 16)
    vpx_img_free(input_img);
    vpx_img_free(wrapped_img);
    delete[] wrapped_data;
    
    // Destroy encoder (API 17)
    vpx_codec_destroy(&encoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}

// Frame buffer callback implementations
static int  id_000045_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate new buffer if needed
    if (!fb_data->data || fb_data->size < min_size) {
        delete[] fb_data->data;
        fb_data->data = new uint8_t[min_size];
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->data;
    fb->size = fb_data->size;
    return 0;
}

static int  id_000045_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    // We don't actually release the buffer here since we reuse it
    return 0;
}

static void  id_000045_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2) {
    // Callback implementation - do nothing for fuzzing
}