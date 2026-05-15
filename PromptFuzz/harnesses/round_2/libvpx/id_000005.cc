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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"vp8_get_last_ref_updates":[[647,7,647,18,0,0,4,0],[649,9,649,20,0,0,4,0],[649,9,649,20,0,0,4,1]],"vp8_get_frame_corrupted":[[687,7,687,16,0,0,4,0],[687,20,687,23,0,0,4,0],[687,20,687,23,0,0,4,1],[689,9,689,22,0,0,4,0]]},"library_calls":["vpx_codec_version","vpx_codec_build_config","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_wrap","vpx_img_flip","vpx_img_free","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_control_","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_version","vpx_codec_build_config","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_wrap","vpx_img_flip","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_control_","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Fuzz driver
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 100) {
        return 0;
    }

    // 1. Get library version information
    int version = vpx_codec_version();
    const char* build_config = vpx_codec_build_config();
    
    // Use version and build_config to avoid unused variable warnings
    (void)version;
    (void)build_config;

    // 2. Create a decoder interface for VP8
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    if (!decoder_iface) {
        return 0;
    }

    // 3. Get decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;

    // 4. Create an encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx(); // Get encoder interface
    
    // Get default encoder configuration
    vpx_codec_err_t err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // 5. Create image from input data
    // We'll use part of the input data as image data
    const size_t image_data_size = size / 2;
    if (image_data_size < 100) {
        return 0;
    }

    // Allocate image data buffer
    unsigned char* img_data = (unsigned char*)malloc(image_data_size);
    if (!img_data) {
        return 0;
    }
    
    // Copy data to image buffer
    memcpy(img_data, data, image_data_size);

    // Create image wrapper
    vpx_image_t img;
    vpx_image_t* wrapped_img = vpx_img_wrap(&img, 
                                           VPX_IMG_FMT_I420,  // Common format
                                           128,               // Width
                                           128,               // Height
                                           1,                 // Stride alignment
                                           img_data);         // Image data
    
    if (!wrapped_img) {
        free(img_data);
        return 0;
    }

    // 6. Flip the image
    vpx_img_flip(&img);

    // 7. Try to create a decoder context with the remaining data
    FILE* in_file = fmemopen((void*)(data + image_data_size), 
                            size - image_data_size, "rb");
    if (!in_file) {
        vpx_img_free(&img);
        free(img_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                VPX_DECODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        // Try to decode the data
        fseek(in_file, 0, SEEK_END);
        long file_size = ftell(in_file);
        fseek(in_file, 0, SEEK_SET);
        
        if (file_size > 0) {
            uint8_t* decode_data = (uint8_t*)malloc(file_size);
            if (decode_data) {
                fread(decode_data, 1, file_size, in_file);
                
                // Try to peek stream info
                vpx_codec_stream_info_t si;
                memset(&si, 0, sizeof(si));
                si.sz = sizeof(si);
                
                vpx_codec_peek_stream_info(decoder_iface, decode_data, 
                                          file_size, &si);
                
                // Try to decode
                vpx_codec_decode(&decoder_ctx, decode_data, file_size, 
                               NULL, 0);
                
                free(decode_data);
            }
        }
        
        // Try to use control API with various control IDs
        // We'll try a few common control IDs
        int control_ids[] = {VP8D_GET_LAST_REF_UPDATES, 
                           VP8D_GET_FRAME_CORRUPTED};
        
        for (int ctrl_id : control_ids) {
            int value = 0;
            vpx_codec_control_(&decoder_ctx, ctrl_id, &value);
        }
        
        // Destroy decoder
        vpx_codec_destroy(&decoder_ctx);
    }

    // 8. Create an encoder context with the flipped image
    vpx_codec_ctx_t encoder_ctx;
    err = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0,
                                VPX_ENCODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        // Try to encode the flipped image
        vpx_codec_encode(&encoder_ctx, &img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Try some encoder controls
        vpx_codec_control_(&encoder_ctx, VP8E_SET_CPUUSED, 8);
        vpx_codec_control_(&encoder_ctx, VP8E_SET_NOISE_SENSITIVITY, 0);
        
        // Destroy encoder
        vpx_codec_destroy(&encoder_ctx);
    }

    // 9. Clean up
    assert_file_closed(&in_file);;
    vpx_img_free(&img);
    // Note: vpx_img_free doesn't free the img_data we allocated,
    // so we need to free it ourselves
    free(img_data);

    assert_file_closed(&in_file);
	return 0;
}