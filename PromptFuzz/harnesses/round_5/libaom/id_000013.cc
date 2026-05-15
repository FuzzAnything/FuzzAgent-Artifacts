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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_error","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Open input as file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Use aom_codec_err_to_string to convert error code
    const char *err_str = aom_codec_err_to_string(err);
    (void)err_str; // Avoid unused variable warning
    
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data from file
    fseek(in_file, 0, SEEK_END);
    size_t file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *buffer = (uint8_t *)malloc(file_size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, in_file);
    
    // Decode the frame
    err = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
    
    // Use aom_codec_error to get error string from decoder context
    const char *decoder_error = aom_codec_error(&decoder);
    (void)decoder_error; // Avoid unused variable warning
    
    if (err != AOM_CODEC_OK) {
        free(buffer);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    err = aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Use aom_img_plane_height
        for (int plane = 0; plane < 3; plane++) {
            int plane_height = aom_img_plane_height(img, plane);
            (void)plane_height; // Avoid unused variable warning
        }
        
        // Use aom_img_get_metadata
        size_t metadata_count = aom_img_num_metadata(img);
        for (size_t i = 0; i < metadata_count; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            (void)metadata; // Avoid unused variable warning
        }
    }

    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(buffer);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder
    if (img) {
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 0;
    }
    
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        free(buffer);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_enc_config_set to update encoder configuration
    if (img) {
        // Modify some configuration parameters
        enc_cfg.rc_target_bitrate = 2000;
        err = aom_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Get error string for encoder
        const char *encoder_error = aom_codec_error(&encoder);
        (void)encoder_error; // Avoid unused variable warning
    }
    
    // Encode the image if available
    if (img) {
        err = aom_codec_encode(&encoder, img, 0, 1, 0);
        
        // Get compressed data
        aom_codec_iter_t enc_iter = NULL;
        const aom_codec_cx_pkt_t *pkt = NULL;
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            // Process encoded packet
            (void)pkt; // Avoid unused variable warning
        }
    }
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Avoid unused variable warning
        
        // Write something to output file
        if (img) {
            // Write image dimensions
            fwrite(&img->d_w, sizeof(img->d_w), 1, out_file);
            fwrite(&img->d_h, sizeof(img->d_h), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    free(buffer);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}