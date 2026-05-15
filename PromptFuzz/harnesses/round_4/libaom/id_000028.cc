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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"aom_decode_frame_from_obus":[[913,33,913,56,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_error","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_error","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. ULEB decoding from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    aom_uleb_decode(data, size, &uleb_value, &uleb_length);

    // 2. Create input file from fuzzed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    int in_fd = fuzz_fileno(in_file);

    // 3. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Peek stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(decoder_iface, data, (size > 1000) ? 1000 : size, &stream_info);

    // 5. Decode frames
    std::vector<uint8_t> buffer(size);
    memcpy(buffer.data(), data, size);
    aom_codec_decode(&decoder, buffer.data(), buffer.size(), nullptr);

    // 6. Get decoded frames and prepare for encoding
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_image = nullptr;
    std::vector<aom_image_t*> images_to_encode;
    
    while ((decoded_image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Use plane width/height APIs
        for (int plane = 0; plane < 3; plane++) {
            int plane_w = aom_img_plane_width(decoded_image, fuzz_int32_t_2);
            int plane_h = aom_img_plane_height(decoded_image, fuzz_int32_t_3);
            (void)plane_w; (void)plane_h; // Use variables to avoid warnings
        }
        
        // Allocate new image for encoding
        aom_image_t *img_for_encode = aom_img_alloc(nullptr, decoded_image->fmt, 
                                                   decoded_image->d_w, decoded_image->d_h, fuzz_uint32_t_4);
        if (img_for_encode) {
            // Copy image data (simplified - in reality would need proper copy)
            images_to_encode.push_back(img_for_encode);
        }
    }

    // 7. Initialize encoder if we have images
    if (!images_to_encode.empty()) {
        aom_codec_ctx_t encoder;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t enc_cfg;
        
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_5) == AOM_CODEC_OK) {
            enc_cfg.g_w = images_to_encode[0]->d_w;
            enc_cfg.g_h = images_to_encode[0]->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            
            if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_6, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                // Set encoder options
                aom_codec_set_option(&encoder, fuzz_str_7, fuzz_str_8);
                aom_codec_control(&encoder, AOME_SET_CPUUSED, 5);
                
                // Encode each image
                for (size_t i = 0; i < images_to_encode.size(); i++) {
                    aom_codec_encode(&encoder, images_to_encode[i], fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        // Process packet if needed
                    }
                }
                
                // Cleanup encoder
                aom_codec_destroy(&encoder);
            }
        }
    }

    // 8. Get error information
    const char *error_str = aom_codec_error(&decoder);
    const char *error_detail = aom_codec_error_detail(&decoder);
    (void)error_str; (void)error_detail; // Use variables to avoid warnings

    // 9. Cleanup
    for (aom_image_t *img : images_to_encode) {
        aom_img_free(img);
    }
    
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}