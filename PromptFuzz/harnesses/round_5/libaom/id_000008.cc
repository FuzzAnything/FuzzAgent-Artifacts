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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_version_extra_str","aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_flip","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode AV1 video frames from input data, process them, and handle version/build information:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get version information (call required APIs)
    int version = aom_codec_version();
    const char* version_extra = aom_codec_version_extra_str();
    const char* build_config = aom_codec_build_config();
    
    // Use the results to prevent compiler optimization removal
    if (version > 0) {
        // Just to use the variables
        (void)version_extra;
        (void)build_config;
    }

    // 2. Open input as FILE* (as per requirement #4)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file descriptor (as per requirement #5)
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning

    // 4. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 5. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    err = aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Decode the input data
    err = aom_codec_decode(&codec_ctx, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 7. Get stream info again from context
    aom_codec_stream_info_t stream_info2;
    memset(&stream_info2, 0, sizeof(stream_info2));
    
    err = aom_codec_get_stream_info(&codec_ctx, &stream_info2);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 8. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img = nullptr;
    
    while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 9. Flip the image (call required API)
        aom_img_flip(img);
        
        // Process image data to ensure it's used
        if (img->planes[0] && img->stride[0] > 0) {
            // Access some pixel data to ensure the image is actually processed
            volatile uint8_t sample_pixel = img->planes[0][0];
            (void)sample_pixel;
        }
    }

    // 10. Open output file (as per requirement #4)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some metadata to output file
        fprintf(out_file, "Version: %d\n", version);
        if (version_extra) {
            fprintf(out_file, "Extra: %s\n", version_extra);
        }
        if (build_config) {
            fprintf(out_file, "Build: %s\n", build_config);
        }
        assert_file_closed(&out_file);;
        
        // Remove the output file to avoid leaving files around
        remove("output_file");
    }

    // 11. Clean up
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}