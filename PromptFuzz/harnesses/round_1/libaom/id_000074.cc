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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 503.5, nr_unique_branch: 221
//<Quality> {"density":19,"unique_branches":{"av1_remove_compressor":[[1825,7,1825,32,0,0,4,0],[1829,7,1829,31,0,0,4,0],[1833,7,1833,27,0,0,4,0],[1837,7,1837,31,0,0,4,0],[1841,7,1841,32,0,0,4,0],[1848,7,1848,31,0,0,4,0]],"av1_init_tile_thread_data":[[971,33,971,39,0,0,4,0],[971,33,971,39,0,0,4,1],[974,9,974,14,0,0,4,0],[974,9,974,14,0,0,4,1],[988,11,988,49,0,0,4,1],[1002,11,1002,25,0,0,4,0],[1002,29,1002,48,0,0,4,1],[1070,9,1070,23,0,0,4,0],[1070,27,1070,53,0,0,4,0],[1070,57,1070,76,0,0,4,0],[1070,57,1070,76,0,0,4,1],[1071,11,1071,17,0,0,4,0],[1072,25,1072,49,0,0,4,0],[1072,25,1072,49,0,0,4,1],[74,9,74,14,1,0,4,1],[74,9,74,14,3,0,4,1],[74,9,74,14,4,0,4,1],[74,9,74,14,12,0,4,1]],"sync_write":[[237,7,237,22,0,0,4,0],[239,9,239,18,0,0,4,0],[239,9,239,18,0,0,4,1]],"decode_reconstruct_tx":[[299,37,299,63,0,0,4,0]],"av1_init_mt_sync":[[855,7,855,20,0,0,4,0],[855,7,855,20,0,0,4,1],[855,24,855,45,0,0,4,0],[857,9,857,35,0,0,4,0],[860,11,860,29,0,0,4,0],[862,9,862,34,0,0,4,0],[865,11,865,28,0,0,4,0],[869,7,869,21,0,0,4,0],[869,7,869,21,0,0,4,1],[872,9,872,32,0,0,4,0],[875,11,875,26,0,0,4,0],[880,9,880,32,0,0,4,0],[883,11,883,26,0,0,4,0],[888,9,888,34,0,0,4,0],[891,11,891,28,0,0,4,0],[902,9,902,29,0,0,4,0],[910,9,910,35,0,0,4,0],[913,11,913,29,0,0,4,0],[917,9,917,32,0,0,4,0],[924,11,924,31,0,0,4,0],[936,9,936,37,0,0,4,0],[939,11,939,31,0,0,4,0],[74,9,74,14,9,0,4,1],[74,9,74,14,10,0,4,1],[74,9,74,14,11,0,4,1],[74,9,74,14,12,0,4,1],[74,9,74,14,13,0,4,1],[74,9,74,14,14,0,4,1],[74,9,74,14,15,0,4,1]],"av1_tf_mt_dealloc":[[2373,7,2373,30,0,0,4,0],[2373,7,2373,30,0,0,4,1]],"dealloc_thread_data_src_diff_buf":[[1996,33,1996,39,0,0,4,0],[1996,33,1996,39,0,0,4,1],[1998,9,1998,36,0,0,4,1]],"get_next_job_info":[[3182,13,3182,54,0,0,4,1],[3187,13,3187,50,0,0,4,1],[3188,13,3189,75,0,0,4,1]],"fp_compute_max_mb_rows":[[1841,21,1841,36,0,0,4,0],[1841,21,1841,36,0,0,4,1],[34,23,34,34,1,0,4,1],[35,23,35,34,3,0,4,1]],"sync_enc_workers":[[1507,7,1507,16,0,0,4,1],[1512,33,1512,38,0,0,4,1],[1520,7,1520,16,0,0,4,1]],"av1_cdef_mt_dealloc":[[3256,7,3256,32,0,0,4,0],[3256,7,3256,32,0,0,4,1]],"compute_num_enc_row_mt_workers":[[1704,21,1704,36,0,0,4,0],[1704,21,1704,36,0,0,4,1],[1705,23,1705,38,0,0,4,0],[1705,23,1705,38,0,0,4,1],[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,1]],"av1_set_speed_features_framesize_independent":[[2709,7,2709,35,0,0,4,0],[2718,9,2718,19,0,0,4,0],[2807,32,2807,62,0,0,4,0],[2808,9,2808,57,0,0,4,0],[2817,9,2817,54,0,0,4,0],[2818,9,2819,66,0,0,4,1]],"av1_terminate_workers":[[1132,19,1132,45,0,0,4,0]],"get_next_job":[[326,7,326,44,0,0,4,0],[326,7,326,44,0,0,4,1]],"row_mt_mem_alloc":[[253,22,253,42,0,0,4,0],[253,22,253,42,0,0,4,1],[254,24,254,44,0,0,4,0],[254,24,254,44,0,0,4,1],[260,11,260,24,0,0,4,1],[74,9,74,14,5,0,4,1]],"encoder_encode":[[3521,9,3521,24,0,0,4,0],[3521,28,3521,68,0,0,4,0],[3524,11,3524,41,0,0,4,0],[3527,11,3527,41,0,0,4,1],[3548,9,3548,62,0,0,4,1],[3565,9,3565,39,0,0,4,0],[3566,23,3566,47,0,0,4,0],[3566,23,3566,47,0,0,4,1],[3570,11,3570,26,0,0,4,0]],"lr_sync_write":[[558,7,558,22,0,0,4,0],[560,9,560,18,0,0,4,1],[565,7,565,10,0,0,4,1]],"compute_num_enc_workers":[[1736,7,1736,23,0,0,4,1],[1737,7,1737,23,0,0,4,0]],"get_sync_range":[[37,7,37,18,0,0,4,1],[39,12,39,25,0,0,4,1],[41,12,41,25,0,0,4,0]],"assign_tile_to_thread":[[315,15,315,30,0,0,4,0],[315,15,315,30,0,0,4,1],[317,9,317,29,0,0,4,0]],"av1_fp_encode_tiles_row_mt":[[2015,33,2015,77,0,0,4,0],[2022,7,2022,22,0,0,4,1],[2027,7,2027,23,0,0,4,0],[2035,7,2035,44,0,0,4,1],[2044,26,2044,46,0,0,4,0],[2044,26,2044,46,0,0,4,1],[2045,28,2045,48,0,0,4,0],[2045,28,2045,48,0,0,4,1],[34,23,34,34,2,0,4,0]],"av1_set_frame_size":[[2701,9,2701,44,0,0,4,0]],"fp_enc_row_mt_worker_hook":[[482,9,482,27,0,0,4,0],[482,31,483,74,0,0,4,0],[482,31,483,74,0,0,4,1],[495,9,495,26,0,0,4,1],[495,30,495,42,0,0,4,0],[495,30,495,42,0,0,4,1]],"av1_row_mt_sync_write":[[148,7,148,19,0,0,4,0],[148,7,148,19,0,0,4,1],[150,9,150,18,0,0,4,1],[155,7,155,10,0,0,4,0],[35,23,35,34,1,0,4,1]],"av1_thread_loop_filter_rows":[[324,22,324,46,0,0,4,1]],"fp_prepare_enc_workers":[[1672,33,1672,39,0,0,4,0],[1672,33,1672,39,0,0,4,1],[1685,9,1685,15,0,0,4,0]],"lr_sync_read":[[532,7,532,8,0,0,4,1],[532,12,532,30,0,0,4,1]],"read_inter_segment_id":[[391,9,391,24,0,0,4,0],[392,11,392,31,0,0,4,1],[408,9,408,31,0,0,4,0]],"av1_highbd_warp_affine_avx2":[[109,15,109,21,0,0,4,0]],"enqueue_lr_jobs":[[712,9,712,67,0,0,4,0],[720,9,720,67,0,0,4,0],[34,23,34,34,6,0,4,1]],"av1_row_mt_sync_read":[[123,7,123,8,0,0,4,0],[123,7,123,8,0,0,4,1],[127,12,128,65,0,0,4,1]],"av1_get_reference_mode_context":[[142,4,142,33,8,0,4,0],[142,37,142,66,8,0,4,0]],"av1_init_lr_mt_buffers":[[834,7,834,26,0,0,4,1]],"read_ref_frames":[[969,17,969,21,0,0,4,1]],"av1_row_mt_mem_dealloc":[[288,22,288,42,0,0,4,0],[289,24,289,44,0,0,4,0],[289,24,289,44,0,0,4,1],[295,11,295,45,0,0,4,0]],"av1_compute_num_fp_contexts":[[1224,7,1224,40,0,0,4,0]],"compute_num_enc_tile_mt_workers":[[34,23,34,34,1,0,4,1]],"build_non_directional_intra_predictors":[[1283,7,1283,16,0,0,4,1],[1293,7,1293,17,0,0,4,1]],"row_mt_sync_mem_alloc":[[185,7,185,26,0,0,4,0],[186,17,186,25,0,0,4,0],[186,17,186,25,0,0,4,1],[193,7,193,25,0,0,4,0],[194,17,194,25,0,0,4,0],[194,17,194,25,0,0,4,1],[74,9,74,14,4,0,4,1],[74,9,74,14,5,0,4,1],[74,9,74,14,6,0,4,1]],"av1_row_mt_sync_mem_dealloc":[[210,7,210,26,0,0,4,0],[214,9,214,36,0,0,4,0],[214,9,214,36,0,0,4,1],[215,19,215,40,0,0,4,0],[215,19,215,40,0,0,4,1],[220,9,220,35,0,0,4,0],[220,9,220,35,0,0,4,1],[221,19,221,40,0,0,4,0],[221,19,221,40,0,0,4,1]],"launch_workers":[[1489,33,1489,39,0,0,4,0],[1489,33,1489,39,0,0,4,1],[1492,9,1492,15,0,0,4,0]],"av1_get_comp_reference_type_context":[[220,13,220,21,0,0,4,1],[142,4,142,33,5,0,4,0],[142,37,142,66,5,0,4,0],[142,4,142,33,7,0,4,0],[142,37,142,66,7,0,4,0],[142,4,142,33,8,0,4,0],[142,37,142,66,8,0,4,0]],"foreach_rest_unit_in_planes_mt":[[934,9,934,67,0,0,4,0],[35,23,35,34,1,0,4,0]],"av1_decode_tg_tiles_and_wrapup":[[5471,13,5471,33,0,0,4,0]],"compute_num_pack_bs_workers":[[3427,7,3427,33,0,0,4,1]],"compute_num_ai_workers":[[3433,7,3433,33,0,0,4,1],[3436,7,3436,24,0,0,4,1],[34,23,34,34,1,0,4,1]],"av1_first_pass_row":[[1234,9,1234,44,0,0,4,0],[1239,11,1239,28,0,0,4,1]],"av1_foreach_rest_unit_in_row":[[1243,20,1243,44,0,0,4,1]],"av1_first_pass":[[1433,7,1433,31,0,0,4,0],[1451,7,1451,51,0,0,4,0]],"av1_create_workers":[[1102,19,1102,34,0,0,4,0],[1102,19,1102,34,0,0,4,1],[1113,9,1113,14,0,0,4,0],[1113,9,1113,14,0,0,4,1],[1115,11,1115,37,0,0,4,1],[74,9,74,14,1,0,4,1],[74,9,74,14,2,0,4,1]],"switch_tile_and_get_next_job":[[346,26,346,46,0,0,4,0],[346,26,346,46,0,0,4,1],[347,28,347,48,0,0,4,0],[347,28,347,48,0,0,4,1],[359,11,359,23,0,0,4,0],[363,11,363,23,0,0,4,0],[371,11,371,61,0,0,4,0],[382,13,382,34,0,0,4,1],[396,7,396,20,0,0,4,0],[34,23,34,34,1,0,4,0]]},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy","aom_uleb_encode","aom_img_alloc","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy","aom_uleb_encode","aom_img_alloc","aom_img_free"],"visited":1}
/**/


