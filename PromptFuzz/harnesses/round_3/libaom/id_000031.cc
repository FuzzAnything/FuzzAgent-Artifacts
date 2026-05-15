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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);

    // Peek stream info from input data
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_err_t err = aom_codec_peek_stream_info(iface, data, size, &si);
    
    // Initialize decoder
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    err = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Decode the input data
    err = aom_codec_decode(&ctx, data, size, NULL);
    
    // Get decoded frames
    aom_codec_iter_t iter = NULL;
    while (aom_image_t *img = aom_codec_get_frame(&ctx, &iter)) {
        // Flip the image
        aom_img_flip(img);
        
        // Get number of metadata in image
        size_t num_metadata = aom_img_num_metadata(img);
        
        // Access each metadata if present
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            if (metadata) {
                // Metadata accessed, could process it here
            }
        }
        
        // Get plane dimensions for all planes
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            
            // Use the dimensions to avoid compiler warnings
            (void)plane_width;
            (void)plane_height;
        }
        
        // Remove metadata from image
        aom_img_remove_metadata(img);
    }

    // Create output file for writing (if needed)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get output file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use to avoid warning
        
        // Could write processed data here
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Clean up resources
    assert_file_closed(&in_file);;
    aom_codec_destroy(&ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}