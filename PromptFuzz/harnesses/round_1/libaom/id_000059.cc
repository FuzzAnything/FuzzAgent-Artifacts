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
//<score> 35, nr_unique_branch: 8
//<Quality> {"density":7,"unique_branches":{"update_extra_cfg":[[1758,7,1758,26,0,0,4,1]],"validate_config":[[667,32,667,49,52,0,4,1]],"encoder_set_option":[[4811,7,4811,30,0,0,4,0]],"arg_parse_uint_helper":[[93,7,93,26,0,0,4,1],[93,30,93,47,0,0,4,1],[94,9,94,27,0,0,4,1],[20,7,20,14,1,0,4,0],[20,7,20,14,2,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if not enough data for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_fixed_buf_t *global_headers = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_image_t *image = nullptr;
    aom_fixed_buf_t output_buf;
    
    // Initialize output buffer
    output_buf.buf = nullptr;
    output_buf.sz = 0;
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for potential future use
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Prevent unused variable warning
    
    // 1. Initialize encoder with default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set some basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Initialize encoder
    if (aom_codec_enc_init(&encoder, iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Call aom_codec_set_option at least once
    // Set a simple encoder option using data from input
    char option_value[32] = {0};
    size_t option_len = size > 32 ? 32 : size;
    memcpy(option_value, data, option_len);
    // Ensure null termination
    option_value[option_len - 1] = '\0';
    
    // Set the option (using a simple option name)
    aom_codec_set_option(&encoder, "cpu-used", option_value);
    
    // 3. Get global headers
    global_headers = aom_codec_get_global_headers(&encoder);
    
    // 4. Create and manipulate an image
    image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
    if (image) {
        // Fill image with some data from input
        size_t copy_size = size > image->sz ? image->sz : size;
        for (int plane = 0; plane < 3; plane++) {
            size_t plane_size = image->stride[plane] * aom_img_plane_height(image, plane);
            size_t to_copy = copy_size > plane_size ? plane_size : copy_size;
            if (to_copy > 0) {
                memcpy(image->planes[plane], data, to_copy);
            }
        }
        
        // Call aom_img_set_rect at least once
        // Use values derived from input data
        unsigned int x = data[0] % 32;
        unsigned int y = data[1] % 32;
        unsigned int w = 8 + (data[2] % 24);
        unsigned int h = 8 + (data[3] % 24);
        unsigned int border = data[4] % 16;
        
        aom_img_set_rect(image, x, y, w, h, border);
        
        // 5. Create metadata using data from input
        uint32_t metadata_type = data[5] | (data[6] << 8) | (data[7] << 16) | (data[8] << 24);
        size_t metadata_size = size > 50 ? 20 : size / 4;
        
        // Ensure we have enough data
        if (metadata_size > 0 && size > 50) {
            metadata = aom_img_metadata_alloc(
                metadata_type,
                data + 20,  // Use part of input data as metadata payload
                metadata_size,
                AOM_MIF_ANY_FRAME
            );
            
            // Add metadata to image if allocation succeeded
            if (metadata) {
                aom_img_add_metadata(image, metadata->type, 
                                    metadata->payload, metadata->sz,
                                    metadata->insert_flag);
            }
        }
    }
    
    // 6. Set up output buffer for encoded data
    // Allocate buffer using input size as hint
    size_t buf_size = size * 2;  // Conservative estimate
    uint8_t *buf = (uint8_t*)malloc(buf_size);
    if (buf) {
        output_buf.buf = buf;
        output_buf.sz = buf_size;
        
        // Call aom_codec_set_cx_data_buf at least once
        unsigned int pad_before = data[9] % 64;
        unsigned int pad_after = data[10] % 64;
        aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);
    }
    
    // 7. Encode the image if we have one
    if (image) {
        // Create output FILE for potential writing
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            int out_fd = fuzz_fileno(out_file);
            (void)out_fd; // Prevent unused variable warning
            
            // Encode the image
            aom_codec_encode(&encoder, image, 0, 1, 0);
            
            // Get encoded data
            aom_codec_iter_t iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Write to file (optional, for completeness)
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            assert_file_closed(&out_file);;
        }
    }
    
    // 8. Clean up all allocated resources
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    if (image) {
        aom_img_free(image);
    }
    
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    aom_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}