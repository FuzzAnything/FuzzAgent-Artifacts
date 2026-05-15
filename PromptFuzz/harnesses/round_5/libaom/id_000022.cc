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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 5
//<Quality> {"density":16,"unique_branches":{"set_good_speed_feature_framesize_dependent":[[737,9,737,16,0,0,4,0]],"set_good_speed_features_framesize_independent":[[1106,43,1106,50,0,0,4,0]],"encoder_init":[[3076,11,3076,57,0,0,4,0]],"aom_codec_enc_init_ver":[[71,12,71,48,0,0,4,0],[72,12,72,55,0,0,4,1]]},"library_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks
static int  id_000022_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Allocate frame buffer
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return 1;
    fb->size = min_size;
    return 0;
}

static int  id_000022_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
        fb->data = NULL;
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data
    
    // 1. First use aom_uleb_encode
    uint64_t uleb_value = 12345;
    uint8_t uleb_buffer[32];
    size_t coded_size = 0;
    aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);
    
    // 2. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t cfg;
    
    // 3. Get default config
    unsigned int usage = 0; // Good quality
    aom_codec_enc_config_default(encoder_iface, &cfg, usage);
    
    // Set basic config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // 4. Initialize encoder with version
    aom_codec_err_t enc_init_res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &cfg, 
                                                          AOM_CODEC_USE_HIGHBITDEPTH, AOM_ENCODER_ABI_VERSION);
    
    if (enc_init_res != AOM_CODEC_OK) {
        // 5. Get error detail if init failed
        const char *error_detail = aom_codec_error_detail(&encoder_ctx);
        return 0;
    }
    
    // 6. Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&encoder_ctx,  id_000022_get_frame_buffer, 
                                         id_000022_release_frame_buffer, NULL);
    
    // 7. Create image from input data
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int stride_align = 32;
    
    // Calculate required buffer size for I420 format
    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    size_t total_size = y_size + 2 * uv_size;
    
    if (size < total_size) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // Create image wrapper using input data
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, 
                                            stride_align, (unsigned char *)data);
    
    if (!wrapped_img) {
        aom_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // 8. Update encoder config (just for demonstration)
    cfg.rc_target_bitrate = 2000;
    aom_codec_enc_config_set(&encoder_ctx, &cfg);
    
    // 9. Set output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        aom_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, 0);
    }
    
    // 10. Add metadata to image (using uleb encoded value)
    uint32_t metadata_type = 1; // Example type
    aom_img_add_metadata(wrapped_img, metadata_type, uleb_buffer, coded_size, 
                        AOM_MIF_KEY_FRAME);
    
    // 11. Encode the image
    aom_codec_encode(&encoder_ctx, wrapped_img, 0, 1, 0);
    
    // 12. Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame packet available
        }
    }
    
    // 13. Free image resources
    aom_img_free(wrapped_img);
    
    // 14. Destroy encoder
    aom_codec_destroy(&encoder_ctx);
    
    // Free output buffer
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    return 0;
}