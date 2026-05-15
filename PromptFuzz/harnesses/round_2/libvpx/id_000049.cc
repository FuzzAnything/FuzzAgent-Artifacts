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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 238, nr_unique_branch: 152
//<Quality> {"density":28,"unique_branches":{"vp9_set_contexts":[[115,9,115,43,0,0,4,1]],"thread_loop_filter_rows":[[78,26,78,32,0,0,4,1],[83,7,83,13,0,0,4,1],[85,12,85,40,0,0,4,0],[85,44,85,72,0,0,4,0],[111,23,111,41,0,0,4,0],[112,17,112,21,0,0,4,1],[113,11,113,27,0,0,4,0],[116,11,116,27,0,0,4,1],[119,11,119,28,0,0,4,1]],"vp9_setup_mask":[[966,19,966,47,0,0,4,0]],"write_modes_sb":[[486,7,486,25,0,0,4,1]],"write_modes_b":[[395,7,395,30,0,0,4,1]],"vp9_initialize_rd_consts":[[428,9,428,61,0,0,4,1],[429,9,429,36,0,0,4,0]],"encode_tiles_mt":[[1016,7,1016,38,0,0,4,0],[1022,10,1022,30,0,0,4,0],[1022,10,1022,30,0,0,4,1],[1024,17,1024,32,0,0,4,0],[1024,17,1024,32,0,0,4,1],[1024,36,1024,56,0,0,4,0],[1036,11,1036,17,0,0,4,0],[1036,11,1036,17,0,0,4,1],[1039,45,1039,70,0,0,4,1],[1040,13,1040,31,0,0,4,1],[1052,11,1052,30,0,0,4,0],[1052,11,1052,30,0,0,4,1],[1059,17,1059,22,0,0,4,0],[1059,17,1059,22,0,0,4,1],[1066,11,1066,36,0,0,4,1],[1076,19,1076,33,0,0,4,0],[1076,19,1076,33,0,0,4,1],[1081,11,1081,32,0,0,4,1],[1081,36,1081,45,0,0,4,0],[1081,36,1081,45,0,0,4,1],[1082,13,1082,39,0,0,4,1],[1089,11,1089,16,0,0,4,0],[1089,11,1089,16,0,0,4,1],[1090,13,1090,47,0,0,4,1],[1098,9,1098,14,0,0,4,1],[25,23,25,34,2,0,4,1]],"vp9_encode_tiles_mt":[[222,9,222,36,0,0,4,0],[230,19,230,35,0,0,4,0],[230,19,230,35,0,0,4,1]],"vp9_set_variance_partition_thresholds":[[642,7,642,55,0,0,4,1]],"block_yrd":[[742,20,742,45,0,0,4,1],[744,20,744,46,0,0,4,1],[799,11,799,24,0,0,4,1],[836,11,836,30,0,0,4,1]],"choose_partitioning":[[1311,24,1311,40,0,0,4,1],[1333,7,1333,27,0,0,4,0],[1338,7,1338,29,0,0,4,1],[1388,22,1388,43,0,0,4,1],[1390,7,1390,31,0,0,4,0],[1391,7,1391,32,0,0,4,0],[1552,7,1552,14,0,0,4,1],[1662,11,1662,67,0,0,4,0],[1698,7,1698,31,0,0,4,0],[1698,35,1698,59,0,0,4,0],[1761,7,1761,10,0,0,4,1]],"set_segment_index":[[178,7,178,20,0,0,4,1],[181,5,181,27,0,0,4,1],[185,5,185,21,0,0,4,1],[204,5,204,23,0,0,4,1],[211,5,211,22,0,0,4,1],[214,5,214,17,0,0,4,1],[215,5,215,23,0,0,4,1],[216,5,216,12,0,0,4,1]],"encode_tiles_buffer_alloc":[[998,15,998,35,0,0,4,0],[998,15,998,35,0,0,4,1],[440,9,440,16,1,0,4,1],[440,9,440,16,2,0,4,1]],"block_rd_txfm":[[837,10,837,27,0,0,4,1]],"set_encoder_config":[[621,7,621,34,0,0,4,1]],"encode_tiles":[[1121,37,1121,57,0,0,4,0],[1121,61,1121,75,0,0,4,0],[1122,7,1122,20,0,0,4,0]],"loopfilter_frame":[[3462,9,3462,29,0,0,4,0]],"filter_selectively_horiz":[[499,15,499,31,0,0,4,0],[511,15,511,38,0,0,4,1],[516,17,516,33,0,0,4,0],[516,17,516,33,0,0,4,1],[519,22,519,38,0,0,4,1],[527,15,527,31,0,0,4,1]],"update_state_rt":[[2439,7,2439,29,0,0,4,0],[2459,22,2459,36,0,0,4,0],[2459,40,2459,52,0,0,4,0]],"set_vt_partitioning":[[492,9,492,32,0,0,4,1],[493,9,493,47,0,0,4,1],[494,9,494,48,0,0,4,1],[500,14,500,31,0,0,4,1],[510,9,510,47,0,0,4,1],[511,9,511,48,0,0,4,1],[518,9,518,48,0,0,4,0],[518,9,518,48,0,0,4,1],[522,11,522,58,0,0,4,0],[523,11,523,58,0,0,4,0],[524,11,524,71,0,0,4,0],[531,9,531,47,0,0,4,0],[531,9,531,47,0,0,4,1],[535,11,535,58,0,0,4,0],[536,11,536,58,0,0,4,0],[537,11,537,71,0,0,4,0]],"fill_variance_4x4avg":[[727,9,727,29,0,0,4,1],[727,33,727,53,0,0,4,1]],"pack_mb_tokens":[[147,17,147,25,0,0,4,1],[156,11,156,20,0,0,4,0],[156,24,156,46,0,0,4,0]],"set_block_size":[[338,7,338,35,0,0,4,1],[338,39,338,67,0,0,4,1]],"encode_nonrd_sb_row":[[5304,7,5304,31,0,0,4,0],[5328,7,5328,14,0,0,4,1]],"encode_superblock":[[6038,7,6038,22,0,0,4,1],[6049,7,6049,26,0,0,4,1],[6067,19,6067,40,0,0,4,1],[6073,11,6073,39,0,0,4,1],[6129,9,6129,28,0,0,4,1],[6129,32,6129,63,0,0,4,1],[25,23,25,34,7,0,4,1],[25,23,25,34,8,0,4,1],[25,23,25,34,9,0,4,1]],"vp9_above_block_mode":[[28,22,28,46,0,0,4,0]],"vp9_bitstream_encode_tiles_buffer_dealloc":[[969,7,969,37,0,0,4,0],[971,17,971,37,0,0,4,0],[971,17,971,37,0,0,4,1]],"vp9_encode_intra_block_plane":[[1117,7,1117,25,0,0,4,1]],"nonrd_use_partition":[[5007,7,5007,28,0,0,4,0],[5007,32,5007,53,0,0,4,0],[5009,13,5009,33,0,0,4,1],[5012,7,5012,21,0,0,4,1],[5012,25,5012,43,0,0,4,1],[5051,5,5051,24,0,0,4,0],[5062,11,5062,37,0,0,4,1]],"estimate_block_intra":[[1083,7,1083,17,0,0,4,1]],"vp9_filter_block_plane_ss11":[[1342,36,1342,60,0,0,4,1],[1381,36,1381,60,0,0,4,1],[1384,9,1384,26,0,0,4,0]],"nonrd_pick_sb_modes":[[4421,7,4421,30,0,0,4,1],[4423,12,4423,75,0,0,4,1],[4426,12,4426,69,0,0,4,1],[4428,12,4428,30,0,0,4,1]],"set_rt_speed_feature_framesize_independent":[[662,7,662,17,0,0,4,0],[663,9,663,37,0,0,4,0],[663,41,663,68,0,0,4,1],[672,9,672,27,0,0,4,0],[675,11,675,48,0,0,4,0],[676,11,676,43,0,0,4,0],[677,12,677,25,0,0,4,0],[685,9,685,37,0,0,4,1],[689,9,689,35,0,0,4,1],[694,9,694,36,0,0,4,1],[440,9,440,16,24,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_control_","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":3}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode video frames from input data and then re-encode them:

