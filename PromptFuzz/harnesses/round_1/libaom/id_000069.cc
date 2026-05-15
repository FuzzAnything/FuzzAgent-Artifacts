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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Print version string
    const char *version = aom_codec_version_str();
    (void)version; // Prevent unused warning

    // 2. Try ULEB decode/encode on part of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t uleb_available = size > 10 ? 10 : size;
    
    int decode_result = aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    
    uint8_t encoded_uleb[10];
    size_t encoded_size = 0;
    if (decode_result == 0) {
        aom_uleb_encode(uleb_value, sizeof(encoded_uleb), encoded_uleb, &encoded_size);
    }

    // 3. Set up input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Prevent unused warnings

    // 4. Decode input as AV1 stream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read and decode data
    std::vector<uint8_t> buffer(size);
    fread(buffer.data(), 1, size, in_file);
    
    res = aom_codec_decode(&decoder, buffer.data(), size, nullptr);
    
    // Get stream info
    aom_codec_stream_info_t stream_info = {0};
    if (aom_codec_get_stream_info(&decoder, &stream_info) == AOM_CODEC_OK) {
        // Got stream info
    }

    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    std::vector<aom_image_t*> decoded_frames;
    
    while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_frames.push_back(decoded_img);
        
        // Use image plane APIs
        for (int plane = 0; plane < 3; plane++) {
            int plane_w = aom_img_plane_width(decoded_img, plane);
            int plane_h = aom_img_plane_height(decoded_img, plane);
            (void)plane_w; (void)plane_h; // Prevent unused warnings
        }
        
        // Check metadata
        size_t metadata_count = aom_img_num_metadata(decoded_img);
        (void)metadata_count;
    }

    // 5. Encode if we got decoded frames
    if (!decoded_frames.empty()) {
        aom_codec_ctx_t encoder;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set dimensions from decoded frame
        if (decoded_frames[0]) {
            enc_cfg.g_w = decoded_frames[0]->d_w;
            enc_cfg.g_h = decoded_frames[0]->d_h;
        } else {
            enc_cfg.g_w = 320;
            enc_cfg.g_h = 240;
        }
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_threads = 1;
        enc_cfg.g_profile = 0;
        enc_cfg.g_bit_depth = AOM_BITS_8;
        enc_cfg.g_input_bit_depth = 8;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        enc_cfg.rc_end_usage = AOM_VBR;
        enc_cfg.kf_mode = AOM_KF_AUTO;
        enc_cfg.kf_min_dist = 0;
        enc_cfg.kf_max_dist = 9999;
        
        res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (res == AOM_CODEC_OK) {
            // Set encoder option
            aom_codec_set_option(&encoder, "cpu-used", "5");
            
            // Re-set config
            aom_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Allocate test image if no decoded frames
            aom_image_t *test_img = nullptr;
            if (decoded_frames.empty()) {
                test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 320, 240, 16);
                if (test_img) {
                    // Fill with some data
                    for (int plane = 0; plane < 3; plane++) {
                        uint8_t *plane_ptr = test_img->planes[plane];
                        int stride = test_img->stride[plane];
                        int height = aom_img_plane_height(test_img, plane);
                        for (int y = 0; y < height; y++) {
                            memset(plane_ptr + y * stride, 128, aom_img_plane_width(test_img, plane));
                        }
                    }
                }
            }
            
            // Encode frames
            for (size_t i = 0; i < decoded_frames.size(); i++) {
                aom_image_t *img = decoded_frames[i] ? decoded_frames[i] : test_img;
                if (img) {
                    aom_codec_encode(&encoder, img, i, 1, 0);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                }
            }
            
            // Flush encoder
            aom_codec_encode(&encoder, nullptr, 0, 1, 0);
            
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Set compressed data buffer
            std::vector<uint8_t> cx_buffer(1024 * 1024);
            aom_fixed_buf_t fb = {cx_buffer.data(), cx_buffer.size()};
            aom_codec_set_cx_data_buf(&encoder, &fb, 0, 0);
            
            if (test_img) {
                aom_img_free(test_img);
            }
            
            aom_codec_destroy(&encoder);
        }
    }

    // 6. Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}