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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_error","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000021_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    // This is a minimal callback that does nothing
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data for potential streaming scenarios
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for potential writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *decoder = nullptr;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_stream_info_t si = {0};
    
    // Determine which decoder to use based on input data
    // Try VP9 first, fall back to VP8
    decoder = vpx_codec_vp9_dx();
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&codec, decoder, &cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // If VP9 fails, try VP8
    if (err != VPX_CODEC_OK) {
        decoder = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&codec, decoder, &cfg, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
    }
    
    if (err != VPX_CODEC_OK) {
        // Get error message
        const char *error_msg = vpx_codec_error(&codec);
        (void)error_msg; // Use error_msg to avoid unused variable warning
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put_slice callback
    err = vpx_codec_register_put_slice_cb(&codec,  id_000021_put_slice_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        // Get error message
        const char *error_msg = vpx_codec_error(&codec);
        (void)error_msg;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&codec, data, size, nullptr, fuzz_int64_t_3);
    
    if (err == VPX_CODEC_OK) {
        // Get stream information
        si.sz = sizeof(si);
        err = vpx_codec_get_stream_info(&codec, &si);
        
        if (err == VPX_CODEC_OK) {
            // Try to get preview frame (though this is typically for encoders)
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
            (void)preview; // Use preview to avoid unused variable warning
            
            // Write some information to output file
            if (out_file && si.w > 0 && si.h > 0) {
                fprintf(out_file, "Stream info: %ux%u, keyframe: %u\n", 
                        si.w, si.h, si.is_kf);
            }
        }
    }
    
    // Get final error message
    const char *final_error = vpx_codec_error(&codec);
    (void)final_error;
    
    // Clean up
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}