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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"vpx_codec_get_cx_data":[[255,14,255,25,0,0,4,0]],"vpx_codec_enc_config_default":[[162,25,162,35,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
        return 0;
    }

    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read some data from the file to determine dimensions
    uint8_t header[4];
    if (fread(header, 1, 4, in_file) != 4) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Reset file pointer
    fseek(in_file, 0, SEEK_SET);

    // Determine image dimensions from input (use first bytes modulo reasonable values)
    unsigned int width = 64 + (header[0] % 192);  // 64-256
    unsigned int height = 64 + (header[1] % 192); // 64-256
    unsigned int usage = header[2] % 3;           // 0-2

    // DECODER PATH
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx(); // Need decoder interface
    
    // Peek stream info first
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    vpx_codec_err_t peek_res = vpx_codec_peek_stream_info(
        decoder_iface, data, static_cast<unsigned int>(size), &si);
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = width;
    dec_cfg.h = height;
    
    vpx_codec_err_t dec_init_res = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    // Try to decode if initialization succeeded
    if (dec_init_res == VPX_CODEC_OK) {
        vpx_codec_err_t decode_res = vpx_codec_decode(
            &decoder, data, static_cast<unsigned int>(size), nullptr, 0);
        
        // Get error info (even if decode succeeded)
        const char* dec_error = vpx_codec_error(&decoder);
        const char* dec_error_detail = vpx_codec_error_detail(&decoder);
        
        // Try to get decoded frame
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
        
        // ENCODER PATH - single encoder
        vpx_codec_ctx_t encoder_single;
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        
        // Get default config
        vpx_codec_enc_cfg_t cfg_single;
        vpx_codec_enc_config_default(encoder_iface, &cfg_single, usage);
        
        // Set dimensions
        cfg_single.g_w = width;
        cfg_single.g_h = height;
        cfg_single.rc_target_bitrate = 256; // Low bitrate for fuzzing
        
        // Initialize single encoder
        vpx_codec_enc_init_ver(
            &encoder_single, encoder_iface, &cfg_single, 0, VPX_ENCODER_ABI_VERSION);
        
        // Create an image for encoding
        vpx_image_t* input_image = vpx_img_alloc(
            nullptr, VPX_IMG_FMT_I420, width, height, 16);
        
        // Fill image with some data if allocation succeeded
        if (input_image) {
            // Fill Y plane
            size_t y_plane_size = width * height;
            for (size_t i = 0; i < y_plane_size && i < size; i++) {
                input_image->planes[0][i] = data[i % size];
            }
            
            // Fill U and V planes (subsampled)
            size_t uv_plane_size = (width * height) / 4;
            for (size_t i = 0; i < uv_plane_size && i < size / 2; i++) {
                input_image->planes[1][i] = data[(i + 128) % size];
                input_image->planes[2][i] = data[(i + 256) % size];
            }
            
            // Encode the image
            vpx_codec_encode(
                &encoder_single, input_image, 0, 1, 0, VPX_DL_REALTIME);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t* pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder_single, &enc_iter))) {
                // Just consume the packet
            }
            
            // Try to get preview frame
            const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&encoder_single);
            
            // Get encoder error info
            const char* enc_error = vpx_codec_error(&encoder_single);
            const char* enc_error_detail = vpx_codec_error_detail(&encoder_single);
            
            // Clean up encoder single
            vpx_codec_destroy(&encoder_single);
            
            // ENCODER PATH - multi encoder
            vpx_codec_ctx_t encoder_multi;
            vpx_codec_enc_cfg_t cfg_multi;
            vpx_codec_enc_config_default(encoder_iface, &cfg_multi, usage);
            cfg_multi.g_w = width;
            cfg_multi.g_h = height;
            cfg_multi.rc_target_bitrate = 256;
            
            // Initialize multi encoder (with num_enc = 1 for simplicity)
            vpx_codec_enc_init_multi_ver(
                &encoder_multi, encoder_iface, &cfg_multi, 1, 0, nullptr, VPX_ENCODER_ABI_VERSION);
            
            // Try to encode with multi encoder too
            vpx_codec_encode(
                &encoder_multi, input_image, 0, 1, 0, VPX_DL_REALTIME);
            
            // Get multi encoder error info
            const char* enc_multi_error = vpx_codec_error(&encoder_multi);
            const char* enc_multi_error_detail = vpx_codec_error_detail(&encoder_multi);
            
            // Clean up multi encoder
            vpx_codec_destroy(&encoder_multi);
            
            // Free the allocated image
            vpx_img_free(input_image);
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    // Close the input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}