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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 204, nr_unique_branch: 287
//<Quality> {"density":18,"unique_branches":{"vp8_get_compressed_data":[[5111,11,5111,36,0,0,4,1],[5120,13,5120,41,0,0,4,1],[5164,7,5164,54,0,0,4,0],[5164,58,5164,76,0,0,4,0]],"sum_coef_counts":[[661,18,661,37,0,0,4,0],[661,18,661,37,0,0,4,1],[662,16,662,40,0,0,4,0],[662,16,662,40,0,0,4,1],[663,14,663,30,0,0,4,0],[663,14,663,30,0,0,4,1],[664,12,664,29,0,0,4,0],[664,12,664,29,0,0,4,1]],"vp8_quantize_mby":[[101,24,101,71,0,0,4,1],[102,24,102,72,0,0,4,1],[106,7,106,20,0,0,4,1]],"calc_partial_ssl_err":[[76,17,76,28,0,0,4,0],[88,15,88,30,0,0,4,0],[88,15,88,30,0,0,4,1],[89,17,89,36,0,0,4,0],[89,17,89,36,0,0,4,1]],"vp8cx_pick_filter_level_fast":[[153,7,153,34,0,0,4,1],[159,7,159,54,0,0,4,1],[167,7,167,42,0,0,4,1],[169,14,169,49,0,0,4,1],[187,10,187,38,0,0,4,0],[187,10,187,38,0,0,4,1],[196,9,196,28,0,0,4,1],[210,7,210,40,0,0,4,0],[214,12,214,39,0,0,4,0],[224,11,224,30,0,0,4,1],[242,7,242,42,0,0,4,1],[244,7,244,42,0,0,4,1]],"vp8cx_remove_encoder_threads":[[624,7,624,54,0,0,4,0],[630,19,630,49,0,0,4,0],[630,19,630,49,0,0,4,1]],"calc_pframe_target_size":[[797,20,797,73,0,0,4,1],[806,13,806,52,0,0,4,1],[808,20,808,36,0,0,4,1]],"yv12_copy_partial_frame":[[41,17,41,28,0,0,4,0],[56,25,56,30,0,0,4,1]],"thread_loopfilter":[[31,9,31,61,0,0,4,0],[31,9,31,61,0,0,4,1],[33,9,33,51,0,0,4,0],[35,11,35,63,0,0,4,0],[35,11,35,63,0,0,4,1]],"encode_mb_row":[[338,7,338,59,0,0,4,0],[341,7,341,59,0,0,4,0],[341,63,341,74,0,0,4,0],[341,63,341,74,0,0,4,1],[411,9,411,61,0,0,4,0],[412,11,412,38,0,0,4,0],[412,11,412,38,0,0,4,1],[416,11,416,17,0,0,4,0],[416,11,416,17,0,0,4,1],[416,21,416,44,0,0,4,0],[416,21,416,44,0,0,4,1],[565,7,565,59,0,0,4,0]],"setup_mbby_copy":[[363,15,363,21,0,0,4,0],[363,15,363,21,0,0,4,1],[421,17,421,23,0,0,4,0],[421,17,421,23,0,0,4,1],[422,18,422,24,0,0,4,0],[422,18,422,24,0,0,4,1]],"vp8cx_encode_inter_macroblock":[[1160,7,1160,17,0,0,4,1],[1223,9,1223,35,0,0,4,1],[1242,9,1242,31,0,0,4,1],[1250,9,1250,51,0,0,4,0],[1262,16,1262,69,0,0,4,1],[1274,9,1274,17,0,0,4,1],[1283,7,1283,15,0,0,4,1],[1286,9,1286,51,0,0,4,1],[1297,9,1297,37,0,0,4,1]],"encode_frame_to_data_rate":[[3498,9,3498,46,0,0,4,1],[3500,16,3500,53,0,0,4,1],[3502,16,3502,53,0,0,4,1],[3992,9,3992,35,0,0,4,1],[4417,7,4417,54,0,0,4,0]],"vp8_loopfilter_frame":[[3159,9,3159,33,0,0,4,0],[3161,11,3161,38,0,0,4,1],[3200,7,3200,54,0,0,4,0]],"pick_intra4x4mby_modes":[[233,9,233,42,0,0,4,1]],"vp8cx_create_encoder_threads":[[494,39,494,67,0,0,4,0],[498,9,498,60,0,0,4,1],[504,9,504,60,0,0,4,0],[504,9,504,60,0,0,4,1],[508,7,508,45,0,0,4,1],[511,7,511,20,0,0,4,0],[535,23,535,41,0,0,4,0],[535,23,535,41,0,0,4,1],[551,11,551,13,0,0,4,1],[554,9,554,11,0,0,4,1],[590,11,590,13,0,0,4,1],[440,9,440,16,1,0,4,1],[440,9,440,16,2,0,4,1],[440,9,440,16,3,0,4,1],[440,9,440,16,4,0,4,1],[440,9,440,16,5,0,4,1]],"vp8_encode_frame":[[755,9,755,56,0,0,4,0],[761,11,761,53,0,0,4,0],[761,11,761,53,0,0,4,1],[770,19,770,34,0,0,4,0],[770,19,770,34,0,0,4,1],[773,19,773,49,0,0,4,0],[773,19,773,49,0,0,4,1],[777,24,777,44,0,0,4,0],[777,24,777,44,0,0,4,1],[806,19,806,49,0,0,4,0],[806,19,806,49,0,0,4,1],[810,24,810,44,0,0,4,0],[810,24,810,44,0,0,4,1],[815,11,815,35,0,0,4,1],[827,19,827,49,0,0,4,0],[827,19,827,49,0,0,4,1],[834,30,834,53,0,0,4,0],[834,30,834,53,0,0,4,1],[839,30,839,55,0,0,4,0],[839,30,839,55,0,0,4,1],[844,25,844,39,0,0,4,0],[844,25,844,39,0,0,4,1],[852,25,852,47,0,0,4,0],[852,25,852,47,0,0,4,1],[857,25,857,47,0,0,4,0],[857,25,857,47,0,0,4,1],[440,9,440,16,4,0,4,1]],"pack_mb_row_tokens":[[324,20,324,48,0,0,4,0],[324,20,324,48,0,0,4,1]],"vp8_set_speed_features":[[809,7,809,44,0,0,4,1],[951,11,951,20,0,0,4,0],[957,11,957,20,0,0,4,0],[965,13,965,45,0,0,4,1],[969,21,969,28,0,0,4,0],[969,21,969,28,0,0,4,1],[977,16,977,24,0,0,4,0],[977,16,977,24,0,0,4,1],[980,15,981,72,0,0,4,1],[989,13,989,26,0,0,4,1],[991,13,991,27,0,0,4,1],[997,13,997,27,0,0,4,1],[1003,13,1003,27,0,0,4,1],[1038,7,1038,37,0,0,4,1],[1041,14,1041,46,0,0,4,1],[1064,7,1064,39,0,0,4,1],[1066,14,1066,42,0,0,4,0]],"vp8_pick_inter_mode":[[618,7,618,44,0,0,4,1],[700,7,700,37,0,0,4,0],[700,7,700,37,0,0,4,1],[705,7,705,34,0,0,4,1],[734,7,734,27,0,0,4,1],[756,7,756,22,0,0,4,0],[756,7,756,22,0,0,4,1],[761,7,761,34,0,0,4,1],[767,7,767,29,0,0,4,1],[775,24,775,46,0,0,4,0],[775,24,775,46,0,0,4,1],[780,9,780,46,0,0,4,0],[780,9,780,46,0,0,4,1],[782,9,782,27,0,0,4,0],[782,9,782,27,0,0,4,1],[787,9,787,51,0,0,4,1],[819,9,819,44,0,0,4,0],[819,9,819,44,0,0,4,1],[820,9,820,47,0,0,4,0],[820,9,820,47,0,0,4,1],[821,11,822,72,0,0,4,0],[821,11,822,72,0,0,4,1],[827,13,827,59,0,0,4,0],[827,13,827,59,0,0,4,1],[858,9,858,34,0,0,4,1],[866,7,866,18,0,0,4,0],[866,7,866,18,0,0,4,1],[871,13,871,35,0,0,4,0],[871,13,871,35,0,0,4,1],[879,15,879,38,0,0,4,0],[879,15,879,38,0,0,4,1],[887,7,887,19,0,0,4,1],[892,7,892,19,0,0,4,0],[892,7,892,19,0,0,4,1],[893,7,893,18,0,0,4,0],[893,7,893,18,0,0,4,1],[894,7,894,18,0,0,4,0],[894,7,894,18,0,0,4,1],[895,7,895,19,0,0,4,0],[895,7,895,19,0,0,4,1],[905,13,905,36,0,0,4,0],[905,13,905,36,0,0,4,1],[911,7,911,17,0,0,4,1],[1081,7,1081,21,0,0,4,1],[1082,7,1082,18,0,0,4,1],[1086,7,1086,18,0,0,4,1],[1106,7,1106,14,0,0,4,1],[1110,9,1110,36,0,0,4,1],[1139,9,1139,26,0,0,4,0],[1139,9,1139,26,0,0,4,1],[1139,30,1139,37,0,0,4,0],[1139,30,1139,37,0,0,4,1],[1154,11,1154,66,0,0,4,0],[1154,11,1154,66,0,0,4,1],[1167,11,1167,57,0,0,4,0],[1167,11,1167,57,0,0,4,1],[1175,9,1175,16,0,0,4,1],[1179,7,1179,53,0,0,4,0],[1179,7,1179,53,0,0,4,1],[1180,7,1180,66,0,0,4,0],[1184,9,1185,45,0,0,4,0],[1184,9,1185,45,0,0,4,1],[1196,9,1196,27,0,0,4,0],[1196,9,1196,27,0,0,4,1],[1204,7,1204,34,0,0,4,1],[1281,7,1281,32,0,0,4,1],[1296,7,1296,15,0,0,4,0],[1296,7,1296,15,0,0,4,1],[1301,7,1301,33,0,0,4,0],[1301,7,1301,33,0,0,4,1],[1306,7,1307,77,0,0,4,1]],"calculate_zeromv_rd_adjustment":[[522,7,522,30,0,0,4,1]],"vp8_pack_bitstream":[[1366,9,1366,56,0,0,4,0]],"vp8_use_as_reference":[[2402,7,2402,26,0,0,4,1]],"set_reference_and_update":[[866,7,867,75,0,0,4,0],[870,9,870,38,0,0,4,0],[872,9,872,36,0,0,4,0],[874,9,874,37,0,0,4,0]],"update_mode":[[65,7,65,31,0,0,4,0],[73,38,73,39,0,0,4,0],[74,14,74,21,0,0,4,0],[74,14,74,21,0,0,4,1]],"thread_encoding_proc":[[53,9,53,61,0,0,4,1],[55,9,55,65,0,0,4,0],[55,9,55,65,0,0,4,1],[71,11,71,63,0,0,4,0],[71,11,71,63,0,0,4,1],[76,34,76,54,0,0,4,0],[76,34,76,54,0,0,4,1],[111,26,111,46,0,0,4,0],[111,26,111,46,0,0,4,1],[112,15,112,42,0,0,4,0],[112,15,112,42,0,0,4,1],[116,15,116,21,0,0,4,0],[116,15,116,21,0,0,4,1],[116,25,116,48,0,0,4,0],[116,25,116,48,0,0,4,1],[154,15,154,48,0,0,4,1],[158,15,158,39,0,0,4,1],[177,15,177,42,0,0,4,0],[177,15,177,42,0,0,4,1],[201,17,201,40,0,0,4,0],[201,17,201,40,0,0,4,1],[202,19,202,61,0,0,4,1],[215,19,215,44,0,0,4,1],[227,17,227,42,0,0,4,0],[227,17,227,42,0,0,4,1],[228,18,228,50,0,0,4,1]],"update_mvcount":[[382,7,382,48,0,0,4,1]],"vp8cx_init_mbrthread_data":[[443,15,443,24,0,0,4,0],[443,15,443,24,0,0,4,1],[476,9,476,23,0,0,4,1]],"vp8_auto_select_speed":[[281,7,281,58,0,0,4,1],[284,9,284,37,0,0,4,1],[287,11,287,70,0,0,4,0],[287,11,287,70,0,0,4,1],[292,13,292,28,0,0,4,1],[297,11,298,63,0,0,4,0],[297,11,298,63,0,0,4,1],[304,13,304,27,0,0,4,0],[304,13,304,27,0,0,4,1],[312,9,312,24,0,0,4,1]],"vp8_convert_rfct_to_prob":[[398,26,398,34,0,0,4,1]],"vp8_estimate_entropy_savings":[[812,16,812,24,0,0,4,1]],"pack_inter_mode_mvs":[[477,13,477,27,0,0,4,0],[482,20,482,28,0,0,4,0],[482,20,482,28,0,0,4,1]],"vp8_loop_filter_partial_frame":[[504,17,504,28,0,0,4,0],[511,20,511,47,0,0,4,0],[511,20,511,47,0,0,4,1],[512,22,512,38,0,0,4,0],[512,22,512,38,0,0,4,1],[513,22,513,60,0,0,4,0],[514,22,514,61,0,0,4,0],[515,22,515,59,0,0,4,0],[515,22,515,59,0,0,4,1],[523,11,523,23,0,0,4,0],[524,13,524,49,0,0,4,0],[531,15,531,25,0,0,4,0],[531,15,531,25,0,0,4,1],[534,15,534,23,0,0,4,0],[534,15,534,23,0,0,4,1],[539,15,539,23,0,0,4,0],[539,15,539,23,0,0,4,1]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":2}
/**/


