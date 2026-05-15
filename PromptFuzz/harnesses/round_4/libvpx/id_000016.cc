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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 225, nr_unique_branch: 107
//<Quality> {"density":15,"unique_branches":{"vp9_pick_intra_mode":[[1209,9,1209,23,0,0,4,0]],"transpose_and_output8x8":[[346,7,346,16,0,0,4,0],[346,7,346,16,0,0,4,1]],"update_coef_probs":[[691,9,691,52,0,0,4,1],[692,10,692,29,0,0,4,0],[692,33,692,76,0,0,4,1]],"encode_term_subexp":[[102,7,102,34,0,0,4,0],[102,7,102,34,0,0,4,1],[104,14,104,41,0,0,4,0]],"vp9_rc_update_rate_correction_factors":[[825,9,825,48,0,0,4,0]],"vp9_encode_block_intra":[[1045,30,1045,34,0,0,4,1]],"vpx_realloc_frame_buffer":[[220,11,220,59,0,0,4,0]],"vp9_fht16x16_sse2":[[1513,5,1513,17,0,0,4,0],[1514,5,1514,18,0,0,4,1]],"convert_distribution":[[39,7,39,19,0,0,4,0],[39,7,39,19,0,0,4,1],[44,7,44,23,0,0,4,0],[44,7,44,23,0,0,4,1]],"choose_partitioning":[[1390,7,1390,31,0,0,4,1],[1391,7,1391,32,0,0,4,1],[1677,7,1677,22,0,0,4,0],[1683,9,1683,22,0,0,4,1],[1690,14,1690,27,0,0,4,1],[1698,7,1698,31,0,0,4,1],[1698,35,1698,59,0,0,4,0],[1698,35,1698,59,0,0,4,1],[1699,7,1700,71,0,0,4,0],[1705,11,1708,51,0,0,4,1]],"build_tree_distribution":[[527,15,527,30,0,0,4,0],[527,15,527,30,0,0,4,1],[528,17,528,30,0,0,4,0],[528,17,528,30,0,0,4,1],[529,19,529,33,0,0,4,0],[529,19,529,33,0,0,4,1],[530,21,530,44,0,0,4,0],[530,21,530,44,0,0,4,1],[536,23,536,46,0,0,4,0],[536,23,536,46,0,0,4,1],[110,36,110,47,2,0,4,0],[110,36,110,47,2,0,4,1]],"block_yrd":[[842,13,842,22,0,0,4,0],[844,18,844,26,0,0,4,1]],"vp9_prob_diff_update_savings_search":[[127,7,127,64,0,0,4,0],[128,25,128,37,0,0,4,0],[128,25,128,37,0,0,4,1],[132,11,132,32,0,0,4,0],[132,11,132,32,0,0,4,1]],"setup_frame_size_with_refs":[[1608,7,1616,25,0,0,4,0]],"recenter_nonneg":[[36,7,36,19,0,0,4,1],[38,12,38,18,0,0,4,1]],"filter_selectively_vert_row2":[[317,11,317,32,0,0,4,0],[318,13,318,48,0,0,4,0]],"build_masks":[[699,7,699,15,0,0,4,0],[699,19,699,37,0,0,4,1]],"tree_to_node":[[401,5,401,21,0,0,4,0],[404,19,404,24,0,0,4,0],[404,19,404,24,0,0,4,1]],"nonrd_use_partition":[[5051,5,5051,24,0,0,4,0],[5062,11,5062,37,0,0,4,1]],"vp9_filter_block_plane_ss11":[[1342,15,1342,32,0,0,4,1],[1381,15,1381,32,0,0,4,1],[1389,9,1389,24,0,0,4,1]],"set_vt_partitioning":[[505,10,505,29,0,0,4,0]],"remap_prob":[[70,7,70,27,0,0,4,0]],"update_coef_probs_common":[[557,5,557,18,0,0,4,0],[561,19,561,34,0,0,4,0],[561,19,561,34,0,0,4,1],[562,21,562,34,0,0,4,0],[562,21,562,34,0,0,4,1],[563,23,563,37,0,0,4,0],[563,23,563,37,0,0,4,1],[564,25,564,48,0,0,4,0],[564,25,564,48,0,0,4,1],[565,27,565,51,0,0,4,0],[565,27,565,51,0,0,4,1],[570,21,570,36,0,0,4,0],[570,21,570,36,0,0,4,1],[577,21,577,26,0,0,4,1],[578,21,578,22,0,0,4,1],[591,11,591,25,0,0,4,0],[628,5,628,12,0,0,4,1],[110,36,110,47,6,0,4,0],[110,36,110,47,6,0,4,1]],"vp9_prob_diff_update_savings_search_model":[[148,29,148,42,0,0,4,0],[148,29,148,42,0,0,4,1],[155,33,155,50,0,0,4,0],[155,33,155,50,0,0,4,1],[163,7,163,64,0,0,4,1]],"vp9_setup_mask":[[910,8,910,44,0,0,4,1],[913,8,913,44,0,0,4,1],[942,11,942,27,0,0,4,0],[945,11,945,27,0,0,4,0],[947,17,947,49,0,0,4,0],[957,11,957,18,0,0,4,1]],"vpx_fdct16x16_sse2":[[605,18,605,26,0,0,4,0],[605,18,605,26,0,0,4,1],[611,28,611,45,0,0,4,0],[611,28,611,45,0,0,4,1],[623,11,623,20,0,0,4,0],[623,11,623,20,0,0,4,1],[1003,11,1003,20,0,0,4,0],[1003,11,1003,20,0,0,4,1]],"filter_selectively_horiz":[[466,11,466,25,0,0,4,0],[467,13,467,34,0,0,4,0]],"write_modes_sb":[[457,7,457,26,0,0,4,0],[460,13,460,38,0,0,4,1]],"vp9_cond_prob_diff_update":[[190,7,190,18,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function declarations
static int  id_000016_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb);
static int  id_000016_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb);
static void  id_000016_put_frame_cb(void *user_priv, const vpx_image_t *img);
static void  id_000016_put_slice_cb(void *user_priv, const vpx_image_t *img, 
                         const vpx_image_rect_t *valid, const vpx_image_rect_t *update);

