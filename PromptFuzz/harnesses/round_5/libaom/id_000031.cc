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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_img_alloc","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_encode","aom_codec_destroy","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_err_to_string","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_img_alloc","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_encode","aom_codec_destroy","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_err_to_string","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Use aom_uleb_decode to decode a value from input
    uint64_t decoded_value = 0;
    size_t uleb_len = 0;
    int decode_result = aom_uleb_decode(data, size, &decoded_value, &uleb_len);
    
    // 2. Use aom_uleb_encode to re-encode the decoded value (or a modified one)
    uint8_t encoded_buffer[16] = {0};
    size_t encoded_size = 0;
    uint64_t value_to_encode = decoded_value + (size & 0xFF); // Modify value based on input size
    int encode_result = aom_uleb_encode(value_to_encode, sizeof(encoded_buffer), 
                                         encoded_buffer, &encoded_size);
    
    // 3. Create an image and add metadata from input
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Allocate image with minimum valid dimensions
    aom_image_t* image_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!image_ptr) {
        return 0; // Can't proceed without image
    }
    
    // Use some input data for metadata
    uint32_t metadata_type = 0;
    if (size >= sizeof(metadata_type)) {
        memcpy(&metadata_type, data, sizeof(metadata_type));
    }
    
    size_t metadata_size = (size > 0) ? (size % 256) : 0; // Limit metadata size
    const uint8_t* metadata_data = (size > 0) ? data : nullptr;
    
    // Add metadata to image
    int metadata_result = aom_img_add_metadata(image_ptr, metadata_type, 
                                               metadata_data, metadata_size, 
                                               AOM_MIF_ANY_FRAME);
    
    // 4. Initialize encoder with default config
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    aom_codec_err_t enc_config_result = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    // 5. Initialize encoder context
    if (enc_config_result == AOM_CODEC_OK) {
        aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder, encoder_iface, 
                                                                  &cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        if (enc_init_result == AOM_CODEC_OK) {
            // 6. Use aom_codec_control with some control IDs
            // Try a few different control IDs based on input
            int control_id = AOME_SET_CPUUSED;
            int control_value = (size & 7) + 1; // Value between 1-8
            aom_codec_control(&encoder, control_id, control_value);
            
            // 7. Encode the image
            aom_codec_pts_t pts = 0;
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = 0;
            
            aom_codec_err_t encode_img_result = aom_codec_encode(&encoder, image_ptr, 
                                                                  pts, duration, flags);
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 8. Try to peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Use first few bytes or entire input for stream info
    size_t peek_size = (size > 0) ? size : 0;
    const uint8_t* peek_data = (size > 0) ? data : nullptr;
    
    if (peek_size > 0 && peek_data != nullptr) {
        aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
        aom_codec_err_t peek_result = aom_codec_peek_stream_info(decoder_iface, 
                                                                  peek_data, peek_size, 
                                                                  &stream_info);
    }
    
    // 9. Use aom_codec_err_to_string for error reporting
    const char* err_str = aom_codec_err_to_string(AOM_CODEC_OK);
    if (decode_result != 0) {
        err_str = aom_codec_err_to_string(AOM_CODEC_ERROR);
    }
    
    // 10. Create a FILE* for reading input data using fmemopen
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the file
        uint8_t read_buffer[64];
        size_t bytes_read = fread(read_buffer, 1, sizeof(read_buffer), in_file);
        
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        assert_file_closed(&in_file);;
    }
    
    // 11. Create a FILE* for writing output
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write the encoded ULEB value
        if (encoded_size > 0) {
            fwrite(encoded_buffer, 1, encoded_size, out_file);
        }
        
        // Write some input data
        if (size > 0) {
            size_t write_size = (size < 1024) ? size : 1024;
            fwrite(data, 1, write_size, out_file);
        }
        
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use out_fd to avoid unused variable warning
        
        assert_file_closed(&out_file);;
    }
    
    // 12. Clean up image resources
    aom_img_free(image_ptr);
    
    // 13. Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}