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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks
int  id_000052_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    static uint8_t *buffer = nullptr;
    static size_t buffer_size = 0;
    
    if (min_size > buffer_size) {
        free(buffer);
        buffer = (uint8_t *)malloc(min_size);
        buffer_size = buffer ? min_size : 0;
    }
    
    if (!buffer) {
        return -1;
    }
    
    fb->data = buffer;
    fb->size = buffer_size;
    return 0;
}

int  id_000052_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    // Don't free here - we manage the buffer in  id_000052_get_frame_buffer
    return 0;
}

// Callback functions
void  id_000052_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Callback for when a frame is available
}

void  id_000052_put_slice_cb(void *user_priv, const vpx_image_t *img,
                  const vpx_image_rect_t *rect, const vpx_image_rect_t *clip) {
    // Callback for when a slice is available
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Open input as file stream for reading encoded data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output file for writing re-encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Try VP9 decoder first, fall back to VP8
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), 
                                                  &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(), 
                                      &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Set frame buffer functions for decoder
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000052_get_frame_buffer, 
                                           id_000052_release_frame_buffer, nullptr);
    
    // Read and decode input data
    std::vector<uint8_t> encoded_data(size);
    size_t bytes_read = fread(encoded_data.data(), 1, size, in_file);
    
    if (bytes_read > 0) {
        // Decode the frame
        vpx_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), 
                        nullptr, 0);
        
        // Get stream info
        vpx_codec_stream_info_t si = {0};
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&decoder, &si);
        
        // Try to get a decoded frame
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);
        
        if (img) {
            // Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_enc_cfg_t enc_cfg;
            vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
            
            // Get default encoder configuration
            if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                
                // Initialize multi-encoder (using 1 encoder for simplicity)
                vpx_rational_t dsf = {1, 1};
                if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                                 1, 0, &dsf, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                    
                    // Register callbacks
                    vpx_codec_register_put_frame_cb(&encoder,  id_000052_put_frame_cb, nullptr);
                    vpx_codec_register_put_slice_cb(&encoder,  id_000052_put_slice_cb, nullptr);
                    
                    // Encode the frame
                    vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                    
                    // Try to get preview frame
                    vpx_codec_get_preview_frame(&encoder);
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    // Clean up encoder
                    vpx_codec_destroy(&encoder);
                }
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}