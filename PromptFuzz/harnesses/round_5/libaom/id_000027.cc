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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_alloc_with_border","aom_codec_destroy","aom_img_set_rect","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_img_metadata_free","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_img_metadata_free","aom_codec_encode","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_alloc_with_border","aom_img_set_rect","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small to be meaningful
    if (size < 100) {
        return 0;
    }

    // Variables to track cleanup
    aom_codec_ctx_t decoder = {};
    aom_codec_ctx_t encoder = {};
    aom_image_t* img_with_border = nullptr;
    aom_metadata_t* metadata = nullptr;
    FILE* in_file = nullptr;
    
    // Create a FILE* from the input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Initialize decoder for AV1
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Allocate an image with border for encoding
    // Use reasonable defaults for fuzzing
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 1;
    const unsigned int size_align = 1;
    const unsigned int border = 16;
    
    img_with_border = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                                width, height, align, 
                                                size_align, border);
    if (!img_with_border) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set a rectangular region in the image
    // Use fuzzed data to determine rectangle parameters
    // Ensure parameters are within bounds
    unsigned int x = data[0] % (width + border * 2);
    unsigned int y = data[1] % (height + border * 2);
    unsigned int w = data[2] % (width + border * 2 - x) + 1;
    unsigned int h = data[3] % (height + border * 2 - y) + 1;
    
    // Use the border value from the image allocation
    if (aom_img_set_rect(img_with_border, x, y, w, h, border) != 0) {
        // On failure, use default values
        aom_img_set_rect(img_with_border, 0, 0, width, height, border);
    }
    
    // 4. Create metadata using part of the input data
    uint32_t metadata_type = 1;  // Arbitrary type
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_KEY_FRAME;
    
    // Use a portion of the input data for metadata payload
    size_t metadata_size = size > 100 ? 100 : size;
    const uint8_t* metadata_data = data + (size - metadata_size) % size;
    
    metadata = aom_img_metadata_alloc(metadata_type, metadata_data, 
                                      metadata_size, insert_flag);
    if (metadata) {
        // Note: The metadata is allocated but not attached to the image
        // as aom_img_add_metadata might not be available in this context
    }
    
    // 5. Initialize encoder for AV1
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(img_with_border);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        if (metadata) {
            aom_img_metadata_free(metadata);
        }
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_min_quantizer = 0;
    enc_cfg.rc_max_quantizer = 63;
    enc_cfg.rc_end_usage = AOM_VBR;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(img_with_border);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        if (metadata) {
            aom_img_metadata_free(metadata);
        }
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Encode the image with border
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    aom_codec_encode(&encoder, img_with_border, pts, duration, flags);
    
    // Cleanup
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    aom_img_free(img_with_border);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}