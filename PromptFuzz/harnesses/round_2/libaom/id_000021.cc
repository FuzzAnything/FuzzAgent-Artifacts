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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use aom_codec_build_config() - just call it for info
    const char* build_config = aom_codec_build_config();
    (void)build_config; // Suppress unused warning

    // Create a file pointer using fmemopen for input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data (assumed to be AV1 bitstream)
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(res);
        (void)error_str; // Suppress unused warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read all data from the memory file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    std::vector<uint8_t> input_buffer(input_size);
    fread(input_buffer.data(), 1, input_size, in_file);
    assert_file_closed(&in_file);;

    // Decode the input data
    res = aom_codec_decode(&decoder_ctx, input_buffer.data(), input_size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(res);
        (void)error_str;
        aom_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_img = nullptr;
    std::vector<aom_image_t*> decoded_images;
    
    while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter))) {
        decoded_images.push_back(decoded_img);
        
        // Allocate a new image to process
        aom_image_t* processed_img = aom_img_alloc(nullptr, decoded_img->fmt, 
                                                   decoded_img->d_w, decoded_img->d_h, 1);
        if (!processed_img) {
            continue;
        }
        
        // Copy the decoded image
        for (int plane = 0; plane < 3; ++plane) {
            int width = aom_img_plane_width(processed_img, plane);
            int height = aom_img_plane_height(processed_img, plane);
            
            if (width > 0 && height > 0) {
                uint8_t* dst = processed_img->planes[plane];
                const uint8_t* src = decoded_img->planes[plane];
                int dst_stride = processed_img->stride[plane];
                int src_stride = decoded_img->stride[plane];
                
                for (int y = 0; y < height; ++y) {
                    memcpy(dst, src, width);
                    dst += dst_stride;
                    src += src_stride;
                }
            }
        }
        
        // Apply image operations
        // Flip the image
        aom_img_flip(processed_img);
        
        // Set a rectangle (random region based on input data)
        if (processed_img->d_w > 10 && processed_img->d_h > 10) {
            unsigned int x = data[0] % (processed_img->d_w - 5);
            unsigned int y = (data[1] % (processed_img->d_h - 5)) + 1;
            unsigned int w = 5;
            unsigned int h = 5;
            
            // Use aom_img_set_rect to clear a small region
            // Note: This function may not be available in all versions, handle gracefully
            // For demonstration, we'll try to use it if available
            #ifdef AOM_IMG_SET_RECT_AVAILABLE
            aom_img_set_rect(processed_img, x, y, w, h, 0);
            #endif
        }
        
        // Add metadata using ULEB encoded value
        uint8_t uleb_buffer[16];
        size_t coded_size = 0;
        
        // Encode a value from input data
        uint64_t value_to_encode = (size > 2) ? data[2] : 42;
        int encode_res = aom_uleb_encode_fixed_size(value_to_encode, sizeof(uleb_buffer), 
                                                   4, uleb_buffer, &coded_size);
        
        if (encode_res == 0 && coded_size > 0) {
            // Add metadata to image
            uint32_t metadata_type = 1; // Custom type
            aom_img_add_metadata(processed_img, metadata_type, uleb_buffer, 
                                coded_size, AOM_MIF_ANY_FRAME);
        }
        
        // Now encode the processed image
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            aom_codec_ctx_t encoder_ctx;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Set basic encoding parameters
                enc_cfg.g_w = processed_img->d_w;
                enc_cfg.g_h = processed_img->d_h;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_threads = 1;
                enc_cfg.g_profile = 0;
                enc_cfg.g_bit_depth = (aom_bit_depth_t)processed_img->bit_depth;
                enc_cfg.g_input_bit_depth = processed_img->bit_depth;
                
                // Initialize encoder
                res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                           0, AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // Set some encoder control
                    // Use a control ID based on availability
                    #if defined(AOME_SET_CPUUSED)
                    aom_codec_control(&encoder_ctx, AOME_SET_CPUUSED, 8);
                    #endif
                    
                    // Encode the processed image
                    aom_codec_encode(&encoder_ctx, processed_img, 0, 1, 0);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t* pkt = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter))) {
                        // Process the packet if needed
                        (void)pkt;
                    }
                    
                    // Clean up encoder
                    aom_codec_destroy(&encoder_ctx);
                }
            }
        }
        
        // Free the processed image
        aom_img_free(processed_img);
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder_ctx);
    
    assert_file_closed(&in_file);
	return 0;
}