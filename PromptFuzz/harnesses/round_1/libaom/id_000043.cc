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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_destroy","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_destroy","aom_img_wrap","aom_img_metadata_alloc","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_encode","aom_codec_err_to_string","aom_codec_destroy","aom_img_metadata_free","aom_codec_destroy"],"critical_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_img_wrap","aom_img_metadata_alloc","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_encode","aom_codec_err_to_string","aom_codec_destroy","aom_img_metadata_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Too small for meaningful testing
    }

    // Event: Decode an encoded AV1 bitstream, get frames, wrap them,
    // add metadata, and attempt to re-encode them
    
    // Step 1: Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    aom_codec_err_t res;
    
    res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, 
                                  AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Use error string API
        const char* err_str = aom_codec_err_to_string(res);
        (void)err_str; // Prevent unused variable warning
        return 0;
    }
    
    // Step 2: Decode input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(res);
        (void)err_str;
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Step 3: Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Step 4: Use codec interface name API
        aom_codec_iface_t* iface = aom_codec_av1_cx();
        const char* iface_name = aom_codec_iface_name(iface);
        (void)iface_name; // Prevent unused variable warning
        
        // Step 5: Create a copy of the image data using aom_img_wrap
        // We'll use a subset of the input data as image data for demonstration
        size_t img_data_size = decoded_img->stride[0] * decoded_img->d_h;
        unsigned char* img_buffer = (unsigned char*)malloc(img_data_size);
        if (!img_buffer) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // Fill with some data (using first part of input)
        size_t copy_size = (size < img_data_size) ? size : img_data_size;
        memcpy(img_buffer, data, copy_size);
        
        aom_image_t wrapped_img;
        aom_image_t* wrapped = aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420,
                                           decoded_img->d_w, decoded_img->d_h,
                                           1, img_buffer);
        
        if (wrapped) {
            // Step 6: Create and add metadata
            uint32_t metadata_type = 1;
            size_t metadata_size = 16;
            uint8_t metadata_data[16];
            
            // Use some input data for metadata
            size_t metadata_copy_size = (size - 10 < metadata_size) ? 
                                        size - 10 : metadata_size;
            if (metadata_copy_size > 0) {
                memcpy(metadata_data, data + 10, metadata_copy_size);
            }
            
            aom_metadata_t* metadata = aom_img_metadata_alloc(
                metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
            
            if (metadata) {
                // Step 7: Initialize encoder for re-encoding attempt
                aom_codec_ctx_t encoder;
                aom_codec_enc_cfg_t enc_cfg;
                
                // Get default encoder configuration
                res = aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
                if (res == AOM_CODEC_OK) {
                    // Set basic parameters
                    enc_cfg.g_w = decoded_img->d_w;
                    enc_cfg.g_h = decoded_img->d_h;
                    enc_cfg.g_timebase.num = 1;
                    enc_cfg.g_timebase.den = 30;
                    enc_cfg.rc_target_bitrate = 1000;
                    
                    // Step 8: Initialize encoder
                    res = aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), 
                                                 &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                    
                    if (res == AOM_CODEC_OK) {
                        // Attempt to encode the wrapped image
                        res = aom_codec_encode(&encoder, wrapped, 0, 1, 0);
                        
                        const char* err_str = aom_codec_err_to_string(res);
                        (void)err_str;
                        
                        aom_codec_destroy(&encoder);
                    }
                }
                
                // Clean up metadata
                aom_img_metadata_free(metadata);
            }
        }
        
        // Clean up
        free(img_buffer);
    }
    
    // Step 9: Clean up decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}