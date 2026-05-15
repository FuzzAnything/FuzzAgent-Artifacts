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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"aom_img_get_metadata":[[433,7,433,12,0,0,4,1]]},"library_calls":["aom_img_alloc","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_error","aom_codec_set_cx_data_buf","aom_img_get_metadata","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_img_alloc","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_error","aom_codec_set_cx_data_buf","aom_img_get_metadata","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Step 1: Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // Step 2: Use fmemopen to create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Step 3: Allocate image buffer
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Extract dimensions from first 4 bytes of data
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    
    if (width == 0) width = 128;
    if (height == 0) height = 128;
    
    // Allocate image with I420 format (most common for video)
    aom_image_t *image_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 16);
    if (!image_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Step 4: Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = width;
    cfg.h = height;
    cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, 
                                                 aom_codec_av1_dx(), 
                                                 &cfg, 
                                                 0, 
                                                 AOM_DECODER_ABI_VERSION);
    
    // Get error message if initialization failed
    const char *error_msg = aom_codec_error(&decoder);

    // Step 5: Prepare fixed buffer from input data
    aom_fixed_buf_t buf;
    buf.buf = (void *)(data + 4);  // Skip first 4 bytes used for dimensions
    buf.sz = size > 4 ? size - 4 : 0;
    
    // Step 6: Set compressed data buffer with padding
    if (buf.sz > 0) {
        aom_codec_set_cx_data_buf(&decoder, &buf, 0, 16);
    }

    // Step 7: Try to get metadata from allocated image
    // (Note: image was just allocated, so there won't be any metadata)
    const aom_metadata_t *metadata = aom_img_get_metadata(image_ptr, 0);
    
    // Step 8: Free metadata if it exists
    // In real usage, you would only free metadata that you allocated
    if (metadata) {
        // Note: aom_img_metadata_free expects a non-const pointer
        aom_metadata_t *non_const_metadata = const_cast<aom_metadata_t*>(metadata);
        aom_img_metadata_free(non_const_metadata);
    }

    // Step 9: Clean up all allocated resources
    aom_img_free(image_ptr);
    
    if (res == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Step 10: Clean up output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}