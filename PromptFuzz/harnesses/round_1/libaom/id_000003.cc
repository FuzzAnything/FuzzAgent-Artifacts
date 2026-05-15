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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_cx_data_buf","aom_codec_error","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_alloc","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_num_metadata","aom_img_get_metadata","aom_img_free","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_alloc","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_img_num_metadata","aom_img_get_metadata","aom_img_free","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate buffer to read the compressed data
    std::vector<uint8_t> compressed_data(size);
    size_t bytes_read = fread(compressed_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != size) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Use aom_codec_error to get error information
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str; // Prevent unused variable warning
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up output buffer for compressed data
    aom_fixed_buf_t cx_data_buf;
    cx_data_buf.buf = compressed_data.data();
    cx_data_buf.sz = size;

    // Set compressed data buffer with padding
    aom_codec_err_t set_buf_result = aom_codec_set_cx_data_buf(
        &decoder, &cx_data_buf, 0, 0);
    
    if (set_buf_result != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str;
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to decode the data
    aom_codec_err_t decode_result = aom_codec_decode(
        &decoder, compressed_data.data(), size, nullptr);
    
    // Get error information if decode failed
    if (decode_result != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str;
    }

    // Try to get a decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);

    if (img) {
        // Allocate a new image with same dimensions as decoded image
        aom_image_t *allocated_img = aom_img_alloc(
            nullptr, 
            static_cast<aom_img_fmt_t>(img->fmt),
            img->d_w,
            img->d_h,
            16); // 16-byte alignment
        
        if (allocated_img) {
            // Try to get metadata from the decoded image
            size_t num_metadata = aom_img_num_metadata(img);
            for (size_t i = 0; i < num_metadata; i++) {
                const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
                if (metadata) {
                    // Allocate a copy of the metadata
                    aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                        metadata->type,
                        metadata->payload,
                        metadata->sz,
                        metadata->insert_flag);
                    
                    if (metadata_copy) {
                        // Add metadata to allocated image
                        aom_img_add_metadata(
                            allocated_img,
                            metadata_copy->type,
                            metadata_copy->payload,
                            metadata_copy->sz,
                            metadata_copy->insert_flag);
                        
                        // Free the allocated metadata copy
                        aom_img_metadata_free(metadata_copy);
                    }
                }
            }

            // Try to get metadata from the allocated image
            size_t allocated_metadata_count = aom_img_num_metadata(allocated_img);
            for (size_t i = 0; i < allocated_metadata_count; i++) {
                const aom_metadata_t *alloc_metadata = aom_img_get_metadata(allocated_img, i);
                if (alloc_metadata) {
                    // Metadata is available, but we don't need to use it
                    (void)alloc_metadata;
                }
            }

            // Clean up allocated image
            aom_img_free(allocated_img);
        }
    }

    // Get final error status
    const char *final_error = aom_codec_error(&decoder);
    (void)final_error; // Prevent unused variable warning

    // Clean up decoder
    aom_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}