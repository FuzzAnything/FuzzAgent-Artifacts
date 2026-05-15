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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 112.5, nr_unique_branch: 213
//<Quality> {"density":9,"unique_branches":{"firstpass_inter_prediction":[[758,7,758,61,0,0,4,1],[778,7,778,40,0,0,4,1],[792,7,792,40,0,0,4,1],[802,7,802,37,0,0,4,1],[809,7,809,40,0,0,4,1],[819,7,819,39,0,0,4,0],[827,7,827,39,0,0,4,0],[831,9,831,75,0,0,4,0],[831,9,831,75,0,0,4,1],[832,9,832,54,0,0,4,0],[836,16,836,56,0,0,4,0],[836,16,836,56,0,0,4,1],[837,16,837,73,0,0,4,1],[850,9,850,34,0,0,4,1]],"av1_first_pass_row":[[1250,9,1250,33,0,0,4,0],[1256,11,1256,32,0,0,4,0],[1256,11,1256,32,0,0,4,1]],"av1_fill_mode_rates":[[221,7,221,31,0,0,4,0],[222,17,222,40,0,0,4,0],[222,17,222,40,0,0,4,1],[227,17,227,33,0,0,4,0],[227,17,227,33,0,0,4,1],[228,19,228,38,0,0,4,0],[228,19,228,38,0,0,4,1],[234,17,234,43,0,0,4,0],[234,17,234,43,0,0,4,1],[239,17,239,42,0,0,4,0],[239,17,239,42,0,0,4,1],[240,19,240,43,0,0,4,0],[240,19,240,43,0,0,4,1],[246,17,246,33,0,0,4,0],[246,17,246,33,0,0,4,1],[247,19,247,35,0,0,4,0],[247,19,247,35,0,0,4,1],[253,17,253,33,0,0,4,0],[253,17,253,33,0,0,4,1],[254,19,254,35,0,0,4,0],[254,19,254,35,0,0,4,1],[260,17,260,41,0,0,4,0],[260,17,260,41,0,0,4,1],[265,17,265,40,0,0,4,0],[265,17,265,40,0,0,4,1],[270,17,270,43,0,0,4,0],[270,17,270,43,0,0,4,1],[275,17,275,40,0,0,4,0],[275,17,275,40,0,0,4,1],[280,17,280,38,0,0,4,0],[280,17,280,38,0,0,4,1],[284,17,284,40,0,0,4,0],[284,17,284,40,0,0,4,1],[287,17,287,36,0,0,4,0],[287,17,287,36,0,0,4,1],[290,17,290,36,0,0,4,0],[290,17,290,36,0,0,4,1],[291,11,291,31,0,0,4,0],[291,11,291,31,0,0,4,1],[296,17,296,38,0,0,4,0],[296,17,296,38,0,0,4,1],[302,17,302,36,0,0,4,0],[302,17,302,36,0,0,4,1],[306,25,306,44,0,0,4,0],[306,25,306,44,0,0,4,1],[310,25,310,44,0,0,4,0],[310,25,310,44,0,0,4,1],[314,17,314,40,0,0,4,0],[314,17,314,40,0,0,4,1],[318,17,318,44,0,0,4,0],[318,17,318,44,0,0,4,1]],"get_prediction_error_bitdepth":[[626,7,626,23,0,0,4,1]],"av1_set_frame_size":[[2712,9,2712,20,0,0,4,0],[2718,11,2718,28,0,0,4,1],[2726,7,2726,31,0,0,4,0],[2726,35,2726,55,0,0,4,1]],"av1_lookahead_peek":[[189,7,189,17,0,0,4,1],[196,14,196,23,0,0,4,0],[198,9,198,38,0,0,4,0],[200,11,200,20,0,0,4,1]],"denoise_and_encode":[[859,23,859,60,0,0,4,1],[860,23,860,48,0,0,4,1],[860,52,860,76,0,0,4,1],[865,7,865,20,0,0,4,1]],"set_good_speed_features_framesize_independent":[[1086,7,1086,14,0,0,4,1],[1086,18,1086,80,0,0,4,1],[1097,30,1097,37,0,0,4,1],[1099,45,1099,52,0,0,4,1],[1118,8,1118,15,0,0,4,1],[1118,19,1118,47,0,0,4,0],[1118,19,1118,47,0,0,4,1],[1120,14,1120,38,0,0,4,1],[1121,37,1121,44,0,0,4,1],[1175,55,1175,62,0,0,4,1],[1177,48,1177,55,0,0,4,1],[1179,10,1179,43,0,0,4,1],[1179,47,1179,75,0,0,4,0],[1179,47,1179,75,0,0,4,1],[1181,16,1181,23,0,0,4,1],[1182,39,1182,46,0,0,4,1],[1202,35,1202,42,0,0,4,1],[1203,38,1203,45,0,0,4,1],[1236,55,1236,62,0,0,4,1],[1241,61,1241,68,0,0,4,1],[1242,45,1242,52,0,0,4,1],[1245,37,1245,44,0,0,4,1],[1256,35,1256,59,0,0,4,1],[1261,50,1261,57,0,0,4,1],[1279,16,1279,23,0,0,4,1],[1289,58,1289,65,0,0,4,1],[1296,51,1296,58,0,0,4,1],[1297,55,1297,62,0,0,4,1],[1301,28,1301,35,0,0,4,1],[1301,43,1301,67,0,0,4,1],[1302,45,1302,52,0,0,4,1],[1321,43,1321,50,0,0,4,1],[1330,9,1330,16,0,0,4,1],[1331,11,1331,73,0,0,4,1],[1333,55,1333,62,0,0,4,1],[1340,9,1340,42,0,0,4,1],[1356,40,1356,73,0,0,4,1],[1360,58,1360,65,0,0,4,1],[1361,62,1361,69,0,0,4,1],[1362,65,1362,72,0,0,4,1],[1364,51,1364,58,0,0,4,1],[1391,35,1391,59,0,0,4,1],[1395,9,1395,42,0,0,4,1],[1397,44,1397,51,0,0,4,1],[1418,37,1418,61,0,0,4,1],[1419,58,1419,65,0,0,4,1],[1420,65,1420,72,0,0,4,1],[1423,47,1423,54,0,0,4,1],[1430,9,1430,42,0,0,4,1],[1454,51,1454,58,0,0,4,1],[1456,37,1456,61,0,0,4,1],[1468,9,1468,16,0,0,4,1],[1468,20,1468,46,0,0,4,0],[1468,20,1468,46,0,0,4,1],[1476,35,1476,59,0,0,4,1]],"encoder_encode":[[3290,11,3290,31,0,0,4,1],[3290,35,3290,60,0,0,4,1],[3392,11,3392,39,0,0,4,1],[3439,11,3439,26,0,0,4,1]],"aom_codec_err_to_string":[[37,11,37,14,0,0,4,0]],"get_block_variance_fn":[[200,5,200,19,0,0,4,0],[200,5,200,19,0,0,4,1],[201,5,201,20,0,0,4,1],[202,5,202,20,0,0,4,1],[203,5,203,12,0,0,4,0],[203,5,203,12,0,0,4,1]],"av1_encode_strategy":[[1470,31,1470,50,0,0,4,0],[1477,7,1477,57,0,0,4,1],[1576,10,1576,45,0,0,4,1],[1577,10,1577,40,0,0,4,1],[1577,44,1577,75,0,0,4,1],[1578,9,1578,21,0,0,4,1],[1581,16,1581,45,0,0,4,0],[1608,9,1608,45,0,0,4,1],[1609,8,1609,42,0,0,4,1],[1714,30,1714,66,0,0,4,1],[1715,30,1715,73,0,0,4,1]],"adjust_frame_rate":[[261,7,261,46,0,0,4,1],[271,9,271,22,0,0,4,0],[276,9,276,13,0,0,4,1],[292,11,292,77,0,0,4,1],[34,23,34,34,1,0,4,0]],"av1_configure_buffer_updates":[[124,7,124,24,0,0,4,1]],"choose_frame_source":[[386,9,386,43,0,0,4,0]],"av1_check_initial_width":[[2554,7,2554,42,0,0,4,1],[2555,7,2555,55,0,0,4,1],[2556,7,2556,49,0,0,4,1],[2557,7,2557,49,0,0,4,1]],"accumulate_mv_stats":[[639,7,639,27,0,0,4,0]],"raw_motion_error_stdev":[[377,7,377,33,0,0,4,1],[380,15,380,40,0,0,4,0],[380,15,380,40,0,0,4,1],[384,15,384,40,0,0,4,0],[384,15,384,40,0,0,4,1]],"av1_init_plane_quantizers":[[833,7,833,20,0,0,4,1],[833,24,833,33,0,0,4,0]],"is_frame_level_cost_upd_freq_set":[[754,7,754,30,0,0,4,1],[755,7,756,75,0,0,4,0],[755,8,755,27,0,0,4,1]],"av1_encode":[[4555,7,4555,45,0,0,4,1]],"av1_scale_references":[[836,32,836,57,0,0,4,0],[836,32,836,57,0,0,4,1],[838,9,838,66,0,0,4,0],[838,9,838,66,0,0,4,1],[843,11,843,22,0,0,4,1],[851,11,851,28,0,0,4,1],[860,11,860,37,0,0,4,1],[867,11,867,41,0,0,4,1],[867,45,867,77,0,0,4,1],[953,11,953,35,0,0,4,0]],"av1_setup_pre_planes":[[737,7,737,18,0,0,4,0],[740,21,740,31,0,0,4,0],[740,21,740,31,0,0,4,1],[34,23,34,34,1,0,4,1]],"av1_first_pass":[[1323,7,1323,30,0,0,4,1],[1366,7,1366,52,0,0,4,1],[1376,7,1376,31,0,0,4,0],[1378,18,1378,76,0,0,4,1],[1381,20,1381,80,0,0,4,1],[1384,19,1384,78,0,0,4,1],[1415,7,1415,31,0,0,4,0],[1443,7,1443,30,0,0,4,1],[1476,7,1476,41,0,0,4,1],[1485,8,1485,41,0,0,4,0],[1486,8,1486,45,0,0,4,0],[1487,8,1488,68,0,0,4,0],[1487,8,1488,68,0,0,4,1],[1489,9,1489,29,0,0,4,0],[1507,7,1507,39,0,0,4,1],[1514,7,1514,39,0,0,4,1],[1526,7,1526,31,0,0,4,0],[26,33,26,40,5,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_err_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_uleb_encode","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_encode","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_err_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_uleb_encode","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_encode","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // 1. Use aom_uleb_decode to decode a value from input
    uint64_t decoded_value = 0;
    size_t uleb_length = 0;
    int decode_result = aom_uleb_decode(data, 
                                        (size > 100) ? 100 : size,  // Use first 100 bytes max
                                        &decoded_value, 
                                        &uleb_length);
    
    // 2. Use aom_codec_err_to_string to convert potential errors
    if (decode_result != 0) {
        const char* err_str = aom_codec_err_to_string(static_cast<aom_codec_err_t>(decode_result));
        // Just to use the API - we don't actually need to do anything with the string
        (void)err_str;
    }
    
    // 3. Create encoder configuration using aom_codec_enc_config_default
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0;  // Good quality
    aom_codec_err_t cfg_result = aom_codec_enc_config_default(encoder_iface, &cfg, usage);
    
    // Set up encoder context
    aom_codec_ctx_t encoder;
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_Q;
    
    // 4. Initialize encoder
    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder, 
                                                             encoder_iface, 
                                                             &cfg, 
                                                             0, 
                                                             AOM_ENCODER_ABI_VERSION);
    
    // 5. Use aom_codec_control at least once
    if (enc_init_result == AOM_CODEC_OK) {
        // Set some control parameter
        int ctrl_id = AOME_SET_CPUUSED;
        int value = 8;  // Reasonable speed setting
        aom_codec_control(&encoder, ctrl_id, value);
    }
    
    // 6. Create an image and add metadata using aom_img_add_metadata
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 16;
    
    // Allocate image
    if (aom_img_alloc(&img, fmt, 64, 64, align) != nullptr) {
        // Fill image planes with some data from input
        for (int plane = 0; plane < 3; ++plane) {
            int plane_width = aom_img_plane_width(&img, plane);
            int plane_height = aom_img_plane_height(&img, plane);
            size_t plane_size = plane_width * plane_height;
            
            if (plane_size > 0) {
                size_t copy_size = (size > plane_size) ? plane_size : size;
                if (copy_size > 0) {
                    memcpy(img.planes[plane], data, copy_size);
                }
            }
        }
        
        // 7. Use aom_uleb_encode to encode the decoded value
        uint8_t encoded_buffer[16];
        size_t encoded_size = 0;
        uint64_t value_to_encode = decoded_value % 1000;  // Keep it reasonable
        aom_uleb_encode(value_to_encode, 
                       sizeof(encoded_buffer), 
                       encoded_buffer, 
                       &encoded_size);
        
        // 8. Add metadata to image
        if (encoded_size > 0) {
            uint32_t metadata_type = 1;  // Arbitrary type
            aom_img_add_metadata(&img, 
                                metadata_type, 
                                encoded_buffer, 
                                encoded_size, 
                                AOM_MIF_ANY_FRAME);
        }
        
        // 9. Use aom_codec_peek_stream_info (if we have some encoded data from previous steps)
        // First, let's encode one frame to get some data
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        aom_codec_err_t encode_result = aom_codec_encode(&encoder, 
                                                         &img, 
                                                         pts, 
                                                         duration, 
                                                         flags);
        
        // Get encoded data
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t* pkt = nullptr;
        std::vector<uint8_t> encoded_stream;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                encoded_stream.insert(encoded_stream.end(),
                                     static_cast<const uint8_t*>(pkt->data.frame.buf),
                                     static_cast<const uint8_t*>(pkt->data.frame.buf) + pkt->data.frame.sz);
            }
        }
        
        // 10. Now use aom_codec_peek_stream_info on the encoded data
        if (!encoded_stream.empty()) {
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            aom_codec_peek_stream_info(aom_codec_av1_dx(),
                                      encoded_stream.data(),
                                      encoded_stream.size(),
                                      &si);
        }
        
        // 11. Try to encode another frame with the same image
        pts = 1;
        aom_codec_encode(&encoder, &img, pts, duration, flags);
        
        // Clean up image
        aom_img_free(&img);
    }
    
    // 12. Clean up encoder
    if (enc_init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
    }
    
    // 13. Also test with FILE* operations as mentioned in instructions
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Read a small amount to demonstrate usage
        uint8_t buffer[16];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something small
        fwrite(data, 1, (size > 16) ? 16 : size, out_file);
        assert_file_closed(&out_file);;
        
        // Remove the output file
        remove("output_file");
    }
    
    // 14. Test with file descriptors
    if (in_file) {
        int fd_in = fuzz_fileno(in_file);
        // Can use fd_in for other operations if needed
        (void)fd_in;
    }
    
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}