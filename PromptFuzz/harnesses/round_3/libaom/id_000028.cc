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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_err_to_string","aom_codec_error_detail","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_err_to_string","aom_codec_error_detail","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0;
    }

    // Use input data as configuration parameters
    uint32_t width = (data[0] << 8) | data[1];
    uint32_t height = (data[2] << 8) | data[3];
    uint64_t frame_index = (static_cast<uint64_t>(data[4]) << 56) |
                          (static_cast<uint64_t>(data[5]) << 48) |
                          (static_cast<uint64_t>(data[6]) << 40) |
                          (static_cast<uint64_t>(data[7]) << 32) |
                          (static_cast<uint64_t>(data[8]) << 24) |
                          (static_cast<uint64_t>(data[9]) << 16) |
                          (static_cast<uint64_t>(data[10]) << 8) |
                          static_cast<uint64_t>(data[11]);
    
    // Limit dimensions to reasonable values for fuzzing
    width = width % 256 + 1;
    height = height % 256 + 1;
    
    // Skip header bytes for image data
    const uint8_t* image_data = data + 12;
    size_t image_data_size = size - 12;
    
    // Create output file for encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Initialize encoder
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    
    // Get default encoder configuration
    res = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Configure encoder
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_Q;
    cfg.g_error_resilient = 0;
    
    aom_codec_ctx_t codec;
    res = aom_codec_enc_init_ver(&codec, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Set encoder option using encoded ULEB value
    uint8_t uleb_buffer[8];
    size_t coded_size;
    int uleb_result = aom_uleb_encode_fixed_size(frame_index, sizeof(uleb_buffer), 4, 
                                                uleb_buffer, &coded_size);
    if (uleb_result == 0) {
        char option_value[17];
        for (size_t i = 0; i < coded_size; i++) {
            snprintf(option_value + i * 2, 3, "%02x", uleb_buffer[i]);
        }
        aom_codec_set_option(&codec, "frame-index", option_value);
    }
    
    // Update encoder configuration
    res = aom_codec_enc_config_set(&codec, &cfg);
    if (res != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(res);
        const char* error_detail = aom_codec_error_detail(&codec);
        // Error strings can be used for debugging
    }
    
    // Allocate image
    aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, 16);
    if (!img) {
        aom_codec_destroy(&codec);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Fill image planes with input data
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = aom_img_plane_width(img, plane);
        int plane_height = aom_img_plane_height(img, plane);
        uint8_t* plane_ptr = img->planes[plane];
        
        for (int y = 0; y < plane_height; y++) {
            if (data_offset + plane_width > image_data_size) {
                break;
            }
            memcpy(plane_ptr, image_data + data_offset, plane_width);
            plane_ptr += img->stride[plane];
            data_offset += plane_width;
        }
    }
    
    // Prepare buffer for encoded data
    aom_fixed_buf_t output_buf;
    const size_t max_output_size = 1024 * 1024; // 1MB
    output_buf.buf = malloc(max_output_size);
    output_buf.sz = max_output_size;
    
    if (output_buf.buf) {
        aom_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    }
    
    // Encode the image
    res = aom_codec_encode(&codec, img, frame_index, 1, 0);
    
    // Get encoded data packets
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write encoded data to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    aom_img_free(img);
    aom_codec_destroy(&codec);
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	return 0;
}