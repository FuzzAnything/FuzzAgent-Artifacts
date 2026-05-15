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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_num_metadata","aom_codec_destroy","aom_img_alloc","aom_img_num_metadata","aom_img_add_metadata","aom_img_num_metadata","aom_img_free","aom_uleb_encode"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_num_metadata","aom_codec_destroy","aom_img_alloc","aom_img_num_metadata","aom_img_add_metadata","aom_img_num_metadata","aom_img_free","aom_uleb_encode"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Get version string (API 1)
    const char *version = aom_codec_version_str();
    (void)version; // Prevent unused variable warning
    
    // 2. Decode ULEB value from input data (API 5)
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    // Use at least 1 byte but no more than available
    size_t uleb_available = size > 0 ? size : 0;
    if (uleb_available > 0) {
        aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    }
    
    // 3. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // 4. Try to get stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Initialize decoder to get stream info
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, 
        aom_codec_av1_dx(), 
        NULL,  // No config
        fuzz_int64_t_1,     // No flags
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_result == AOM_CODEC_OK) {
        // Try to get stream info (API 3)
        aom_codec_get_stream_info(&codec_ctx, &stream_info);
        
        // 5. Try to decode the input data
        aom_codec_decode(&codec_ctx, data, size, NULL);
        
        // 6. Get error detail if available (API 2)
        const char *error_detail = aom_codec_error_detail(&codec_ctx);
        (void)error_detail; // Prevent unused variable warning
        
        // 7. Try to get decoded frames and check metadata
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = NULL;
        
        while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != NULL) {
            // 8. Get number of metadata in image (API 4)
            size_t metadata_count = aom_img_num_metadata(img);
            (void)metadata_count; // Prevent unused variable warning
        }
        
        // 9. Clean up decoder
        aom_codec_destroy(&codec_ctx);
    }
    
    // 10. Create a test image to demonstrate metadata API
    aom_image_t test_img;
    memset(&test_img, 0, sizeof(test_img));
    
    // Try to allocate a small test image
    aom_img_alloc(&test_img, AOM_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4);
    
    if (test_img.planes[0] != NULL) {
        // Get metadata count (should be 0 for new image)
        size_t test_metadata_count = aom_img_num_metadata(&test_img);
        (void)test_metadata_count;
        
        // Try to add some metadata if we have enough data
        if (size > sizeof(uint32_t)) {
            uint32_t metadata_type = 0;
            memcpy(&metadata_type, data, sizeof(uint32_t));
            
            size_t metadata_size = size > sizeof(uint32_t) ? 
                                  size - sizeof(uint32_t) : 0;
            if (metadata_size > 0) {
                aom_img_add_metadata(&test_img, fuzz_uint32_t_5, 
                                    data + sizeof(uint32_t), 
                                    metadata_size, 
                                    AOM_MIF_ANY_FRAME);
            }
            
            // Get metadata count again
            test_metadata_count = aom_img_num_metadata(&test_img);
            (void)test_metadata_count;
        }
        
        // Free the test image
        aom_img_free(&test_img);
    }
    
    // 11. Try ULEB encoding with the decoded value
    if (uleb_available > 0 && uleb_length > 0) {
        uint8_t encoded_buffer[10];
        size_t encoded_size = 0;
        
        // Try to encode the decoded ULEB value
        aom_uleb_encode(fuzz_uint64_t_6, sizeof(encoded_buffer), 
                       encoded_buffer, &encoded_size);
    }
    
    // 12. Use FILE* operations as specified
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process some data from the memory file
        uint8_t buffer[16];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        (void)bytes_read; // Prevent unused variable warning
        
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Prevent unused variable warning
        
        assert_file_closed(&in_file);;
    }
    
    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Prevent unused variable warning
        
        assert_file_closed(&out_file);;
    }
    
    // 14. Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}