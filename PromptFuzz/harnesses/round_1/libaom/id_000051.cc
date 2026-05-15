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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"aom_img_metadata_alloc":[[340,7,340,12,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_codec_get_frame","aom_img_num_metadata","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_codec_get_frame","aom_img_num_metadata","aom_img_alloc","aom_img_wrap","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

// Get frame buffer callback
int  id_000051_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Ensure buffer is large enough
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer.size();
    fb->priv = fb_data;
    
    return 0;
}

// Release frame buffer callback
int  id_000051_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // Nothing to do here - buffer is managed by FrameBufferData
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer for input using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file (will be unused in fuzzing but needed for API demonstration)
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (not strictly needed but following instructions)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize decoder interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 0, AOM_DECODER_ABI_VERSION);
    
    // Use error string API
    const char* err_str = aom_codec_err_to_string(err);
    (void)err_str; // Mark as used
    
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set frame buffer functions
    FrameBufferData fb_data;
    err = aom_codec_set_frame_buffer_functions(&decoder,  id_000051_get_frame_buffer, 
                                                id_000051_release_frame_buffer, &fb_data);
    
    // Try to peek stream info first
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    // Create buffer for reading
    std::vector<uint8_t> buffer(size);
    if (size > 0) {
        fread(buffer.data(), 1, size, in_file);
    }
    
    // Decode the input data
    err = aom_codec_decode(&decoder, buffer.data(), buffer.size(), nullptr);
    
    // Get preview frame (if available)
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
    if (preview_frame) {
        // Use metadata API on preview frame
        size_t metadata_count = aom_img_num_metadata(preview_frame);
        (void)metadata_count; // Mark as used
    }
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* frame = nullptr;
    
    while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Use metadata API
        size_t metadata_count = aom_img_num_metadata(frame);
        (void)metadata_count; // Mark as used
        
        // Demonstrate aom_img_alloc
        aom_image_t* allocated_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                                   frame->d_w, frame->d_h, 1);
        
        if (allocated_img) {
            // Create a buffer for wrapped image
            size_t img_size = frame->d_w * frame->d_h * 3 / 2; // I420 size
            std::vector<unsigned char> img_buffer(img_size);
            
            // Demonstrate aom_img_wrap
            aom_image_t* wrapped_img = aom_img_wrap(nullptr, AOM_IMG_FMT_I420,
                                                   frame->d_w, frame->d_h, 1,
                                                   img_buffer.data());
            
            // Demonstrate metadata allocation and freeing
            aom_metadata_t* metadata = aom_img_metadata_alloc(
                1, nullptr, 0, AOM_MIF_ANY_FRAME);
            
            if (metadata) {
                aom_img_metadata_free(metadata);
            }
            
            if (wrapped_img) {
                aom_img_free(wrapped_img);
            }
            
            aom_img_free(allocated_img);
        }
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}