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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_img_wrap","aom_img_set_rect","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy"],"critical_calls":["aom_img_wrap","aom_img_set_rect","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // 1. Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Read image dimensions and format from input
    unsigned int width, height;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    // Use first few bytes for dimensions (simplified parsing)
    if (size >= 8) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        width = width % 256 + 64;  // Ensure reasonable dimensions
        height = height % 256 + 64;
    } else {
        width = 64;
        height = 64;
    }

    // 3. Allocate image buffer
    size_t img_size = width * height * 3 / 2; // For I420
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Read image data from input
    fseek(in_file, 4, SEEK_SET); // Skip dimension bytes
    size_t read_size = fread(img_data, 1, img_size, in_file);
    assert_file_closed(&in_file);;
    
    if (read_size < img_size) {
        // Pad with zeros if insufficient data
        memset(img_data + read_size, 0, img_size - read_size);
    }

    // 5. Wrap image data using aom_img_wrap
    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, fmt, width, height, 
                                            1, img_data);
    if (!wrapped_img) {
        free(img_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set rectangle on image using aom_img_set_rect
    unsigned int rect_x = 0, rect_y = 0;
    unsigned int rect_w = width / 2, rect_h = height / 2;
    if (rect_w > 0 && rect_h > 0) {
        aom_img_set_rect(wrapped_img, rect_x, rect_y, 
                        rect_w, rect_h, 0);
    }

    // 7. Create metadata and add to image
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        1, data, size % 100, AOM_MIF_ANY_FRAME);
    if (metadata) {
        // Simulate getting metadata count
        size_t num_metadata = aom_img_num_metadata(wrapped_img);
        (void)num_metadata; // Use value to prevent optimization
        
        // Free metadata using aom_img_metadata_free
        aom_img_metadata_free(metadata);
    }

    // 8. Initialize encoder
    aom_codec_ctx_t codec;
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    aom_codec_enc_config_default(encoder, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (aom_codec_enc_init(&codec, encoder, &cfg, 0) != AOM_CODEC_OK) {
        free(img_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // 9. Set output buffer using aom_codec_set_cx_data_buf
    size_t output_buf_size = 1024 * 1024;
    uint8_t *output_buf = (uint8_t *)malloc(output_buf_size);
    aom_fixed_buf_t buf = {output_buf, output_buf_size};
    
    aom_codec_set_cx_data_buf(&codec, &buf, 0, 0);

    // 10. Encode image using aom_codec_encode
    aom_codec_encode(&codec, wrapped_img, 0, 1, 0);
    
    // 11. Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet if needed
        (void)pkt;
    }

    // 12. Cleanup
    aom_codec_destroy(&codec);
    free(img_data);
    free(output_buf);

    assert_file_closed(&in_file);
	return 0;
}