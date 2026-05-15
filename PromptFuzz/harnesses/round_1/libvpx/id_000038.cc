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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" {
}

// Callback function for put_frame
static void  id_000038_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Empty callback for demonstration
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Open memory file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open file for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Try both VP8 and VP9 codecs
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // First try VP9
    decoder_iface = vpx_codec_vp9_dx();
    encoder_iface = vpx_codec_vp9_cx();
    
    if (!decoder_iface || !encoder_iface) {
        // Try VP8
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    if (!decoder_iface || !encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, std::min(size, (size_t)1000), &stream_info);
    
    // 2. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(
        &decoder, data, std::min(size, (size_t)1000), nullptr, 0);
    
    // 4. Get decoded frame
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
    
    // 5. Use vpx_img_wrap to wrap image data
    const size_t img_data_size = decoded_img->d_w * decoded_img->d_h * 3 / 2;
    unsigned char *img_data = (unsigned char *)malloc(img_data_size);
    if (!img_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill with some data
    memset(img_data, 0x80, img_data_size);
    
    vpx_image_t wrapped_img;
    vpx_image_t *wrapped_result = vpx_img_wrap(
        &wrapped_img, VPX_IMG_FMT_I420,
        decoded_img->d_w, decoded_img->d_h,
        1, img_data);
    
    // 6. Use vpx_img_set_rect to set a rectangle region
    if (decoded_img->d_w > 10 && decoded_img->d_h > 10) {
        vpx_img_set_rect(&wrapped_img, 5, 5, 
                        decoded_img->d_w - 10, decoded_img->d_h - 10);
    }
    
    // 7. Initialize multiple encoders using vpx_codec_enc_init_multi_ver
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
        encoder_iface, &enc_cfg, 0);
    
    if (cfg_err != VPX_CODEC_OK) {
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    vpx_rational_t dsf[1];
    dsf[0].num = 1;
    dsf[0].den = 1;
    
    vpx_codec_err_t enc_init_err = vpx_codec_enc_init_multi_ver(
        &encoder, encoder_iface, &enc_cfg, 1, 0, dsf, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_err != VPX_CODEC_OK) {
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Use vpx_codec_enc_config_set to update encoder configuration
    enc_cfg.rc_target_bitrate = 500;  // Change bitrate
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // 9. Register put frame callback
    vpx_codec_register_put_frame_cb(&encoder,  id_000038_put_frame_callback, nullptr);
    
    // 10. Use vpx_codec_set_cx_data_buf to set output buffer
    vpx_fixed_buf_t output_buf;
    output_buf.sz = 1024 * 1024;  // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    if (output_buf.buf) {
        vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
    }
    
    // 11. Use vpx_codec_get_stream_info
    vpx_codec_stream_info_t decoder_stream_info;
    memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
    decoder_stream_info.sz = sizeof(decoder_stream_info);
    vpx_codec_get_stream_info(&decoder, &decoder_stream_info);
    
    // 12. Encode the wrapped image
    vpx_codec_encode(&encoder, &wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    if (output_buf.buf) free(output_buf.buf);
    free(img_data);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}