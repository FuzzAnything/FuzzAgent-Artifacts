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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks for external buffer management
static int  id_000058_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = static_cast<uint8_t*>(cb_priv);
    fb->size = 0; // Will be set by decoder
    return 0;
}

static int  id_000058_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for processing
    
    // Create a FILE* from input data
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
    
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_codec_stream_info_t si = {0};
    vpx_fixed_buf_t *global_headers = nullptr;
    
    // Initialize decoder with VP9
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for external buffer management
    // Allocate buffer for frame data
    uint8_t *frame_buffer = static_cast<uint8_t*>(malloc(1024 * 1024)); // 1MB buffer
    if (frame_buffer) {
        vpx_codec_set_frame_buffer_functions(&decoder,  id_000058_get_frame_buffer, 
                                            id_000058_release_frame_buffer, frame_buffer);
    }
    
    // Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_1);
    
    // Get stream information from decoder
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&decoder, &si);
    
    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Initialize encoder with VP9
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Get default encoder configuration
        if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_2) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            if (frame_buffer) free(frame_buffer);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder configuration based on decoded image
        cfg.g_w = decoded_img->d_w;
        cfg.g_h = decoded_img->d_h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_error_resilient = 1;
        
        // Initialize encoder
        if (vpx_codec_enc_init(&encoder, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            if (frame_buffer) free(frame_buffer);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Get global headers from encoder
        global_headers = vpx_codec_get_global_headers(&encoder);
        if (global_headers && global_headers->buf && global_headers->sz > 0) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
        }
        
        // Create image for encoding
        img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
        if (img) {
            // Copy decoded image data (simplified - in reality would need proper conversion)
            if (decoded_img->fmt == VPX_IMG_FMT_I420) {
                for (int plane = 0; plane < 3; plane++) {
                    int src_stride = decoded_img->stride[plane];
                    int dst_stride = img->stride[plane];
                    int height = (plane == 0) ? cfg.g_h : cfg.g_h / 2;
                    int width = (plane == 0) ? cfg.g_w : cfg.g_w / 2;
                    
                    for (int y = 0; y < height; y++) {
                        memcpy(img->planes[plane] + y * dst_stride,
                               decoded_img->planes[plane] + y * src_stride,
                               width);
                    }
                }
                
                // Encode the image
                vpx_codec_encode(&encoder, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
                
                // Get preview frame after encoding
                const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                if (preview) {
                    // Preview frame available - could process it here
                    (void)preview;
                }
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }
        }
    }
    
    // Cleanup
    if (img) vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    if (frame_buffer) free(frame_buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}