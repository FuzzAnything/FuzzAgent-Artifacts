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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_control_","vpx_codec_get_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000049_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    // Callback implementation - no-op for fuzzing
}

// Callback function for put_slice  
static void  id_000049_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice,
                         const vpx_image_rect_t *update) {
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)update;
    // Callback implementation - no-op for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
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
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    if (size < 1024) {
        return 0; // Need minimum data for meaningful fuzzing
    }

    // Initialize variables
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    const vpx_image_t *decoded_img = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    FILE *in_file = nullptr;
    
    // 1. Get VP8 encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
	return 0;
	}
	
    
    // 2. Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    (void)caps;
    
    // 3. Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Configure encoder with fuzz data
    enc_cfg.g_w = (data[0] % 64 + 1) * 16;
    enc_cfg.g_h = (data[1] % 64 + 1) * 16;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    
    // 4. Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // 5. Set encoder configuration (redundant but testing API)
    if (vpx_codec_enc_config_set(&encoder, &enc_cfg) != VPX_CODEC_OK) {
        // Continue anyway - this might fail for valid reasons
    }
    
    // 6. Register put_frame callback
    if (vpx_codec_register_put_frame_cb(&encoder,  id_000049_put_frame_cb, nullptr) != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    // 7. Register put_slice callback
    if (vpx_codec_register_put_slice_cb(&encoder,  id_000049_put_slice_cb, nullptr) != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    // 8. Allocate image for encoding
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image with fuzz data
    size_t y_plane_size = enc_cfg.g_w * enc_cfg.g_h;
    size_t uv_plane_size = y_plane_size / 4;
    
    if (size >= y_plane_size + 2 * uv_plane_size) {
        // Use fuzz data for image content
        memcpy(img->planes[0], data, y_plane_size);
        memcpy(img->planes[1], data + y_plane_size, uv_plane_size);
        memcpy(img->planes[2], data + y_plane_size + uv_plane_size, uv_plane_size);
    } else {
        // Fill with pattern if not enough data
        for (size_t i = 0; i < y_plane_size; i++) {
            img->planes[0][i] = (i % 256);
        }
        for (size_t i = 0; i < uv_plane_size; i++) {
            img->planes[1][i] = 128;
            img->planes[2][i] = 128;
        }
    }
    
    // 9. Encode the image
    if (vpx_codec_encode(&encoder, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7) != VPX_CODEC_OK) {
        // Continue to cleanup
    }
    
    // 10. Get preview frame
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
    (void)preview;
    
    // 11. Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if it's a frame
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // We have encoded data - try to decode it
            
            // 12. Get decoder interface
            vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
            if (!decoder_iface) break;
            
            // 13. Initialize decoder
            dec_cfg.threads = 1;
            dec_cfg.w = enc_cfg.g_w;
            dec_cfg.h = enc_cfg.g_h;
            
            if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
                // 14. Decode the encoded frame
                if (vpx_codec_decode(&decoder, (const uint8_t*)pkt->data.frame.buf, 
                                     pkt->data.frame.sz, nullptr, fuzz_int64_t_8) == VPX_CODEC_OK) {
                    
                    // 15. Try a control operation (example: flush decoder)
                    vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_UPDATES, nullptr);
                    
                    // 16. Get decoded frame
                    iter = nullptr;
                    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                        // Frame available - do nothing with it for fuzzing
                        (void)decoded_img;
                    }
                }
                
                // 17. Get global headers (for encoder context)
                global_headers = vpx_codec_get_global_headers(&encoder);
                (void)global_headers;
                
                // 18. Destroy decoder
                vpx_codec_destroy(&decoder);
            }
        }
    }
    
    // 12. Get error message (testing API)
    const char *error_msg = vpx_codec_error(&encoder);
    (void)error_msg;
    
    // 19. Destroy encoder
    vpx_codec_destroy(&encoder);
    
    // 20. Free image
    vpx_img_free(img);
    
    // Also try to decode the original fuzz data as VP8 stream
    vpx_codec_ctx_t direct_decoder = {0};
    vpx_codec_iface_t *direct_decoder_iface = vpx_codec_vp8_dx();
    
    if (direct_decoder_iface) {
        dec_cfg.threads = 1;
        dec_cfg.w = 0;
        dec_cfg.h = 0;
        
        if (vpx_codec_dec_init_ver(&direct_decoder, direct_decoder_iface, &dec_cfg, fuzz_int64_t_9, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Use fmemopen to create FILE* from fuzz data
            in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Read and decode in chunks
                uint8_t buffer[4096];
                size_t bytes_read;
                
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                    vpx_codec_decode(&direct_decoder, buffer, bytes_read, nullptr, fuzz_int64_t_10);
                    
                    // Get any available frames
                    iter = nullptr;
                    while ((decoded_img = vpx_codec_get_frame(&direct_decoder, &iter)) != nullptr) {
                        (void)decoded_img;
                    }
                }
                
                assert_file_closed(&in_file);;
            }
            
            vpx_codec_destroy(&direct_decoder);
        }
    }
    
    return 0;
}