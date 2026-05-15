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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 11
//<Quality> {"density":11,"unique_branches":{"vp9_release_frame_buffer":[[80,7,80,13,0,0,4,1]],"vp9_receive_compressed_data":[[443,7,443,43,0,0,4,1]],"flush_all_fb_on_key":[[2626,7,2626,34,0,0,4,0],[2626,38,2626,65,0,0,4,1]],"decoder_decode":[[357,7,357,23,0,0,4,1]],"check_resync":[[300,32,300,53,0,0,4,1],[301,8,301,30,0,0,4,1],[301,34,301,69,0,0,4,0]],"decode_block":[[930,9,930,36,0,0,4,0]],"read_uncompressed_header":[[2703,9,2703,25,0,0,4,0],[2779,7,2779,23,0,0,4,0]]},"library_calls":["vpx_codec_version","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_control_","vpx_img_wrap","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_control_","vpx_img_wrap","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_destroy"],"visited":1}
/*To create a fuzz driver that uses the specified libvpx APIs to process encoded video data, I'll implement a program that decodes a video stream and attempts to get a preview frame. Here's the step-by-step implementation:

*/



extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // 1. Get codec version (required API)
    int version = vpx_codec_version();
    
    // 2. Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();  // Use VP9 decoder
    
    // 3. Check decoder capabilities (required API)
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // 4. Initialize decoder with default configuration
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Get error string (required API)
        const char *error_msg = vpx_codec_error(&decoder);
        return 0;  // Early return if initialization fails
    }
    
    // 5. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Read and decode the video data
    // We'll read chunks and feed them to the decoder
    const size_t BUFFER_SIZE = 4096;
    uint8_t buffer[BUFFER_SIZE];
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img = NULL;
    
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, in_file);
        if (bytes_read == 0) break;
        
        // Decode the chunk
        err = vpx_codec_decode(&decoder, buffer, bytes_read, NULL, fuzz_int64_t_2);
        if (err != VPX_CODEC_OK) {
            // Get error details (required API)
            const char *error_msg = vpx_codec_error(&decoder);
        }
        
        // Get decoded frames
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
            // We've successfully decoded a frame
            // Now try to get a preview frame (required API)
            // Note: vpx_codec_get_preview_frame is for encoder, not decoder
            // We'll use control API to demonstrate usage
            int ctrl_id = 0;
            err = vpx_codec_control_(&decoder, ctrl_id);
            // Control might fail, but we call it as required
        }
        
        // Reset iterator for next decode call
        iter = NULL;
    }
    
    // 7. Create an image using vpx_img_wrap (required API)
    // We'll create a small test image using some of the fuzzer data
    const size_t IMG_SIZE = 100;
    if (size >= IMG_SIZE + sizeof(vpx_image_t)) {
        vpx_image_t image;
        unsigned char *img_data = (unsigned char *)malloc(IMG_SIZE);
        if (img_data) {
            // Copy some fuzzer data for image content
            memcpy(img_data, data, IMG_SIZE < size ? IMG_SIZE : size);
            
            // Wrap the image (required API)
            vpx_image_t *wrapped_img = vpx_img_wrap(&image, VPX_IMG_FMT_I420, 
                                                    fuzz_uint32_t_3, fuzz_uint32_t_4,  // dimensions
                                                    fuzz_uint32_t_5,        // stride alignment
                                                    img_data);
            
            if (wrapped_img) {
                // Free the image (required API)
                vpx_img_free(wrapped_img);
            }
            free(img_data);
        }
    }
    
    // 8. Try to get preview frame one more time (required API)
    // This will likely fail since we're using a decoder, but we call it anyway
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    if (preview_frame) {
        // If we somehow got a preview frame, we could process it here
    }
    
    // 9. Cleanup
    assert_file_closed(&in_file);;
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}