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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 100
//<Quality> {"density":1,"unique_branches":{"build_y_mask":[[755,7,755,15,0,0,4,1]],"vp9_setup_mask":[[939,45,939,73,0,0,4,0]],"vp9_update_noise_estimate":[[112,44,112,61,0,0,4,0]],"vp9_initialize_rd_consts":[[428,9,428,61,0,0,4,0]],"vpx_lpf_horizontal_8_dual_sse2":[[1289,14,1289,21,0,0,4,0],[1289,14,1289,21,0,0,4,1]],"img_alloc_helper":[[80,5,80,28,0,0,4,0],[94,5,94,12,0,0,4,0],[103,5,103,28,0,0,4,0],[124,7,124,39,0,0,4,0],[128,7,128,39,0,0,4,0],[155,20,155,52,0,0,4,0]],"block_rd_txfm":[[821,7,821,41,0,0,4,0]],"encode_nonrd_sb_row":[[5304,7,5304,31,0,0,4,1],[5328,7,5328,14,0,0,4,0],[5335,14,5335,42,0,0,4,0],[5335,46,5335,69,0,0,4,1],[5352,15,5352,38,0,0,4,0]],"vp9_set_variance_partition_thresholds":[[642,7,642,55,0,0,4,0],[643,7,643,55,0,0,4,1]],"filter_selectively_horiz":[[475,13,475,32,0,0,4,0],[483,15,483,38,0,0,4,1],[488,17,488,33,0,0,4,1],[491,22,491,38,0,0,4,1],[503,18,503,30,0,0,4,1]],"transpose":[[1662,12,1662,43,0,0,4,0],[1662,12,1662,43,0,0,4,1]],"is_valid_img_fmt":[[31,5,31,28,0,0,4,0]],"vp9_idct4x4_add":[[121,7,121,14,0,0,4,0]],"update_state_rt":[[24,23,24,34,1,0,4,0]],"vp9_set_contexts":[[95,7,95,14,0,0,4,1],[110,7,110,14,0,0,4,1]],"pack_mb_tokens":[[153,12,153,34,0,0,4,0],[156,11,156,20,0,0,4,1],[156,24,156,46,0,0,4,1],[169,11,169,25,0,0,4,0],[181,13,181,33,0,0,4,1]],"set_rt_speed_feature_framesize_independent":[[662,7,662,17,0,0,4,1]],"vp9_rd_pick_intra_mode_sb":[[3258,17,3258,24,0,0,4,1]],"vp9_filter_block_plane_ss11":[[1342,36,1342,60,0,0,4,1]],"choose_partitioning":[[1311,44,1311,61,0,0,4,0],[1552,7,1552,14,0,0,4,0],[1552,18,1552,46,0,0,4,0],[1761,7,1761,10,0,0,4,0],[440,9,440,16,40,0,4,1]],"d117_predictor":[[86,15,86,21,0,0,4,0],[86,15,86,21,0,0,4,1],[91,15,91,21,0,0,4,0],[91,15,91,21,0,0,4,1],[96,15,96,21,0,0,4,0],[96,15,96,21,0,0,4,1],[100,15,100,21,0,0,4,0],[100,15,100,21,0,0,4,1],[101,17,101,23,0,0,4,0],[101,17,101,23,0,0,4,1]],"vp9_idct8x8_add":[[142,7,142,15,0,0,4,1],[145,12,145,21,0,0,4,0]],"get_skip_encode_frame":[[5384,15,5384,39,0,0,4,0],[5384,15,5384,39,0,0,4,1],[5389,10,5389,42,0,0,4,1]],"conditional_skipintra":[[1046,28,1046,53,0,0,4,1],[1049,27,1049,52,0,0,4,1],[1050,7,1050,34,0,0,4,1],[1052,28,1052,53,0,0,4,1],[1053,7,1053,34,0,0,4,1],[1055,28,1055,53,0,0,4,1]],"build_intra_predictors":[[301,9,301,23,0,0,4,0],[302,11,302,36,0,0,4,1],[315,21,315,27,0,0,4,0],[315,21,315,27,0,0,4,1],[343,23,343,37,0,0,4,0],[352,9,352,21,0,0,4,0],[354,11,354,35,0,0,4,1],[379,13,379,20,0,0,4,1],[383,15,383,22,0,0,4,1],[389,23,389,37,0,0,4,0],[389,23,389,37,0,0,4,1]],"encode_frame_internal":[[5751,7,5751,25,0,0,4,0]],"filter_selectively_vert_row2":[[310,8,310,12,0,0,4,0],[311,9,311,24,0,0,4,0],[311,9,311,24,0,0,4,1],[317,11,317,32,0,0,4,1],[328,11,328,30,0,0,4,0],[328,11,328,30,0,0,4,1],[329,13,329,46,0,0,4,0],[340,11,340,30,0,0,4,1],[352,11,352,34,0,0,4,0],[352,11,352,34,0,0,4,1],[353,13,353,50,0,0,4,1]],"tokenize_b":[[391,12,391,14,0,0,4,0]],"set_rt_speed_feature_framesize_dependent":[[24,23,24,34,1,0,4,1],[24,23,24,34,2,0,4,1],[24,23,24,34,3,0,4,1]],"set_vt_partitioning":[[510,9,510,47,0,0,4,0],[511,9,511,48,0,0,4,1],[531,9,531,47,0,0,4,0],[535,11,535,58,0,0,4,0],[536,11,536,58,0,0,4,0],[537,11,537,71,0,0,4,0]],"duplicate_mode_info_in_sb":[[24,23,24,34,1,0,4,0]]},"library_calls":["vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data"],"critical_calls":["vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_get_cx_data","vpx_codec_err_to_string","vpx_codec_encode","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_img_set_rect","vpx_img_alloc","vpx_img_alloc","vpx_codec_get_frame","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_vp9_dx"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and then re-encode VP9 video data:

