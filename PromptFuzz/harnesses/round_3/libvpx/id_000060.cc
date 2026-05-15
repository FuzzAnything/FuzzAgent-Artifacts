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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 17
//<Quality> {"density":12,"unique_branches":{"rc_pick_q_and_bounds_one_pass_vbr":[[1323,9,1323,37,0,0,4,1]],"vp9_rc_postencode_update":[[1949,9,1949,31,0,0,4,0],[1949,35,1949,61,0,0,4,0],[1950,9,1950,35,0,0,4,1]],"vp9_lookahead_pop":[[168,26,168,31,0,0,4,0]],"set_rt_speed_feature_framesize_independent":[[560,41,560,68,0,0,4,0],[597,41,597,68,0,0,4,0],[598,9,598,37,0,0,4,1],[653,41,653,68,0,0,4,0],[654,9,654,26,0,0,4,0],[654,30,654,47,0,0,4,0],[828,7,828,29,0,0,4,0],[829,9,829,37,0,0,4,1],[833,9,833,43,0,0,4,0],[839,9,839,50,0,0,4,0],[440,9,440,16,29,0,4,1],[440,9,440,16,30,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve the event of decoding and re-encoding video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<88) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Determine codec type from data (simplified - check for VP8/VP9 markers)
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Simple heuristic: check for VP9 superframe marker
    bool is_vp9 = false;
    if (size >= 4) {
        // Check for VP9 frame marker (bits 0-2 = 0b100)
        if ((data[0] & 0xE0) == 0x80) {
            is_vp9 = true;
        }
    }
    
    if (is_vp9) {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Use vpx_codec_error to get error message
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg; // Use error message to satisfy API requirement
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream info using vpx_codec_get_stream_info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    if (vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info) == VPX_CODEC_OK) {
        // Use stream info if available
        (void)stream_info.w;
        (void)stream_info.h;
        (void)stream_info.is_kf;
    }
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg;
        
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img = nullptr;
    std::vector<vpx_image_t*> decoded_frames;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_frames.push_back(img);
    }
    
    // Initialize encoder if we have decoded frames
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Use vpx_codec_enc_config_default to get default config
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder based on decoded frames
    if (!decoded_frames.empty()) {
        img = decoded_frames[0];
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.rc_target_bitrate = 1000; // 1000 kbps
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30; // 30 fps
        enc_cfg.g_threads = 1;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.rc_end_usage = VPX_VBR;
    } else {
        // Default values if no frames decoded
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.rc_end_usage = VPX_VBR;
    }
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&encoder);
        (void)error_msg;
        
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use vpx_codec_get_global_headers to get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Encode decoded frames
    for (size_t i = 0; i < decoded_frames.size(); i++) {
        vpx_image_t *frame = decoded_frames[i];
        
        // Encode the frame
        if (vpx_codec_encode(&encoder, frame, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7) != VPX_CODEC_OK) {
            const char *error_msg = vpx_codec_error(&encoder);
            (void)error_msg;
            break;
        }
        
        // Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Flush encoder
    while (true) {
        if (vpx_codec_encode(&encoder, nullptr, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11) != VPX_CODEC_OK) {
            break;
        }
        
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        bool got_data = false;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            got_data = true;
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        if (!got_data) break;
    }
    
    // Create and free an image using vpx_img_alloc and vpx_img_free
    // This satisfies the requirement to use vpx_img_free
    vpx_image_t *temp_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 16, 16, fuzz_uint32_t_12);
    if (temp_img) {
        vpx_img_free(temp_img);
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}