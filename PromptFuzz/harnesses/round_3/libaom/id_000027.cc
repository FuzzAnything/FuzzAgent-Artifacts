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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 6
//<Quality> {"density":22,"unique_branches":{"validate_config":[[667,11,667,28,52,0,4,1]],"validate_img":[[990,7,990,31,0,0,4,0]],"encoder_encode":[[3264,9,3264,28,0,0,4,1],[3363,7,3363,26,0,0,4,1]],"encoder_set_config":[[1626,7,1626,31,0,0,4,0],[1627,9,1627,33,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_err_to_string","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_error_detail","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_err_to_string","aom_codec_enc_config_set","aom_codec_err_to_string","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_encode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_err_to_string","aom_codec_enc_config_set","aom_codec_err_to_string","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_encode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Initialize encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
	return 0;
	}
	
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    memset(&encoder, 0, sizeof(encoder));
    
    // Set default encoder configuration
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    unsigned int usage = 0;  // Good quality
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &cfg, usage);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        return 0;
    }
    
    // Initialize encoder
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        const char *err_detail = aom_codec_error_detail(&encoder);
        return 0;
    }
    
    // Use ULEB encoding on some data
    uint64_t uleb_value = 0;
    if (size >= sizeof(uint64_t)) {
        memcpy(&uleb_value, data, sizeof(uint64_t));
    }
    uint8_t uleb_buffer[16];
    size_t uleb_coded_size = 0;
    size_t pad_to_size = 8;
    int uleb_result = aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 
                                                pad_to_size, uleb_buffer, &uleb_coded_size);
    
    // Set encoder options using ULEB encoded data as string
    char option_value[32];
    snprintf(option_value, sizeof(option_value), "%u", (unsigned int)uleb_value);
    err = aom_codec_set_option(&encoder, "cpu-used", option_value);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
    }
    
    // Update encoder configuration with modified settings
    cfg.g_w = 64 + (data[0] % 192);  // Width between 64-256
    cfg.g_h = 64 + (data[1] % 192);  // Height between 64-256
    cfg.rc_target_bitrate = 100 + (data[2] % 900);  // Bitrate 100-1000 kbps
    err = aom_codec_enc_config_set(&encoder, &cfg);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
    }
    
    // Allocate image for encoding
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image planes with fuzzer data
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_w = aom_img_plane_width(img, plane);
        int plane_h = aom_img_plane_height(img, plane);
        uint8_t *plane_ptr = img->planes[plane];
        
        for (int y = 0; y < plane_h; y++) {
            size_t bytes_to_copy = plane_w;
            if (data_offset + bytes_to_copy > size) {
                bytes_to_copy = size - data_offset;
            }
            if (bytes_to_copy > 0) {
                memcpy(plane_ptr, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            plane_ptr += img->stride[plane];
            
            if (data_offset >= size) break;
        }
        if (data_offset >= size) break;
    }
    
    // Prepare output buffer for encoded data
    std::vector<uint8_t> output_buffer(size * 2);  // Allocate double input size
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = output_buffer.data();
    cx_buf.sz = output_buffer.size();
    
    // Set output buffer for encoded data
    err = aom_codec_set_cx_data_buf(&encoder, &cx_buf, 0, 0);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
    }
    
    // Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    err = aom_codec_encode(&encoder, img, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        const char *err_detail = aom_codec_error_detail(&encoder);
    }
    
    // Get encoded data packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process encoded packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame data available in pkt->data.frame.buf
        }
    }
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    return 0;
}