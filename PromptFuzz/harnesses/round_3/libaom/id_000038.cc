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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    uint8_t* data;
    size_t size;
};

// Frame buffer get callback
static int  id_000038_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or reallocate buffer if needed
    if (fb_data->size < min_size) {
        free(fb_data->data);
        fb_data->data = static_cast<uint8_t*>(malloc(min_size));
        if (!fb_data->data) {
            return -1;
        }
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->data;
    fb->size = fb_data->size;
    return 0;
}

// Frame buffer release callback
static int  id_000038_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // In this simple implementation, we don't actually release the buffer
    // The buffer will be freed when FrameBufferData is destroyed
    (void)cb_priv;
    (void)fb;
    return 0;
}

// The main fuzzer function
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create a FILE* for input using fmemopen as specified
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img = nullptr;
    aom_fixed_buf_t* global_headers = nullptr;
    FrameBufferData fb_data = {nullptr, 0};
    
    // 1. Get AV1 decoder interface
    aom_codec_iface_t* iface = aom_codec_av1_dx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Get interface name
    const char* iface_name = aom_codec_iface_name(iface);
    (void)iface_name; // Mark as used
    
    // 3. Get build configuration
    const char* build_config = aom_codec_build_config();
    (void)build_config; // Mark as used
    
    // 4. Initialize decoder
    cfg.threads = 1;
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // 5. Set frame buffer functions
    if (err == AOM_CODEC_OK) {
        err = aom_codec_set_frame_buffer_functions(&decoder,  id_000038_get_frame_buffer, 
                                                   id_000038_release_frame_buffer, &fb_data);
    }
    
    // 6. Get error string if initialization failed
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
        (void)err_str; // Mark as used
        
        // Get error detail
        const char* err_detail = aom_codec_error_detail(&decoder);
        (void)err_detail; // Mark as used
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        if (fb_data.data) {
            free(fb_data.data);
        }
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Get global headers (may be null for decoder)
    global_headers = aom_codec_get_global_headers(&decoder);
    (void)global_headers; // Mark as used
    
    // 8. Set cx data buffer (for encoder output, but we'll demonstrate usage)
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = malloc(1024);
    cx_buf.sz = 1024;
    if (cx_buf.buf) {
        // This is typically for encoder, but we call it for coverage
        aom_codec_set_cx_data_buf(&decoder, &cx_buf, 0, 0);
        free(cx_buf.buf);
    }
    
    // 9. Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 10. Get decoded frames
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 11. Get metadata if available
        size_t metadata_count = aom_img_num_metadata(img);
        if (metadata_count > 0) {
            const aom_metadata_t* metadata = aom_img_get_metadata(img, 0);
            (void)metadata; // Mark as used
        }
        
        // 12. Get plane dimensions
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            (void)plane_width;   // Mark as used
            (void)plane_height;  // Mark as used
            
            // 13. Set rectangle on the image (small region)
            if (plane_width > 4 && plane_height > 4) {
                aom_img_set_rect(img, 0, 0, 4, 4, 0);
            }
        }
        
        // Write some information to output file
        fwrite(&img->d_w, sizeof(img->d_w), 1, out_file);
        fwrite(&img->d_h, sizeof(img->d_h), 1, out_file);
    }
    
    // 14. Destroy decoder
    aom_codec_destroy(&decoder);
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    if (fb_data.data) {
        free(fb_data.data);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}