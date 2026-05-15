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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"aom_codec_control":[[105,8,105,31,0,0,4,1]],"at_ctrl_map_end":[[192,10,192,25,0,0,4,0],[192,29,192,42,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_err_to_string","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_err_to_string","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a fuzz driver that uses the specified libaom APIs to decode AV1 bitstreams and process decoded frames:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Use aom_uleb_decode to decode a ULEB128 value
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_result = aom_uleb_decode(data, 
                                      (size > 100) ? 100 : size, 
                                      &uleb_value, 
                                      &uleb_length);
    
    // 2. Initialize AV1 decoder
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    
    aom_codec_err_t init_err = aom_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, 
                                                      AOM_DECODER_ABI_VERSION);
    
    if (init_err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(init_err);
        return 0; // Can't continue without decoder
    }
    
    // 3. Set compressed data buffer (even though we're decoding, this API is called)
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = (void*)data;
    cx_buf.sz = size;
    aom_codec_err_t set_buf_err = aom_codec_set_cx_data_buf(&codec_ctx, &cx_buf, 0, 0);
    
    // 4. Decode the input data
    aom_codec_err_t decode_err = aom_codec_decode(&codec_ctx, 
                                                  data, 
                                                  size, 
                                                  nullptr);
    
    // 5. Get stream information
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_err_t stream_info_err = aom_codec_get_stream_info(&codec_ctx, &stream_info);
    
    // 6. Try to use codec control with a common control ID
    // AOMD_GET_FRAME_CORRUPTED = 11 (common control for checking frame corruption)
    aom_codec_err_t control_err = aom_codec_control(&codec_ctx, 11, nullptr);
    
    // 7. Allocate an image with border for potential use
    aom_image_t allocated_img;
    aom_img_alloc_with_border(&allocated_img, 
                              AOM_IMG_FMT_I420, 
                              128,  // width
                              128,  // height
                              16,   // align
                              16,   // size_align  
                              8);   // border
    
    // 8. Try to get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
    
    // 9. Iterate through all decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *frame = nullptr;
    while ((frame = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 10. Get number of metadata in the frame
        size_t metadata_count = aom_img_num_metadata(frame);
        
        // 11. Access metadata if present
        for (size_t i = 0; i < metadata_count; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(frame, i);
            if (metadata) {
                // Metadata accessed successfully
            }
        }
        
        // 12. Get plane dimensions
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(frame, plane);
            int plane_height = aom_img_plane_height(frame, plane);
        }
        
        // 13. Convert error codes to strings for any errors encountered
        if (decode_err != AOM_CODEC_OK) {
            const char *decode_err_str = aom_codec_err_to_string(decode_err);
        }
        if (stream_info_err != AOM_CODEC_OK) {
            const char *stream_info_err_str = aom_codec_err_to_string(stream_info_err);
        }
    }
    
    // 14. Cleanup allocated image
    aom_img_free(&allocated_img);
    
    // 15. Destroy codec context
    aom_codec_err_t destroy_err = aom_codec_destroy(&codec_ctx);
    
    return 0;
}