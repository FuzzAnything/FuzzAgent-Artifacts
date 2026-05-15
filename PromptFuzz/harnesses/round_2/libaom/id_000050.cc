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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"aom_codec_err_to_string":[[42,5,42,29,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_error","aom_codec_error_detail","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_control","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_codec_error","aom_codec_error_detail","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode an AV1 bitstream and process the decoded frames:

*/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Decode a ULEB value from the input (demonstrates aom_uleb_decode)
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size >= 10) { // Ensure we have enough data for ULEB decoding
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }

    // 2. Create AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    // 3. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t init_err = aom_codec_dec_init(&codec_ctx, decoder_iface, &dec_cfg, 0);
    if (init_err != AOM_CODEC_OK) {
        // Use aom_codec_err_to_string to get error message
        const char *error_str = aom_codec_err_to_string(init_err);
        (void)error_str; // Prevent unused variable warning
        return 0;
    }

    // 4. Create a fixed buffer for encoded data
    aom_fixed_buf_t encoded_buf;
    encoded_buf.buf = (void *)data;
    encoded_buf.sz = size;
    
    // 5. Set encoded data buffer with padding (demonstrates aom_codec_set_cx_data_buf)
    // Note: This API is typically used for encoding, but we demonstrate its usage
    aom_codec_set_cx_data_buf(&codec_ctx, &encoded_buf, 0, 0);

    // 6. Decode the input data
    aom_codec_decode(&codec_ctx, data, size, nullptr);

    // 7. Get stream information
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_get_stream_info(&codec_ctx, &stream_info);

    // 8. Use aom_codec_control to get various decoder properties
    // Try to get a few different control IDs
    int control_result = 0;
    aom_codec_control(&codec_ctx, AOMD_GET_FRAME_CORRUPTED, &control_result);
    aom_codec_control(&codec_ctx, AOMD_GET_LAST_REF_UPDATES, &control_result);

    // 9. Allocate an image with border (for potential use)
    aom_image_t *allocated_img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                                          stream_info.w ? stream_info.w : 64,
                                                          stream_info.h ? stream_info.h : 64,
                                                          16, 1, 8);

    // 10. Try to get preview frame (may return nullptr)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);

    // 11. Iterate through decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = nullptr;
    int frame_count = 0;
    
    while ((frame = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        frame_count++;
        
        // 12. Get metadata count
        size_t metadata_count = aom_img_num_metadata(frame);
        
        // 13. Access metadata if available
        for (size_t i = 0; i < metadata_count && i < 10; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(frame, i);
            if (metadata) {
                // Access metadata fields to ensure they're used
                (void)metadata->type;
                (void)metadata->sz;
            }
        }
        
        // 14. Get plane dimensions for each plane
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(frame, plane);
            int plane_height = aom_img_plane_height(frame, plane);
            (void)plane_width; // Prevent unused variable warnings
            (void)plane_height;
            
            // Access plane data to ensure it's touched
            if (frame->planes[plane] && plane_width > 0 && plane_height > 0) {
                // Touch a few pixels from each plane
                for (int y = 0; y < plane_height && y < 10; y++) {
                    for (int x = 0; x < plane_width && x < 10; x++) {
                        volatile uint8_t pixel = frame->planes[plane][y * frame->stride[plane] + x];
                        (void)pixel;
                    }
                }
            }
        }
        
        // Limit the number of frames processed to avoid timeouts
        if (frame_count >= 10) {
            break;
        }
    }

    // 15. Check for any errors and get error string
    const char *codec_error = aom_codec_error(&codec_ctx);
    if (codec_error) {
        // Convert any error code to string
        aom_codec_err_t err = aom_codec_error_detail(&codec_ctx) ? AOM_CODEC_ERROR : AOM_CODEC_OK;
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str;
    }

    // 16. Cleanup allocated resources
    if (allocated_img) {
        aom_img_free(allocated_img);
    }

    // 17. Destroy codec context
    aom_codec_destroy(&codec_ctx);

    // Also demonstrate using a FILE* as requested
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a small amount of data to demonstrate file usage
        uint8_t buffer[16];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    // Demonstrate output file creation
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(data, 1, size < 100 ? size : 100, out_file);
        assert_file_closed(&out_file);;
        
        // Demonstrate using file descriptor
        FILE *reopen_file = fopen("output_file", "rb");
        if (reopen_file) {
            int fd = fuzz_fileno(reopen_file);
            (void)fd; // Prevent unused variable warning
            assert_file_closed(&reopen_file);;
        }
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}