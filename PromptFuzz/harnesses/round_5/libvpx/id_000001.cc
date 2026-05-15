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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_img_alloc","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_error_detail","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Get version string
    const char *version_str = vpx_codec_version_str();
    (void)version_str; // Use to avoid unused variable warning
    
    // Step 2: Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Get codec interface for VP9 decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Step 4: Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use to avoid unused variable warning
    
    // Step 5: Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_flags_t flags = 0;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, flags, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Get error detail
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use to avoid unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Allocate image buffer for decoded frame
    // Use reasonable default dimensions for fuzzing
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 640, 480, 1);
    if (!img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 7: Read and decode data
    // Create buffer for reading
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read input data
    fseek(in_file, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, in_file);
    
    // Try to decode the data
    err = vpx_codec_decode(&decoder, buffer, bytes_read, NULL, 0);
    
    // Get error detail if decoding failed
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use to avoid unused variable warning
    }
    
    // Step 8: Try to get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    (void)decoded_img; // Use to avoid unused variable warning
    
    // Step 9: Initialize encoder for re-encoding
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Get error detail
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail; // Use to avoid unused variable warning
    } else {
        // Initialize encoder with default config
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            const char *error_detail = vpx_codec_error_detail(&encoder);
            (void)error_detail; // Use to avoid unused variable warning
        } else {
            // Try to encode if we have a valid image
            if (img) {
                // Set some image data for fuzzing
                // Fill image planes with pattern based on input data
                size_t offset = 0;
                for (int plane = 0; plane < 3; plane++) {
                    int plane_width = (plane == 0) ? img->d_w : img->d_w / 2;
                    int plane_height = (plane == 0) ? img->d_h : img->d_h / 2;
                    
                    for (int y = 0; y < plane_height; y++) {
                        for (int x = 0; x < plane_width; x++) {
                            if (offset < size) {
                                img->planes[plane][y * img->stride[plane] + x] = data[offset % size];
                                offset++;
                            }
                        }
                    }
                }
                
                // Encode the image
                err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                if (err != VPX_CODEC_OK) {
                    const char *error_detail = vpx_codec_error_detail(&encoder);
                    (void)error_detail; // Use to avoid unused variable warning
                }
            }
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Step 10: Clean up
    free(buffer);
    vpx_img_free(img);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}