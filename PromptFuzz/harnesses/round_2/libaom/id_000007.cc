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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_img_alloc","aom_img_wrap","aom_img_set_rect","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_err_to_string","aom_img_free","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_img_num_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_img_alloc","aom_img_wrap","aom_img_set_rect","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_err_to_string","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_img_num_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for any meaningful processing
    if (size < 16) {
        return 0;
    }

    // 1. Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Allocate image using aom_img_alloc
    aom_image_t img;
    unsigned int width = 128;
    unsigned int height = 128;
    unsigned int align = 16;
    aom_image_t *allocated_img = aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, align);
    if (!allocated_img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Wrap another image using aom_img_wrap
    unsigned char *img_data = (unsigned char *)malloc(width * height * 3 / 2);
    aom_image_t wrapped_img;
    aom_image_t *wrapped = aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, width, height, align, img_data);
    
    // 4. Set rectangle on allocated image using aom_img_set_rect
    unsigned int rect_x = 10;
    unsigned int rect_y = 10;
    unsigned int rect_w = 50;
    unsigned int rect_h = 50;
    unsigned int border = 2;
    aom_img_set_rect(allocated_img, rect_x, rect_y, rect_w, rect_h, border);

    // 5. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Get interface name using aom_codec_iface_name
    const char *iface_name = aom_codec_iface_name(iface);
    
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init(&decoder, iface, &cfg, 0);
    
    // 6. Get error information using aom_codec_error
    const char *error_detail = aom_codec_error(&decoder);
    
    // 7. Convert error code to string using aom_codec_err_to_string
    const char *error_string = aom_codec_err_to_string(res);
    
    if (res != AOM_CODEC_OK) {
        // Clean up and exit if decoder init failed
        aom_img_free(allocated_img);
        free(img_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Decode input data
    size_t bytes_read = fread(img_data, 1, width * height / 4, in_file);
    if (bytes_read > 0) {
        aom_codec_decode(&decoder, img_data, bytes_read, NULL);
    }
    
    // 9. Get compressed data from encoder (simulating encoding scenario)
    // Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(enc_iface, &enc_cfg, 0);
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    
    if (aom_codec_enc_init(&encoder, enc_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Encode the allocated image
        aom_codec_encode(&encoder, allocated_img, 0, 1, 0);
        
        // 10. Get compressed data using aom_codec_get_cx_data
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process packet if needed
                break;
            }
        }
        
        // 11. Get number of metadata using aom_img_num_metadata
        size_t num_metadata = aom_img_num_metadata(allocated_img);
        
        aom_codec_destroy(&encoder);
    }
    
    // 12. Clean up all allocated resources
    aom_codec_destroy(&decoder);
    aom_img_free(allocated_img);
    free(img_data);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}