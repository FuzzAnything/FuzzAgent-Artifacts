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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode VP8/VP9 video streams and process them:

*/


// Include libvpx headers

// Custom put_slice callback function
static void  id_000020_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    // Callback implementation - can be empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    vpx_codec_ctx_t codec_ctx = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    const vpx_image_t *preview_frame = NULL;
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t *codec_iface = NULL;
    
    // Determine which codec to try based on input data
    // For fuzzing, we'll try both in sequence if one fails
    int attempts = 0;
    vpx_codec_err_t res = VPX_CODEC_UNSUP_BITSTREAM;
    
    while (attempts < 2 && res != VPX_CODEC_OK) {
        if (attempts == 0) {
            codec_iface = vpx_codec_vp8_dx();
        } else {
            codec_iface = vpx_codec_vp9_dx();
        }
        
        // Initialize decoder configuration
        dec_cfg.threads = 1;
        dec_cfg.w = 0;
        dec_cfg.h = 0;
        
        // Initialize decoder
        res = vpx_codec_dec_init_ver(&codec_ctx, codec_iface, &dec_cfg, 
                                    fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            // Register put_slice callback
            vpx_codec_register_put_slice_cb(&codec_ctx,  id_000020_put_slice_callback, NULL);
            
            // Decode the input data
            vpx_codec_decode(&codec_ctx, data, (unsigned int)size, NULL, fuzz_int64_t_2);
            
            // Get stream information
            stream_info.sz = sizeof(stream_info);
            vpx_codec_get_stream_info(&codec_ctx, &stream_info);
            
            // Get preview frame (may return NULL)
            preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
            
            // Check for errors
            const char *error_msg = vpx_codec_error(&codec_ctx);
            if (error_msg && strlen(error_msg) > 0) {
                // Error occurred, but we continue fuzzing
            }
            
            // Clean up current decoder before trying next one if needed
            vpx_codec_destroy(&codec_ctx);
            
            // If we got here successfully, break out of the loop
            if (res == VPX_CODEC_OK) {
                break;
            }
        }
        
        attempts++;
    }
    
    // If we couldn't initialize any decoder, just return
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Clean up resources (already done in loop, but ensure cleanup)
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}