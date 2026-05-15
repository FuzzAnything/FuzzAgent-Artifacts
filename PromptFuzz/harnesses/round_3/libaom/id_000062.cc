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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 8
//<Quality> {"density":21,"unique_branches":{"set_good_speed_features_framesize_independent":[[1113,7,1113,35,0,0,4,0],[1490,7,1490,40,0,0,4,0]],"arg_match_helper":[[54,10,54,42,0,0,4,1],[55,10,55,43,0,0,4,1]],"arg_parse_uint_helper":[[93,7,93,26,0,0,4,1],[93,30,93,47,0,0,4,1],[20,7,20,14,2,0,4,0]],"encoder_set_option":[[4374,14,4375,42,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_free","aom_img_wrap","aom_codec_av1_cx","aom_img_free","aom_codec_enc_config_default","aom_img_free","aom_codec_enc_init_ver","aom_img_free","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_err_to_string","aom_codec_enc_config_set","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_err_to_string","aom_codec_enc_config_set","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks for aom_codec_set_frame_buffer_functions
static int  id_000062_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000062_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. ULEB decode/encode operations
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    const uint8_t *uleb_ptr = data;
    size_t uleb_available = size > 50 ? 50 : size;
    
    // Decode ULEB from input
    int decode_result = aom_uleb_decode(uleb_ptr, uleb_available, &uleb_value, &uleb_length);
    
    // Encode ULEB with fixed size
    uint8_t coded_buffer[20];
    size_t coded_size = 0;
    if (decode_result == 0) {
        aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_buffer), 10, 
                                   coded_buffer, &coded_size);
    }
    
    // 2. Create image using aom_img_alloc
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    
    aom_image_t *img_alloc = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, align);
    if (!img_alloc) {
	return 0;
	}
	
    
    // Fill image planes with some data
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_w = aom_img_plane_width(img_alloc, plane);
        int plane_h = aom_img_plane_height(img_alloc, plane);
        size_t plane_size = plane_w * plane_h;
        
        if (data_offset + plane_size > size) break;
        
        for (int y = 0; y < plane_h; ++y) {
            uint8_t *row = img_alloc->planes[plane] + y * img_alloc->stride[plane];
            size_t copy_size = plane_w;
            if (data_offset + copy_size > size) copy_size = size - data_offset;
            if (copy_size > 0) {
                memcpy(row, data + data_offset, copy_size);
                data_offset += copy_size;
            }
        }
    }
    
    // 3. Create another image using aom_img_wrap
    aom_image_t img_wrap;
    unsigned char *img_data = (unsigned char *)malloc(width * height * 3 / 2);
    if (!img_data) {
        aom_img_free(img_alloc);
        return 0;
    }
    
    // Copy some data to image buffer
    size_t copy_size = width * height * 3 / 2;
    if (copy_size > size) copy_size = size;
    if (copy_size > 0) {
        memcpy(img_data, data, copy_size);
    }
    
    aom_image_t *wrapped_img = aom_img_wrap(&img_wrap, AOM_IMG_FMT_I420, width, height, 
                                            align, img_data);
    
    // 4. Initialize encoder
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    if (!encoder) {
        free(img_data);
        aom_img_free(img_alloc);
        return 0;
    }
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    unsigned int usage = 0; // Good quality
    if (aom_codec_enc_config_default(encoder, &cfg, usage) != AOM_CODEC_OK) {
        free(img_data);
        aom_img_free(img_alloc);
        return 0;
    }
    
    // Set some configuration from input data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        free(img_data);
        aom_img_free(img_alloc);
        return 0;
    }
    
    // 5. Set encoder options from input data
    // Use part of input as option name/value
    if (size > 100) {
        char opt_name[50];
        char opt_value[50];
        size_t name_len = size % 50;
        if (name_len > 49) name_len = 49;
        
        memcpy(opt_name, data, name_len);
        opt_name[name_len] = '\0';
        
        size_t value_len = (size / 2) % 50;
        if (value_len > 49) value_len = 49;
        
        memcpy(opt_value, data + name_len, value_len);
        opt_value[value_len] = '\0';
        
        aom_codec_set_option(&codec, opt_name, opt_value);
    }
    
    // 6. Set frame buffer functions
    void *cb_priv = NULL;
    aom_codec_set_frame_buffer_functions(&codec,  id_000062_get_frame_buffer, 
                                          id_000062_release_frame_buffer, cb_priv);
    
    // 7. Set output buffer
    aom_fixed_buf_t output_buf;
    uint8_t *output_data = (uint8_t *)malloc(10000);
    if (output_data) {
        output_buf.buf = output_data;
        output_buf.sz = 10000;
        aom_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    }
    
    // 8. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    aom_codec_encode(&codec, img_alloc, pts, duration, flags);
    
    // 9. Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet if needed
    }
    
    // 10. Get error details
    const char *error_detail = aom_codec_error_detail(&codec);
    const char *error_str = aom_codec_err_to_string(codec.err);
    
    // 11. Try to set config again (just to use the API)
    aom_codec_enc_config_set(&codec, &cfg);
    
    // 12. Cleanup
    if (output_data) free(output_data);
    aom_codec_destroy(&codec);
    free(img_data);
    aom_img_free(img_alloc);
    
    return 0;
}