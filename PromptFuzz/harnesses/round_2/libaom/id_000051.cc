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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 285, nr_unique_branch: 266
//<Quality> {"density":19,"unique_branches":{"launch_workers":[[1489,33,1489,39,0,0,4,0],[1489,33,1489,39,0,0,4,1],[1492,9,1492,15,0,0,4,0],[1492,9,1492,15,0,0,4,1]],"av1_remove_compressor":[[1825,7,1825,32,0,0,4,0],[1829,7,1829,31,0,0,4,0],[1833,7,1833,27,0,0,4,0],[1837,7,1837,31,0,0,4,0],[1841,7,1841,32,0,0,4,0],[1848,7,1848,31,0,0,4,0]],"set_good_speed_feature_framesize_dependent":[[773,9,773,26,0,0,4,1],[779,9,779,26,0,0,4,0],[787,9,787,26,0,0,4,0],[808,9,808,26,0,0,4,0],[814,9,814,26,0,0,4,0],[823,9,823,26,0,0,4,0],[831,9,831,26,0,0,4,0],[843,9,843,26,0,0,4,1],[845,9,845,26,0,0,4,0],[846,45,846,56,0,0,4,1],[862,9,862,27,0,0,4,1],[866,37,866,44,0,0,4,0],[870,9,870,27,0,0,4,1],[890,9,890,26,0,0,4,0],[891,23,891,48,0,0,4,0],[891,23,891,48,0,0,4,1],[900,9,900,26,0,0,4,0],[903,52,903,63,0,0,4,1],[917,9,917,26,0,0,4,0],[918,47,918,54,0,0,4,0],[923,9,923,26,0,0,4,0],[925,47,925,54,0,0,4,0],[938,9,938,26,0,0,4,0],[954,9,954,26,0,0,4,1],[956,9,956,26,0,0,4,0],[962,9,962,26,0,0,4,0],[973,9,973,26,0,0,4,0],[978,9,978,26,0,0,4,0],[984,9,984,27,0,0,4,1],[995,9,995,26,0,0,4,1],[1001,9,1001,26,0,0,4,0],[1003,12,1003,38,0,0,4,1],[1003,42,1003,65,0,0,4,0],[1020,30,1020,48,0,0,4,1],[1032,9,1032,26,0,0,4,0],[1036,9,1036,27,0,0,4,1],[1041,9,1041,26,0,0,4,0],[1049,9,1049,26,0,0,4,0],[1055,9,1055,26,0,0,4,0],[1061,9,1061,27,0,0,4,1]],"thread_loop":[[61,7,61,34,0,0,4,1]],"assign_tile_to_thread":[[315,15,315,30,0,0,4,0],[315,15,315,30,0,0,4,1],[317,9,317,29,0,0,4,0]],"switch_tile_and_get_next_job":[[346,26,346,46,0,0,4,0],[346,26,346,46,0,0,4,1],[347,28,347,48,0,0,4,0],[347,28,347,48,0,0,4,1],[359,11,359,23,0,0,4,0],[363,11,363,23,0,0,4,0],[371,11,371,61,0,0,4,0],[382,13,382,34,0,0,4,1],[396,7,396,20,0,0,4,0],[34,23,34,34,1,0,4,0],[34,23,34,34,1,0,4,1]],"fp_prepare_enc_workers":[[1672,33,1672,39,0,0,4,0],[1672,33,1672,39,0,0,4,1],[1685,9,1685,15,0,0,4,0],[1685,9,1685,15,0,0,4,1]],"compute_num_ai_workers":[[34,23,34,34,1,0,4,1]],"av1_cdef_mt_dealloc":[[3256,7,3256,32,0,0,4,0],[3256,7,3256,32,0,0,4,1]],"av1_create_workers":[[1102,19,1102,34,0,0,4,0],[1102,19,1102,34,0,0,4,1],[1113,9,1113,14,0,0,4,0],[1113,9,1113,14,0,0,4,1],[1115,11,1115,37,0,0,4,1],[74,9,74,14,1,0,4,1],[74,9,74,14,2,0,4,1]],"av1_row_mt_mem_dealloc":[[288,22,288,42,0,0,4,0],[289,24,289,44,0,0,4,0],[289,24,289,44,0,0,4,1],[295,11,295,45,0,0,4,0]],"av1_terminate_workers":[[1132,19,1132,45,0,0,4,0]],"encoder_encode":[[3521,9,3521,24,0,0,4,0],[3521,28,3521,68,0,0,4,0],[3524,11,3524,41,0,0,4,0],[3527,11,3527,41,0,0,4,1],[3548,9,3548,62,0,0,4,1],[3565,9,3565,39,0,0,4,0],[3566,23,3566,47,0,0,4,0],[3566,23,3566,47,0,0,4,1],[3570,11,3570,26,0,0,4,0]],"av1_first_pass":[[1433,7,1433,31,0,0,4,0]],"av1_row_mt_sync_write":[[148,7,148,19,0,0,4,0],[148,7,148,19,0,0,4,1],[150,9,150,18,0,0,4,1],[155,7,155,10,0,0,4,0],[155,7,155,10,0,0,4,1],[35,23,35,34,1,0,4,1]],"get_sync_range":[[37,7,37,18,0,0,4,1],[39,12,39,25,0,0,4,0]],"av1_set_frame_size":[[2694,7,2694,30,0,0,4,1],[2701,9,2701,44,0,0,4,0]],"row_mt_sync_mem_alloc":[[185,7,185,26,0,0,4,0],[186,17,186,25,0,0,4,0],[186,17,186,25,0,0,4,1],[193,7,193,25,0,0,4,0],[194,17,194,25,0,0,4,0],[194,17,194,25,0,0,4,1],[74,9,74,14,4,0,4,1],[74,9,74,14,5,0,4,1],[74,9,74,14,6,0,4,1]],"fp_compute_max_mb_rows":[[1841,21,1841,36,0,0,4,0],[1841,21,1841,36,0,0,4,1],[34,23,34,34,1,0,4,1],[35,23,35,34,3,0,4,1]],"av1_row_mt_sync_read":[[123,7,123,8,0,0,4,0],[123,7,123,8,0,0,4,1],[127,12,128,65,0,0,4,0],[127,12,128,65,0,0,4,1]],"compute_num_enc_row_mt_workers":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0]],"round_shift_16bit_ssse3":[[61,7,61,14,0,0,4,1],[66,14,66,21,0,0,4,1]],"av1_fp_encode_tiles_row_mt":[[2015,33,2015,77,0,0,4,0],[2022,7,2022,22,0,0,4,1],[2027,7,2027,23,0,0,4,0],[2035,7,2035,44,0,0,4,1],[2044,26,2044,46,0,0,4,0],[2044,26,2044,46,0,0,4,1],[2045,28,2045,48,0,0,4,0],[2045,28,2045,48,0,0,4,1],[34,23,34,34,2,0,4,0],[34,23,34,34,2,0,4,1]],"av1_quant":[[315,7,315,60,0,0,4,1],[54,19,54,41,2,0,4,1]],"sync_enc_workers":[[1507,7,1507,16,0,0,4,1],[1512,33,1512,38,0,0,4,0],[1512,33,1512,38,0,0,4,1],[1514,9,1514,34,0,0,4,1],[1520,7,1520,16,0,0,4,1]],"write_sequence_header":[[2655,9,2655,50,0,0,4,0]],"get_next_job":[[326,7,326,44,0,0,4,0],[326,7,326,44,0,0,4,1]],"av1_init_tile_thread_data":[[971,33,971,39,0,0,4,0],[971,33,971,39,0,0,4,1],[974,9,974,14,0,0,4,0],[974,9,974,14,0,0,4,1],[988,11,988,49,0,0,4,0],[988,11,988,49,0,0,4,1],[992,13,992,31,0,0,4,1],[1002,11,1002,25,0,0,4,0],[1002,29,1002,48,0,0,4,0],[1002,29,1002,48,0,0,4,1],[1004,13,1004,45,0,0,4,1],[1009,25,1009,30,0,0,4,0],[1009,25,1009,30,0,0,4,1],[1029,13,1029,52,0,0,4,0],[1034,27,1034,32,0,0,4,0],[1034,27,1034,32,0,0,4,1],[1042,13,1042,58,0,0,4,0],[1049,13,1049,68,0,0,4,1],[1060,13,1060,78,0,0,4,1],[1070,9,1070,23,0,0,4,0],[1070,27,1070,53,0,0,4,0],[1070,57,1070,76,0,0,4,0],[1070,57,1070,76,0,0,4,1],[1071,11,1071,17,0,0,4,0],[1071,11,1071,17,0,0,4,1],[1072,25,1072,49,0,0,4,0],[1072,25,1072,49,0,0,4,1],[74,9,74,14,1,0,4,1],[74,9,74,14,3,0,4,1],[74,9,74,14,4,0,4,1],[74,9,74,14,5,0,4,1],[74,9,74,14,6,0,4,1],[74,9,74,14,7,0,4,1],[74,9,74,14,8,0,4,1],[74,9,74,14,9,0,4,1],[74,9,74,14,12,0,4,1],[74,9,74,14,13,0,4,1]],"dealloc_thread_data_src_diff_buf":[[1996,33,1996,39,0,0,4,0],[1996,33,1996,39,0,0,4,1],[1998,9,1998,36,0,0,4,0],[1998,9,1998,36,0,0,4,1]],"av1_tf_mt_dealloc":[[2373,7,2373,30,0,0,4,0],[2373,7,2373,30,0,0,4,1]],"av1_init_mt_sync":[[855,7,855,20,0,0,4,0],[855,7,855,20,0,0,4,1],[855,24,855,45,0,0,4,0],[857,9,857,35,0,0,4,0],[860,11,860,29,0,0,4,0],[862,9,862,34,0,0,4,0],[865,11,865,28,0,0,4,0],[869,7,869,21,0,0,4,0],[869,7,869,21,0,0,4,1],[872,9,872,32,0,0,4,0],[875,11,875,26,0,0,4,0],[880,9,880,32,0,0,4,0],[883,11,883,26,0,0,4,0],[888,9,888,34,0,0,4,0],[891,11,891,28,0,0,4,0],[902,9,902,29,0,0,4,0],[910,9,910,35,0,0,4,0],[913,11,913,29,0,0,4,0],[917,9,917,32,0,0,4,0],[924,11,924,31,0,0,4,0],[936,9,936,37,0,0,4,0],[939,11,939,31,0,0,4,0],[74,9,74,14,9,0,4,1],[74,9,74,14,10,0,4,1],[74,9,74,14,11,0,4,1],[74,9,74,14,12,0,4,1],[74,9,74,14,13,0,4,1],[74,9,74,14,14,0,4,1],[74,9,74,14,15,0,4,1]],"firstpass_intra_prediction":[[511,7,511,21,0,0,4,1]],"av1_first_pass_row":[[1165,7,1165,18,0,0,4,1],[1170,7,1170,20,0,0,4,1],[1175,7,1175,19,0,0,4,1],[1234,9,1234,44,0,0,4,0],[1239,11,1239,28,0,0,4,1],[1269,9,1269,23,0,0,4,1]],"build_non_directional_intra_predictors":[[1283,7,1283,16,0,0,4,1],[1293,7,1293,17,0,0,4,1]],"is_fpmt_config":[[1159,7,1159,42,0,0,4,0]],"av1_set_speed_features_framesize_independent":[[2709,7,2709,35,0,0,4,0],[2718,9,2718,19,0,0,4,0],[2718,9,2718,19,0,0,4,1],[2732,10,2732,43,0,0,4,1],[2732,47,2732,77,0,0,4,1],[2807,32,2807,62,0,0,4,0],[2808,9,2808,57,0,0,4,0],[2817,9,2817,54,0,0,4,0],[2818,9,2819,66,0,0,4,0],[2818,9,2819,66,0,0,4,1]],"av1_row_mt_sync_mem_dealloc":[[210,7,210,26,0,0,4,0],[214,9,214,36,0,0,4,0],[214,9,214,36,0,0,4,1],[215,19,215,40,0,0,4,0],[215,19,215,40,0,0,4,1],[220,9,220,35,0,0,4,0],[220,9,220,35,0,0,4,1],[221,19,221,40,0,0,4,0],[221,19,221,40,0,0,4,1]],"av1_compute_num_fp_contexts":[[1224,7,1224,40,0,0,4,0]],"init_seq_coding_tools":[[604,7,604,37,0,0,4,0],[604,41,604,66,0,0,4,0]],"encoder_set_option":[[4449,14,4450,42,0,0,4,0]],"av1_fwd_txfm":[[235,7,235,26,0,0,4,1]],"row_mt_mem_alloc":[[253,22,253,42,0,0,4,0],[253,22,253,42,0,0,4,1],[254,24,254,44,0,0,4,0],[254,24,254,44,0,0,4,1],[260,11,260,24,0,0,4,1],[74,9,74,14,5,0,4,1]],"av1_init_lr_mt_buffers":[[834,7,834,26,0,0,4,1]],"fp_enc_row_mt_worker_hook":[[482,9,482,27,0,0,4,0],[482,9,482,27,0,0,4,1],[482,31,483,74,0,0,4,0],[482,31,483,74,0,0,4,1],[495,9,495,26,0,0,4,0],[495,9,495,26,0,0,4,1],[495,30,495,42,0,0,4,0],[495,30,495,42,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_get_global_headers","aom_codec_encode","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_get_global_headers","aom_codec_encode","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":2}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Initialize encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
	return 0;
	}
	
    
    // Get codec name (not essential for fuzzing but required by instructions)
    const char *codec_name = aom_codec_iface_name(iface);
    (void)codec_name; // Prevent unused variable warning
    
    // 2. Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0; // Good quality mode
    
    aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, usage);
    if (err != AOM_CODEC_OK) {
	return 0;
	}
	
    
    // Set some configuration from input data
    cfg.g_w = (data[0] % 64 + 1) * 16;
    cfg.g_h = (data[1] % 64 + 1) * 16;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.rc_target_bitrate = 100 + (data[2] % 900); // 100-1000 kbps
    cfg.g_error_resilient = data[3] & 1;
    cfg.g_threads = 1 + (data[4] % 4);
    
    // 3. Initialize encoder context
    aom_codec_ctx_t ctx;
    err = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Get error message (required by instructions)
        const char *error_msg = aom_codec_error(&ctx);
        (void)error_msg; // Prevent unused variable warning
        return 0;
    }
    
    // 4. Set encoder options from input data
    char option_value[32];
    snprintf(option_value, sizeof(option_value), "%d", data[5] % 10);
    err = aom_codec_set_option(&ctx, "cpu-used", option_value);
    (void)err; // We continue even if option setting fails
    
    snprintf(option_value, sizeof(option_value), "%d", 63 - (data[6] % 64));
    aom_codec_set_option(&ctx, "cq-level", option_value);
    
    // 5. Create test image using input data
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    const unsigned int width = cfg.g_w;
    const unsigned int height = cfg.g_h;
    const unsigned int align = 32;
    
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, align);
    
    // Fill image planes with data from input
    size_t data_offset = 7;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = aom_img_plane_width(&img, plane);
        int plane_height = aom_img_plane_height(&img, plane);
        unsigned char *plane_ptr = img.planes[plane];
        
        for (int y = 0; y < plane_height; ++y) {
            size_t bytes_to_copy = plane_width;
            if (data_offset + bytes_to_copy > size) {
                bytes_to_copy = size - data_offset;
                if (bytes_to_copy == 0) break;
            }
            
            if (data_offset < size) {
                memcpy(plane_ptr, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            
            plane_ptr += img.stride[plane];
        }
    }
    
    // 6. Add and remove metadata (as required by instructions)
    // First add some metadata
    uint32_t metadata_type = 1;
    uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
    aom_img_add_metadata(&img, metadata_type, metadata_data, 
                         sizeof(metadata_data), AOM_MIF_ANY_FRAME);
    
    // Get number of metadata entries
    size_t num_metadata = aom_img_num_metadata(&img);
    
    // Now remove all metadata
    if (num_metadata > 0) {
        aom_img_remove_metadata(&img);
    }
    
    // 7. Get global headers (required by instructions)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 8. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    err = aom_codec_encode(&ctx, &img, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char *error_msg = aom_codec_error(&ctx);
        (void)error_msg;
    }
    
    // 9. Use ULEB encode (required by instructions)
    uint64_t uleb_value = (data[0] << 8) | data[1];
    uint8_t coded_buffer[10];
    size_t coded_size = 0;
    
    aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_buffer), 
                               sizeof(coded_buffer), coded_buffer, &coded_size);
    
    // 10. Get encoded data packets
    const aom_codec_cx_pkt_t *pkt = NULL;
    aom_codec_iter_t iter = NULL;
    
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        switch (pkt->kind) {
            case AOM_CODEC_CX_FRAME_PKT:
                // Got an encoded frame
                break;
            case AOM_CODEC_STATS_PKT:
                // Got stats packet
                break;
            default:
                break;
        }
    }
    
    // 11. Write output to file (using FILE* as required by instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write ULEB encoded value
        if (coded_size > 0) {
            fwrite(coded_buffer, 1, coded_size, out_file);
        }
        
        // Write a simple header with image dimensions
        uint8_t header[4] = {
            (uint8_t)(width >> 8), (uint8_t)(width & 0xFF),
            (uint8_t)(height >> 8), (uint8_t)(height & 0xFF)
        };
        fwrite(header, 1, sizeof(header), out_file);
        
        assert_file_closed(&out_file);;
    }
    
    // 12. Clean up resources (in reverse order of creation)
    aom_img_free(&img);
    aom_codec_destroy(&ctx);
    
    assert_file_closed(&out_file);
	return 0;
}