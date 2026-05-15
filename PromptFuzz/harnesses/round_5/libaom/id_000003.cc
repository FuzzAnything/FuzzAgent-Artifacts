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
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_img_wrap","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_free","aom_codec_enc_config_set","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_img_wrap","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free"],"visited":1}
/**/


// Include necessary libaom headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum input size for basic operations
    if (size < 1024) {
	return 0;
	}
	
    
    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Extract file descriptor for potential use
    int fd = fuzz_fileno(in_file);
    (void)fd; // Prevent unused variable warning
    
    // 1. Create an image using aom_img_wrap
    // For I420 format, we need width*height*1.5 bytes
    unsigned int width = 64;
    unsigned int height = 64;
    size_t image_size = width * height * 3 / 2;
    
    if (size < image_size) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    unsigned char *img_data = (unsigned char *)malloc(image_size);
    if (!img_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read image data from input
    fread(img_data, 1, image_size, in_file);
    
    aom_image_t img;
    aom_image_t *img_ptr = aom_img_wrap(&img, AOM_IMG_FMT_I420, 
                                        width, height, 1, img_data);
    if (!img_ptr) {
        free(img_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Initialize encoder
    aom_codec_ctx_t codec;
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    
    // Default encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(encoder, &cfg, 0);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(&img);
        free(img_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Set encoder configuration (redundant but demonstrates API usage)
    aom_codec_enc_config_set(&codec, &cfg);
    
    // 4. Setup frame buffer functions
    // Define dummy callback functions
    aom_get_frame_buffer_cb_fn_t get_cb = [](void *cb_priv, size_t min_size, 
                                             aom_codec_frame_buffer_t *fb) -> int {
        // Simple allocation for demonstration
        fb->data = (uint8_t *)malloc(min_size);
        if (!fb->data) return -1;
        fb->size = min_size;
        return 0;
    };
    
    aom_release_frame_buffer_cb_fn_t release_cb = [](void *cb_priv, 
                                                    aom_codec_frame_buffer_t *fb) -> int {
        free(fb->data);
        return 0;
    };
    
    aom_codec_set_frame_buffer_functions(&codec, get_cb, release_cb, nullptr);
    
    // 5. Setup output buffer for compressed data
    size_t output_buffer_size = 1024 * 1024; // 1MB buffer
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(output_buffer_size);
    output_buf.sz = output_buffer_size;
    
    if (output_buf.buf) {
        aom_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    }
    
    // 6. Encode the image
    aom_codec_encode(&codec, &img, 0, 1, 0);
    
    // 7. Retrieve encoded data (if any)
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
        (void)pkt;
    }
    
    // 8. Clean up resources
    
    // Free output buffer if allocated
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    // Destroy codec context
    aom_codec_destroy(&codec);
    
    // Free image resources
    aom_img_free(&img);
    free(img_data);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}