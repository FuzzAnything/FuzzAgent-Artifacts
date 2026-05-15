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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"vpx_codec_error_detail":[[61,12,61,21,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read initial data for configuration
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Read stream info
    fread(&si.w, sizeof(unsigned int), 1, in_file);
    fread(&si.h, sizeof(unsigned int), 1, in_file);
    fread(&si.is_kf, sizeof(unsigned int), 1, in_file);
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        if (error_detail) {
            // Use error detail
        }
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream info from decoder context
    vpx_codec_stream_info_t decoder_si;
    memset(&decoder_si, 0, sizeof(decoder_si));
    decoder_si.sz = sizeof(decoder_si);
    decoder_si.w = si.w;
    decoder_si.h = si.h;
    res = vpx_codec_get_stream_info(&decoder, &decoder_si);
    
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        if (error_detail) {
            // Use error detail
        }
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t cfg;
    res = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        if (error_detail) {
            // Use error detail
        }
    }
    
    // Set encoder configuration
    cfg.g_w = si.w;
    cfg.g_h = si.h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    
    res = vpx_codec_enc_config_set(&encoder, &cfg);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        if (error_detail) {
            // Use error detail
        }
    }
    
    // Initialize encoder with config
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        if (error_detail) {
            // Use error detail
        }
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a test image for encoding
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, si.w, si.h, 16);
    if (img) {
        // Fill image with test data from input
        size_t y_plane_size = si.w * si.h;
        size_t uv_plane_size = y_plane_size / 4;
        
        if (size > y_plane_size + 2 * uv_plane_size) {
            fread(img->planes[0], 1, y_plane_size, in_file);
            fread(img->planes[1], 1, uv_plane_size, in_file);
            fread(img->planes[2], 1, uv_plane_size, in_file);
        }
        
        // Encode the image
        res = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        if (res != VPX_CODEC_OK) {
            const char *error_detail = vpx_codec_error_detail(&encoder);
            if (error_detail) {
                // Use error detail
            }
        }
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Encoded frame available
                break;
            }
        }
        
        vpx_img_free(img);
    }
    
    // Try to decode some data
    fseek(in_file, 0, SEEK_SET);
    size_t remaining = size;
    while (remaining > 0) {
        size_t chunk_size = remaining > 4096 ? 4096 : remaining;
        uint8_t buffer[4096];
        size_t read = fread(buffer, 1, chunk_size, in_file);
        if (read == 0) break;
        
        res = vpx_codec_decode(&decoder, buffer, read, NULL, 0);
        if (res != VPX_CODEC_OK) {
            const char *error_detail = vpx_codec_error_detail(&decoder);
            if (error_detail) {
                // Use error detail
            }
        }
        
        // Get decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *frame;
        while ((frame = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
            // Frame successfully decoded
            // Use frame information if needed
        }
        
        remaining -= read;
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}