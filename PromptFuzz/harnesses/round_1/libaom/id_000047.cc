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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 6
//<Quality> {"density":14,"unique_branches":{"set_good_speed_feature_framesize_dependent":[[728,7,728,24,0,0,4,0],[730,9,730,26,0,0,4,1]],"does_level_match":[[458,10,458,36,0,0,4,1]],"av1_noise_estimate_init":[[49,14,49,31,0,0,4,0]],"set_bitstream_level_tier":[[475,14,476,33,0,0,4,1],[478,14,479,33,0,0,4,0]]},"library_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_img_alloc_with_border","aom_codec_destroy","aom_img_set_rect","aom_img_add_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_img_alloc_with_border","aom_img_set_rect","aom_img_add_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Create FILE* for reading input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused variable warnings
    (void)out_fd;
    
    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_image_t *image = nullptr;
    aom_codec_iter_t iter = nullptr;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    // Event: Decode compressed video, process frame with metadata, then re-encode
    
    // 1. Get codec interface name
    const char *decoder_name = aom_codec_iface_name(aom_codec_av1_dx());
    const char *encoder_name = aom_codec_iface_name(aom_codec_av1_cx());
    (void)decoder_name;
    (void)encoder_name;
    
    // 2. Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        // Cleanup and exit
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Decode the input data
    if (aom_codec_decode(&decoder, data, size, nullptr) != AOM_CODEC_OK) {
        // Try to get error detail
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail;
        
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Get global headers from decoder
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&decoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        // Process global headers if needed
        (void)global_headers;
    }
    
    // 5. Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    (void)preview_frame;
    
    // 6. Create a new image with border for processing
    image = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 640, 480, 1, 1, 16);
    if (!image) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Set a rectangle on the image
    if (aom_img_set_rect(image, 10, 10, 300, 200, 8) != 0) {
        // Handle error
    }
    
    // 8. Add metadata to the image
    // Create some metadata from input data
    uint32_t metadata_type = 1; // Example type
    size_t metadata_size = size > 1000 ? 100 : size / 10;
    std::vector<uint8_t> metadata_data(data, data + metadata_size);
    
    if (aom_img_add_metadata(image, metadata_type, metadata_data.data(), metadata_size, AOM_MIF_KEY_FRAME) != 0) {
        // Handle error
    }
    
    // 9. Initialize encoder
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 10. Encode the image
    if (aom_codec_encode(&encoder, image, 0, 1, 0) != AOM_CODEC_OK) {
        // Get error detail
        const char *encode_error = aom_codec_error_detail(&encoder);
        (void)encode_error;
    }
    
    // 11. Get encoded data packets
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write the encoded data to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 12. Use ULEB encoding
    uint8_t coded_buffer[10];
    size_t coded_size = 0;
    uint64_t value_to_encode = size; // Use input size as value
    
    if (aom_uleb_encode_fixed_size(value_to_encode, sizeof(coded_buffer), 5, coded_buffer, &coded_size) == 0) {
        // Successfully encoded
        if (coded_size > 0) {
            // Write encoded ULEB to output
            fwrite(coded_buffer, 1, coded_size, out_file);
        }
    }
    
    // 13. Cleanup
    aom_codec_destroy(&encoder);
    aom_img_free(image);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}