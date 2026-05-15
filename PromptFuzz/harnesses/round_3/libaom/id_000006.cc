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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_get_metadata","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_get_metadata","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* buffer;
    size_t size;
};

static int  id_000006_get_frame_buffer(void* cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate new buffer if needed
    if (!fb_data->buffer || fb_data->size < min_size) {
        free(fb_data->buffer);
        fb_data->buffer = static_cast<uint8_t*>(malloc(min_size));
        if (!fb_data->buffer) {
            return -1;
        }
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer;
    fb->size = fb_data->size;
    return 0;
}

static int  id_000006_release_frame_buffer(void* cb_priv, 
                               aom_codec_frame_buffer_t* fb) {
    // Don't free the buffer here - we'll manage it separately
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 100) {
        return 0;
    }
    
    // Create a file handle for the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read all data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate buffer for compressed data
    aom_fixed_buf_t compressed_buf;
    compressed_buf.sz = file_size;
    compressed_buf.buf = malloc(file_size);
    if (!compressed_buf.buf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read data into buffer
    size_t bytes_read = fread(compressed_buf.buf, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != (size_t)file_size) {
        free(compressed_buf.buf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    cfg.allow_lowbitdepth = 1;
    
    // Get interface name
    const char* iface_name = aom_codec_iface_name(iface);
    (void)iface_name; // Use variable to avoid unused warning
    
    // Initialize decoder with version check
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, 
                                                        AOM_DECODER_ABI_VERSION);
    
    // Get build configuration
    const char* build_config = aom_codec_build_config();
    (void)build_config; // Use variable to avoid unused warning
    
    // Try to set compressed data buffer (this API is for encoding, but we'll test it anyway)
    // Using pad values derived from input data
    unsigned int pad_before = data[0] % 64;
    unsigned int pad_after = data[1] % 64;
    aom_codec_err_t set_buf_result = aom_codec_set_cx_data_buf(&decoder, &compressed_buf, 
                                                              pad_before, pad_after);
    
    // Convert error to string for logging
    const char* set_buf_error_str = aom_codec_err_to_string(set_buf_result);
    (void)set_buf_error_str; // Use variable to avoid unused warning
    
    if (init_result != AOM_CODEC_OK) {
        // Get error message
        const char* error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Use variable to avoid unused warning
        
        free(compressed_buf.buf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions
    FrameBufferData fb_data = {nullptr, 0};
    aom_codec_err_t fb_result = aom_codec_set_frame_buffer_functions(
        &decoder,  id_000006_get_frame_buffer,  id_000006_release_frame_buffer, &fb_data);
    
    // Convert frame buffer error to string
    const char* fb_error_str = aom_codec_err_to_string(fb_result);
    (void)fb_error_str; // Use variable to avoid unused warning
    
    // Decode the data
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, 
                                                    (const uint8_t*)compressed_buf.buf, 
                                                    compressed_buf.sz, nullptr);
    
    // Get error message if decode failed
    if (decode_result != AOM_CODEC_OK) {
        const char* decode_error = aom_codec_error(&decoder);
        (void)decode_error; // Use variable to avoid unused warning
    }
    
    // Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Get number of metadata entries
        size_t num_metadata = aom_img_num_metadata(img);
        
        // Try to get metadata if available
        if (num_metadata > 0) {
            // Use first index (0) or derive from input data
            size_t metadata_index = data[2] % (num_metadata > 0 ? num_metadata : 1);
            const aom_metadata_t* metadata = aom_img_get_metadata(img, metadata_index);
            (void)metadata; // Use variable to avoid unused warning
        }
        
        // Also try to get metadata with index derived from input
        size_t test_index = data[3] % 10; // Try indices 0-9
        const aom_metadata_t* test_metadata = aom_img_get_metadata(img, test_index);
        (void)test_metadata; // Use variable to avoid unused warning
    }
    
    // Clean up
    aom_codec_destroy(&decoder);
    free(compressed_buf.buf);
    free(fb_data.buffer);
    
    assert_file_closed(&in_file);
	return 0;
}