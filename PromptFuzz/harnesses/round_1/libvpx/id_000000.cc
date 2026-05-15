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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.3333335, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"],"visited":2}
/*To achieve the event of decoding a VP9 video stream using libvpx, we need to create a fuzz driver that processes the input data as a compressed video stream, decodes it, and handles the decoded frames. Here's a step-by-step implementation that uses all the specified APIs at least once:

*/


// Include libvpx headers

// Callback function for put_frame
static void  id_000000_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Simple callback that does nothing for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for any meaningful processing
    if (size < 4) {
	return 0;
	}
	
    
    // Use the input data directly
    // We'll treat it as compressed VP9 video data
    
    // 1. Get VP9 decoder interface
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_dx();
    if (!vp9_iface) {
	return 0;
	}
	
    
    // 2. Get VP8 encoder interface (just to use the API, not actually needed for decoding)
    vpx_codec_iface_t *vp8_enc_iface = vpx_codec_vp8_cx();
    if (!vp8_enc_iface) {
	return 0;
	}
	
    
    // 3. Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(vp9_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // 4. Peek at stream info to get basic information
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        vp9_iface, data, (unsigned int)size, &si);
    
    // We'll continue even if peek fails, as the data might be malformed
    
    // 5. Create and initialize decoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(
        &codec, vp9_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_err != VPX_CODEC_OK) {
        // Decoder initialization failed
        return 0;
    }
    
    // 6. Allocate an image (though we won't use it directly for decoding)
    // This demonstrates vpx_img_alloc usage
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                      si.w > 0 ? si.w : 64, 
                                      si.h > 0 ? si.h : 64, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // 7. Set up a buffer for compressed data
    // We'll use the input data as is, but wrap it in vpx_fixed_buf_t
    vpx_fixed_buf_t buf;
    buf.buf = (void*)data;
    buf.sz = size;
    
    // Use vpx_codec_set_cx_data_buf - though typically used for encoding,
    // we use it here to demonstrate the API
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    vpx_codec_err_t set_buf_err = vpx_codec_set_cx_data_buf(
        &codec, &buf, pad_before, pad_after);
    (void)set_buf_err; // May fail in decoding context, but that's OK for fuzzing
    
    // 8. Register put_frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(
        &codec,  id_000000_put_frame_callback, NULL);
    (void)reg_err; // May fail or succeed, continue anyway
    
    // 9. Decode the input data
    // We'll decode in a loop to handle potential partial data
    vpx_codec_err_t decode_err = vpx_codec_decode(
        &codec, data, (unsigned int)size, NULL, 0);
    
    // 10. Get stream info from decoder context (if decode was successful)
    if (decode_err == VPX_CODEC_OK) {
        vpx_codec_stream_info_t si_from_ctx;
        memset(&si_from_ctx, 0, sizeof(si_from_ctx));
        si_from_ctx.sz = sizeof(si_from_ctx);
        
        vpx_codec_err_t get_info_err = vpx_codec_get_stream_info(
            &codec, &si_from_ctx);
        (void)get_info_err; // May fail, but that's OK
    }
    
    // 11. Try to get decoded frames
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
        // Frame successfully decoded - we don't need to do anything with it
        // for fuzzing purposes
        (void)decoded_img;
    }
    
    // 12. Clean up resources
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}