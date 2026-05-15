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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_set_rect","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_multi_ver","vpx_codec_error","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_set_rect","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_error","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_error","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*Looking at the provided APIs, I'll create a fuzzer that simulates a video encoding scenario where we:
1. Parse input data as an encoded stream
2. Setup a multi-encoder configuration
3. Allocate and manipulate image buffers
4. Register callback functions for frame/slice processing

Here's the complete implementation:

*/


// Callback function prototypes
static void  id_000044_put_slice_callback(void* user_priv, const vpx_image_t* img, 
                               const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

static void  id_000044_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void *)data, size, "rb");
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
    
    // Initialize variables
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t* img = nullptr;
    vpx_codec_stream_info_t si;
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = NULL;
    int flags = 0;
    int ret = 0;
    
    // Step 1: Peek stream info from input data
    // Try VP9 first, then VP8 if that fails
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (vpx_codec_peek_stream_info(decoder_iface, data, 
                                    (unsigned int)(size > 1000 ? 1000 : size), &si) != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        if (vpx_codec_peek_stream_info(decoder_iface, data, 
                                        (unsigned int)(size > 1000 ? 1000 : size), &si) != VPX_CODEC_OK) {
            // Cannot determine stream info, use defaults
            si.w = 640;
            si.h = 480;
        }
    }
    
    // Step 2: Allocate image buffer
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, si.w, si.h, 16);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set rectangle on image (using partial image area)
    unsigned int rect_w = si.w > 100 ? 100 : si.w / 2;
    unsigned int rect_h = si.h > 100 ? 100 : si.h / 2;
    ret = vpx_img_set_rect(img, 0, 0, rect_w, rect_h);
    if (ret != 0) {
        // If rectangle fails, try smaller rectangle
        vpx_img_set_rect(img, 0, 0, 10, 10);
    }
    
    // Step 4: Get codec capabilities
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    
    // Step 5: Initialize multi-encoder
    // Create configuration for 2 encoders
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        encoder_iface = vpx_codec_vp8_cx();
        caps = vpx_codec_get_caps(encoder_iface);
        if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
            vpx_img_free(img);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Configure encoder settings based on fuzzer input
    cfg.g_w = si.w;
    cfg.g_h = si.h;
    cfg.g_threads = (data[0] % 4) + 1; // 1-4 threads
    cfg.rc_target_bitrate = 1000 + (data[1] % 4000); // 1000-5000 kbps
    
    // Create downsampling factors for multi-encoder
    vpx_rational_t dsf[2];
    dsf[0].num = 1;
    dsf[0].den = 1;
    dsf[1].num = 2;
    dsf[1].den = 1;
    
    // Initialize multi-encoder (2 encoders)
    if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 2, 
                                     flags, dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Check error
        const char* error = vpx_codec_error(&encoder);
        (void)error; // Suppress unused warning
        
        // Try single encoder initialization as fallback
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 
                                   flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
            vpx_img_free(img);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Step 6: Set encoder configuration
    // Modify some settings based on fuzzer input
    cfg.rc_min_quantizer = data[2] % 63;
    cfg.rc_max_quantizer = 63 - (data[3] % 32);
    if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&encoder);
        (void)error; // Suppress unused warning
    }
    
    // Step 7: Register callbacks
    if (vpx_codec_register_put_slice_cb(&encoder,  id_000044_put_slice_callback, NULL) != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&encoder);
        (void)error; // Suppress unused warning
    }
    
    if (vpx_codec_register_put_frame_cb(&encoder,  id_000044_put_frame_callback, NULL) != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&encoder);
        (void)error; // Suppress unused warning
    }
    
    // Step 8: Fill image with some data from fuzzer input
    // Simple pattern filling for fuzzing
    size_t data_offset = 0;
    for (unsigned int plane = 0; plane < 3; plane++) {
        unsigned int plane_width = (plane == 0) ? si.w : si.w / 2;
        unsigned int plane_height = (plane == 0) ? si.h : si.h / 2;
        
        for (unsigned int y = 0; y < plane_height && data_offset < size; y++) {
            for (unsigned int x = 0; x < plane_width && data_offset < size; x++) {
                img->planes[plane][y * img->stride[plane] + x] = data[data_offset++];
            }
        }
    }
    
    // Step 9: Encode the image
    // Encode multiple frames if we have enough data
    int frame_count = 0;
    while (frame_count < 3 && data_offset + 100 < size) {
        vpx_codec_encode(&encoder, img, frame_count * 1000, 
                        100, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Modify image slightly for next frame
        if (data_offset + img->stride[0] < size) {
            memcpy(img->planes[0], &data[data_offset], 
                   img->stride[0] > size - data_offset ? size - data_offset : img->stride[0]);
            data_offset += img->stride[0];
        }
        
        frame_count++;
    }
    
    // Step 10: Flush encoder
    vpx_codec_encode(&encoder, NULL, frame_count * 1000, 
                    100, 0, VPX_DL_REALTIME);
    
    // Get remaining encoded data
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_img_free(img);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}