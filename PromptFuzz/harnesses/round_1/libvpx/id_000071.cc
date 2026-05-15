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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 387.5, nr_unique_branch: 134
//<Quality> {"density":25,"unique_branches":{"update_coef_probs_common":[[557,5,557,18,0,0,4,0],[561,19,561,34,0,0,4,0],[561,19,561,34,0,0,4,1],[562,21,562,34,0,0,4,0],[562,21,562,34,0,0,4,1],[563,23,563,37,0,0,4,0],[563,23,563,37,0,0,4,1],[564,25,564,48,0,0,4,0],[564,25,564,48,0,0,4,1],[565,27,565,51,0,0,4,0],[565,27,565,51,0,0,4,1],[570,21,570,36,0,0,4,0],[570,21,570,36,0,0,4,1],[577,21,577,26,0,0,4,0],[577,21,577,26,0,0,4,1],[577,30,577,42,0,0,4,0],[578,21,578,22,0,0,4,0],[578,21,578,22,0,0,4,1],[591,11,591,25,0,0,4,1],[591,29,591,40,0,0,4,1],[596,19,596,34,0,0,4,0],[596,19,596,34,0,0,4,1],[597,21,597,34,0,0,4,0],[597,21,597,34,0,0,4,1],[598,23,598,37,0,0,4,0],[598,23,598,37,0,0,4,1],[599,25,599,48,0,0,4,0],[599,25,599,48,0,0,4,1],[601,27,601,51,0,0,4,0],[601,27,601,51,0,0,4,1],[606,21,606,36,0,0,4,0],[606,21,606,36,0,0,4,1],[613,21,613,26,0,0,4,0],[613,21,613,26,0,0,4,1],[613,30,613,43,0,0,4,0],[615,21,615,22,0,0,4,0],[615,21,615,22,0,0,4,1],[628,5,628,12,0,0,4,1],[110,36,110,47,6,0,4,0],[110,36,110,47,6,0,4,1],[110,36,110,47,10,0,4,0],[110,36,110,47,10,0,4,1]],"update_coef_probs":[[691,9,691,52,0,0,4,1],[692,10,692,29,0,0,4,1]],"vp9_encode_block_intra":[[1073,30,1073,34,0,0,4,1]],"recenter_nonneg":[[36,7,36,19,0,0,4,0],[36,7,36,19,0,0,4,1],[38,12,38,18,0,0,4,0],[38,12,38,18,0,0,4,1]],"vp9_filter_block_plane_ss11":[[1342,15,1342,32,0,0,4,1],[1381,15,1381,32,0,0,4,1],[1389,9,1389,24,0,0,4,1]],"build_tree_distribution":[[527,15,527,30,0,0,4,0],[527,15,527,30,0,0,4,1],[528,17,528,30,0,0,4,0],[528,17,528,30,0,0,4,1],[529,19,529,33,0,0,4,0],[529,19,529,33,0,0,4,1],[530,21,530,44,0,0,4,0],[530,21,530,44,0,0,4,1],[536,23,536,46,0,0,4,0],[536,23,536,46,0,0,4,1],[110,36,110,47,2,0,4,0],[110,36,110,47,2,0,4,1]],"vp9_prob_diff_update_savings_search_model":[[148,29,148,42,0,0,4,0],[148,29,148,42,0,0,4,1],[155,33,155,50,0,0,4,0],[155,33,155,50,0,0,4,1],[163,7,163,64,0,0,4,0],[163,7,163,64,0,0,4,1],[164,25,164,54,0,0,4,0],[164,25,164,54,0,0,4,1],[165,11,165,19,0,0,4,1],[165,23,165,33,0,0,4,1],[168,37,168,54,0,0,4,0],[168,37,168,54,0,0,4,1],[172,11,172,32,0,0,4,0],[172,11,172,32,0,0,4,1]],"vp9_set_variance_partition_thresholds":[[642,7,642,55,0,0,4,1]],"vp9_rd_pick_intra_mode_sb":[[3258,17,3258,24,0,0,4,0]],"vp9_initialize_rd_consts":[[428,9,428,61,0,0,4,1],[429,9,429,36,0,0,4,0]],"vp9_cond_prob_diff_update":[[190,7,190,18,0,0,4,0]],"vp9_idct8x8_add":[[142,7,142,15,0,0,4,0]],"remap_prob":[[70,7,70,27,0,0,4,0],[70,7,70,27,0,0,4,1]],"vp9_get_preview_raw_frame":[[6732,7,6732,22,0,0,4,1],[6739,9,6739,26,0,0,4,0]],"encode_frame_internal":[[5751,7,5751,25,0,0,4,1]],"build_y_mask":[[755,7,755,15,0,0,4,0],[755,19,755,37,0,0,4,1]],"vp9_tokenize_sb":[[478,7,478,15,0,0,4,0],[479,9,479,17,0,0,4,0],[479,21,479,30,0,0,4,0]],"filter_selectively_horiz":[[503,18,503,30,0,0,4,0],[504,13,504,32,0,0,4,1],[527,15,527,31,0,0,4,1]],"decode_tiles_mt":[[2509,12,2509,23,0,0,4,0]],"vp9_update_noise_estimate":[[112,44,112,61,0,0,4,1]],"set_rt_speed_feature_framesize_independent":[[662,7,662,17,0,0,4,0],[663,9,663,37,0,0,4,0],[663,41,663,68,0,0,4,1],[672,9,672,27,0,0,4,0],[675,11,675,48,0,0,4,0],[676,11,676,43,0,0,4,0],[677,12,677,25,0,0,4,0],[685,9,685,37,0,0,4,1],[689,9,689,35,0,0,4,1],[694,9,694,36,0,0,4,1],[440,9,440,16,24,0,4,1]],"convert_distribution":[[39,7,39,19,0,0,4,0],[39,7,39,19,0,0,4,1],[44,7,44,23,0,0,4,0],[44,7,44,23,0,0,4,1]],"build_masks":[[699,7,699,15,0,0,4,0],[699,19,699,37,0,0,4,1]],"vp9_prob_diff_update_savings_search":[[127,7,127,64,0,0,4,0],[128,25,128,37,0,0,4,0],[128,25,128,37,0,0,4,1],[132,11,132,32,0,0,4,0],[132,11,132,32,0,0,4,1]],"encode_nonrd_sb_row":[[5304,7,5304,31,0,0,4,0],[5328,7,5328,14,0,0,4,1]],"choose_partitioning":[[1311,44,1311,61,0,0,4,1],[1391,7,1391,32,0,0,4,1],[1552,7,1552,14,0,0,4,1],[1705,11,1708,51,0,0,4,1],[1761,7,1761,10,0,0,4,1]],"encoder_get_preview":[[1802,7,1802,60,0,0,4,0]],"encode_term_subexp":[[102,7,102,34,0,0,4,0],[102,7,102,34,0,0,4,1],[104,14,104,41,0,0,4,0]],"rd_pick_intra_sby_mode":[[1388,11,1388,21,0,0,4,0]],"vp9_setup_mask":[[910,8,910,44,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_error","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_error","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000071_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback receives decoded frames
    (void)user_priv;
    (void)img;
    // We don't process the frame further in this fuzzer
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // Initialize decoder for VP9
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    // Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    // Initialize encoder for VP9
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set encoder configuration based on decoded frame
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_threads = 2;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.kf_mode = VPX_KF_AUTO;
    enc_cfg.kf_min_dist = 0;
    enc_cfg.kf_max_dist = 9999;
    
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000071_put_frame_callback, nullptr);

    // Create image for encoding
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                     decoded_img->d_w, decoded_img->d_h, 16);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    // Copy decoded image data to new image
    // Note: This is simplified - in reality would need proper YUV plane copying
    for (int plane = 0; plane < 3; ++plane) {
        if (decoded_img->planes[plane] && img->planes[plane]) {
            size_t plane_size = decoded_img->stride[plane] * 
                               (plane == 0 ? decoded_img->d_h : decoded_img->d_h / 2);
            if (plane_size <= img->stride[plane] * img->d_h) {
                memcpy(img->planes[plane], decoded_img->planes[plane], plane_size);
            }
        }
    }

    // Set control parameters
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
    vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
    vpx_codec_control_(&encoder, VP9E_SET_TILE_COLUMNS, 1);

    // Encode the image
    res = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    if (res != VPX_CODEC_OK) {
        // Get error info
        const char *error = vpx_codec_error(&encoder);
        (void)error;
    }

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // Set up buffer for encoded data
    uint8_t output_buffer[4096];
    vpx_fixed_buf_t output_buf;
    output_buf.buf = output_buffer;
    output_buf.sz = sizeof(output_buffer);
    
    vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);

    // Get encoded data packets
    const vpx_codec_cx_pkt_t *pkt;
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Get preview frame (if available)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
    if (preview_frame) {
        // Could encode the preview frame as well
        (void)preview_frame;
    }

    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	return 0;
}