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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"aom_img_get_metadata":[[433,7,433,12,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_get_stream_info","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_img_get_metadata","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_get_metadata","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small for meaningful processing

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Get error message
        const char *error_str = aom_codec_error(&decoder);
        return 0;
    }

    // Try to get stream info from the input data
    aom_codec_stream_info_t stream_info = {0};
    err = aom_codec_get_stream_info(&decoder, &stream_info);
    // Note: This may fail if we haven't decoded anything yet, but we're calling it anyway

    // Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        // Get error message
        const char *error_str = aom_codec_error(&decoder);
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Get metadata from the decoded image
        const aom_metadata_t *meta = aom_img_get_metadata(decoded_img, 0);
        
        // Set a rectangle on the image (use safe coordinates)
        unsigned int x = data[0] % (decoded_img->d_w > 0 ? decoded_img->d_w : 1);
        unsigned int y = data[1] % (decoded_img->d_h > 0 ? decoded_img->d_h : 1);
        unsigned int w = data[2] % ((decoded_img->d_w > x ? decoded_img->d_w - x : 1));
        unsigned int h = data[3] % ((decoded_img->d_h > y ? decoded_img->d_h - y : 1));
        w = w > 0 ? w : 1;
        h = h > 0 ? h : 1;
        
        aom_img_set_rect(decoded_img, x, y, w, h, 0);
        
        // Create and add metadata
        if (size > 20) {
            uint32_t meta_type = 1; // Example type
            size_t meta_data_size = size > 100 ? 50 : size / 2;
            aom_metadata_insert_flags_t flags = AOM_MIF_ANY_FRAME;
            
            // Allocate metadata
            aom_metadata_t *new_meta = aom_img_metadata_alloc(
                meta_type, 
                data + 10, 
                meta_data_size, 
                flags
            );
            
            if (new_meta) {
                // Add metadata to image
                aom_img_add_metadata(
                    decoded_img, 
                    meta_type, 
                    data + 10, 
                    meta_data_size, 
                    flags
                );
                // Free the allocated metadata
                aom_img_metadata_free(new_meta);
            }
        }
    }

    // Create an image using aom_img_wrap
    const int wrap_width = 64;
    const int wrap_height = 64;
    const int stride_align = 1;
    size_t img_size = wrap_width * wrap_height * 3 / 2; // For I420 format
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    
    if (img_data) {
        // Initialize with some data
        size_t init_size = size < img_size ? size : img_size;
        memcpy(img_data, data, init_size);
        
        aom_image_t wrapped_img;
        aom_image_t *img_ptr = aom_img_wrap(
            &wrapped_img, 
            AOM_IMG_FMT_I420, 
            wrap_width, 
            wrap_height, 
            stride_align, 
            img_data
        );
        
        if (img_ptr) {
            // Use the wrapped image briefly
            int plane_width = aom_img_plane_width(img_ptr, 0);
            int plane_height = aom_img_plane_height(img_ptr, 0);
        }
        free(img_data);
    }

    // Initialize encoder for set_cx_data_buf
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err == AOM_CODEC_OK) {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;
        
        err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (err == AOM_CODEC_OK) {
            // Create buffer for set_cx_data_buf
            aom_fixed_buf_t buf;
            size_t buf_size = 1024;
            buf.buf = malloc(buf_size);
            buf.sz = buf_size;
            
            if (buf.buf) {
                // Initialize with some data
                size_t copy_size = size < buf_size ? size : buf_size;
                memcpy(buf.buf, data, copy_size);
                
                // Set buffer for encoder
                aom_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
                
                free(buf.buf);
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }

    // Clean up decoder
    aom_codec_destroy(&decoder);

    return 0;
}