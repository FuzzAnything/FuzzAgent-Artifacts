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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 6
//<Quality> {"density":19,"unique_branches":{"vp8e_encode":[[925,7,925,10,0,0,4,1],[989,9,989,20,0,0,4,1]],"validate_img":[[293,5,293,26,0,0,4,1],[295,5,295,12,0,0,4,0]],"vp8_get_compressed_data":[[4916,9,4916,14,0,0,4,0],[4916,18,4916,32,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000022_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Store or process the decoded frame if needed
    // For fuzzing, we just acknowledge the callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd if needed
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try VP9 decoder first
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    
    // Also try VP8 decoder interface
    vpx_codec_iface_t *vp8_dec_iface = vpx_codec_vp8_dx();
    
    // Get codec interface names
    const char *vp9_name = vpx_codec_iface_name(dec_iface);
    const char *vp8_name = vpx_codec_iface_name(vp8_dec_iface);
    (void)vp9_name;
    (void)vp8_name;
    
    // Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, dec_iface, &dec_cfg, 0, 
                                                  VPX_DECODER_ABI_VERSION);
    
    // If VP9 fails, try VP8
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        err = vpx_codec_dec_init_ver(&decoder, vp8_dec_iface, &dec_cfg, 0, 
                                      VPX_DECODER_ABI_VERSION);
    }
    
    // Check error
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        const char *ctx_err = vpx_codec_error(&decoder);
        (void)err_str;
        (void)ctx_err;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000022_put_frame_callback, nullptr);
    
    // Create encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx(); // Use VP8 encoder
    err = vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0);
    
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.rc_target_bitrate = 256;
    
    // Create encoder context
    vpx_codec_ctx_t encoder;
    vpx_rational_t dsf = {1, 1};
    
    // Initialize encoder (multi-encoder with 1 instance)
    err = vpx_codec_enc_init_multi_ver(&encoder, enc_iface, &enc_cfg, 1, 0, &dsf, 
                                        VPX_ENCODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        const char *ctx_err = vpx_codec_error(&encoder);
        (void)err_str;
        (void)ctx_err;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set encoder configuration again
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img;
    std::vector<vpx_image_t*> frames;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        frames.push_back(img);
        
        // Encode the decoded frame
        vpx_codec_encode(&encoder, img, frames.size() - 1, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Flush encoder
    vpx_codec_encode(&encoder, nullptr, -1, 1, 0, VPX_DL_REALTIME);
    
    // Get remaining encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}