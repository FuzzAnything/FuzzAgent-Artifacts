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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 980, nr_unique_branch: 172
//<Quality> {"density":20,"unique_branches":{"av1_filter_block_plane_horz_opt_chroma":[[2056,9,2056,21,0,0,4,1],[2056,46,2056,61,0,0,4,1],[2062,16,2062,26,0,0,4,0],[2062,16,2062,26,0,0,4,1],[2062,30,2062,47,0,0,4,0],[2062,51,2062,65,0,0,4,0],[2062,51,2062,65,0,0,4,1]],"av1_rd_pick_palette_intra_sby":[[713,11,713,34,0,0,4,0],[742,13,742,36,0,0,4,0]],"av1_cost_coeffs_txb":[[684,7,684,15,0,0,4,0]],"get_intra_q_and_bounds":[[1878,9,1878,36,0,0,4,0]],"perform_top_color_palette_search":[[369,16,369,64,0,0,4,0]],"av1_receive_raw_frame":[[4766,7,4767,76,0,0,4,0]],"aom_codec_get_cx_data":[[221,9,223,41,0,0,4,1]],"copy_unit_info":[[1856,7,1856,46,0,0,4,0]],"skip_trellis_opt_based_on_satd":[[1934,14,1934,27,0,0,4,0],[1944,7,1944,25,0,0,4,0],[66,4,66,11,3,0,4,0]],"aom_lpf_horizontal_8_quad_avx2":[[305,7,305,62,0,0,4,0],[305,7,305,62,0,0,4,1],[378,9,378,64,0,0,4,0],[378,9,378,64,0,0,4,1]],"loop_restoration_write_sb_coeffs":[[1966,14,1966,43,0,0,4,0],[1972,9,1972,35,0,0,4,0]],"aom_lpf_horizontal_6_quad_avx2":[[88,7,88,62,0,0,4,0],[157,9,157,64,0,0,4,0]],"aom_copy_metadata_to_frame_buffer":[[290,23,290,43,0,0,4,0]],"filter_vert":[[1041,7,1041,13,0,0,4,0],[1053,14,1053,41,0,0,4,0],[1056,7,1056,13,0,0,4,0],[1056,7,1056,13,0,0,4,1],[1061,7,1061,13,0,0,4,0],[1061,7,1061,13,0,0,4,1],[1067,7,1067,13,0,0,4,0],[1067,7,1067,13,0,0,4,1],[1073,7,1073,14,0,0,4,0],[1073,7,1073,14,0,0,4,1],[1079,7,1079,14,0,0,4,0],[1079,7,1079,14,0,0,4,1]],"search_tx_type":[[2059,45,2059,54,0,0,4,1],[2080,7,2080,18,0,0,4,0],[2113,7,2113,47,0,0,4,1],[2115,7,2115,19,0,0,4,1],[2152,9,2152,21,0,0,4,1],[2182,16,2182,27,0,0,4,0]],"av1_filter_block_plane_horz_opt":[[1988,9,1988,21,0,0,4,1],[1988,46,1988,67,0,0,4,1],[1995,16,1995,33,0,0,4,0],[1995,37,1995,57,0,0,4,0],[1995,37,1995,57,0,0,4,1]],"set_one_param_for_line_chroma":[[34,23,34,34,4,0,4,0]],"is_iter_over":[[328,10,328,25,0,0,4,1]],"move_decoder_metadata_to_img":[[801,7,801,20,0,0,4,0],[801,24,801,27,0,0,4,0]],"intra_block_yrd":[[1206,42,1206,75,0,0,4,1]],"search_selfguided_restoration":[[833,7,833,29,0,0,4,1],[843,19,843,53,0,0,4,0],[843,19,843,53,0,0,4,1],[851,9,851,34,0,0,4,1]],"av1_set_speed_features_qindex_dependent":[[3020,9,3020,48,0,0,4,0],[3021,9,3021,43,0,0,4,0],[3021,47,3021,64,0,0,4,0]],"sub_avg_block_avx2":[[531,9,531,37,0,0,4,0],[532,9,532,37,0,0,4,0],[533,9,533,60,0,0,4,1]],"compute_stats_win5_avx2":[[781,9,781,37,0,0,4,0],[1019,11,1019,39,0,0,4,0],[621,7,621,35,3,0,4,0],[622,7,622,59,3,0,4,1],[614,7,614,35,6,0,4,0],[621,7,621,35,7,0,4,0],[622,7,622,59,7,0,4,1],[640,9,640,37,10,0,4,0],[614,7,614,35,11,0,4,0],[621,7,621,35,12,0,4,0],[622,7,622,59,12,0,4,1],[640,9,640,37,15,0,4,0],[614,7,614,35,16,0,4,0],[621,7,621,35,17,0,4,0],[622,7,622,59,17,0,4,1],[640,9,640,37,20,0,4,0],[614,7,614,35,21,0,4,0],[621,7,621,35,22,0,4,0],[622,7,622,59,22,0,4,1],[640,9,640,37,25,0,4,0],[621,7,621,35,27,0,4,0],[622,7,622,59,27,0,4,1],[621,7,621,35,35,0,4,0],[622,7,622,59,35,0,4,1],[621,7,621,35,41,0,4,0],[622,7,622,59,41,0,4,1],[621,7,621,35,47,0,4,0],[622,7,622,59,47,0,4,1],[621,7,621,35,53,0,4,0],[622,7,622,59,53,0,4,1]],"split_partition_search":[[4556,13,4556,33,0,0,4,1]],"av1_filter_block_plane_vert_opt":[[1392,9,1392,21,0,0,4,1],[1392,46,1392,68,0,0,4,1],[1399,16,1399,33,0,0,4,0],[1399,37,1399,58,0,0,4,0],[1399,37,1399,58,0,0,4,1]],"filter_horz":[[1643,7,1643,13,0,0,4,0],[1655,14,1655,41,0,0,4,0],[1658,7,1658,13,0,0,4,0],[1658,7,1658,13,0,0,4,1],[1663,7,1663,13,0,0,4,0],[1663,7,1663,13,0,0,4,1],[1669,7,1669,13,0,0,4,0],[1669,7,1669,13,0,0,4,1],[1675,7,1675,14,0,0,4,0],[1675,7,1675,14,0,0,4,1],[1681,7,1681,14,0,0,4,0],[1681,7,1681,14,0,0,4,1]],"av1_rd_pick_palette_intra_sbuv":[[903,11,903,65,0,0,4,0],[909,13,909,34,0,0,4,0],[909,13,909,34,0,0,4,1],[911,13,911,47,0,0,4,0],[911,13,911,47,0,0,4,1]],"set_one_param_for_line_luma":[[707,9,707,17,0,0,4,0],[708,22,708,37,0,0,4,1],[710,10,710,17,0,0,4,1],[710,21,710,34,0,0,4,0],[34,23,34,34,6,0,4,0]],"finalize_sym_filter":[[31,26,31,36,7,0,4,0],[31,46,31,56,7,0,4,0],[31,26,31,36,8,0,4,0],[31,46,31,56,8,0,4,0]],"rd_pick_intrabc_mode_sb":[[3426,7,3426,33,0,0,4,0]],"encode_restoration_mode":[[1792,7,1792,26,0,0,4,0],[1810,25,1810,65,0,0,4,1],[1817,9,1817,22,0,0,4,0]],"write_wiener_filter":[[1851,7,1851,31,0,0,4,0],[1870,7,1870,31,0,0,4,0]],"finer_search_wiener":[[1504,7,1504,54,0,0,4,0]],"search_filter_level":[[175,13,175,40,0,0,4,1]],"lpf_internal_14_dual_sse2":[[460,7,460,62,0,0,4,0],[460,7,460,62,0,0,4,1],[583,9,583,65,0,0,4,0],[583,9,583,65,0,0,4,1]],"av1_filter_block_plane_vert_opt_chroma":[[1461,9,1461,21,0,0,4,1],[1461,46,1461,62,0,0,4,1],[1467,16,1467,26,0,0,4,0],[1467,16,1467,26,0,0,4,1],[1467,30,1467,47,0,0,4,0],[1467,51,1467,66,0,0,4,0],[1467,51,1467,66,0,0,4,1]],"search_wiener":[[1631,24,1631,42,0,0,4,0],[1631,46,1631,75,0,0,4,1],[1632,9,1632,21,0,0,4,1],[1637,11,1637,54,0,0,4,1],[1698,9,1698,52,0,0,4,0],[1726,7,1726,32,0,0,4,0],[1731,7,1731,50,0,0,4,0],[1733,14,1733,57,0,0,4,0],[1745,7,1745,32,0,0,4,0],[1746,7,1746,30,0,0,4,0]],"none_partition_search":[[4376,7,4376,14,0,0,4,1]],"av1_pick_filter_restoration":[[2158,8,2158,42,0,0,4,1]],"lpf_internal_6_dual_sse2":[[1062,7,1062,62,0,0,4,0],[1062,7,1062,62,0,0,4,1]],"perform_k_means_palette_search":[[422,16,422,64,0,0,4,0]],"encode_b":[[1447,9,1447,49,0,0,4,0],[1447,53,1447,73,0,0,4,1]],"encoder_encode":[[3484,11,3485,68,0,0,4,0]],"predict_dc_only_block":[[2013,9,2013,21,0,0,4,0]],"aom_lpf_horizontal_14_quad_avx2":[[663,7,663,62,0,0,4,0],[742,9,742,64,0,0,4,0],[848,11,848,67,0,0,4,0],[848,11,848,67,0,0,4,1]],"av1_lookahead_push":[[160,7,160,66,0,0,4,0]],"lpf_internal_8_dual_sse2":[[1544,7,1544,62,0,0,4,0],[1544,7,1544,62,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_err_to_string","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_remove_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_wrap","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_remove_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_wrap","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks for decoder
static int  id_000051_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000051_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0;  // Good quality
    
    // Get default config
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
        return 0;
    }
    
    // Set some reasonable dimensions
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_threads = 1;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_end_usage = AOM_Q;
    enc_cfg.rc_min_quantizer = 20;
    enc_cfg.rc_max_quantizer = 40;
    
    aom_codec_ctx_t enc_ctx;
    err = aom_codec_enc_init_ver(&enc_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
        return 0;
    }
    
    // Set an encoder option
    aom_codec_set_option(&enc_ctx, "cpu-used", "8");
    
    // Allocate image for encoding
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 16;
    
    if (!aom_img_alloc(&img, fmt, enc_cfg.g_w, enc_cfg.g_h, align)) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }
    
    // Fill image planes with fuzzer data
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_w = aom_img_plane_width(&img, plane);
        int plane_h = aom_img_plane_height(&img, plane);
        uint8_t *plane_ptr = img.planes[plane];
        
        for (int y = 0; y < plane_h; ++y) {
            size_t bytes_to_copy = plane_w;
            if (data_offset + bytes_to_copy > size) {
                bytes_to_copy = size - data_offset;
            }
            if (bytes_to_copy > 0) {
                memcpy(plane_ptr, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            plane_ptr += img.stride[plane];
        }
    }
    
    // Add metadata to image if we have enough data
    if (size - data_offset > 10) {
        uint32_t metadata_type = 1;  // Arbitrary type
        size_t metadata_size = (size - data_offset) % 100;  // Limit size
        aom_img_add_metadata(&img, metadata_type, 
                            data + data_offset, metadata_size,
                            AOM_MIF_ANY_FRAME);
        data_offset += metadata_size;
    }
    
    // Prepare buffer for encoded data
    aom_fixed_buf_t enc_buf;
    enc_buf.sz = size * 2;  // Allocate enough space
    enc_buf.buf = malloc(enc_buf.sz);
    
    if (!enc_buf.buf) {
        aom_img_free(&img);
        aom_codec_destroy(&enc_ctx);
        return 0;
    }
    
    // Set buffer for encoded data
    aom_codec_set_cx_data_buf(&enc_ctx, &enc_buf, 0, 0);
    
    // Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    err = aom_codec_encode(&enc_ctx, &img, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
    }
    
    // Get encoded data
    aom_codec_iter_t enc_iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&enc_ctx, &enc_iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                              (uint8_t*)pkt->data.frame.buf,
                              (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // Remove metadata from image
    aom_img_remove_metadata(&img);
    
    // Now decode the encoded data
    if (!encoded_data.empty()) {
        // Initialize decoder
        aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
        aom_codec_ctx_t dec_ctx;
        aom_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 1;
        dec_cfg.allow_lowbitdepth = 1;
        
        err = aom_codec_dec_init_ver(&dec_ctx, decoder_iface, &dec_cfg, 0, 
                                    AOM_DECODER_ABI_VERSION);
        if (err == AOM_CODEC_OK) {
            // Set frame buffer functions
            aom_codec_set_frame_buffer_functions(&dec_ctx,  id_000051_get_frame_buffer, 
                                                id_000051_release_frame_buffer, NULL);
            
            // Decode the data
            err = aom_codec_decode(&dec_ctx, encoded_data.data(), 
                                  encoded_data.size(), NULL);
            
            if (err == AOM_CODEC_OK) {
                // Get stream info
                aom_codec_stream_info_t si;
                memset(&si, 0, sizeof(si));
                aom_codec_get_stream_info(&dec_ctx, &si);
                
                // Try to get decoded frames
                aom_codec_iter_t dec_iter = NULL;
                aom_image_t *dec_img;
                while ((dec_img = aom_codec_get_frame(&dec_ctx, &dec_iter)) != NULL) {
                    // Wrap the decoded image data (just for demonstration)
                    if (dec_img->img_data) {
                        aom_image_t wrapped_img;
                        if (aom_img_wrap(&wrapped_img, dec_img->fmt, dec_img->d_w,
                                       dec_img->d_h, 1, dec_img->img_data)) {
                            // Do nothing, just called the API
                        }
                    }
                }
            }
            
            aom_codec_destroy(&dec_ctx);
        }
    }
    
    // Cleanup
    free(enc_buf.buf);
    aom_img_free(&img);
    aom_codec_destroy(&enc_ctx);
    
    return 0;
}