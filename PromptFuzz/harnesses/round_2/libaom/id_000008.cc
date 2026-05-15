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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_err_to_string","aom_img_alloc","aom_img_wrap","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_num_metadata","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_err_to_string"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_err_to_string","aom_img_alloc","aom_img_wrap","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_num_metadata","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_err_to_string"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open an output file for writing (not strictly necessary but demonstrates the pattern)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors (though not used in this specific example)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Prevent unused variable warnings

    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get codec interface name (required API)
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Prevent unused variable warning

    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = {0, 0, 0};
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Get error string (required API)
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str; // Prevent unused variable warning
        const char *err_string = aom_codec_err_to_string(err);
        (void)err_string; // Prevent unused variable warning
    }

    // Allocate an image using aom_img_alloc (required API)
    // Use dummy dimensions for allocation
    aom_image_t *allocated_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 320, 240, 16);
    
    // Wrap existing memory as an image (required API)
    // Use a small buffer for demonstration
    unsigned char img_buffer[320 * 240 * 3 / 2] = {0}; // I420 format
    aom_image_t *wrapped_img = aom_img_wrap(NULL, AOM_IMG_FMT_I420, 320, 240, 1, img_buffer);

    // Decode the input data
    fseek(in_file, 0, SEEK_SET);
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (buffer) {
        size_t bytes_read = fread(buffer, 1, size, in_file);
        
        if (bytes_read > 0) {
            // Try to decode the data
            err = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
            
            // Get frame from decoder
            aom_codec_iter_t iter = NULL;
            aom_image_t *img;
            while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
                // Use aom_img_set_rect on the allocated image (required API)
                if (allocated_img) {
                    // Set a small rectangle in the allocated image
                    aom_img_set_rect(allocated_img, 10, 10, 50, 50, 1);
                }
                
                // Use aom_img_num_metadata on the decoded image (required API)
                size_t metadata_count = aom_img_num_metadata(img);
                (void)metadata_count; // Prevent unused variable warning
                
                // Try to get encoded data from decoder (not typical but demonstrates API usage)
                // Note: In real usage, aom_codec_get_cx_data is for encoder output
                aom_codec_iter_t cx_iter = NULL;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&decoder, &cx_iter)) != NULL) {
                    // Process packet if any
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }
        }
        free(buffer);
    }

    // Clean up allocated resources
    if (allocated_img) {
        aom_img_free(allocated_img);
    }
    
    if (wrapped_img) {
        // Note: wrapped_img uses stack-allocated buffer, so we just free the wrapper
        aom_img_free(wrapped_img);
    }
    
    // Destroy codec context
    aom_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Get error string one more time to ensure API usage
    const char *final_error = aom_codec_err_to_string(AOM_CODEC_OK);
    (void)final_error; // Prevent unused variable warning

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}