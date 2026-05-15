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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve the event of initializing an encoder and getting its preview frame:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // Use input data to create a FILE* for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create and configure encoder
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    
    // Get VP9 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Get codec interface name (required API call)
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Prevent unused variable warning
    
    // Initialize encoder configuration with defaults
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Use input size to influence encoder configuration
    cfg.g_w = 128 + (size % 512);  // Width based on input size
    cfg.g_h = 128 + ((size >> 8) % 512);  // Height based on input size
    cfg.rc_target_bitrate = 1000 + (size % 4000);
    
    // Initialize encoder (required API call)
    if (vpx_codec_enc_init_ver(&codec_ctx, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Allocate image buffer for encoder input (required API call)
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Try to read some data from input file to populate the image
    // We'll use the input data to partially fill the Y plane
    size_t bytes_to_read = img->stride[0] * img->h;
    if (bytes_to_read > 0) {
        size_t read_size = fread(img->planes[0], 1, 
                                 (size < bytes_to_read) ? size : bytes_to_read, in_file);
        (void)read_size; // Prevent unused variable warning
    }

    // Step 4: Encode a frame
    vpx_codec_encode(&codec_ctx, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);

    // Step 5: Get global headers (required API call)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx);
    (void)global_headers; // Prevent unused variable warning

    // Step 6: Try to get preview frame (required API call)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    (void)preview_frame; // Prevent unused variable warning

    // Step 7: Clean up encoder context
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        // Just iterate through packets to ensure proper cleanup
        (void)pkt;
    }

    // Step 8: Clean up resources
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}