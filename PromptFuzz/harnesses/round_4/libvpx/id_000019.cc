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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 13
//<Quality> {"density":21,"unique_branches":{"img_alloc_helper":[[72,5,72,26,0,0,4,1],[81,5,81,28,0,0,4,0],[89,5,89,26,0,0,4,1],[94,5,94,12,0,0,4,0],[98,5,98,26,0,0,4,1],[104,5,104,12,0,0,4,0],[124,7,124,39,0,0,4,0],[128,7,128,39,0,0,4,0],[155,20,155,52,0,0,4,0]],"is_valid_img_fmt":[[23,5,23,26,0,0,4,1],[30,5,30,28,0,0,4,0]],"vp9_set_row_mt":[[6857,35,6857,57,0,0,4,0]],"vpx_img_set_rect":[[205,11,205,48,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Event: Decode input video data, then re-encode it with a put_frame callback
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION)) {
        return 0; // Initialization failed
    }
    
    // 2. Decode input data
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    
    // 3. Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        return 0; // No frame decoded
    }
    
    // 4. Allocate image for encoding
    vpx_image_t enc_img;
    vpx_img_alloc(&enc_img, decoded_img->fmt, decoded_img->d_w, decoded_img->d_h, 1);
    
    // Copy decoded image data to allocated image
    for (int plane = 0; plane < 3; ++plane) {
        int width = (plane > 0) ? (decoded_img->d_w >> decoded_img->x_chroma_shift)
                                : decoded_img->d_w;
        int height = (plane > 0) ? (decoded_img->d_h >> decoded_img->y_chroma_shift)
                                 : decoded_img->d_h;
        
        for (int y = 0; y < height; ++y) {
            memcpy(enc_img.planes[plane] + y * enc_img.stride[plane],
                   decoded_img->planes[plane] + y * decoded_img->stride[plane],
                   width);
        }
    }
    
    // 5. Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3)) {
        vpx_img_free(&enc_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    cfg.g_w = decoded_img->d_w;
    cfg.g_h = decoded_img->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION)) {
        vpx_img_free(&enc_img);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 6. Set encoder configuration
    vpx_codec_enc_config_set(&encoder, &cfg);
    
    // 7. Define and register put_frame callback
    struct CallbackData {
        FILE* output_file;
    };
    
    auto put_frame_callback = [](void* user_priv, const vpx_image_t* img) {
        // Dummy callback - in real usage would process the frame
        (void)user_priv;
        (void)img;
    };
    
    CallbackData cb_data;
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        cb_data.output_file = out_file;
        vpx_codec_register_put_frame_cb(&encoder, put_frame_callback, &cb_data);
    }
    
    // 8. Encode the frame multiple times with different parameters
    // Use some data from input to vary encoding parameters
    int pts_increment = 1000000 / 30; // 30 fps
    vpx_codec_pts_t pts = 0;
    
    // Encode a few frames using different flags based on input data
    for (int i = 0; i < 3 && i * 10 < (int)size; ++i) {
        vpx_enc_frame_flags_t flags = 0;
        if (data[i * 10] & 0x01) {
            flags |= VPX_EFLAG_FORCE_KF; // Force keyframe
        }
        
        // Encode with deadline based on input data
        long deadline = (data[i * 10 + 1] & 0x01) ? VPX_DL_REALTIME : VPX_DL_GOOD_QUALITY;
        
        vpx_codec_encode(&encoder, &enc_img, fuzz_int64_t_5, pts_increment, fuzz_int64_t_6, deadline);
        pts += pts_increment;
        
        // Get encoded data
        const vpx_codec_cx_pkt_t* pkt;
        vpx_codec_iter_t enc_iter = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT && out_file) {
                // Write encoded data to file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // 9. Flush encoder
    vpx_codec_encode(&encoder, nullptr, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
    
    // 10. Cleanup
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    vpx_codec_destroy(&encoder);
    vpx_img_free(&enc_img);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&out_file);
	return 0;
}