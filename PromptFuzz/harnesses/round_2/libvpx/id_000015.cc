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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*To create a fuzz driver that uses the specified libvpx APIs to decode and re-encode video data, I'll implement a fuzzer that:

1. Decodes the input video stream (VP8 or VP9)
2. Uses all the specified APIs during the encoding process
3. Properly handles resource allocation and cleanup

Here's the complete implementation:

*/


// Include necessary libvpx headers

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small to be valid
    if (size < 1) {
	return 0;
	}
	

    // Use vpx_codec_version_str() - API #3
    const char* version_str = vpx_codec_version_str();
    (void)version_str; // Use to avoid unused variable warning

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Try VP9 first, then VP8
    const vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0; // Can't initialize decoder
        }
    }

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Get stream info using vpx_codec_get_stream_info() - API #1
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    err = vpx_codec_get_stream_info(&decoder, &si);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Use vpx_codec_get_caps() - API #4
    vpx_codec_caps_t caps = vpx_codec_get_caps(vpx_codec_vp9_cx());
    (void)caps; // Use to avoid unused variable warning

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    // Use vpx_codec_enc_init_multi_ver() - API #7 with num_enc=1
    const vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Set encoder configuration based on decoded image
    cfg.g_w = decoded_img->d_w;
    cfg.g_h = decoded_img->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    // Initialize encoder with multi_ver (num_enc = 1)
    err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, 0, nullptr, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Use vpx_codec_enc_config_set() - API #2 to reconfigure encoder
    // Modify some settings first
    cfg.rc_target_bitrate = 500;
    vpx_codec_enc_config_set(&encoder, &cfg);

    // Use vpx_img_alloc() - API #8 to allocate an image
    vpx_image_t* encoded_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                           decoded_img->d_w, decoded_img->d_h, 16);
    if (!encoded_img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Copy decoded image to allocated image
    for (int plane = 0; plane < 3; ++plane) {
        uint8_t* src = decoded_img->planes[plane];
        uint8_t* dst = encoded_img->planes[plane];
        int src_stride = decoded_img->stride[plane];
        int dst_stride = encoded_img->stride[plane];
        int height = (plane == 0) ? decoded_img->d_h : (decoded_img->d_h + 1) / 2;
        
        for (int y = 0; y < height; ++y) {
            memcpy(dst, src, (plane == 0) ? decoded_img->d_w : (decoded_img->d_w + 1) / 2);
            src += src_stride;
            dst += dst_stride;
        }
    }

    // Use vpx_img_set_rect() - API #9
    // Set a rectangle to encode (smaller than full image)
    unsigned int rect_w = decoded_img->d_w > 32 ? 32 : decoded_img->d_w;
    unsigned int rect_h = decoded_img->d_h > 32 ? 32 : decoded_img->d_h;
    vpx_img_set_rect(encoded_img, 0, 0, rect_w, rect_h);

    // Prepare output buffer for encoded data
    std::vector<uint8_t> output_buffer(size * 2); // Allocate enough space
    vpx_fixed_buf_t output_buf;
    output_buf.buf = output_buffer.data();
    output_buf.sz = output_buffer.size();

    // Use vpx_codec_set_cx_data_buf() - API #5
    vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);

    // Encode the image
    err = vpx_codec_encode(&encoder, encoded_img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(encoded_img);
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Use vpx_codec_get_cx_data() - API #6 to get encoded packets
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process the packet if needed
        (void)pkt; // Use to avoid unused variable warning
    }

    // Cleanup
    vpx_img_free(encoded_img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}