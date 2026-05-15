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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"ctrl_get_last_quantizer":[[1065,7,1065,18,0,0,4,1],[1066,7,1066,32,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_control","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_set_rect","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_control","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_set_rect","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_destroy"],"visited":0}
/**/


// libaom headers
extern "C" {
    #include "aom/aom.h"
    #include "aom/aom_codec.h"
    #include "aom/aom_decoder.h"
    #include "aom/aom_encoder.h"
    #include "aom/aom_image.h"
    #include "aom/aom_integer.h"
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

// Frame buffer allocation callback
int  id_000013_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or resize buffer
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer.size();
    return 0;
}

// Frame buffer release callback
int  id_000013_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // Nothing to do here - buffer is managed by FrameBufferData
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // 1. Use aom_uleb_decode to decode a LEB128 value from input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t decode_offset = 0;
    
    if (size > 0) {
        // Use first 10 bytes or available size for ULEB decoding
        size_t decode_size = size > 10 ? 10 : size;
        aom_uleb_decode(data, decode_size, &uleb_value, &uleb_length);
        decode_offset = uleb_length;
    }
    
    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Use aom_codec_set_option to set decoder options
    // We'll try to set a few common decoder options
    const char* option_names[] = {"tile_columns", "tile_rows", "row-mt"};
    const char* option_values[] = {"1", "1", "1"};
    
    for (size_t i = 0; i < sizeof(option_names)/sizeof(option_names[0]); i++) {
        aom_codec_set_option(&decoder, option_names[i], option_values[i]);
    }
    
    // 4. Use aom_codec_set_frame_buffer_functions to set custom frame buffer callbacks
    FrameBufferData frame_buffer_data;
    aom_codec_set_frame_buffer_functions(&decoder,  id_000013_get_frame_buffer,  id_000013_release_frame_buffer, &frame_buffer_data);
    
    // 5. Use aom_codec_get_stream_info to get stream information
    // First, peek at stream info
    aom_codec_stream_info_t stream_info = {0};
    if (decode_offset < size) {
        size_t remaining = size - decode_offset;
        aom_codec_peek_stream_info(decoder_iface, data + decode_offset, remaining, &stream_info);
        
        // Now get stream info from decoder context
        aom_codec_get_stream_info(&decoder, &stream_info);
    }
    
    // 6. Decode the frame if we have enough data
    if (decode_offset < size) {
        size_t remaining = size - decode_offset;
        
        // Decode the frame
        aom_codec_decode(&decoder, data + decode_offset, remaining, nullptr);
        
        // 7. Use aom_codec_control for additional decoder control
        // Try a few control codes (these are common control codes)
        int ctrl_ids[] = {AOMD_GET_LAST_QUANTIZER, AOMD_GET_LAST_REF_UPDATES};
        for (size_t i = 0; i < sizeof(ctrl_ids)/sizeof(ctrl_ids[0]); i++) {
            int value = 0;
            aom_codec_control(&decoder, ctrl_ids[i], &value);
        }
        
        // 8. Try to get preview frame (if available)
        const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
        
        // 9. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t* img = nullptr;
        
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 10. Use aom_img_set_rect to set a region of interest
            // Use safe coordinates within image bounds
            unsigned int x = stream_info.w > 4 ? 0 : 0;
            unsigned int y = stream_info.h > 4 ? 0 : 0;
            unsigned int w = stream_info.w > 4 ? (stream_info.w / 2) : stream_info.w;
            unsigned int h = stream_info.h > 4 ? (stream_info.h / 2) : stream_info.h;
            unsigned int border = 0;
            
            if (w > 0 && h > 0) {
                // Create a temporary image to test aom_img_set_rect
                aom_image_t temp_img;
                memset(&temp_img, 0, sizeof(temp_img));
                temp_img.fmt = AOM_IMG_FMT_I420;
                temp_img.d_w = w;
                temp_img.d_h = h;
                temp_img.r_w = w;
                temp_img.r_h = h;
                
                // Try to set rectangle (may fail but that's OK for fuzzing)
                aom_img_set_rect(&temp_img, x, y, w, h, border);
            }
            
            // 11. Use aom_img_remove_metadata if image has metadata
            // First check if image has metadata
            size_t metadata_count = aom_img_num_metadata(img);
            if (metadata_count > 0) {
                aom_img_remove_metadata(img);
            }
            
            // Write some information about the frame to output file
            if (out_file) {
                fwrite(&img->d_w, sizeof(img->d_w), 1, out_file);
                fwrite(&img->d_h, sizeof(img->d_h), 1, out_file);
            }
        }
    }
    
    // 12. Clean up allocated resources
    
    // Destroy decoder
    aom_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}