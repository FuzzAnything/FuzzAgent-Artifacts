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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_img_alloc_with_border","aom_img_alloc","aom_img_free","aom_img_free","aom_img_free","aom_img_free","aom_img_free","aom_img_wrap","aom_img_free","aom_img_free","aom_img_set_rect","aom_img_free","aom_img_free","aom_img_free","aom_img_free","aom_img_metadata_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_err_to_string","aom_codec_destroy","aom_img_metadata_free","aom_img_free","aom_img_free"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_img_alloc_with_border","aom_img_alloc","aom_img_wrap","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_err_to_string","aom_codec_destroy","aom_img_metadata_free","aom_img_free","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
        return 0;
    }

    // Initialize variables
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = nullptr;
    aom_image_t *img = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_codec_caps_t caps;
    const char *error_str = nullptr;
    aom_codec_err_t codec_err = AOM_CODEC_OK;
    
    // Create input file from data buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get codec capabilities
    iface = aom_codec_av1_cx();
    caps = aom_codec_get_caps(iface);

    // 2. Allocate image with border
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    const unsigned int size_align = 32;
    const unsigned int border = 16;
    
    img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                    width, height, align, 
                                    size_align, border);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Allocate regular image without border
    aom_image_t *img_no_border = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                               width, height, align);
    if (!img_no_border) {
        aom_img_free(img);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Read some data from input to use as image data
    size_t read_size = fread(img_no_border->planes[0], 1, 
                            img_no_border->stride[0] * height, in_file);
    if (read_size < img_no_border->stride[0] * height) {
        aom_img_free(img);
        aom_img_free(img_no_border);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Wrap image data
    unsigned char *img_data = (unsigned char *)malloc(img_no_border->sz);
    if (!img_data) {
        aom_img_free(img);
        aom_img_free(img_no_border);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data to our buffer
    memcpy(img_data, img_no_border->planes[0], img_no_border->sz);
    
    aom_image_t wrapped_img;
    aom_image_t *wrapped = aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420,
                                       width, height, align, img_data);
    if (!wrapped) {
        free(img_data);
        aom_img_free(img);
        aom_img_free(img_no_border);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set rectangle in image
    unsigned int rect_x = data[0] % (width - 10);
    unsigned int rect_y = data[1] % (height - 10);
    unsigned int rect_w = 10;
    unsigned int rect_h = 10;
    
    int rect_result = aom_img_set_rect(img, rect_x, rect_y, rect_w, rect_h, border);
    (void)rect_result; // Result not used in fuzzing context

    // 7. Create metadata
    uint32_t meta_type = data[2];
    size_t meta_size = data[3] % 100;
    uint8_t *meta_data = (uint8_t *)malloc(meta_size);
    if (!meta_data) {
        free(img_data);
        aom_img_free(img);
        aom_img_free(img_no_border);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use some input data for metadata
    size_t meta_read = fread(meta_data, 1, meta_size, in_file);
    if (meta_read < meta_size) {
        free(meta_data);
        free(img_data);
        aom_img_free(img);
        aom_img_free(img_no_border);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    metadata = aom_img_metadata_alloc(meta_type, meta_data, meta_size, 
                                     AOM_MIF_ANY_FRAME);
    free(meta_data);

    // 8. Try to use codec control (simulate setting some parameter)
    // Initialize encoder context
    aom_codec_enc_cfg_t cfg;
    codec_err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (codec_err == AOM_CODEC_OK) {
        codec_err = aom_codec_enc_init_ver(&codec_ctx, iface, &cfg, 0, 
                                           AOM_ENCODER_ABI_VERSION);
        
        if (codec_err == AOM_CODEC_OK) {
            // Try to set a control (using a valid control ID for AOM)
            int ctrl_id = AOME_SET_CPUUSED; // Example control ID
            int ctrl_value = data[4] % 8; // Valid range for cpu_used
            
            aom_codec_control(&codec_ctx, ctrl_id, ctrl_value);
            
            // Get error string for any potential error
            error_str = aom_codec_err_to_string(codec_err);
            
            // Clean up codec context
            aom_codec_destroy(&codec_ctx);
        }
    }

    // 9. Free metadata if allocated
    if (metadata) {
        aom_img_metadata_free(metadata);
    }

    // Cleanup
    free(img_data);
    aom_img_free(img);
    aom_img_free(img_no_border);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}