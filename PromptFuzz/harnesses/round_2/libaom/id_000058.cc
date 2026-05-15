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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_add_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_add_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Fuzz driver implementation
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small
    if (size < 100) {
        return 0;
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Suppress unused variable warning

    // Use aom_uleb_decode on a portion of the input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t decode_size = (size > 50) ? 50 : size;
    aom_uleb_decode(data, decode_size, &uleb_value, &uleb_length);

    // Get AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Use aom_codec_iface_name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Suppress unused variable warning

    // Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Suppress unused variable warning

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                  0, AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Decode the input data
    ret = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Try to get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Get metadata from decoded image
        size_t num_metadata = aom_img_num_metadata(decoded_img);
        for (size_t i = 0; i < num_metadata; ++i) {
            const aom_metadata_t *metadata = aom_img_get_metadata(decoded_img, i);
            if (metadata) {
                // Free the metadata
                aom_img_metadata_free((aom_metadata_t *)metadata);
            }
        }
    }

    // Create a dummy image using aom_img_wrap for testing metadata operations
    const unsigned int dummy_width = 64;
    const unsigned int dummy_height = 64;
    const aom_img_fmt_t dummy_fmt = AOM_IMG_FMT_I420;
    
    // Calculate buffer size for I420 format
    size_t y_plane_size = dummy_width * dummy_height;
    size_t uv_plane_size = (dummy_width / 2) * (dummy_height / 2);
    size_t total_size = y_plane_size + 2 * uv_plane_size;
    
    unsigned char *img_buffer = (unsigned char *)malloc(total_size);
    if (img_buffer) {
        aom_image_t wrapped_img;
        aom_img_wrap(&wrapped_img, dummy_fmt, dummy_width, dummy_height, 
                     1, img_buffer);
        
        // Add metadata to the wrapped image
        // Use a small portion of input data for metadata payload
        size_t metadata_payload_size = (size > 100) ? 100 : size;
        uint32_t metadata_type = 1; // Example type
        aom_img_add_metadata(&wrapped_img, metadata_type, data, 
                            metadata_payload_size, AOM_MIF_ANY_FRAME);
        
        // Free the wrapped image
        aom_img_free(&wrapped_img);
        free(img_buffer);
    }

    // Initialize encoder to test aom_codec_enc_config_set
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (encoder_iface) {
        aom_codec_enc_cfg_t enc_cfg;
        unsigned int usage = 0; // Good quality
        
        // Get default encoder configuration
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage) == AOM_CODEC_OK) {
            // Set encoder configuration
            enc_cfg.g_w = 320;
            enc_cfg.g_h = 240;
            enc_cfg.rc_target_bitrate = 1000;
            
            // Initialize encoder
            if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                       0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                // Try to set the configuration again
                aom_codec_enc_config_set(&encoder, &enc_cfg);
                
                // Destroy encoder
                aom_codec_destroy(&encoder);
            }
        }
    }

    // Create output file for testing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor for writing
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out; // Suppress unused variable warning
        
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, (size > 100) ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    // Close input file
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}