// Include libvpx headers

// Callback function prototypes
static void  id_000062_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
}

static void  id_000062_put_slice_cb(void* user_priv, const vpx_image_t* img, 
                        const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

static int  id_000062_get_frame_buffer_cb(void* cb_priv, size_t min_size, 
                              vpx_codec_frame_buffer_t* fb) {
    // Simple frame buffer allocation
    (void)min_size;
    uint8_t* buf = (uint8_t*)malloc(4096);
    if (!buf) return -1;
    
    fb->data = buf;
    fb->size = 4096;
    // Store pointer for release
    *(void**)cb_priv = buf;
    return 0;
}

static int  id_000062_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // Release allocated frame buffer
    (void)cb_priv;
    if (fb && fb->data) {
        free(fb->data);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Use fmemopen to create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // May be unused
    
    // Open output file for writing encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    (void)fd_out; // May be unused
    
    // 1. Get version information
    const char* version_str = vpx_codec_version_str();
    const char* build_config = vpx_codec_build_config();
    (void)version_str; // May be unused
    (void)build_config; // May be unused
    
    // 2. Get VP8 encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Set up encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Adjust configuration with fuzzed data
    cfg.g_w = (data[0] % 64 + 1) * 16;  // Width between 16 and 1024
    cfg.g_h = (data[1] % 64 + 1) * 16;  // Height between 16 and 1024
    cfg.rc_target_bitrate = (data[2] % 1000 + 100) * 1000;  // 100kbps to 1.1Mbps
    cfg.g_threads = (data[3] % 8) + 1;  // 1 to 8 threads
    
    // 4. Initialize encoder context
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Re-set configuration (optional, to test the API)
    err = vpx_codec_enc_config_set(&codec, &cfg);
    (void)err; // May fail, that's OK for fuzzing
    
    // 6. Register callbacks
    void* user_priv = nullptr;
    vpx_codec_register_put_slice_cb(&codec,  id_000062_put_slice_cb, user_priv);
    vpx_codec_register_put_frame_cb(&codec,  id_000062_put_frame_cb, user_priv);
    
    // 7. Set frame buffer functions
    void* cb_priv = nullptr;
    vpx_codec_set_frame_buffer_functions(&codec,  id_000062_get_frame_buffer_cb, 
                                          id_000062_release_frame_buffer_cb, &cb_priv);
    
    // 8. Create input image from fuzzed data
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Fill image with fuzzed data (simple pattern for testing)
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        uint8_t* plane_ptr = img->planes[plane];
        int stride = img->stride[plane];
        int plane_height = (plane == 0) ? cfg.g_h : cfg.g_h / 2;
        int plane_width = (plane == 0) ? cfg.g_w : cfg.g_w / 2;
        
        for (int y = 0; y < plane_height; y++) {
            size_t bytes_to_copy = (size_t)plane_width;
            if (data_offset + bytes_to_copy > size) {
                bytes_to_copy = size - data_offset;
                if (bytes_to_copy == 0) break;
            }
            
            if (data_offset < size) {
                memcpy(plane_ptr, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            plane_ptr += stride;
        }
    }
    
    // 9. Encode the image multiple times
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    // First encode
    err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
    (void)err; // May fail, that's OK for fuzzing
    
    // 10. Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 11. Second encode with different parameters
    pts += duration;
    flags = VP8_EFLAG_NO_REF_LAST | VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_REF_ARF;
    err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
    
    // 12. Get encoded data again
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 13. Try to get preview frame
    const vpx_image_t* preview = vpx_codec_get_preview_frame(&codec);
    (void)preview; // May be null
    
    // 14. Check for errors
    const char* error = vpx_codec_error(&codec);
    const char* error_detail = vpx_codec_error_detail(&codec);
    (void)error;
    (void)error_detail;
    
    // 15. Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}