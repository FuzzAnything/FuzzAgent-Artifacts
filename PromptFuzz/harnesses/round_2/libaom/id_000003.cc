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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_destroy","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 1. Get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use variable to avoid unused warning

    // 3. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Try to get error detail
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use variable
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode data
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 4. Decode the input
    err = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail;
        free(buffer);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    free(buffer);

    // 5. Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // 6. Get plane height
        int plane_height = aom_img_plane_height(img, 0);
        (void)plane_height;

        // 7. Check metadata count
        size_t metadata_count = aom_img_num_metadata(img);
        (void)metadata_count;

        // 8. Create and add metadata
        uint32_t metadata_type = 1;
        const uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
        size_t metadata_size = sizeof(metadata_data);
        
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            metadata_type, 
            metadata_data, 
            metadata_size, 
            AOM_MIF_ANY_FRAME
        );
        
        if (metadata) {
            // Note: aom_img_add_metadata would be needed to actually add to image
            // For demonstration, we just allocate and free
            
            // 9. Free metadata
            aom_img_metadata_free(metadata);
        }

        // 10. Initialize encoder (simplified configuration)
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default configuration
            err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (err == AOM_CODEC_OK) {
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_threads = 1;
                
                err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                            AOM_ENCODER_ABI_VERSION);
                if (err == AOM_CODEC_OK) {
                    // 11. Encode the frame
                    aom_codec_encode(&encoder, img, 0, 1, 0);
                    
                    // Destroy encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
    }

    // 12. Clean up decoder
    aom_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}