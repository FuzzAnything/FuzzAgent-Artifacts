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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc_with_border","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc_with_border","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create file handle for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* iface = aom_codec_av1_dx();
    
    // Get decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Suppress unused variable warning
    
    // Initialize decoder with default config
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    aom_codec_err_t err = aom_codec_dec_init(&decoder, iface, &cfg, 0);
    
    if (err != AOM_CODEC_OK) {
        // Use error to string function
        const char* err_str = aom_codec_err_to_string(err);
        (void)err_str; // Suppress unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data to create a buffer
    uint8_t* buffer = (uint8_t*)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Try to decode the data
    aom_codec_decode(&decoder, buffer, size, nullptr);
    
    // Try to get stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Try to use aom_codec_control with a common control ID
    // Use AOMD_GET_FRAME_CORRUPTED to check if frame is corrupted
    int frame_corrupted = 0;
    aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    
    // Create a fixed buffer for cx_data_buf
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = malloc(1024);
    cx_buf.sz = 1024;
    
    if (cx_buf.buf) {
        // Try to set cx data buffer (though this is typically for encoding)
        aom_codec_set_cx_data_buf(&decoder, &cx_buf, 0, 0);
        free(cx_buf.buf);
    }
    
    // Try to get preview frame
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // If we got a preview frame, try to get metadata
    if (preview_frame) {
        size_t metadata_count = aom_img_num_metadata(preview_frame);
        for (size_t i = 0; i < metadata_count; i++) {
            const aom_metadata_t* metadata = aom_img_get_metadata(preview_frame, i);
            (void)metadata; // Suppress unused variable warning
        }
    }
    
    // Create an image with border using aom_img_alloc_with_border
    // Use values from stream info if available, otherwise use defaults
    unsigned int width = stream_info.w > 0 ? stream_info.w : 64;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 64;
    
    aom_image_t img;
    aom_image_t* allocated_img = aom_img_alloc_with_border(
        &img, 
        AOM_IMG_FMT_I420, 
        width, 
        height, 
        1,  // align
        1,  // size_align
        16  // border
    );
    
    // Free the allocated image if successful
    if (allocated_img) {
        aom_img_free(allocated_img);
    }
    
    // Cleanup
    free(buffer);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}