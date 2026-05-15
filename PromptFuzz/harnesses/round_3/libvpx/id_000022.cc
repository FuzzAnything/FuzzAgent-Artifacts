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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"vpx_codec_err_to_string":[[39,5,39,32,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000022_put_slice_callback(void* user_priv, 
                               const vpx_image_t* img,
                               const vpx_image_rect_t* slice,
                               const vpx_image_rect_t* clip) {
    // This is a minimal callback that does nothing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clip;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output (though we won't actually write in this fuzzer)
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t* decoder_iface = nullptr;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_err_t res;
    
    // Try both VP8 and VP9 decoders based on input data
    // Use first byte to decide which codec to try
    if ((data[0] & 0x1) == 0) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    
    // Initialize decoder with version 4 (VPX_DECODER_ABI_VERSION)
    res = vpx_codec_dec_init_ver(&codec_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, fuzz_int32_t_2);
    
    // Get error message if initialization failed
    const char* error_msg = vpx_codec_error(&codec_ctx);
    (void)error_msg; // Use error_msg to avoid unused variable warning
    
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put_slice callback
    res = vpx_codec_register_put_slice_cb(&codec_ctx,  id_000022_put_slice_callback, nullptr);
    // Note: This may fail for some codecs, but we still continue
    
    // Read data from file to simulate actual usage
    uint8_t* buffer = (uint8_t*)malloc(size);
    if (!buffer) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, size, in_file);
    
    // Try to get stream information
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // First try peek_stream_info (similar to get_stream_info but doesn't require decoder)
    vpx_codec_peek_stream_info(decoder_iface, buffer, size, &stream_info);
    
    // Then try get_stream_info on the decoder context
    res = vpx_codec_get_stream_info(&codec_ctx, &stream_info);
    
    // Decode the frame
    res = vpx_codec_decode(&codec_ctx, buffer, size, nullptr, fuzz_int64_t_3);
    
    // Try to get preview frame (this may return NULL for decoders)
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    (void)preview_frame; // Use preview_frame to avoid unused variable warning
    
    // Iterate through decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* img;
    while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // Do nothing with the image in fuzzing context
        (void)img;
    }
    
    // Cleanup
    free(buffer);
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}