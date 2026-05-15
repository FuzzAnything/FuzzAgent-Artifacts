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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;

    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Get error string
        const char *err_str = aom_codec_err_to_string(err);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data from input file and decode
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(buffer, 1, size, in_file);
    if (bytes_read > 0) {
        err = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
        
        // Get error details if any
        const char *decoder_error = aom_codec_error(&decoder);
        (void)decoder_error; // Use variable to avoid unused warning
        
        // Get stream information
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_err_t stream_err = aom_codec_get_stream_info(&decoder, &stream_info);
        if (stream_err == AOM_CODEC_OK) {
            // Use stream_info.w and stream_info.h for encoder configuration
        }

        // Get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = NULL;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
            // Use aom_img_plane_height API
            int plane_height = aom_img_plane_height(img, 0);
            (void)plane_height; // Use variable to avoid unused warning
            
            // Use aom_img_get_metadata API
            size_t num_metadata = aom_img_num_metadata(img);
            if (num_metadata > 0) {
                const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
                (void)metadata; // Use variable to avoid unused warning
            }
            
            // Initialize encoder context for re-encoding
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (err == AOM_CODEC_OK) {
                // Set encoder configuration based on decoded image
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.g_threads = 1;
                enc_cfg.rc_target_bitrate = 1000;
                
                err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                if (err == AOM_CODEC_OK) {
                    // Set the encoder configuration using aom_codec_enc_config_set
                    err = aom_codec_enc_config_set(&encoder, &enc_cfg);
                    
                    // Encode the image
                    aom_codec_encode(&encoder, img, 0, 1, 0);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = NULL;
                    const aom_codec_cx_pkt_t *pkt = NULL;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            FILE *out_file = fopen("output_file", "wb");
                            if (out_file) {
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                assert_file_closed(&out_file);;
                            }
                        }
                    }
                    
                    aom_codec_destroy(&encoder);
                }
            }
        }
    }

    // Cleanup
    free(buffer);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}