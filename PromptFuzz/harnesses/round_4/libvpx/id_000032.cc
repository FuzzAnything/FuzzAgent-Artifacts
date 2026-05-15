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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 570, nr_unique_branch: 339
//<Quality> {"density":19,"unique_branches":{"vp9_block_error_avx2":[[29,7,29,23,0,0,4,0]],"vp9_setup_mask":[[913,8,913,44,0,0,4,0],[939,13,939,41,0,0,4,0],[939,45,939,73,0,0,4,0],[957,11,957,18,0,0,4,0],[958,30,958,40,0,0,4,0],[958,30,958,40,0,0,4,1],[966,19,966,47,0,0,4,0],[966,19,966,47,0,0,4,1],[966,51,966,79,0,0,4,0],[966,51,966,79,0,0,4,1],[970,17,970,33,0,0,4,0],[970,17,970,33,0,0,4,1],[973,17,973,32,0,0,4,1],[979,17,979,32,0,0,4,1],[985,17,985,24,0,0,4,0],[985,17,985,24,0,0,4,1],[989,35,989,44,0,0,4,0],[989,35,989,44,0,0,4,1],[996,25,996,52,0,0,4,1],[997,25,997,52,0,0,4,0],[997,25,997,52,0,0,4,1]],"variance4_sse2":[[113,15,113,20,0,0,4,0],[113,15,113,20,0,0,4,1]],"d117_predictor":[[86,15,86,21,0,0,4,0],[86,15,86,21,0,0,4,1],[91,15,91,21,0,0,4,0],[91,15,91,21,0,0,4,1],[96,15,96,21,0,0,4,0],[96,15,96,21,0,0,4,1],[100,15,100,21,0,0,4,0],[100,15,100,21,0,0,4,1],[101,17,101,23,0,0,4,0],[101,17,101,23,0,0,4,1]],"conditional_skipintra":[[1046,7,1046,24,0,0,4,0],[1046,7,1046,24,0,0,4,1],[1046,28,1046,53,0,0,4,0],[1046,28,1046,53,0,0,4,1],[1047,7,1047,35,0,0,4,0],[1049,7,1049,23,0,0,4,0],[1049,7,1049,23,0,0,4,1],[1049,27,1049,52,0,0,4,0],[1049,27,1049,52,0,0,4,1],[1050,7,1050,34,0,0,4,0],[1052,7,1052,24,0,0,4,0],[1052,7,1052,24,0,0,4,1],[1052,28,1052,53,0,0,4,0],[1052,28,1052,53,0,0,4,1],[1053,7,1053,34,0,0,4,0],[1055,7,1055,24,0,0,4,0],[1055,7,1055,24,0,0,4,1],[1055,28,1055,53,0,0,4,0],[1055,28,1055,53,0,0,4,1],[1056,7,1056,35,0,0,4,0]],"vp9_foreach_transformed_block":[[83,19,83,39,0,0,4,0],[83,19,83,39,0,0,4,1]],"vp9_filter_block_plane_ss11":[[1342,36,1342,60,0,0,4,1],[1384,9,1384,26,0,0,4,0]],"vp9_iht16x16_256_add_sse2":[[202,5,202,17,0,0,4,1],[206,5,206,18,0,0,4,0],[210,5,210,18,0,0,4,1],[214,5,214,12,0,0,4,1]],"set_vt_partitioning":[[485,7,485,23,0,0,4,0],[490,7,490,25,0,0,4,0],[492,9,492,32,0,0,4,0],[493,9,493,47,0,0,4,0],[493,9,493,47,0,0,4,1],[494,9,494,48,0,0,4,0],[494,9,494,48,0,0,4,1],[495,9,495,53,0,0,4,0],[495,9,495,53,0,0,4,1],[510,9,510,47,0,0,4,1],[518,9,518,48,0,0,4,0],[522,11,522,58,0,0,4,0],[523,11,523,58,0,0,4,0],[524,11,524,71,0,0,4,0],[531,9,531,47,0,0,4,1]],"right_shift_8x8":[[217,7,217,15,0,0,4,0],[217,7,217,15,0,0,4,1],[238,7,238,15,0,0,4,0],[238,7,238,15,0,0,4,1]],"write_modes_sb":[[441,7,441,28,0,0,4,0],[441,32,441,53,0,0,4,0],[448,7,448,26,0,0,4,0],[487,8,487,26,0,0,4,0]],"fill_variance_4x4avg":[[727,9,727,29,0,0,4,1]],"vp9_tokenize_sb":[[478,7,478,15,0,0,4,1],[484,7,484,15,0,0,4,0]],"tokenize_b":[[386,10,386,17,0,0,4,0],[386,10,386,17,0,0,4,1],[391,12,391,14,0,0,4,0],[391,12,391,14,0,0,4,1],[409,7,409,17,0,0,4,0],[409,7,409,17,0,0,4,1]],"vpx_satd_avx2":[[479,15,479,25,0,0,4,0],[479,15,479,25,0,0,4,1]],"rd_pick_intra_sby_mode":[[1383,24,1383,39,0,0,4,0],[1383,24,1383,39,0,0,4,1],[1384,9,1384,36,0,0,4,0],[1387,11,1387,53,0,0,4,0],[1387,11,1387,53,0,0,4,1],[1388,11,1388,21,0,0,4,0],[1388,11,1388,21,0,0,4,1],[1396,9,1396,39,0,0,4,0],[1396,9,1396,39,0,0,4,1],[1401,9,1401,26,0,0,4,0],[1401,9,1401,26,0,0,4,1]],"build_y_mask":[[740,7,740,20,0,0,4,1],[746,17,746,22,0,0,4,0],[746,17,746,22,0,0,4,1],[755,7,755,15,0,0,4,0],[755,7,755,15,0,0,4,1],[755,19,755,37,0,0,4,1],[763,7,763,26,0,0,4,0],[763,7,763,26,0,0,4,1]],"super_block_yrd":[[1031,22,1031,26,0,0,4,1],[1035,7,1035,54,0,0,4,0]],"vp9_set_contexts":[[95,7,95,14,0,0,4,0],[95,7,95,14,0,0,4,1],[95,18,95,42,0,0,4,1],[110,7,110,14,0,0,4,0],[110,7,110,14,0,0,4,1],[110,18,110,43,0,0,4,1]],"vp9_iht8x8_64_add_sse2":[[68,5,68,17,0,0,4,1],[72,5,72,18,0,0,4,0],[72,5,72,18,0,0,4,1],[76,5,76,18,0,0,4,0],[76,5,76,18,0,0,4,1],[80,5,80,12,0,0,4,0],[80,5,80,12,0,0,4,1]],"vp9_rd_pick_intra_mode_sb":[[3239,7,3239,25,0,0,4,0],[3239,7,3239,25,0,0,4,1],[3240,9,3241,67,0,0,4,1],[3247,9,3248,66,0,0,4,1],[3258,7,3258,13,0,0,4,0],[3258,7,3258,13,0,0,4,1],[3258,17,3258,24,0,0,4,0],[25,23,25,34,5,0,4,0],[25,23,25,34,5,0,4,1]],"choose_largest_tx_size":[[24,23,24,34,1,0,4,0]],"rd_pick_intra_sbuv_mode":[[1481,24,1481,39,0,0,4,0],[1481,24,1481,39,0,0,4,1],[1482,9,1482,65,0,0,4,0],[1482,9,1482,65,0,0,4,1],[1491,9,1492,53,0,0,4,1],[1499,9,1499,26,0,0,4,0],[1506,11,1506,29,0,0,4,1]],"block_rd_txfm":[[734,7,734,23,0,0,4,1],[736,7,736,26,0,0,4,0],[739,9,739,14,0,0,4,1],[744,9,744,27,0,0,4,1],[752,11,752,25,0,0,4,0],[762,11,762,56,0,0,4,1],[821,7,821,41,0,0,4,0],[821,7,821,41,0,0,4,1],[827,28,827,61,0,0,4,0],[827,28,827,61,0,0,4,1],[828,27,828,60,0,0,4,0],[828,27,828,60,0,0,4,1],[834,7,834,17,0,0,4,0],[834,7,834,17,0,0,4,1],[836,9,836,37,0,0,4,0],[836,9,836,37,0,0,4,1],[837,10,837,27,0,0,4,0],[837,31,837,40,0,0,4,0],[837,31,837,40,0,0,4,1],[837,44,837,57,0,0,4,0],[846,7,846,36,0,0,4,0],[846,7,846,36,0,0,4,1],[24,23,24,34,16,0,4,0],[24,23,24,34,16,0,4,1]],"vp9_fht4x4_sse2":[[161,5,161,17,0,0,4,0],[161,5,161,17,0,0,4,1],[162,5,162,18,0,0,4,0],[162,5,162,18,0,0,4,1],[168,5,168,18,0,0,4,0],[168,5,168,18,0,0,4,1],[174,5,174,12,0,0,4,1]],"d135_predictor":[[118,15,118,25,0,0,4,0],[118,15,118,25,0,0,4,1],[125,15,125,25,0,0,4,0],[125,15,125,25,0,0,4,1],[129,15,129,21,0,0,4,0],[129,15,129,21,0,0,4,1]],"super_block_uvrd":[[1431,7,1431,22,0,0,4,1],[1433,7,1433,25,0,0,4,1],[1443,19,1443,39,0,0,4,0],[1443,19,1443,39,0,0,4,1],[1447,9,1447,26,0,0,4,1],[1457,7,1457,21,0,0,4,1]],"vp9_iht8x8_add":[[192,7,192,25,0,0,4,0],[192,7,192,25,0,0,4,1]],"vp9_iht16x16_add":[[201,7,201,25,0,0,4,1]],"filter_selectively_horiz":[[474,18,474,30,0,0,4,1],[503,18,503,30,0,0,4,1]],"build_intra_predictors":[[337,13,337,20,0,0,4,0],[337,24,337,39,0,0,4,0],[337,24,337,39,0,0,4,1],[337,43,337,57,0,0,4,0],[337,43,337,57,0,0,4,1],[351,7,351,43,0,0,4,0],[352,9,352,21,0,0,4,0],[352,9,352,21,0,0,4,1],[354,11,354,35,0,0,4,1],[379,13,379,20,0,0,4,1],[383,15,383,22,0,0,4,1],[389,23,389,37,0,0,4,1]],"nonrd_pick_sb_modes":[[25,23,25,34,1,0,4,1]],"filter_selectively_vert_row2":[[340,11,340,30,0,0,4,0],[341,13,341,46,0,0,4,1],[352,11,352,34,0,0,4,0],[353,13,353,50,0,0,4,1]],"quantize_b_16":[[91,7,91,45,0,0,4,1]],"vp9_rc_update_rate_correction_factors":[[797,7,797,30,0,0,4,0],[805,7,805,31,0,0,4,0],[805,35,805,58,0,0,4,1],[810,7,810,30,0,0,4,0],[816,9,816,48,0,0,4,1],[825,9,825,48,0,0,4,1]],"vpx_sum_squares_2d_i16_sse2":[[19,7,19,16,0,0,4,0],[19,7,19,16,0,0,4,1],[84,16,84,24,0,0,4,1],[91,14,91,15,0,0,4,1]],"rd_pick_intra_sub_8x8_y_mode":[[1319,17,1319,24,0,0,4,0],[1319,17,1319,24,0,0,4,1],[1320,19,1320,26,0,0,4,0],[1320,19,1320,26,0,0,4,1],[1325,11,1325,46,0,0,4,0],[1337,11,1337,40,0,0,4,1],[1345,19,1345,42,0,0,4,1],[1347,19,1347,42,0,0,4,1],[1350,11,1350,30,0,0,4,1]],"vp9_get_entropy_contexts":[[559,5,559,16,0,0,4,0],[559,5,559,16,0,0,4,1],[563,5,563,16,0,0,4,0],[563,5,563,16,0,0,4,1],[564,19,564,32,0,0,4,0],[564,19,564,32,0,0,4,1],[566,19,566,32,0,0,4,0],[566,19,566,32,0,0,4,1],[569,5,569,18,0,0,4,1],[575,5,575,12,0,0,4,1]],"block_yrd":[[844,18,844,26,0,0,4,0]],"vp9_encode_block_intra":[[894,7,894,24,0,0,4,0],[897,12,897,22,0,0,4,0],[897,12,897,22,0,0,4,1],[1045,30,1045,34,0,0,4,0],[1057,30,1057,34,0,0,4,0],[1060,5,1060,12,0,0,4,0],[1062,11,1062,26,0,0,4,0],[1063,13,1063,31,0,0,4,0],[1063,13,1063,31,0,0,4,1],[1070,11,1070,29,0,0,4,1],[1073,11,1073,26,0,0,4,0],[1073,30,1073,34,0,0,4,0],[1073,30,1073,34,0,0,4,1],[1074,13,1074,31,0,0,4,0],[1074,13,1074,31,0,0,4,1],[1084,7,1084,11,0,0,4,0]],"write_mb_modes_kf":[[356,7,356,25,0,0,4,1],[359,7,359,25,0,0,4,1],[366,19,366,26,0,0,4,0],[366,19,366,26,0,0,4,1],[367,21,367,28,0,0,4,0],[367,21,367,28,0,0,4,1]],"build_masks":[[699,7,699,15,0,0,4,1],[720,7,720,26,0,0,4,0],[722,7,722,27,0,0,4,0]],"hybrid_intra_mode_search":[[4350,34,4350,53,0,0,4,0]],"write_partition":[[417,27,417,35,0,0,4,1],[420,14,420,22,0,0,4,1]],"encode_superblock":[[6120,23,6120,43,0,0,4,1],[25,23,25,34,3,0,4,1],[25,23,25,34,5,0,4,1]],"variance8_avx2":[[165,15,165,20,0,0,4,0],[165,15,165,20,0,0,4,1]],"choose_partitioning":[[1390,7,1390,31,0,0,4,0],[1645,13,1645,64,0,0,4,0],[1657,9,1657,28,0,0,4,1],[1662,11,1662,67,0,0,4,0],[1677,7,1677,22,0,0,4,1],[1698,7,1698,31,0,0,4,0],[1705,11,1708,51,0,0,4,0],[1709,21,1709,26,0,0,4,0],[1709,21,1709,26,0,0,4,1],[1715,28,1715,41,0,0,4,1],[1718,15,1721,43,0,0,4,0],[1718,15,1721,43,0,0,4,1],[1722,25,1722,30,0,0,4,0],[1722,25,1722,30,0,0,4,1],[1725,19,1725,36,0,0,4,0],[1726,21,1730,70,0,0,4,0],[1726,21,1730,70,0,0,4,1],[25,23,25,34,45,0,4,0]],"vp9_iht4x4_16_add_sse2":[[23,5,23,17,0,0,4,1],[27,5,27,18,0,0,4,1],[31,5,31,18,0,0,4,0],[35,5,35,12,0,0,4,1]],"txfm_rd_in_plane":[[870,7,870,17,0,0,4,0],[870,7,870,17,0,0,4,1],[878,7,878,22,0,0,4,0],[878,7,878,22,0,0,4,1]],"vp9_fht8x8_sse2":[[647,5,647,17,0,0,4,1],[648,5,648,18,0,0,4,0],[655,5,655,18,0,0,4,0],[662,5,662,12,0,0,4,0]],"vp9_idct8x8_add":[[142,7,142,15,0,0,4,0],[142,7,142,15,0,0,4,1],[145,12,145,21,0,0,4,1]],"pack_mb_tokens":[[147,29,147,51,0,0,4,0],[148,9,148,30,0,0,4,0],[148,9,148,30,0,0,4,1],[153,12,153,34,0,0,4,0],[153,12,153,34,0,0,4,1],[156,11,156,20,0,0,4,1],[156,24,156,46,0,0,4,1],[169,11,169,25,0,0,4,0],[169,11,169,25,0,0,4,1],[181,13,181,33,0,0,4,0],[181,13,181,33,0,0,4,1],[189,20,189,21,0,0,4,0],[189,20,189,21,0,0,4,1]],"vp9_idct4x4_add":[[121,7,121,14,0,0,4,0],[121,7,121,14,0,0,4,1]],"vpx_idct8x8_64_add_sse2":[[167,15,167,20,0,0,4,0],[167,15,167,20,0,0,4,1]],"vp9_fht16x16_sse2":[[1513,5,1513,17,0,0,4,1],[1514,5,1514,18,0,0,4,0]],"set_block_size":[[338,7,338,35,0,0,4,1]],"vp9_iht4x4_add":[[184,7,184,25,0,0,4,0],[184,7,184,25,0,0,4,1]],"pixel_sse":[[538,7,538,28,0,0,4,0],[538,7,538,28,0,0,4,1],[539,8,539,39,0,0,4,0],[539,43,539,75,0,0,4,0]],"nonrd_use_partition":[[5007,7,5007,28,0,0,4,0],[5007,32,5007,53,0,0,4,0],[5077,11,5077,29,0,0,4,0],[5098,39,5098,57,0,0,4,0]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get codec interfaces
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Use vpx_codec_iface_name
    const char *enc_name = vpx_codec_iface_name(encoder_iface);
    const char *dec_name = vpx_codec_iface_name(decoder_iface);
    (void)enc_name; (void)dec_name; // Prevent unused variable warnings

    // 4. Get codec capabilities
    vpx_codec_caps_t enc_caps = vpx_codec_get_caps(encoder_iface);
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(decoder_iface);
    (void)enc_caps; (void)dec_caps; // Prevent unused variable warnings

    // 5. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Decode input data
    res = vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 7. Get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 8. Create image for encoding
        vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                         decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_3);
        if (img) {
            // Copy decoded image data
            for (int plane = 0; plane < 3; ++plane) {
                int plane_width = (plane == 0) ? decoded_img->d_w : decoded_img->d_w / 2;
                int plane_height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
                for (int y = 0; y < plane_height; ++y) {
                    memcpy(img->planes[plane] + y * img->stride[plane],
                           decoded_img->planes[plane] + y * decoded_img->stride[plane],
                           plane_width);
                }
            }

            // 9. Use vpx_img_set_rect to set a rectangle
            unsigned int rect_w = (img->d_w > 10) ? 10 : img->d_w;
            unsigned int rect_h = (img->d_h > 10) ? 10 : img->d_h;
            vpx_img_set_rect(img, fuzz_uint32_t_4, fuzz_uint32_t_5, rect_w, rect_h);

            // 10. Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_enc_cfg_t enc_cfg;
            
            res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_6);
            if (res == VPX_CODEC_OK) {
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_pass = VPX_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                
                res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
                if (res == VPX_CODEC_OK) {
                    // 11. Encode the image
                    res = vpx_codec_encode(&encoder, img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
                    
                    if (res == VPX_CODEC_OK) {
                        // Get encoded data
                        vpx_codec_iter_t enc_iter = NULL;
                        const vpx_codec_cx_pkt_t *pkt;
                        
                        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                // Write to output file
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                    }
                    
                    vpx_codec_destroy(&encoder);
                }
            }
            
            vpx_img_free(img);
        }
    }

    // 12. Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}