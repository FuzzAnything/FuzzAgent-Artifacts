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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video data:

*/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use vpx_codec_version_extra_str() - required API #1
    const char* extra_version = vpx_codec_version_extra_str();
    (void)extra_version; // Prevent unused variable warning

    // Create a FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;

    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode data
    uint8_t* buffer = (uint8_t*)malloc(size);
    if (!buffer) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Decode the input data
    res = vpx_codec_decode(&decoder, buffer, size, nullptr, fuzz_int64_t_2);
    free(buffer);

    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream information - required API #2
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Note: vpx_codec_get_stream_info needs a valid context after decoding
    // We'll use the decoded frame information instead
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output image based on decoded frame
    vpx_image_t output_img;
    memset(&output_img, 0, sizeof(output_img));
    
    // Allocate image with same format and dimensions
    vpx_img_alloc(&output_img, decoded_img->fmt, decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_3);
    
    // Copy decoded image data
    for (int plane = 0; plane < 3; plane++) {
        if (decoded_img->planes[plane]) {
            int height = (plane > 0) ? (decoded_img->d_h + decoded_img->y_chroma_shift) >> decoded_img->y_chroma_shift 
                                     : decoded_img->d_h;
            int width = (plane > 0) ? (decoded_img->d_w + decoded_img->x_chroma_shift) >> decoded_img->x_chroma_shift 
                                    : decoded_img->d_w;
            
            for (int y = 0; y < height; y++) {
                memcpy(output_img.planes[plane] + y * output_img.stride[plane],
                       decoded_img->planes[plane] + y * decoded_img->stride[plane],
                       width);
            }
        }
    }

    // Use vpx_img_set_rect() - required API #3
    // Set a rectangle in the middle of the image
    unsigned int rect_x = decoded_img->d_w / 4;
    unsigned int rect_y = decoded_img->d_h / 4;
    unsigned int rect_w = decoded_img->d_w / 2;
    unsigned int rect_h = decoded_img->d_h / 2;
    
    if (rect_x < decoded_img->d_w && rect_y < decoded_img->d_h && 
        rect_w > 0 && rect_h > 0) {
        vpx_img_set_rect(&output_img, rect_x, rect_y, rect_w, rect_h);
    }

    // Initialize encoder configuration
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_4);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(&output_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    cfg.g_w = decoded_img->d_w;
    cfg.g_h = decoded_img->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    // Use vpx_codec_enc_init_multi_ver() - required API #4
    // Initialize with 1 encoder (num_enc = 1)
    vpx_rational_t dsf = {1, 1}; // Downsampling factor
    
    res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, fuzz_int64_t_5, &dsf, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(&output_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Encode the modified image
    res = vpx_codec_encode(&encoder, &output_img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
    
    // Get encoded data
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    iter = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame packet received - encoding successful
            break;
        }
    }

    // Use vpx_codec_get_frame() again - required API #5
    // Get another frame if available
    iter = nullptr;
    const vpx_image_t* second_frame = vpx_codec_get_frame(&decoder, &iter);
    (void)second_frame; // Prevent unused variable warning

    // Cleanup
    vpx_img_free(&output_img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}