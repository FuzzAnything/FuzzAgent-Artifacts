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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_codec_encode","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_codec_encode","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a step-by-step implementation of the fuzz driver that uses the specified libaom APIs:

*/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0; // Need minimum data for testing
    }

    // Use the input data as a byte stream of libaom's output data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get build configuration information
    const char *build_config = aom_codec_build_config();
    
    // Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Set encoder parameters based on input data
    cfg.g_w = 128 + (data[0] % 64);  // Use input data to determine width
    cfg.g_h = 128 + (data[1] % 64);  // Use input data to determine height
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_Q;
    
    // Create codec context
    aom_codec_ctx_t codec;
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder control parameters using input data
    int cpu_used = data[2] % 9;  // Values 0-8 for CPU usage
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
    
    // Create an image with border using input dimensions
    aom_image_t img;
    unsigned int border = 16 + (data[3] % 16);
    aom_img_alloc_with_border(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 
                              16, 16, border);
    
    // Fill image planes with input data
    size_t data_offset = 4;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = aom_img_plane_width(&img, plane);
        int plane_height = aom_img_plane_height(&img, plane);
        size_t plane_size = plane_width * plane_height;
        
        for (unsigned int row = 0; row < plane_height; row++) {
            size_t bytes_to_copy = std::min(plane_width, 
                                          (int)(size - data_offset));
            if (bytes_to_copy > 0) {
                memcpy(&img.planes[plane][row * img.stride[plane]], 
                       &data[data_offset], bytes_to_copy);
                data_offset += bytes_to_copy;
            }
        }
    }
    
    // Flip the image
    aom_img_flip(&img);
    
    // Encode the image
    aom_codec_encode(&codec, &img, 0, 1, 0);
    
    // Try to encode a ULEB value using input data
    uint64_t uleb_value = 0;
    size_t uleb_data_offset = std::min(size - 8, data_offset);
    if (uleb_data_offset + 8 <= size) {
        memcpy(&uleb_value, &data[uleb_data_offset], 8);
    }
    
    uint8_t coded_value[16];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, 16, 8, coded_value, &coded_size);
    
    // Get encoded data from encoder
    const aom_codec_cx_pkt_t *pkt = NULL;
    aom_codec_iter_t iter = NULL;
    
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write encoded frame to output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                assert_file_closed(&out_file);;
            }
        }
    }
    
    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}