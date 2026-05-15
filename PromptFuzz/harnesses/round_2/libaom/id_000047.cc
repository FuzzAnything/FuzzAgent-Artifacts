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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_set_cx_data_buf","aom_codec_decode","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_control","aom_codec_err_to_string","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_set_cx_data_buf","aom_codec_decode","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_control","aom_codec_err_to_string","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libaom APIs to achieve video decoding and metadata extraction:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create a file pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int file_descriptor = fuzz_fileno(in_file);
    (void)file_descriptor; // Use descriptor to avoid unused variable warning
    
    // 1. Get codec capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // 2. Initialize decoder
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Check initialization result
    const char *init_error_str = aom_codec_err_to_string(init_result);
    (void)init_error_str; // Use error string
    
    // 3. Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_err_t stream_info_result = aom_codec_get_stream_info(
        &codec_ctx, &stream_info);
    (void)stream_info_result;
    
    // 4. Create a fixed buffer for encoded data
    aom_fixed_buf_t encoded_buffer;
    encoded_buffer.buf = malloc(size);
    if (!encoded_buffer.buf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(file_descriptor);
	return 0;
    }
    memcpy(encoded_buffer.buf, data, size);
    encoded_buffer.sz = size;
    
    // 5. Set encoded data buffer (though this is typically for encoder)
    aom_codec_err_t set_buf_result = aom_codec_set_cx_data_buf(
        &codec_ctx, &encoded_buffer, 0, 0);
    (void)set_buf_result;
    
    // 6. Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec_ctx, data, size, NULL);
    
    // 7. Allocate an image with border for potential use
    aom_image_t allocated_image;
    aom_image_t *image_ptr = aom_img_alloc_with_border(
        &allocated_image, 
        AOM_IMG_FMT_I420, 
        64, 64,  // dimensions
        1,       // align
        1,       // size_align
        16);     // border
    
    // 8. Try to get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
    
    // 9. Get metadata if preview frame exists
    if (preview_frame) {
        size_t metadata_count = aom_img_num_metadata(preview_frame);
        if (metadata_count > 0) {
            const aom_metadata_t *metadata = aom_img_get_metadata(
                preview_frame, 0);
            (void)metadata;
        }
    }
    
    // 10. Try various control codes (common ones)
    int control_ids[] = {AOMD_GET_LAST_REF_UPDATES, AOMD_GET_FRAME_CORRUPTED};
    for (size_t i = 0; i < sizeof(control_ids)/sizeof(control_ids[0]); i++) {
        int value = 0;
        aom_codec_err_t control_result = aom_codec_control(
            &codec_ctx, control_ids[i], &value);
        
        // Convert error to string for logging (not used in fuzzing)
        const char *control_error_str = aom_codec_err_to_string(control_result);
        (void)control_error_str;
    }
    
    // 11. Iterate through decoded frames
    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&codec_ctx, &iter))) {
        // Get metadata from each frame
        size_t frame_metadata_count = aom_img_num_metadata(frame);
        for (size_t i = 0; i < frame_metadata_count; i++) {
            const aom_metadata_t *frame_metadata = aom_img_get_metadata(
                frame, i);
            (void)frame_metadata;
        }
    }
    
    // 12. Create output file (for demonstration)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use file descriptor
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    free(encoded_buffer.buf);
    
    if (image_ptr) {
        aom_img_free(&allocated_image);
    }
    
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(file_descriptor);
	return 0;
}