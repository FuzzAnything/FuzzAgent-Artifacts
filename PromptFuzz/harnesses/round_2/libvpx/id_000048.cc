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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"vpx_codec_control_":[[101,41,101,50,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE handle to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Variables to track resources
    vpx_codec_ctx_t codec_ctx = {0};
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_image_t *allocated_img = nullptr;
    std::vector<uint8_t> buffer;
    bool decoder_initialized = false;
    
    // Determine which decoder to use (VP8 or VP9)
    const vpx_codec_iface_t *interfaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Try to peek stream info with each decoder interface
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    for (auto iface : interfaces) {
        // Use vpx_codec_iface_name
        const char *iface_name = vpx_codec_iface_name((vpx_codec_iface_t *)iface);
        (void)iface_name; // Use the result to avoid unused variable warning
        
        // Use vpx_codec_get_caps
        vpx_codec_caps_t caps = vpx_codec_get_caps((vpx_codec_iface_t *)iface);
        (void)caps; // Use the result
        
        // Try to peek stream info
        if (vpx_codec_peek_stream_info((vpx_codec_iface_t *)iface, data, 
                                      (unsigned int)size, &stream_info) == VPX_CODEC_OK) {
            decoder_iface = (vpx_codec_iface_t *)iface;
            break;
        }
    }
    
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&codec_ctx, decoder_iface, &dec_cfg, 
                               0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    decoder_initialized = true;
    
    // Read data from file and decode
    buffer.resize(4096);
    size_t bytes_read = 0;
    
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), in_file)) > 0) {
        // Decode the chunk
        if (vpx_codec_decode(&codec_ctx, buffer.data(), 
                            (unsigned int)bytes_read, nullptr, 0) != VPX_CODEC_OK) {
            break;
        }
        
        // Use vpx_codec_control_ (get post-processing control)
        int ctrl_id = 8; // Common control ID for noise sensitivity
        vpx_codec_control_(&codec_ctx, ctrl_id);
        
        // Try to get preview frame (might not exist for decoder)
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
        (void)preview_frame; // Use the result
        
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *img;
        
        while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
            // Allocate an image using vpx_img_alloc
            allocated_img = vpx_img_alloc(nullptr, img->fmt, 
                                         img->d_w, img->d_h, 1);
            
            if (allocated_img) {
                // Process the image (here we just demonstrate usage)
                // In real usage, you might copy data or process the image
                vpx_img_free(allocated_img);
                allocated_img = nullptr;
            }
        }
        
        // Try to get encoded data (should return null for decoder)
        iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
            // Process packet if needed
            (void)pkt;
        }
    }
    
    // Cleanup
    if (decoder_initialized) {
        vpx_codec_destroy(&codec_ctx);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}