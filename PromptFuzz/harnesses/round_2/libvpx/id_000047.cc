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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 1, nr_unique_branch: 0
//<Quality> {"density":1,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_free","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_img_alloc","vpx_codec_get_frame","vpx_codec_control_","vpx_codec_decode","vpx_img_free","vpx_img_alloc","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_caps","vpx_codec_iface_name"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* for reading input if needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Iterate through available decoders
    for (auto iface : decoder_ifaces) {
        // Skip if interface is null
        if (!iface) continue;
        
        // Get interface name
        const char *iface_name = vpx_codec_iface_name(iface);
        
        // Get decoder capabilities
        vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
        
        // Create decoder context
        vpx_codec_ctx_t codec;
        vpx_codec_err_t ret;
        
        // Initialize decoder with default configuration
        ret = vpx_codec_dec_init_ver(&codec, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (ret != VPX_CODEC_OK) {
            continue;  // Try next decoder
        }
        
        // Try to peek at stream info
        vpx_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.sz = sizeof(si);
        
        ret = vpx_codec_peek_stream_info(iface, data, static_cast<unsigned int>(size), &si);
        
        // If peek was successful, allocate an image with the stream dimensions
        if (ret == VPX_CODEC_OK) {
            // Use stream dimensions for image allocation
            unsigned int width = si.w;
            unsigned int height = si.h;
            
            // Allocate image using stream dimensions
            vpx_image_t *allocated_img = vpx_img_alloc(
                NULL, 
                VPX_IMG_FMT_I420, 
                width ? width : 128, 
                height ? height : 128, 
                1
            );
            
            if (allocated_img) {
                vpx_img_free(allocated_img);
            }
        }
        
        // Decode the input data
        ret = vpx_codec_decode(&codec, data, static_cast<unsigned int>(size), NULL, 0);
        
        if (ret == VPX_CODEC_OK) {
            // Try to get a control value (dummy control - 0 is usually invalid)
            vpx_codec_control_(&codec, 0);
            
            // Iterate through available decoded frames
            vpx_codec_iter_t iter = NULL;
            const vpx_image_t *img;
            
            while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
                // Allocate a new image with the same dimensions as decoded frame
                vpx_image_t *new_img = vpx_img_alloc(
                    NULL,
                    static_cast<vpx_img_fmt_t>(img->fmt),
                    img->d_w,
                    img->d_h,
                    1
                );
                
                if (new_img) {
                    vpx_img_free(new_img);
                }
            }
            
            // Try to get preview frame (may return NULL)
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
            
            // Try to get encoded data (should return NULL for decoder)
            iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process packet if available
                // This shouldn't happen with decoder, but handle it if it does
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&codec);
    }
    
    // Close the memory file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}