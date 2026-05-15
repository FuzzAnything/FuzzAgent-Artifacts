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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode VP9 video data:

*/


// Include libvpx headers
extern "C" {
}

// Callback function for put_slice
static void  id_000052_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *rect, 
                         const vpx_image_rect_t *rect2) {
    // Callback implementation
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

// Callback function for put_frame
static void  id_000052_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Callback implementation
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get decoder interface and check capabilities
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Use variable
    
    // 3. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                 &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Register put_slice callback for decoder
    vpx_codec_register_put_slice_cb(&decoder,  id_000052_put_slice_cb, nullptr);
    
    // 5. Decode input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Get decoder interface for encoder initialization
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // 7. Get default encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder settings
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_error_resilient = 0;
    
    // 8. Initialize encoder
    vpx_codec_ctx_t encoder;
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, 
                                VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Allocate image for encoding
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_5);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 10. Set a rectangular region in the image (using vpx_img_set_rect)
    vpx_img_set_rect(img, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8, fuzz_uint32_t_9);
    
    // Fill image with synthetic data (using input data for fuzzing)
    size_t y_plane_size = img->stride[0] * img->d_h;
    size_t uv_plane_size = img->stride[1] * (img->d_h / 2);
    
    // Use input data to fill planes (with bounds checking)
    if (y_plane_size + uv_plane_size * 2 <= size) {
        memcpy(img->planes[0], data, y_plane_size);
        memcpy(img->planes[1], data + y_plane_size, uv_plane_size);
        memcpy(img->planes[2], data + y_plane_size + uv_plane_size, uv_plane_size);
    } else {
        // Fill with pattern if not enough data
        for (size_t i = 0; i < y_plane_size; ++i) {
            img->planes[0][i] = (uint8_t)(i % 256);
        }
        for (size_t i = 0; i < uv_plane_size; ++i) {
            img->planes[1][i] = 128;
            img->planes[2][i] = 128;
        }
    }
    
    // 11. Register put_frame callback for encoder
    vpx_codec_register_put_frame_cb(&encoder,  id_000052_put_frame_cb, nullptr);
    
    // 12. Encode the image
    err = vpx_codec_encode(&encoder, img, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
    
    // 13. Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write encoded data to output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                assert_file_closed(&out_file);;
            }
            break;
        }
    }
    
    // 14. Clean up resources
    vpx_img_free(img);  // Free allocated image
    vpx_codec_destroy(&encoder);  // Destroy encoder context
    vpx_codec_destroy(&decoder);  // Destroy decoder context
    assert_file_closed(&in_file);;  // Close input file
    
    assert_file_closed(&in_file);
	return 0;
}