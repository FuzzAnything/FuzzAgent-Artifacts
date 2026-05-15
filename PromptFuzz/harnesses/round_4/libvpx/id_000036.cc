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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_set_frame_buffer_functions","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_control_","vpx_img_alloc","vpx_img_free","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_error","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_control_","vpx_img_alloc","vpx_img_free","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_error","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* buffer;
    size_t buffer_size;
};

static int  id_000036_get_frame_buffer(void* cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (min_size > fb_data->buffer_size) {
        // Reallocate buffer if needed
        uint8_t* new_buffer = static_cast<uint8_t*>(realloc(fb_data->buffer, min_size));
        if (!new_buffer) {
            return -1;
        }
        fb_data->buffer = new_buffer;
        fb_data->buffer_size = min_size;
    }
    
    fb->data = fb_data->buffer;
    fb->size = fb_data->buffer_size;
    return 0;
}

static int  id_000036_release_frame_buffer(void* cb_priv, 
                               vpx_codec_frame_buffer_t* fb) {
    // Nothing to do for this simple implementation
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Determine which codec to use based on first byte
    vpx_codec_iface_t* codec_iface = nullptr;
    if (data[0] & 0x01) {
        codec_iface = vpx_codec_vp8_dx();
    } else {
        codec_iface = vpx_codec_vp9_dx();
    }
    
    if (!codec_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate and initialize decoder context
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&codec_ctx, codec_iface, 
                                                 &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Check error using vpx_codec_error
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&codec_ctx);
        (void)error_str; // Suppress unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up frame buffer callbacks
    FrameBufferData fb_data = {nullptr, 0};
    err = vpx_codec_set_frame_buffer_functions(&codec_ctx, 
                                                id_000036_get_frame_buffer,
                                                id_000036_release_frame_buffer,
                                               &fb_data);
    
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_error(&codec_ctx);
        (void)error_str;
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to get global headers (more relevant for encoding, but we'll call it)
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&codec_ctx);
    (void)global_headers; // May be NULL for decoder
    
    // Try a control operation - set decoder to use post-processing
    int ctrl_id = VP8_SET_POSTPROC;
    int value = 1; // Enable post-processing
    err = vpx_codec_control_(&codec_ctx, ctrl_id, value);
    // Note: control may fail for VP9, but we call it as requested
    
    // Allocate a dummy image (not used for decoding but to call vpx_img_alloc)
    vpx_image_t dummy_img;
    vpx_image_t* img_ptr = vpx_img_alloc(&dummy_img, VPX_IMG_FMT_I420, 
                                         16, 16, fuzz_uint32_t_2);
    if (img_ptr) {
        // Clean up the image
        vpx_img_free(img_ptr);
    }
    
    // Read and decode data from the file
    uint8_t* file_buffer = static_cast<uint8_t*>(malloc(size));
    if (file_buffer) {
        fread(file_buffer, 1, size, in_file);
        
        // Decode the data
        err = vpx_codec_decode(&codec_ctx, file_buffer, (unsigned int)size, 
                               nullptr, fuzz_int64_t_3);
        
        // Try to get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t* decoded_img;
        while ((decoded_img = vpx_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
            // Frame available - do nothing with it for fuzzing
        }
        
        free(file_buffer);
    }
    
    // Clean up
    const char* final_error = vpx_codec_error(&codec_ctx);
    (void)final_error;
    
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    if (fb_data.buffer) {
        free(fb_data.buffer);
    }
    
    assert_file_closed(&in_file);
	return 0;
}