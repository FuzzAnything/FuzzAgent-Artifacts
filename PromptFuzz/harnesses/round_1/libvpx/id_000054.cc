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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":1}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    uint8_t* buffer;
    size_t size;
};

// Frame buffer get callback
int  id_000054_get_frame_buffer_cb(void* cb_priv, size_t min_size, 
                       vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (fb_data->size < min_size) {
        return -1;
    }
    
    fb->data = fb_data->buffer;
    fb->size = fb_data->size;
    return 0;
}

// Frame buffer release callback
int  id_000054_release_frame_buffer_cb(void* cb_priv, 
                           vpx_codec_frame_buffer_t* fb) {
    // In this implementation, we don't actually release anything
    // since the buffer is owned by the fuzzer input
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 100) {
        return 0;
    }
    
    // Initialize variables
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_rational_t dsf = {1, 1};  // Downsampling factor
    vpx_image_t img_wrapper;
    FrameBufferData fb_data;
    
    // Open input file from memory
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd;  // Mark as used
    
    // 1. First, wrap the input data as an image using vpx_img_wrap
    // We'll use the first part of the data as image data
    // For simplicity, assume I420 format with 16x16 dimensions
    size_t image_data_needed = 16 * 16 * 3 / 2;  // I420 16x16
    uint8_t* image_data = nullptr;
    
    if (size >= image_data_needed) {
        image_data = (uint8_t*)data;
        vpx_img_wrap(&img_wrapper, VPX_IMG_FMT_I420, 16, 16, 
                    1, image_data);
        
        // 2. Set a rectangle on the image using vpx_img_set_rect
        // Use valid coordinates within the 16x16 image
        vpx_img_set_rect(&img_wrapper, 2, 2, 8, 8);
    } else {
        // Not enough data for image, but we can still test other APIs
        // with dummy values
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Setup frame buffer callbacks using vpx_codec_set_frame_buffer_functions
    fb_data.buffer = (uint8_t*)data;
    fb_data.size = size;
    
    // 4. Initialize encoder with VP8 codec using vpx_codec_enc_init_ver
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set some reasonable values for fuzzing
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.rc_target_bitrate = 256;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Initialize single encoder
    vpx_codec_err_t ret = vpx_codec_enc_init_ver(&codec_ctx, iface, &cfg, 
                                                0, VPX_ENCODER_ABI_VERSION);
    if (ret != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        vpx_img_free(&img_wrapper);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&codec_ctx,  id_000054_get_frame_buffer_cb, 
                                         id_000054_release_frame_buffer_cb, &fb_data);
    
    // 5. Initialize multiple encoders using vpx_codec_enc_init_multi_ver
    // We'll create a second context for multi-encoder test
    vpx_codec_ctx_t multi_codec_ctx;
    int num_enc = 1;  // Single encoder for simplicity
    ret = vpx_codec_enc_init_multi_ver(&multi_codec_ctx, iface, &cfg, 
                                      num_enc, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    // Clean up multi encoder if initialized
    if (ret == VPX_CODEC_OK) {
        vpx_codec_destroy(&multi_codec_ctx);
    }
    
    // 6. Try to get a preview frame using vpx_codec_get_preview_frame
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    (void)preview_frame;  // Mark as used
    
    // 7. Encode the image (using additional API from the exported list)
    // We need to encode at least one frame to potentially get a preview
    ret = vpx_codec_encode(&codec_ctx, &img_wrapper, 0, 1, 0, 0);
    
    // Try to get preview frame again after encoding
    preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    
    // 8. Get compressed data (using additional API from the exported list)
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        // Process packets if needed
        (void)pkt;
    }
    
    // 9. Cleanup
    vpx_codec_destroy(&codec_ctx);
    vpx_img_free(&img_wrapper);
    assert_file_closed(&in_file);;
    
    // 10. Open an output file to demonstrate file operations
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to demonstrate file operations
        const char* test_output = "Fuzzer test output\n";
        fwrite(test_output, 1, strlen(test_output), out_file);
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd;  // Mark as used
        assert_file_closed(&out_file);;
        
        // Remove the output file
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}