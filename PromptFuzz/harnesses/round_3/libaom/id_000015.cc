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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_height","aom_img_plane_width","aom_uleb_encode_fixed_size","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_uleb_encode_fixed_size","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_set_option at least once
    // Set a random encoder option based on fuzzer data
    const char *option_name = "cpu-used";
    char option_value[32];
    snprintf(option_value, sizeof(option_value), "%u", data[0] % 9); // cpu-used 0-8
    aom_codec_set_option(&encoder, option_name, option_value);
    
    // Create test image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with some data from fuzzer input
    size_t img_data_needed = 64 * 64 * 3 / 2; // I420 format
    size_t read_size = (size - 1 < img_data_needed) ? size - 1 : img_data_needed;
    
    // Read image data from fuzzer input
    fseek(in_file, 0, SEEK_SET);
    for (unsigned int plane = 0; plane < 3; ++plane) {
        unsigned char *plane_ptr = img->planes[plane];
        int stride = img->stride[plane];
        int plane_height = aom_img_plane_height(img, plane);
        int plane_width = aom_img_plane_width(img, plane);
        
        for (int y = 0; y < plane_height; ++y) {
            size_t bytes_read = fread(plane_ptr + y * stride, 1, plane_width, in_file);
            if (bytes_read < (size_t)plane_width) {
                // Pad with zeros if not enough data
                memset(plane_ptr + y * stride + bytes_read, 0, plane_width - bytes_read);
            }
        }
    }
    
    // Use aom_uleb_encode_fixed_size at least once
    uint64_t uleb_value = static_cast<uint64_t>(data[0]) | (static_cast<uint64_t>(data[1]) << 8);
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_buffer), 8, coded_buffer, &coded_size);
    
    // Prepare buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.sz = 1024 * 1024; // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    if (!output_buf.buf) {
        aom_img_free(img);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_set_cx_data_buf at least once
    aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
    
    // Encode the image
    aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Check for errors and use aom_codec_error_detail
    const char *error_detail = aom_codec_error_detail(&encoder);
    if (error_detail) {
        // We can log or handle the error detail if needed
    }
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    free(output_buf.buf);
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}