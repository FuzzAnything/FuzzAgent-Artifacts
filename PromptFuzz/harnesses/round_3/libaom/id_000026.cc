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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"validate_config":[[667,32,667,49,52,0,4,1]],"update_extra_cfg":[[1758,7,1758,26,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set some configuration values based on input data
    cfg.g_w = 64 + (data[0] % 64);
    cfg.g_h = 64 + (data[0] % 64);
    cfg.g_threads = 1 + (data[0] % 4);
    cfg.rc_target_bitrate = 100 + (data[0] % 900);
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Use aom_uleb_encode_fixed_size to encode a value
    uint64_t uleb_value = size;
    uint8_t uleb_buffer[10];
    size_t coded_size = 0;
    int uleb_result = aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 
                                                  sizeof(uleb_buffer), uleb_buffer, &coded_size);

    // Set encoder options based on input data
    char option_value[32];
    snprintf(option_value, sizeof(option_value), "%u", (unsigned int)data[0] % 100);
    err = aom_codec_set_option(&encoder, "cpu-used", option_value);
    
    snprintf(option_value, sizeof(option_value), "%u", (unsigned int)data[0] % 10);
    err = aom_codec_set_option(&encoder, "end-usage", option_value);

    // Apply configuration again
    err = aom_codec_enc_config_set(&encoder, &cfg);

    // Allocate image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Fill image planes with data from input
    size_t data_offset = 1;
    for (int plane = 0; plane < 3; plane++) {
        int plane_w = aom_img_plane_width(img, plane);
        int plane_h = aom_img_plane_height(img, plane);
        int stride = img->stride[plane];
        
        for (int y = 0; y < plane_h; y++) {
            if (data_offset >= size) {
                data_offset = 1; // Wrap around
            }
            
            uint8_t *row = img->planes[plane] + y * stride;
            size_t bytes_to_copy = std::min(plane_w, (int)(size - data_offset));
            if (bytes_to_copy > 0) {
                memcpy(row, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
        }
    }

    // Prepare output buffer for encoded data
    size_t output_buf_size = 1024 * 1024;
    uint8_t *output_buffer = (uint8_t *)malloc(output_buf_size);
    if (!output_buffer) {
        aom_img_free(img);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    aom_fixed_buf_t output_fb;
    output_fb.buf = output_buffer;
    output_fb.sz = output_buf_size;

    // Set output buffer for encoded data
    err = aom_codec_set_cx_data_buf(&encoder, &output_fb, 0, 0);

    // Encode the image
    err = aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Check error and get error details if any
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        const char *error_detail = aom_codec_error_detail(&encoder);
        (void)error_str;
        (void)error_detail;
    }

    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process the packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Packet contains encoded frame data
        }
    }

    // Clean up
    free(output_buffer);
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}