// Include necessary libaom headers (assumed to be available)
extern "C" {
    #include "aom/aom.h"
    #include "aom/aom_codec.h"
    #include "aom/aom_decoder.h"
    #include "aom/aom_encoder.h"
    #include "aom/aom_image.h"
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // 1. Get version string
    const char *version = aom_codec_version_str();
    (void)version; // Prevent unused variable warning

    // 2. Try to decode a ULEB value from the beginning of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, std::min<size_t>(size, 10), &uleb_value, &uleb_length);
    }

    // 3. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t dec_init_res = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION
    );

    if (dec_init_res == AOM_CODEC_OK) {
        // 4. Try to decode the input as AV1 bitstream
        aom_codec_decode(&decoder, data, size, nullptr);

        // 5. Get stream information
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_get_stream_info(&decoder, &stream_info);

        // 6. Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
        
        if (img) {
            // 7. Get image plane information
            int plane_width = aom_img_plane_width(img, 0);
            int plane_height = aom_img_plane_height(img, 0);
            (void)plane_width;
            (void)plane_height;

            // 8. Get metadata count
            size_t metadata_count = aom_img_num_metadata(img);
            (void)metadata_count;

            // 9. Create encoder context
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_threads = 2;
                enc_cfg.rc_target_bitrate = 1000;
                
                // 10. Initialize encoder
                aom_codec_enc_init_ver(
                    &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION
                );

                // 11. Set an encoder option
                aom_codec_set_option(&encoder, "cpu-used", "8");

                // 12. Prepare output buffer for encoded data
                const size_t output_buf_size = 1024 * 1024;
                std::vector<uint8_t> output_buffer(output_buf_size);
                aom_fixed_buf_t output_buf;
                output_buf.buf = output_buffer.data();
                output_buf.sz = output_buf_size;

                // 13. Set output buffer for encoder
                aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);

                // 14. Encode the image
                aom_codec_encode(&encoder, img, 0, 1, 0);

                // 15. Get encoded data packets
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process packet if needed
                    (void)pkt;
                }

                // 16. Destroy encoder
                aom_codec_destroy(&encoder);
            }
        }

        // 17. Destroy decoder
        aom_codec_destroy(&decoder);
    }

    // 18. Try to encode the ULEB value we decoded earlier
    uint8_t encoded_uleb[10];
    size_t encoded_size = 0;
    aom_uleb_encode(uleb_value, sizeof(encoded_uleb), encoded_uleb, &encoded_size);

    // 19. Create an image using image allocation
    aom_image_t allocated_img;
    memset(&allocated_img, 0, sizeof(allocated_img));
    aom_image_t *img_ptr = aom_img_alloc(
        &allocated_img, AOM_IMG_FMT_I420, 64, 64, 16
    );
    
    if (img_ptr) {
        // Use the allocated image if needed
        aom_img_free(img_ptr);
    }

    return 0;
}