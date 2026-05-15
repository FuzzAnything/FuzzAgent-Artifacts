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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc_with_border","aom_img_alloc","aom_img_free","aom_img_free","aom_codec_destroy","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc_with_border","aom_img_alloc","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get codec interface names
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    (void)encoder_name; (void)decoder_name; // Prevent unused warnings
    
    // 3. Get codec capabilities
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    (void)encoder_caps; (void)decoder_caps; // Prevent unused warnings
    
    // 4. Set up encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create codec context
    aom_codec_ctx_t codec_ctx;
    err = aom_codec_enc_init_ver(&codec_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Allocate images with and without border
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int border = 16;
    
    // Image with border
    aom_image_t img_with_border;
    aom_image_t *img_border_ptr = aom_img_alloc_with_border(&img_with_border, 
                                                           AOM_IMG_FMT_I420, 
                                                           width, height, 
                                                           32, 32, border);
    
    // Image without border
    aom_image_t img_no_border;
    aom_image_t *img_ptr = aom_img_alloc(&img_no_border, 
                                        AOM_IMG_FMT_I420, 
                                        width, height, 32);
    
    if (!img_border_ptr || !img_ptr) {
        if (img_border_ptr) aom_img_free(&img_with_border);
        if (img_ptr) aom_img_free(&img_no_border);
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Set rectangle in image with border
    // Use some data from input to determine rectangle parameters
    unsigned int rect_x = data[0] % (width - 8);
    unsigned int rect_y = data[1] % (height - 8);
    unsigned int rect_w = 8 + (data[2] % 8);
    unsigned int rect_h = 8 + (data[3] % 8);
    
    int set_rect_result = aom_img_set_rect(&img_with_border, 
                                          rect_x, rect_y, 
                                          rect_w, rect_h, border);
    (void)set_rect_result; // Prevent unused warning
    
    // 8. Allocate metadata
    // Use some input data for metadata
    size_t metadata_size = data[4] % 100;
    if (metadata_size > 0 && metadata_size <= size - 10) {
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            0x1234,  // Arbitrary type
            data + 10,  // Use input data as metadata payload
            metadata_size,
            AOM_MIF_ANY_FRAME
        );
        
        if (metadata) {
            // We would normally attach this to an image, but for this fuzzer
            // we'll just allocate and free it
            aom_img_metadata_free(metadata);
        }
    }
    
    // 9. Try to decode some data
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                AOM_DECODER_ABI_VERSION);
    
    if (err == AOM_CODEC_OK) {
        // Decode the input data
        aom_codec_decode(&decoder_ctx, data, size, NULL);
        
        // 10. Get frames from decoder
        aom_codec_iter_t iter = NULL;
        aom_image_t *decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter);
        
        // 11. Try to get preview frame (though this is typically for encoder)
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        (void)preview_frame; // Prevent unused warning
        
        // Clean up decoder
        aom_codec_destroy(&decoder_ctx);
    }
    
    // 12. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        fwrite(data, 1, size < 1024 ? size : 1024, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 13. Clean up all resources
    aom_img_free(&img_with_border);
    aom_img_free(&img_no_border);
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}