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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 216, nr_unique_branch: 17
//<Quality> {"density":24,"unique_branches":{"handle_tuning":[[1902,7,1902,39,0,0,4,1],[1903,7,1903,48,0,0,4,1],[1934,7,1934,39,0,0,4,1]],"is_fpmt_config":[[1156,7,1156,42,0,0,4,0]],"arg_parse_enum_helper":[[167,7,167,14,0,0,4,0],[171,7,171,26,0,0,4,0],[171,30,171,47,0,0,4,1],[178,35,178,48,0,0,4,0],[179,9,179,41,0,0,4,0]],"aom_codec_enc_init_ver":[[74,14,74,34,0,0,4,0],[75,14,75,55,0,0,4,0]],"encoder_set_option":[[4419,14,4420,42,0,0,4,0]],"init_rd_sf":[[2456,36,2456,73,0,0,4,1]],"aom_codec_error_detail":[[63,12,63,21,0,0,4,1]],"av1_change_config":[[1006,7,1006,47,0,0,4,0],[1008,17,1008,32,0,0,4,0],[1008,17,1008,32,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_codec_dec_init_ver","aom_img_free","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_uleb_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_uleb_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libaom APIs to encode and decode AV1 video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small for meaningful fuzzing
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get encoder and decoder interfaces
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get interface names (API requirement)
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    
    // Get capabilities (API requirement)
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);

    // Set up encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0; // Good quality
    aom_codec_err_t res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder parameters based on fuzz data
    enc_cfg.g_w = 64 + (data[0] % 192);  // Width: 64-256
    enc_cfg.g_h = 64 + (data[1] % 192);  // Height: 64-256
    enc_cfg.g_threads = 1 + (data[2] % 4); // Threads: 1-4
    enc_cfg.g_error_resilient = (data[3] & 0x1);
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.rc_end_usage = AOM_VBR;
    enc_cfg.rc_target_bitrate = 200 + (data[4] % 800); // 200-1000 kbps
    enc_cfg.rc_min_quantizer = data[5] % 64;
    enc_cfg.rc_max_quantizer = std::max(enc_cfg.rc_min_quantizer, (unsigned int)(data[6] % 64));
    enc_cfg.g_bit_depth = (data[7] & 0x1) ? AOM_BITS_8 : AOM_BITS_10;
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set some encoder options (API requirement)
    aom_codec_set_option(&encoder, "cpu-used", "1");
    aom_codec_set_option(&encoder, "tune", "psnr");

    // Create image for encoding
    aom_image_t *img = nullptr;
    unsigned int border = 16;
    
    // Choose between different allocation methods based on fuzz data
    if (data[8] & 0x1) {
        img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16);
    } else {
        img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16, 1, border);
    }
    
    if (!img) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Fill image with fuzz data
    size_t data_offset = 100;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = aom_img_plane_width(img, plane);
        int plane_height = aom_img_plane_height(img, plane);
        int stride = img->stride[plane];
        
        for (int y = 0; y < plane_height; y++) {
            size_t bytes_to_copy = std::min((size_t)plane_width, 
                                           std::min(size - data_offset, (size_t)1024));
            if (bytes_to_copy > 0) {
                uint8_t *row_ptr = img->planes[plane] + y * stride;
                fread(row_ptr, 1, bytes_to_copy, in_file);
                data_offset += bytes_to_copy;
            } else {
                // Fill with pattern if we run out of fuzz data
                memset(img->planes[plane] + y * stride, 128 + y, plane_width);
            }
        }
    }

    // Set a rectangle in the image (API requirement)
    unsigned int rect_x = data[9] % (enc_cfg.g_w / 2);
    unsigned int rect_y = data[10] % (enc_cfg.g_h / 2);
    unsigned int rect_w = 1 + (data[11] % (enc_cfg.g_w - rect_x));
    unsigned int rect_h = 1 + (data[12] % (enc_cfg.g_h - rect_y));
    aom_img_set_rect(img, rect_x, rect_y, rect_w, rect_h, border);

    // Create metadata (API requirement)
    uint32_t metadata_type = data[13];
    size_t metadata_size = std::min((size_t)100, size - data_offset);
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type, 
        data + data_offset, 
        metadata_size, 
        AOM_MIF_ANY_FRAME
    );
    
    if (metadata) {
        // Note: We would normally attach this to an image, but for fuzzing 
        // we just create and free it to meet API requirement
        aom_img_metadata_free(metadata);
    }
    data_offset += metadata_size;

    // Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    res = aom_codec_encode(&encoder, img, pts, duration, flags);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&encoder);
        aom_img_free(img);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    size_t encoded_size = 0;
    uint8_t *encoded_data = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_size = pkt->data.frame.sz;
            encoded_data = (uint8_t*)pkt->data.frame.buf;
            break;
        }
    }

    // Try to get preview frame (API requirement)
    const aom_image_t *preview = aom_codec_get_preview_frame(&encoder);

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1 + (data[14] % 4);
    dec_cfg.allow_lowbitdepth = 1;
    
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the encoded data if we have any
    if (encoded_data && encoded_size > 0) {
        res = aom_codec_decode(&decoder, encoded_data, encoded_size, nullptr);
        
        // Get decoded frames
        aom_codec_iter_t dec_iter = nullptr;
        const aom_image_t *decoded_img;
        while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
            // Just access the decoded image to exercise the API
            int width = decoded_img->d_w;
            int height = decoded_img->d_h;
        }
    }

    // Test ULEB encoding (API requirement)
    uint64_t test_value = (uint64_t)data[15] << 32 | (uint64_t)data[16] << 24 | 
                         (uint64_t)data[17] << 16 | (uint64_t)data[18] << 8 | data[19];
    uint8_t uleb_buffer[10];
    size_t coded_size = 0;
    aom_uleb_encode(test_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);

    // Clean up
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}