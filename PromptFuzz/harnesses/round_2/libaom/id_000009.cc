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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_flip","aom_img_plane_width","aom_uleb_encode_fixed_size","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_height","aom_img_plane_width","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_img_plane_width","aom_uleb_encode_fixed_size","aom_img_alloc_with_border","aom_img_plane_height","aom_img_plane_width","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/*Here's a fuzz driver that uses all the specified APIs to decode AV1 video, flip it, and re-encode it:

*/


// Include necessary libaom headers

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get build configuration info
    const char *build_config = aom_codec_build_config();
    // Could optionally use build_config, but we don't need it for fuzzing
    
    // 2. Decode the input AV1 stream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Decode the input data
    if (aom_codec_decode(&decoder, data, size, NULL) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Get the decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t *decoded_image = aom_codec_get_frame(&decoder, &iter);
    if (!decoded_image) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Flip the image
    aom_img_flip(decoded_image);
    
    // 4. Get plane width for plane 0
    int plane_width = aom_img_plane_width(decoded_image, 0);
    
    // 5. Encode the plane width using uleb encoding
    uint8_t uleb_buffer[8];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(plane_width, sizeof(uleb_buffer), 4, 
                               uleb_buffer, &coded_size);
    
    // 6. Allocate an image with border for encoding
    aom_image_t *image_with_border = aom_img_alloc_with_border(
        NULL, decoded_image->fmt, decoded_image->d_w, decoded_image->d_h,
        32, 32, 32);  // 32-byte alignment and border
    
    if (!image_with_border) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy data from decoded image to image with border
    for (int plane = 0; plane < 3; plane++) {
        int src_stride = decoded_image->stride[plane];
        int dst_stride = image_with_border->stride[plane];
        int height = aom_img_plane_height(decoded_image, plane);
        int width = aom_img_plane_width(decoded_image, plane);
        
        for (int y = 0; y < height; y++) {
            memcpy(image_with_border->planes[plane] + y * dst_stride,
                   decoded_image->planes[plane] + y * src_stride,
                   width);
        }
    }
    
    // 7. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(image_with_border);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set configuration based on image
    cfg.g_w = image_with_border->d_w;
    cfg.g_h = image_with_border->d_h;
    cfg.g_bit_depth = (aom_bit_depth_t)image_with_border->bit_depth;
    cfg.g_input_bit_depth = image_with_border->bit_depth;
    cfg.g_threads = 1;  // Use single thread for fuzzing
    cfg.g_limit = 1;    // Limit to 1 frame
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(image_with_border);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 8. Use aom_codec_control to set encoder parameters
    // Set quality parameters
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 6);
    aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, 32);
    
    // 9. Encode the flipped image
    if (aom_codec_encode(&encoder, image_with_border, 0, 1, 0) != AOM_CODEC_OK) {
        // Continue anyway to clean up
    }
    
    // Get encoded data
    aom_codec_iter_t enc_iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Store or process encoded data if needed
            encoded_data.insert(encoded_data.end(),
                               (uint8_t*)pkt->data.frame.buf,
                               (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // 10. Create output file and write encoded data using FILE*
    // Note: We're using a fixed output file name as per requirements
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (!encoded_data.empty()) {
            fwrite(encoded_data.data(), 1, encoded_data.size(), out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 11. Create input file using fmemopen to demonstrate FILE* usage
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the file to demonstrate usage
        uint8_t test_buffer[16];
        if (size >= sizeof(test_buffer)) {
            fread(test_buffer, 1, sizeof(test_buffer), in_file);
        }
        assert_file_closed(&in_file);;
    }
    
    // 12. Clean up all allocated resources
    aom_img_free(image_with_border);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}