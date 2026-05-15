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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 1
//<Quality> {"density":31,"unique_branches":{"aom_codec_destroy":[[72,7,72,18,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_obu_type_to_string","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_error","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_img_alloc","aom_codec_enc_config_default","aom_img_free","aom_codec_enc_init_ver","aom_codec_error","aom_img_free","aom_codec_enc_config_set","aom_codec_error","aom_codec_get_global_headers","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_obu_type_to_string","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_error","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_img_alloc","aom_codec_enc_config_default","aom_img_free","aom_codec_enc_init_ver","aom_codec_error","aom_img_free","aom_codec_enc_config_set","aom_codec_error","aom_codec_get_global_headers","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (!data || size == 0) {
	return 0;
	}
	
    
    // Initialize variables
    aom_codec_ctx_t decoder_ctx = {};
    aom_codec_ctx_t encoder_ctx = {};
    aom_codec_stream_info_t stream_info = {};
    aom_codec_enc_cfg_t encoder_cfg = {};
    aom_codec_dec_cfg_t decoder_cfg = {};
    aom_image_t *decoded_img = nullptr;
    aom_image_t *allocated_img = nullptr;
    aom_codec_iter_t iter = nullptr;
    aom_fixed_buf_t *global_headers = nullptr;
    
    // 1. Get version information
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Mark as used
    
    // 2. Get codec interfaces
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (!decoder_iface || !encoder_iface) {
	return 0;
	}
	
    
    // 3. Get interface names
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    (void)decoder_name; // Mark as used
    (void)encoder_name; // Mark as used
    
    // 4. OBU type string (use a valid OBU_TYPE value from enumeration)
    const char *obu_str = aom_obu_type_to_string(OBU_FRAME);
    (void)obu_str; // Mark as used
    
    // Initialize decoder
    decoder_cfg.threads = 1;
    decoder_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                &decoder_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        (void)error_detail; // Mark as used
        goto cleanup;
    }
    
    // Set frame buffer functions (use nullptr callbacks for simplicity)
    aom_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, nullptr);
    
    // Decode input data
    err = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        (void)error_detail; // Mark as used
    }
    
    // Get stream information
    err = aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        (void)error_detail; // Mark as used
    }
    
    // Get decoded frames
    while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // Use image plane functions
        int plane_width = aom_img_plane_width(decoded_img, 0);
        int plane_height = aom_img_plane_height(decoded_img, 0);
        (void)plane_width; // Mark as used
        (void)plane_height; // Mark as used
        
        // Remove metadata
        aom_img_remove_metadata(decoded_img);
        
        // Allocate a new image for encoding
        allocated_img = aom_img_alloc(nullptr, decoded_img->fmt, 
                                     decoded_img->d_w, decoded_img->d_h, 1);
        if (!allocated_img) continue;
        
        // Copy image data (simplified - just use first plane)
        if (allocated_img->planes[0] && decoded_img->planes[0]) {
            size_t copy_size = std::min(
                static_cast<size_t>(allocated_img->stride[0] * allocated_img->d_h),
                static_cast<size_t>(decoded_img->stride[0] * decoded_img->d_h)
            );
            memcpy(allocated_img->planes[0], decoded_img->planes[0], copy_size);
        }
        
        // Initialize encoder configuration
        err = aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
        if (err != AOM_CODEC_OK) {
            aom_img_free(allocated_img);
            allocated_img = nullptr;
            continue;
        }
        
        // Set encoder dimensions from decoded image
        encoder_cfg.g_w = decoded_img->d_w;
        encoder_cfg.g_h = decoded_img->d_h;
        encoder_cfg.g_bit_depth = static_cast<aom_bit_depth_t>(decoded_img->bit_depth);
        encoder_cfg.g_input_bit_depth = decoded_img->bit_depth;
        
        // Initialize encoder
        err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, 
                                    &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (err != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error(&encoder_ctx);
            (void)error_detail; // Mark as used
            aom_img_free(allocated_img);
            allocated_img = nullptr;
            continue;
        }
        
        // Set encoder configuration (again to ensure it's applied)
        err = aom_codec_enc_config_set(&encoder_ctx, &encoder_cfg);
        if (err != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error(&encoder_ctx);
            (void)error_detail; // Mark as used
        }
        
        // Get global headers
        global_headers = aom_codec_get_global_headers(&encoder_ctx);
        if (global_headers) {
            free(global_headers->buf);
            free(global_headers);
            global_headers = nullptr;
        }
        
        // Encode the image
        err = aom_codec_encode(&encoder_ctx, allocated_img, 0, 1, 0);
        if (err != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error(&encoder_ctx);
            (void)error_detail; // Mark as used
        }
        
        // Get encoded data (iterate through packets)
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t encoder_iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &encoder_iter)) != nullptr) {
            // Process encoded packet if needed
            (void)pkt; // Mark as used
        }
        
        // Clean up encoder for this frame
        aom_codec_destroy(&encoder_ctx);
        aom_img_free(allocated_img);
        allocated_img = nullptr;
    }

cleanup:
    // Free allocated resources
    if (allocated_img) {
        aom_img_free(allocated_img);
    }
    
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    aom_codec_destroy(&decoder_ctx);
    aom_codec_destroy(&encoder_ctx);
    
    return 0;
}