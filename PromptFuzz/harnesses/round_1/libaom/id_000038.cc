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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_get_caps","aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_codec_encode","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_get_caps","aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_remove_metadata","aom_codec_encode","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Looking at this scenario, I'll create a fuzz driver that simulates processing an AV1 video stream: decoding it, modifying the decoded frames, and re-encoding. This fits well with the provided APIs.

*/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    aom_codec_ctx_t decoder, encoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    // Step 1: Check codec capabilities
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    
    // Step 2: Try to parse ULEB encoded value from the beginning of data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 10) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // Step 3: Initialize decoder with basic config
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Step 4: Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Step 5: Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Step 6: Try to get frames and process them
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    // Initialize encoder with basic configuration
    aom_codec_enc_cfg_t enc_cfg;
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Step 7: Initialize encoder
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Process up to 5 frames to limit execution time
    int frame_count = 0;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr && frame_count < 5) {
        frame_count++;
        
        // Step 8: Get metadata if available
        const aom_metadata_t *metadata = nullptr;
        size_t num_metadata = aom_img_num_metadata(img);
        if (num_metadata > 0) {
            metadata = aom_img_get_metadata(img, 0);
        }
        
        // Step 9: Create a copy of the image to modify
        aom_image_t modified_img;
        memset(&modified_img, 0, sizeof(modified_img));
        
        // Try to allocate image
        aom_img_alloc(&modified_img, img->fmt, img->d_w, img->d_h, 16);
        
        // Copy image data if allocation succeeded
        if (modified_img.planes[0] != nullptr) {
            for (int plane = 0; plane < 3; plane++) {
                if (img->planes[plane] && modified_img.planes[plane]) {
                    int plane_width = aom_img_plane_width(&modified_img, plane);
                    int plane_height = aom_img_plane_height(&modified_img, plane);
                    int copy_size = plane_width * plane_height;
                    
                    if (img->stride[plane] == modified_img.stride[plane]) {
                        memcpy(modified_img.planes[plane], img->planes[plane], copy_size);
                    }
                }
            }
            
            // Step 10: Apply rectangle modification based on input data
            // Use some bytes from input to determine rectangle parameters
            unsigned int rect_x = (data[0] % (img->d_w / 2)) & ~0x1;
            unsigned int rect_y = (data[1] % (img->d_h / 2)) & ~0x1;
            unsigned int rect_w = (data[2] % 32) + 16;
            unsigned int rect_h = (data[3] % 32) + 16;
            unsigned int border = data[4] % 8;
            
            // Ensure rectangle is within bounds
            if (rect_x + rect_w > img->d_w) rect_w = img->d_w - rect_x;
            if (rect_y + rect_h > img->d_h) rect_h = img->d_h - rect_y;
            
            if (rect_w > 0 && rect_h > 0) {
                aom_img_set_rect(&modified_img, rect_x, rect_y, rect_w, rect_h, border);
            }
            
            // Step 11: Remove metadata from the modified image (if it had any)
            aom_img_remove_metadata(&modified_img);
            
            // Step 12: Encode the modified frame
            aom_codec_encode(&encoder, &modified_img, frame_count, 1, 0);
            
            // Clean up the modified image
            aom_img_free(&modified_img);
        }
    }
    
    // Step 13: Flush encoder
    aom_codec_encode(&encoder, nullptr, -1, 0, 0);
    
    // Step 14: Get encoded data (just to ensure encoding completed)
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Packet processed
    }
    
    // Step 15: Cleanup
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    return 0;
}