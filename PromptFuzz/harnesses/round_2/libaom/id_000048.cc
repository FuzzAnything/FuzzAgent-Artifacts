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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode an AV1 bitstream and process the decoded frames:

*/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. Get decoder interface capabilities
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    
    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Use aom_codec_err_to_string
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Suppress unused warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create a buffer for compressed data
    aom_fixed_buf_t comp_buf;
    comp_buf.buf = malloc(size);
    if (!comp_buf.buf) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(comp_buf.buf, data, size);
    comp_buf.sz = size;
    
    // Use aom_codec_set_cx_data_buf (though typically used for encoding)
    err = aom_codec_set_cx_data_buf(&decoder, &comp_buf, 0, 0);
    // Error is expected since this is decoder context, but we still call it
    
    // 4. Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 5. Get stream information
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    err = aom_codec_get_stream_info(&decoder, &si);
    
    // 6. Use aom_codec_control with various control IDs
    int ctrl_value;
    // Try to get some decoder control values
    err = aom_codec_control(&decoder, AOMD_GET_LAST_REF_UPDATES, &ctrl_value);
    err = aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, &ctrl_value);
    
    // 7. Allocate an image with border for potential processing
    aom_image_t *img_with_border = nullptr;
    if (si.w > 0 && si.h > 0) {
        img_with_border = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                                    si.w, si.h, 1, 1, 16);
    }
    
    // 8. Get preview frame if available
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // 9. Process decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *img;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 10. Get metadata from image if available
        size_t num_metadata = aom_img_num_metadata(img);
        for (size_t i = 0; i < num_metadata && i < 5; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            (void)metadata; // Suppress unused warning
        }
        
        // 11. Use the image with border if we have one
        if (img_with_border && img->d_w <= si.w && img->d_h <= si.h) {
            // Copy some data from decoded image to bordered image
            for (int plane = 0; plane < 3; plane++) {
                int src_width = aom_img_plane_width(img, plane);
                int src_height = aom_img_plane_height(img, plane);
                int dst_width = aom_img_plane_width(img_with_border, plane);
                int dst_height = aom_img_plane_height(img_with_border, plane);
                
                if (src_width > 0 && src_height > 0 && 
                    dst_width >= src_width && dst_height >= src_height) {
                    for (int y = 0; y < src_height && y < dst_height; y++) {
                        uint8_t *src_row = img->planes[plane] + y * img->stride[plane];
                        uint8_t *dst_row = img_with_border->planes[plane] + y * img_with_border->stride[plane];
                        if (src_row && dst_row) {
                            memcpy(dst_row, src_row, std::min(src_width, dst_width));
                        }
                    }
                }
            }
        }
    }
    
    // 12. Use aom_codec_err_to_string for any errors
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Suppress unused warning
    }
    
    // 13. Cleanup
    if (img_with_border) {
        aom_img_free(img_with_border);
    }
    
    free(comp_buf.buf);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}