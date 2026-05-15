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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 162, nr_unique_branch: 10
//<Quality> {"density":27,"unique_branches":{"validate_config":[[667,11,667,28,52,0,4,1],[667,32,667,49,52,0,4,1]],"aom_img_set_rect":[[247,7,247,22,0,0,4,1]],"aom_img_metadata_alloc":[[340,16,340,23,0,0,4,0]],"arg_parse_uint_helper":[[93,7,93,26,0,0,4,1],[93,30,93,47,0,0,4,1],[94,9,94,27,0,0,4,1],[20,7,20,14,1,0,4,0],[20,7,20,14,2,0,4,0]],"update_extra_cfg":[[1758,7,1758,26,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_img_alloc","aom_codec_destroy","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_set_option","aom_codec_encode","aom_codec_get_global_headers","aom_uleb_encode_fixed_size","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_img_alloc","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_option","aom_codec_encode","aom_codec_get_global_headers","aom_uleb_encode_fixed_size","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful fuzzing

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get codec interface name - API 6
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Use to avoid unused variable warning
    
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to peek stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, std::min(size, (size_t)100), &si);

    // Create image for processing
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    
    aom_image_t *img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, align);
    if (!img) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set rectangle on image - API 4
    // Use fuzzer data to determine rectangle parameters
    unsigned int x = data[0] % (width / 2);
    unsigned int y = data[1] % (height / 2);
    unsigned int w = 16 + (data[2] % 32);
    unsigned int h = 16 + (data[3] % 32);
    aom_img_set_rect(img, x, y, w, h, 0);

    // Create metadata - API 7
    uint32_t metadata_type = *reinterpret_cast<const uint32_t*>(data + 4);
    size_t metadata_size = std::min<size_t>(data[8] % 100, size - 10);
    
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type,
        data + 10,
        metadata_size,
        AOM_MIF_ANY_FRAME
    );
    
    if (metadata) {
        // Add metadata to image
        aom_img_add_metadata(img, metadata->type, metadata->payload, metadata->sz, metadata->insert_flag);
        
        // Get metadata from image - API 5
        const aom_metadata_t *retrieved_metadata = aom_img_get_metadata(img, 0);
        (void)retrieved_metadata; // Use to avoid unused variable warning
        
        aom_img_metadata_free(metadata);
    }

    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0)) {
        aom_img_free(img);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION)) {
        aom_img_free(img);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration - API 3
    aom_codec_enc_config_set(&encoder, &enc_cfg);

    // Set options on encoder - API 2
    // Use fuzzer data to create option values
    char option_value[64];
    size_t option_len = std::min<size_t>(data[9] % 60, size - 50);
    if (option_len > 0) {
        memcpy(option_value, data + 50, option_len);
        option_value[option_len] = '\0';
        aom_codec_set_option(&encoder, "cpu-used", option_value);
    }

    // Encode the image
    aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Get global headers - API 1
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }

    // Encode ULEB value with fixed size - API 8
    uint64_t uleb_value = *reinterpret_cast<const uint64_t*>(data + 20);
    uint8_t coded_value[10];
    size_t coded_size;
    
    // Try different pad sizes based on fuzzer data
    size_t pad_size = data[28] % 6 + 5; // Pad to 5-10 bytes
    
    aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_value), pad_size, 
                               coded_value, &coded_size);

    // Try to decode some data
    if (size > 200) {
        size_t decode_size = std::min<size_t>(size - 100, 1000);
        aom_codec_decode(&decoder, data + 100, decode_size, nullptr);
        
        // Get decoded frame
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_img;
        while ((decoded_img = aom_codec_get_frame(&decoder, &iter))) {
            // Process decoded image
            (void)decoded_img;
        }
    }

    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}