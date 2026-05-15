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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"vp8_dc2quant":[[59,14,59,24,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom callback structures
struct CallbackData {
    std::vector<vpx_image_t*> frames;
    FILE* output_file;
};

// Put frame callback
static void  id_000078_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    CallbackData* cb_data = static_cast<CallbackData*>(user_priv);
    if (!cb_data || !img) return;
    
    // Store a reference to the frame (not copying, just keeping pointer)
    // In real usage we would copy the image data
    cb_data->frames.push_back(const_cast<vpx_image_t*>(img));
    
    // Write some metadata to output file
    if (cb_data->output_file) {
        fprintf(cb_data->output_file, "Frame received: %dx%d, fmt: %d\n", 
                img->d_w, img->d_h, img->fmt);
    }
}

// Put slice callback (simplified version)
static void  id_000078_put_slice_cb(void* user_priv, const vpx_image_t* img, 
                        const vpx_image_rect_t* rect, const vpx_image_rect_t* skip) {
    CallbackData* cb_data = static_cast<CallbackData*>(user_priv);
    if (!cb_data || !img || !rect) return;
    
    if (cb_data->output_file) {
        fprintf(cb_data->output_file, "Slice: x=%u, y=%u, w=%u, h=%u\n",
                rect->x, rect->y, rect->w, rect->h);
    }
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need minimal data for headers
    
    // Initialize output file for callbacks
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	return 0;
	}
	
    
    CallbackData cb_data;
    cb_data.output_file = out_file;
    
    // Step 1: Peek at stream info to determine codec type
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    if (peek_result != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        peek_result = vpx_codec_peek_stream_info(
            decoder_iface, data, size, &stream_info);
    }
    
    if (peek_result != VPX_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 2: Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_result = vpx_codec_dec_init(
        &decoder, decoder_iface, &dec_cfg, 0);
    
    if (init_result != VPX_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 3: Register callbacks
    vpx_codec_register_put_frame_cb(&decoder,  id_000078_put_frame_cb, &cb_data);
    vpx_codec_register_put_slice_cb(&decoder,  id_000078_put_slice_cb, &cb_data);
    
    // Step 4: Decode the input data
    vpx_codec_decode(&decoder, data, size, NULL, 0);
    
    // Step 5: Get stream info from decoder context
    vpx_codec_stream_info_t decoder_stream_info;
    memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
    decoder_stream_info.sz = sizeof(decoder_stream_info);
    
    vpx_codec_get_stream_info(&decoder, &decoder_stream_info);
    
    // Step 6: Initialize encoder (using VP8 encoder as example)
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    if (vpx_codec_enc_init(&encoder, encoder_iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 7: Set up encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set some basic configuration
    enc_cfg.g_w = stream_info.w ? stream_info.w : 320;
    enc_cfg.g_h = stream_info.h ? stream_info.h : 240;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Set the encoder configuration
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Step 8: Create a test image and use vpx_img_set_rect
    vpx_image_t* test_img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                          enc_cfg.g_w, enc_cfg.g_h, 1);
    if (test_img) {
        // Set a rectangle on the image (using fuzzed data for coordinates)
        if (size >= 8) {
            unsigned int x = data[0] % (enc_cfg.g_w / 2);
            unsigned int y = data[1] % (enc_cfg.g_h / 2);
            unsigned int w = 16 + (data[2] % (enc_cfg.g_w / 4));
            unsigned int h = 16 + (data[3] % (enc_cfg.g_h / 4));
            
            vpx_img_set_rect(test_img, x, y, w, h);
        }
        
        // Step 9: Encode the test image and get encoded data
        vpx_codec_encode(&encoder, test_img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Step 10: Get encoded data using vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write some packet info to output
                fprintf(out_file, "Encoded packet: %zu bytes, pts: %ld\n",
                        pkt->data.frame.sz, pkt->data.frame.pts);
            }
        }
        
        vpx_img_free(test_img);
    }
    
    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	return 0;
}