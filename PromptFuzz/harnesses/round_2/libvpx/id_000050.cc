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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"vp9_filter_block_plane_non420":[[1154,15,1154,33,0,0,4,1]],"ctrl_get_last_ref_updates":[[530,7,530,18,0,0,4,0],[531,9,531,25,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_img_wrap","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_img_wrap","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback for put_frame
static void  id_000050_dummy_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    (void)user_priv;
    (void)img;
    // Do nothing - just for API call
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Need minimum data for basic operations

    // 1. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Check codec capabilities
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning

    // 3. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    // Use error string API
    const char* err_str = vpx_codec_err_to_string(err);
    (void)err_str;

    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000050_dummy_put_frame_cb, NULL);
    (void)err;

    // 5. Peek stream info to get dimensions
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                      size > 4096 ? 4096 : size, &si);
    if (err == VPX_CODEC_OK && si.w > 0 && si.h > 0) {
        // 6. Get stream info from decoder context
        vpx_codec_stream_info_t si2;
        memset(&si2, 0, sizeof(si2));
        si2.sz = sizeof(si2);
        vpx_codec_get_stream_info(&decoder, &si2);

        // 7. Create image using vpx_img_wrap for preview frame
        unsigned int width = si.w;
        unsigned int height = si.h;
        
        // Allocate buffer for image data
        size_t img_size = width * height * 3 / 2; // I420 format
        unsigned char* img_data = (unsigned char*)malloc(img_size);
        if (img_data) {
            vpx_image_t img;
            vpx_image_t* wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, 
                                                     width, height, 1, img_data);
            
            if (wrapped_img) {
                // Set some image properties
                wrapped_img->bit_depth = 8;
                
                // 8. Try to get preview frame (though decoder may not have one)
                // This is more applicable to encoder, but we call it anyway
                const vpx_image_t* preview = vpx_codec_get_preview_frame(&decoder);
                (void)preview;
                
                // 9. Use control API
                int ctrl_id = VP8D_GET_LAST_REF_UPDATES;
                vpx_codec_control_(&decoder, ctrl_id);
                
                // 10. Try to get global headers (though decoder may return NULL)
                vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&decoder);
                if (headers) {
                    free(headers->buf);
                    free(headers);
                }
                
                // Clean up image
                free(img_data);
            }
        }
    }

    // 11. Decode the input data
    err = vpx_codec_decode(&decoder, data, size, NULL, 0);
    
    // Get error string again
    err_str = vpx_codec_err_to_string(err);
    (void)err_str;

    // 12. Iterate through decoded frames
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t* img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Process frame if needed
        (void)img;
    }

    // 13. Cleanup
    assert_file_closed(&in_file);;
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}