// Frame buffer pool structure
struct FrameBufferPool {
    uint8_t *data;
    size_t size;
    int in_use;
};

// Global state for callbacks
static struct {
    FrameBufferPool *pool;
    size_t pool_size;
    int frame_count;
    int slice_count;
}  id_000016_g_cb_state = {nullptr, 0, 0, 0};

// Frame buffer allocation callback
static int  id_000016_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferPool *pool = (FrameBufferPool *)cb_priv;
    
    // Find available buffer
    for (size_t i = 0; i <  id_000016_g_cb_state.pool_size; i++) {
        if (!pool[i].in_use && pool[i].size >= min_size) {
            pool[i].in_use = 1;
            fb->data = pool[i].data;
            fb->size = pool[i].size;
            return 0;
        }
    }
    
    // Allocate new buffer if none available
    for (size_t i = 0; i <  id_000016_g_cb_state.pool_size; i++) {
        if (!pool[i].in_use) {
            free(pool[i].data);
            pool[i].data = (uint8_t *)calloc(1, min_size);
            pool[i].size = min_size;
            pool[i].in_use = 1;
            fb->data = pool[i].data;
            fb->size = min_size;
            return 0;
        }
    }
    
    return -1;
}

// Frame buffer release callback
static int  id_000016_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    FrameBufferPool *pool = (FrameBufferPool *)cb_priv;
    
    for (size_t i = 0; i <  id_000016_g_cb_state.pool_size; i++) {
        if (pool[i].data == fb->data) {
            pool[i].in_use = 0;
            return 0;
        }
    }
    
    return -1;
}

// Put frame callback
static void  id_000016_put_frame_cb(void *user_priv, const vpx_image_t *img) {
     id_000016_g_cb_state.frame_count++;
}

// Put slice callback
static void  id_000016_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *valid, const vpx_image_rect_t *update) {
     id_000016_g_cb_state.slice_count++;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
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
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize callback state
    const size_t POOL_SIZE = 4;
     id_000016_g_cb_state.pool = (FrameBufferPool *)calloc(POOL_SIZE, sizeof(FrameBufferPool));
     id_000016_g_cb_state.pool_size = POOL_SIZE;
     id_000016_g_cb_state.frame_count = 0;
     id_000016_g_cb_state.slice_count = 0;
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free( id_000016_g_cb_state.pool);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free( id_000016_g_cb_state.pool);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free( id_000016_g_cb_state.pool);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for decoder
    res = vpx_codec_set_frame_buffer_functions(&decoder,  id_000016_get_frame_buffer_cb, 
                                                id_000016_release_frame_buffer_cb,  id_000016_g_cb_state.pool);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000016_put_frame_cb, nullptr);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Register put slice callback
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000016_put_slice_cb, nullptr);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t enc_cfg;
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Configure encoder
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Set encoder configuration
    res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Create output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.sz = 1024 * 1024; // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    
    // Set output buffer for encoder
    res = vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_5, fuzz_uint32_t_6);
    if (res != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(res);
    }
    
    // Create a dummy image for encoding
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_7);
    if (img) {
        // Fill with some data
        memset(img->planes[0], 128, img->stride[0] * img->h);
        memset(img->planes[1], 128, img->stride[1] * img->h / 2);
        memset(img->planes[2], 128, img->stride[2] * img->h / 2);
        
        // Encode the image
        res = vpx_codec_encode(&encoder, img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
        if (res != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(res);
        }
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        vpx_img_free(img);
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    free(output_buf.buf);
    
    // Clean up callback pool
    for (size_t i = 0; i <  id_000016_g_cb_state.pool_size; i++) {
        free( id_000016_g_cb_state.pool[i].data);
    }
    free( id_000016_g_cb_state.pool);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}