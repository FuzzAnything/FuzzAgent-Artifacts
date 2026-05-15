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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_decode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Create a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Initialize encoder context
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    
    // Default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some basic parameters from input data
    cfg.g_w = (data[0] % 64 + 1) * 16;  // 16-1024 pixels
    cfg.g_h = (data[1] % 64 + 1) * 16;  // 16-1024 pixels
    cfg.rc_target_bitrate = (data[2] % 1000 + 100) * 1000;  // 100-1100 kbps
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &cfg, 0, 
                               VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Allocate image buffer for encoding
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with data from input file
    size_t bytes_to_read = cfg.g_w * cfg.g_h * 3 / 2;  // I420 format
    if (bytes_to_read > 0) {
        // Read Y plane
        fread(img->planes[0], 1, cfg.g_w * cfg.g_h, in_file);
        // Read U plane (quarter size)
        fread(img->planes[1], 1, cfg.g_w * cfg.g_h / 4, in_file);
        // Read V plane (quarter size)
        fread(img->planes[2], 1, cfg.g_w * cfg.g_h / 4, in_file);
    }
    
    // 5. Set output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.sz = size;  // Use input size as buffer size
    output_buf.buf = malloc(size);
    if (!output_buf.buf) {
        vpx_img_free(img);
        vpx_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (vpx_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, 0) != VPX_CODEC_OK) {
        free(output_buf.buf);
        vpx_img_free(img);
        vpx_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Encode the image
    vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    size_t encoded_size = 0;
    uint8_t *encoded_data = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            encoded_data = (uint8_t*)pkt->data.frame.buf;
            encoded_size = pkt->data.frame.sz;
            break;
        }
    }
    
    // 7. Initialize decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, 0, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        free(output_buf.buf);
        vpx_img_free(img);
        vpx_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Decode the encoded data (if any) OR original input
    if (encoded_data && encoded_size > 0) {
        vpx_codec_decode(&decoder_ctx, encoded_data, encoded_size, NULL, 0);
    } else {
        // Decode from original input data
        fseek(in_file, 0, SEEK_SET);
        uint8_t *input_copy = (uint8_t*)malloc(size);
        if (input_copy) {
            fread(input_copy, 1, size, in_file);
            vpx_codec_decode(&decoder_ctx, input_copy, size, NULL, 0);
            free(input_copy);
        }
    }
    
    // Get decoded frame
    vpx_codec_iter_t dec_iter = NULL;
    vpx_image_t *decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder_ctx, &dec_iter)) != NULL) {
        // Write some data to output file (simulating processing)
        if (out_file && decoded_img->planes[0]) {
            size_t bytes_to_write = decoded_img->d_w * decoded_img->d_h;
            if (bytes_to_write > 1000) bytes_to_write = 1000;
            fwrite(decoded_img->planes[0], 1, bytes_to_write, out_file);
        }
    }
    
    // 9. Cleanup
    free(output_buf.buf);
    vpx_img_free(img);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}