*/


// Include libvpx headers

// Callback for put_frame
static void  id_000064_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This callback can be left empty for fuzzing
    (void)user_priv;
    (void)img;
}

// Callback for put_slice  
static void  id_000064_put_slice_callback(void* user_priv, const vpx_image_t* img,
                              const vpx_image_rect_t* rect,
                              const vpx_image_rect_t* rect2) {
    // This callback can be left empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Track all allocated resources for cleanup
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t* global_headers = nullptr;
    FILE* out_file = nullptr;
    
    // Cleanup function to ensure all resources are released
    auto cleanup = [&]() {
        if (global_headers) {
            // vpx_codec_get_global_headers returns a pointer to internal data
            // We shouldn't free it directly
        }
        if (img) {
            vpx_img_free(img);
        }
        if (vpx_codec_destroy(&decoder) != VPX_CODEC_OK) {
            // Log error but continue cleanup
        }
        if (vpx_codec_destroy(&encoder) != VPX_CODEC_OK) {
            // Log error but continue cleanup
        }
        if (out_file) {
            assert_file_closed(&out_file);;
        }
    };

    try {
        // 1. Get decoder interface
        vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
        if (!decoder_iface) {
            cleanup();
            return 0;
        }

        // 2. Get decoder capabilities
        vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
        (void)decoder_caps; // Use in condition to avoid unused warning

        // 3. Get interface name
        const char* iface_name = vpx_codec_iface_name(decoder_iface);
        (void)iface_name; // Use in condition to avoid unused warning

        // 4. Peek at stream info to get width/height
        stream_info.sz = sizeof(stream_info);
        vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
            decoder_iface, data, static_cast<unsigned int>(size), &stream_info);
        
        if (peek_err != VPX_CODEC_OK) {
            // Could not get stream info, try default initialization
            stream_info.w = 128;
            stream_info.h = 128;
        }

        // 5. Initialize decoder with default config
        vpx_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 1;
        dec_cfg.w = stream_info.w;
        dec_cfg.h = stream_info.h;
        
        vpx_codec_err_t init_err = vpx_codec_dec_init_ver(
            &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (init_err != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_err_to_string(init_err);
            (void)error_str; // Use in condition to avoid unused warning
            cleanup();
            return 0;
        }

        // 6. Decode the input data
        vpx_codec_err_t decode_err = vpx_codec_decode(
            &decoder, data, static_cast<unsigned int>(size), nullptr, 0);
        
        if (decode_err != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_err_to_string(decode_err);
            (void)error_str; // Use in condition to avoid unused warning
            cleanup();
            return 0;
        }

        // 7. Get stream info from decoder context
        vpx_codec_stream_info_t dec_stream_info = {0};
        dec_stream_info.sz = sizeof(dec_stream_info);
        vpx_codec_err_t stream_info_err = vpx_codec_get_stream_info(
            &decoder, &dec_stream_info);
        
        if (stream_info_err != VPX_CODEC_OK) {
            // Continue anyway
        }

        // 8. Get decoded frame
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
        
        if (!decoded_img) {
            // No frame decoded, create a dummy image for encoding
            img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                               stream_info.w, stream_info.h, 1);
            if (!img) {
                cleanup();
                return 0;
            }
            
            // Fill with some data
            for (int plane = 0; plane < 3; ++plane) {
                unsigned char* plane_ptr = img->planes[plane];
                int stride = img->stride[plane];
                int height = (plane == 0) ? img->d_h : img->d_h / 2;
                int width = (plane == 0) ? img->d_w : img->d_w / 2;
                
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        plane_ptr[y * stride + x] = static_cast<unsigned char>(
                            (x + y) % 256);
                    }
                }
            }
        } else {
            // Use the decoded image
            img = vpx_img_alloc(nullptr, decoded_img->fmt, 
                               decoded_img->d_w, decoded_img->d_h, 1);
            if (!img) {
                cleanup();
                return 0;
            }
            
            // Copy image data
            for (int plane = 0; plane < 3; ++plane) {
                if (decoded_img->planes[plane]) {
                    int height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
                    int width = (plane == 0) ? decoded_img->d_w : decoded_img->d_w / 2;
                    for (int y = 0; y < height; ++y) {
                        memcpy(img->planes[plane] + y * img->stride[plane],
                               decoded_img->planes[plane] + y * decoded_img->stride[plane],
                               width);
                    }
                }
            }
        }

        // 9. Set image rectangle
        unsigned int rect_x = stream_info.w > 16 ? 8 : 0;
        unsigned int rect_y = stream_info.h > 16 ? 8 : 0;
        unsigned int rect_w = stream_info.w > 32 ? 16 : stream_info.w / 2;
        unsigned int rect_h = stream_info.h > 32 ? 16 : stream_info.h / 2;
        
        if (rect_w > 0 && rect_h > 0) {
            vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);
        }

        // 10. Get encoder interface
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        if (!encoder_iface) {
            cleanup();
            return 0;
        }

        // 11. Get default encoder config
        vpx_codec_enc_cfg_t enc_cfg;
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
            encoder_iface, &enc_cfg, 0);
        
        if (cfg_err != VPX_CODEC_OK) {
            cleanup();
            return 0;
        }

        // Configure encoder
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256; // Low bitrate for fuzzing
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0; // No lookahead for fuzzing

        // 12. Initialize encoder
        vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(
            &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (enc_init_err != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_err_to_string(enc_init_err);
            (void)error_str; // Use in condition to avoid unused warning
            cleanup();
            return 0;
        }

        // 13. Set encoder config
        vpx_codec_err_t set_cfg_err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
        if (set_cfg_err != VPX_CODEC_OK) {
            // Continue anyway
        }

        // 14. Get global headers
        global_headers = vpx_codec_get_global_headers(&encoder);

        // 15. Set output buffer
        std::vector<uint8_t> output_buffer(4096);
        vpx_fixed_buf_t buf;
        buf.buf = output_buffer.data();
        buf.sz = output_buffer.size();
        
        vpx_codec_err_t set_buf_err = vpx_codec_set_cx_data_buf(
            &encoder, &buf, 0, 0);
        if (set_buf_err != VPX_CODEC_OK) {
            // Continue anyway
        }

        // 16. Register callbacks
        vpx_codec_err_t frame_cb_err = vpx_codec_register_put_frame_cb(
            &encoder,  id_000064_put_frame_callback, nullptr);
        if (frame_cb_err != VPX_CODEC_OK) {
            // Continue anyway
        }
        
        vpx_codec_err_t slice_cb_err = vpx_codec_register_put_slice_cb(
            &encoder,  id_000064_put_slice_callback, nullptr);
        if (slice_cb_err != VPX_CODEC_OK) {
            // Continue anyway
        }

        // 17. Encode the image
        vpx_codec_err_t encode_err = vpx_codec_encode(
            &encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        
        if (encode_err != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_err_to_string(encode_err);
            (void)error_str; // Use in condition to avoid unused warning
        }

        // 18. Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        
        // Open output file
        out_file = fopen("output_file", "wb");
        if (out_file) {
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }

    } catch (...) {
        // Catch any exceptions to ensure cleanup
    }

    // 19. Cleanup (handled by cleanup lambda)
    cleanup();
    
    return 0;
}