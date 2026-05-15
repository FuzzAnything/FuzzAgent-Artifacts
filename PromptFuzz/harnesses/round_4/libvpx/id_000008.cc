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
//<score> 73.333336, nr_unique_branch: 15
//<Quality> {"density":22,"unique_branches":{"vp9_alloc_context_buffers":[[136,9,136,38,0,0,4,0]],"vp8e_set_config":[[471,7,471,31,0,0,4,1],[471,35,471,59,0,0,4,1],[484,7,484,56,0,0,4,1],[488,7,488,26,0,0,4,1],[504,7,504,45,0,0,4,1]],"vp9_lookahead_pop":[[168,7,168,10,0,0,4,1]],"vp8_get_compressed_data":[[4916,9,4916,14,0,0,4,0],[4916,18,4916,32,0,0,4,1]],"alloc_compressor_data":[[1386,7,1386,59,0,0,4,0]],"vp9_enc_alloc_mi":[[886,7,886,15,0,0,4,0]],"vp8e_encode":[[925,7,925,10,0,0,4,1],[989,9,989,20,0,0,4,1]],"vpx_codec_encode":[[199,16,199,19,0,0,4,1]],"encoder_encode":[[1401,7,1401,18,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy"],"visited":2}
/**/


// Include libvpx headers

// Callback function for put_frame
static void  id_000008_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is called when a frame is decoded
    // We can process the frame here if needed
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Variables for codec interfaces
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Try to determine codec type by peeking at stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    
    // If VP9 fails, try VP8
    if (peek_err != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        peek_err = vpx_codec_peek_stream_info(
            decoder_iface, data, (unsigned int)size, &stream_info);
    }
    
    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Mark as used
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_error(&decoder);
        (void)error_str; // Mark as used
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put_frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(
        &decoder,  id_000008_put_frame_callback, nullptr);
    (void)reg_err; // Mark as used
    
    // Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(
        &decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    
    // Get stream info from decoder
    vpx_codec_stream_info_t dec_stream_info;
    memset(&dec_stream_info, 0, sizeof(dec_stream_info));
    dec_stream_info.sz = sizeof(dec_stream_info);
    
    vpx_codec_err_t get_info_err = vpx_codec_get_stream_info(
        &decoder, &dec_stream_info);
    (void)get_info_err; // Mark as used
    
    // Initialize encoder for re-encoding
    if (decoder_iface == vpx_codec_vp9_dx()) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
        encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    
    if (cfg_err == VPX_CODEC_OK && dec_stream_info.w > 0 && dec_stream_info.h > 0) {
        // Set encoder configuration based on decoded stream info
        enc_cfg.g_w = dec_stream_info.w;
        enc_cfg.g_h = dec_stream_info.h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256; // Low bitrate for fuzzing
        
        // Initialize encoder
        vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(
            &encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
        
        if (enc_init_err == VPX_CODEC_OK) {
            // Set encoder configuration (this API must be called)
            vpx_codec_err_t set_cfg_err = vpx_codec_enc_config_set(
                &encoder, &enc_cfg);
            (void)set_cfg_err; // Mark as used
            
            // Get decoded frames and re-encode them
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t *img;
            
            while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Encode the frame
                vpx_codec_err_t encode_err = vpx_codec_encode(
                    &encoder, img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);
                (void)encode_err; // Mark as used
            }
            
            // Flush encoder
            vpx_codec_err_t flush_err = vpx_codec_encode(
                &encoder, nullptr, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
            (void)flush_err; // Mark as used
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write to output file (for demonstration)
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Get error message from decoder (this API must be called)
    const char *dec_error = vpx_codec_error(&decoder);
    (void)dec_error; // Mark as used
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}