*/


extern "C" {
}

// Function to read data from FILE stream
size_t  id_000049_read_data(FILE* file, uint8_t* buffer, size_t size) {
    return fread(buffer, 1, size, file);
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Variables to be cleaned up
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t *allocated_image = nullptr;
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_cx_pkt_t *cx_packet = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    // API 1: Get codec interface name
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Prevent unused variable warning

    // API 2: Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Prevent unused variable warning

    // Read input data
    uint8_t *encoded_data = (uint8_t *)malloc(size);
    if (!encoded_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read =  id_000049_read_data(in_file, encoded_data, size);
    assert_file_closed(&in_file);;

    if (bytes_read == 0) {
        free(encoded_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // API 3: Peek stream info
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, encoded_data, bytes_read, &stream_info);

    if (peek_result != VPX_CODEC_OK) {
        free(encoded_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        free(encoded_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the frame
    if (vpx_codec_decode(&decoder, encoded_data, bytes_read, nullptr, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        free(encoded_data);
        assert_file_closed(&in_file);
	return 0;
    }

    free(encoded_data);

    // API 4: Get decoded frame
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // API 5: Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    (void)preview_frame; // Prevent unused variable warning

    if (!decoded_frame) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // API 6: Allocate image using vpx_img_alloc
    allocated_image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                   decoded_frame->d_w, decoded_frame->d_h, 1);
    if (!allocated_image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Copy decoded frame to allocated image
    for (int plane = 0; plane < 3; ++plane) {
        const uint8_t *src_plane = decoded_frame->planes[plane];
        uint8_t *dst_plane = allocated_image->planes[plane];
        int src_stride = decoded_frame->stride[plane];
        int dst_stride = allocated_image->stride[plane];
        int plane_height = (plane == 0) ? decoded_frame->d_h : decoded_frame->d_h / 2;
        
        for (int y = 0; y < plane_height; ++y) {
            memcpy(dst_plane, src_plane, decoded_frame->d_w / (plane == 0 ? 1 : 2));
            src_plane += src_stride;
            dst_plane += dst_stride;
        }
    }

    // Initialize encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg = {0};
    
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(allocated_image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = decoded_frame->d_w;
    enc_cfg.g_h = decoded_frame->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.kf_mode = VPX_KF_DISABLED;

    if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(allocated_image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // API 7: Use vpx_codec_control_
    // Try to set quality parameter
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
    
    // Encode the frame
    if (vpx_codec_encode(&encoder, allocated_image, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(allocated_image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // API 8: Get encoded data
    iter = nullptr;
    while ((cx_packet = (vpx_codec_cx_pkt_t*)vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (cx_packet->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame if needed
            break;
        }
    }

    // Open output file (for demonstration, though we don't write in fuzzing)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Prevent unused variable warning
        
        // In a real scenario, we might write the encoded data here
        assert_file_closed(&out_file);;
    }

    // Clean up resources
    vpx_codec_destroy(&encoder);
    vpx_img_free(allocated_image);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}