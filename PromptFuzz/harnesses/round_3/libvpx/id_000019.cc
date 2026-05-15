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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"vpx_codec_enc_init_multi_ver":[[77,71,77,75,0,0,4,0],[23,32,23,37,9,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Variables for error handling
    const char *error_str = nullptr;
    vpx_codec_err_t res = VPX_CODEC_OK;
    
    // Determine codec type from first byte (simplistic heuristic)
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (data[0] % 2 == 0) {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    }

    // 1. Use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    res = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_err_to_string(res);
        // Continue anyway for fuzzing
    }

    // 2. Initialize decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    res = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_err_to_string(res);
        const char *ctx_error = vpx_codec_error(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Decode the input data
    res = vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_err_to_string(res);
        const char *ctx_error = vpx_codec_error(&decoder_ctx);
    }

    // Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img;
    std::vector<vpx_image_t*> decoded_frames;
    
    while ((img = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // Store reference to decoded frames for potential re-encoding
        decoded_frames.push_back(const_cast<vpx_image_t*>(img));
    }

    // 4. Initialize multiple encoders using vpx_codec_enc_init_multi_ver
    // This is for demonstration - we'll initialize but may not actually use all
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_rational_t timebase = {1, 30};
    int num_encoders = 2;
    
    // Get default encoder configuration
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (res == VPX_CODEC_OK) {
        // Set basic configuration
        enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 640;
        enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 480;
        enc_cfg.g_timebase = timebase;
        enc_cfg.rc_target_bitrate = 1000;
        
        // Initialize multiple encoder instances
        res = vpx_codec_enc_init_multi_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                          num_encoders, fuzz_int64_t_4, nullptr, VPX_ENCODER_ABI_VERSION);
        
        if (res != VPX_CODEC_OK) {
            error_str = vpx_codec_err_to_string(res);
            const char *ctx_error = vpx_codec_error(&encoder_ctx);
        } else {
            // If we have decoded frames, try to encode them
            for (size_t i = 0; i < decoded_frames.size() && i < 1; i++) {
                vpx_image_t *frame = decoded_frames[i];
                
                // Adjust encoder config to match frame dimensions if needed
                if (enc_cfg.g_w != frame->d_w || enc_cfg.g_h != frame->d_h) {
                    enc_cfg.g_w = frame->d_w;
                    enc_cfg.g_h = frame->d_h;
                    vpx_codec_enc_config_set(&encoder_ctx, &enc_cfg);
                }
                
                // Encode the frame
                res = vpx_codec_encode(&encoder_ctx, frame, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);
                if (res != VPX_CODEC_OK) {
                    error_str = vpx_codec_err_to_string(res);
                    const char *ctx_error = vpx_codec_error(&encoder_ctx);
                }
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }
            
            // Clean up encoder
            vpx_codec_destroy(&encoder_ctx);
        }
    }

    // 5. Clean up decoder
    vpx_codec_destroy(&decoder_ctx);

    // 6. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // 7. Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}