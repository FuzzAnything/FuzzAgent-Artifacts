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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_img_alloc","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_control_","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to encode and decode video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input FILE from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t *img = nullptr;
    vpx_codec_stream_info_t si = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_rational_t dsf = {1, 1};  // Downsampling factor
    int num_enc = 1;
    vpx_codec_flags_t flags = 0;
    int ver = VPX_ENCODER_ABI_VERSION;

    // 1. First try to peek stream info to determine if this is VP8 or VP9 data
    si.sz = sizeof(si);
    
    // Try VP9 first
    vpx_codec_err_t err = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &si);
    vpx_codec_iface_t *codec_iface = nullptr;
    
    if (err == VPX_CODEC_OK) {
        codec_iface = vpx_codec_vp9_dx();
    } else {
        // Try VP8
        err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, &si);
        if (err == VPX_CODEC_OK) {
            codec_iface = vpx_codec_vp8_dx();
        }
    }
    
    if (!codec_iface) {
        // Not valid VP8/VP9 data
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Initialize decoder
    err = vpx_codec_dec_init_ver(&decoder, codec_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Get stream info again after decoding
    vpx_codec_stream_info_t si2 = {0};
    si2.sz = sizeof(si2);
    vpx_codec_get_stream_info(&decoder, &si2);

    // 5. Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Determine appropriate encoder interface
    vpx_codec_iface_t *encoder_iface = nullptr;
    if (codec_iface == vpx_codec_vp9_dx()) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        encoder_iface = vpx_codec_vp8_cx();
    }

    // 7. Allocate image for encoding
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, decoded_img->d_w, decoded_img->d_h, 16);
    if (!img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Copy decoded image data
    for (int plane = 0; plane < 3; ++plane) {
        uint8_t *src = decoded_img->planes[plane];
        uint8_t *dst = img->planes[plane];
        int src_stride = decoded_img->stride[plane];
        int dst_stride = img->stride[plane];
        int height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
        
        for (int y = 0; y < height; ++y) {
            memcpy(dst, src, (plane == 0) ? decoded_img->d_w : decoded_img->d_w / 2);
            src += src_stride;
            dst += dst_stride;
        }
    }

    // 8. Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoder settings
    cfg.g_w = decoded_img->d_w;
    cfg.g_h = decoded_img->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    // 9. Initialize encoder with multi-version API
    err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, num_enc, flags, &dsf, ver);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 10. Set encoder configuration
    err = vpx_codec_enc_config_set(&encoder, &cfg);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 11. Use codec control (set CPU used to fastest)
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);

    // 12. Encode the image
    err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 13. Try to get preview frame (if available)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
    if (preview_frame) {
        // Preview frame available
    }

    // 14. Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter))) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Clean up
    vpx_codec_destroy(&encoder);
    vpx_img_free(img);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}