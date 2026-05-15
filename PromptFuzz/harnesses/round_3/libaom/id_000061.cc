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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_wrap","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_img_free","aom_img_free","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_wrap","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_img_free"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks
static int  id_000061_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000061_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. ULEB decode first value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int decode_result = aom_uleb_decode(data, 
                                       size > 10 ? 10 : size, 
                                       &uleb_value, 
                                       &uleb_length);
    
    // 2. ULEB encode the value back with fixed size
    uint8_t encoded_buffer[20];
    size_t encoded_size = 0;
    int encode_result = aom_uleb_encode_fixed_size(
        uleb_value, 
        20, 
        10,  // pad to 10 bytes
        encoded_buffer, 
        &encoded_size
    );
    
    // 3. Create image using input data
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int stride_align = 1;
    
    // Calculate required image buffer size for I420
    size_t y_plane = width * height;
    size_t uv_plane = (width/2) * (height/2);
    size_t total_image_size = y_plane + 2 * uv_plane;
    
    // Allocate image buffer from input data
    uint8_t *img_data = nullptr;
    if (size >= total_image_size + 10) {
        img_data = (uint8_t *)malloc(total_image_size);
        if (img_data) {
            memcpy(img_data, data + 10, total_image_size);
        }
    } else {
        // If not enough data, allocate zeroed buffer
        img_data = (uint8_t *)calloc(1, total_image_size);
    }
    
    if (!img_data) {
	return 0;
	}
	
    
    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, 
                                           AOM_IMG_FMT_I420, 
                                           width, 
                                           height, 
                                           stride_align, 
                                           img_data);
    
    if (!wrapped_img) {
        free(img_data);
        return 0;
    }
    
    // 4. Also test img_alloc
    aom_image_t *allocated_img = aom_img_alloc(nullptr, 
                                              AOM_IMG_FMT_I420, 
                                              width, 
                                              height, 
                                              1);
    
    // 5. Get plane dimensions
    int plane_width = aom_img_plane_width(wrapped_img, 0);
    int plane_height = aom_img_plane_height(wrapped_img, 0);
    
    // 6. Initialize encoder
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Get default config
    aom_codec_enc_config_default(encoder, &cfg, 0);
    
    // Set some configuration from input data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = (unsigned int)(uleb_value % 1000) + 100;
    
    // Initialize encoder
    aom_codec_err_t init_result = aom_codec_enc_init_ver(&codec, 
                                                        encoder, 
                                                        &cfg, 
                                                        0, 
                                                        AOM_ENCODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(init_result);
        aom_img_free(wrapped_img);
        if (allocated_img) aom_img_free(allocated_img);
        free(img_data);
        return 0;
    }
    
    // 7. Set encoder options
    char bitrate_str[20];
    snprintf(bitrate_str, sizeof(bitrate_str), "%u", cfg.rc_target_bitrate);
    aom_codec_set_option(&codec, "bitrate", bitrate_str);
    
    // 8. Set frame buffer functions
    void *cb_priv = nullptr;
    aom_codec_set_frame_buffer_functions(&codec, 
                                         id_000061_get_frame_buffer, 
                                         id_000061_release_frame_buffer, 
                                        cb_priv);
    
    // 9. Re-set config (test config set API)
    aom_codec_enc_config_set(&codec, &cfg);
    
    // 10. Prepare output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.sz = 1024 * 1024;  // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    
    if (output_buf.buf) {
        aom_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    }
    
    // 11. Encode the image
    aom_codec_encode(&codec, wrapped_img, 0, 1, 0);
    
    // 12. Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame data available in pkt->data.frame.buf
        }
    }
    
    // 13. Get error detail if any
    const char *error_detail = aom_codec_error_detail(&codec);
    
    // 14. Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(wrapped_img);
    if (allocated_img) aom_img_free(allocated_img);
    free(img_data);
    if (output_buf.buf) free(output_buf.buf);
    
    return 0;
}