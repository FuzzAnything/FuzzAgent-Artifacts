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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 288, nr_unique_branch: 37
//<Quality> {"density":24,"unique_branches":{"get_cdef_filter_strengths":[[53,5,53,31,0,0,4,0],[59,5,59,31,0,0,4,1]],"set_one_param_for_line_chroma":[[34,23,34,34,4,0,4,0]],"set_one_param_for_line_luma":[[707,9,707,17,0,0,4,0],[708,22,708,37,0,0,4,1],[710,10,710,17,0,0,4,1],[710,21,710,34,0,0,4,0],[34,23,34,34,6,0,4,0]],"av1_set_speed_features_qindex_dependent":[[2929,16,2929,25,0,0,4,0],[2930,11,2930,29,0,0,4,0],[3006,7,3006,17,0,0,4,0],[3007,11,3007,44,0,0,4,0],[34,23,34,34,6,0,4,1]],"decode_tiles":[[2907,33,2907,64,0,0,4,0]],"search_wiener":[[1631,46,1631,75,0,0,4,0]],"search_tx_type":[[2109,7,2109,62,0,0,4,0],[2113,7,2113,47,0,0,4,0],[2113,7,2113,47,0,0,4,1],[2115,7,2115,19,0,0,4,0],[2115,7,2115,19,0,0,4,1],[2121,7,2121,38,0,0,4,0],[2121,42,2121,68,0,0,4,0],[2122,7,2122,41,0,0,4,0],[2123,8,2123,30,0,0,4,0],[2186,16,2186,47,0,0,4,0]],"get_tx_mask":[[1874,11,1874,61,0,0,4,1]],"av1_filter_block_plane_vert_opt":[[1399,16,1399,33,0,0,4,1]],"av1_rd_pick_intra_sby_mode":[[1689,7,1689,52,0,0,4,0],[1696,28,1696,59,0,0,4,0],[1696,28,1696,59,0,0,4,1],[1698,11,1698,61,0,0,4,0],[1700,13,1700,56,0,0,4,1],[1713,13,1715,33,0,0,4,0],[1713,13,1715,33,0,0,4,1],[1720,9,1720,56,0,0,4,1]],"finalize_sym_filter":[[31,26,31,36,7,0,4,0],[31,26,31,36,8,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<103) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_16, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. First use aom_uleb_decode on the input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    aom_uleb_decode(data, size, &uleb_value, &uleb_length);

    // 2. Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Initialize decoder for AV1
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, 
                                                  AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Peek at stream info
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);

    // 5. Decode the input data
    ret = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    if (ret != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&decoder_ctx);
        aom_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // Use plane width/height functions
        int plane_width = aom_img_plane_width(decoded_img, fuzz_int32_t_2);
        int plane_height = aom_img_plane_height(decoded_img, fuzz_int32_t_3);
        
        // 7. Initialize encoder for AV1
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        ret = aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4);
        if (ret != AOM_CODEC_OK) {
            continue;
        }
        
        // Set encoder configuration based on decoded image
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        ret = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_5, 
                                      AOM_ENCODER_ABI_VERSION);
        if (ret != AOM_CODEC_OK) {
            continue;
        }
        
        // 8. Set encoder options
        aom_codec_set_option(&encoder_ctx, fuzz_str_6, fuzz_str_7);
        
        // 9. Use codec control
        aom_codec_control(&encoder_ctx, AOME_SET_CPUUSED, 5);
        
        // 10. Allocate a new image for encoding (optional, but to use aom_img_alloc)
        aom_image_t *img_for_encode = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                                    decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_8);
        if (img_for_encode) {
            // Copy data from decoded image (simplified)
            for (int plane = 0; plane < 3; plane++) {
                int copy_size = aom_img_plane_width(decoded_img, fuzz_int32_t_9) * 
                               aom_img_plane_height(decoded_img, fuzz_int32_t_10);
                if (copy_size > 0) {
                    memcpy(img_for_encode->planes[plane], decoded_img->planes[plane], copy_size);
                }
            }
            
            // 11. Encode the image
            ret = aom_codec_encode(&encoder_ctx, img_for_encode, fuzz_int64_t_11, fuzz_uint64_t_12, fuzz_int64_t_13);
            
            // 12. Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Packet data available
                }
            }
            
            aom_img_free(img_for_encode);
        }
        
        // 13. Encode the original decoded image as well
        ret = aom_codec_encode(&encoder_ctx, decoded_img, fuzz_int64_t_14, fuzz_uint64_t_15, fuzz_int64_t_16);
        
        // Get encoded data again
        aom_codec_iter_t enc_iter2 = nullptr;
        const aom_codec_cx_pkt_t *pkt2;
        while ((pkt2 = aom_codec_get_cx_data(&encoder_ctx, &enc_iter2)) != nullptr) {
            if (pkt2->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Packet data available
            }
        }
        
        // 14. Destroy encoder
        aom_codec_destroy(&encoder_ctx);
    }

    // 15. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        fwrite(data, 1, size < 100 ? size : 100, out_file);
        assert_file_closed(&out_file);;
    }

    // 16. Clean up
    aom_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}