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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 266, nr_unique_branch: 193
//<Quality> {"density":19,"unique_branches":{"thread_decoding_proc":[[591,9,591,63,0,0,4,1],[593,9,593,65,0,0,4,0],[594,11,594,65,0,0,4,0],[594,11,594,65,0,0,4,1]],"vp8_decoder_create_threads":[[630,7,630,21,0,0,4,0],[639,9,639,55,0,0,4,1],[644,23,644,59,0,0,4,0],[644,23,644,59,0,0,4,1],[645,11,645,68,0,0,4,1],[653,11,654,78,0,0,4,1],[661,9,662,40,0,0,4,1],[440,9,440,16,9,0,4,1],[440,9,440,16,10,0,4,1],[440,9,440,16,11,0,4,1],[440,9,440,16,12,0,4,1]],"setup_token_decoder":[[808,7,808,60,0,0,4,0]],"mt_decode_mb_rows":[[277,15,277,33,0,0,4,0],[277,15,277,33,0,0,4,1],[296,31,296,51,0,0,4,0],[296,31,296,51,0,0,4,1],[308,9,308,19,0,0,4,0],[308,9,308,19,0,0,4,1],[328,9,328,33,0,0,4,0],[362,22,362,42,0,0,4,0],[362,22,362,42,0,0,4,1],[363,11,363,38,0,0,4,0],[363,11,363,38,0,0,4,1],[367,11,367,17,0,0,4,0],[367,11,367,17,0,0,4,1],[367,21,367,44,0,0,4,0],[367,21,367,44,0,0,4,1],[409,11,409,24,0,0,4,0],[409,11,409,24,0,0,4,1],[412,16,412,36,0,0,4,0],[412,16,412,36,0,0,4,1],[421,11,421,62,0,0,4,1],[443,11,443,36,0,0,4,1],[449,11,449,35,0,0,4,0],[449,11,449,35,0,0,4,1],[450,24,450,66,0,0,4,0],[450,24,450,66,0,0,4,1],[451,24,451,67,0,0,4,0],[452,24,452,65,0,0,4,1],[461,13,461,38,0,0,4,0],[472,13,472,38,0,0,4,0],[475,15,475,50,0,0,4,0],[476,25,476,31,0,0,4,0],[476,25,476,31,0,0,4,1],[480,25,480,30,0,0,4,0],[480,25,480,30,0,0,4,1],[490,13,490,25,0,0,4,0],[490,13,490,25,0,0,4,1],[491,15,491,51,0,0,4,0],[491,15,491,51,0,0,4,1],[500,17,500,27,0,0,4,1],[505,17,505,25,0,0,4,0],[511,17,511,27,0,0,4,0],[511,17,511,27,0,0,4,1],[516,17,516,25,0,0,4,0],[521,17,521,27,0,0,4,0],[521,17,521,27,0,0,4,1],[525,17,525,25,0,0,4,0],[530,17,530,27,0,0,4,0],[530,17,530,27,0,0,4,1],[534,17,534,25,0,0,4,0],[550,9,550,33,0,0,4,1],[580,7,580,71,0,0,4,1]],"vpx_internal_error":[[123,7,123,10,0,0,4,1]],"vp8_decode_frame":[[1208,7,1208,56,0,0,4,0],[1209,7,1209,49,0,0,4,0],[1209,7,1209,49,0,0,4,1],[1211,9,1211,38,0,0,4,0],[1217,22,1217,57,0,0,4,1]],"setup_decoding_thread_data":[[48,15,48,24,0,0,4,0],[48,15,48,24,0,0,4,1],[82,9,82,23,0,0,4,0],[82,9,82,23,0,0,4,1],[85,15,85,30,0,0,4,0],[85,15,85,30,0,0,4,1]],"vp8mt_decode_mb_rows":[[853,7,853,19,0,0,4,0],[862,17,862,32,0,0,4,0],[862,17,862,32,0,0,4,1],[872,17,872,32,0,0,4,0],[872,17,872,32,0,0,4,1],[887,15,887,45,0,0,4,0],[887,15,887,45,0,0,4,1],[897,17,897,47,0,0,4,0],[897,17,897,47,0,0,4,1]],"vp8_decode":[[420,13,420,62,0,0,4,0],[434,21,434,61,0,0,4,0],[468,13,468,62,0,0,4,0],[493,11,493,31,0,0,4,0]],"mt_decode_macroblock":[[99,7,99,48,0,0,4,1],[101,14,101,47,0,0,4,0],[101,14,101,47,0,0,4,1],[111,7,111,31,0,0,4,0],[111,7,111,31,0,0,4,1],[142,7,142,59,0,0,4,0],[148,9,148,23,0,0,4,0],[148,9,148,23,0,0,4,1],[157,11,157,52,0,0,4,0],[157,11,157,52,0,0,4,1],[161,19,161,25,0,0,4,0],[161,19,161,25,0,0,4,1],[172,13,172,18,0,0,4,0],[172,13,172,18,0,0,4,1],[172,22,172,46,0,0,4,0],[178,13,178,23,0,0,4,0],[178,13,178,23,0,0,4,1],[178,27,178,51,0,0,4,0],[186,14,186,20,0,0,4,0],[186,14,186,20,0,0,4,1],[186,24,186,30,0,0,4,0],[186,24,186,30,0,0,4,1],[186,34,186,41,0,0,4,0],[186,34,186,41,0,0,4,1],[186,46,186,70,0,0,4,0],[195,13,195,24,0,0,4,0],[195,13,195,24,0,0,4,1],[196,15,196,30,0,0,4,0],[196,15,196,30,0,0,4,1],[216,7,216,49,0,0,4,0],[216,7,216,49,0,0,4,1],[218,9,218,23,0,0,4,0],[218,9,218,23,0,0,4,1],[221,11,221,26,0,0,4,0],[225,13,225,29,0,0,4,1]],"vp8mt_alloc_temp_buffers":[[742,7,742,56,0,0,4,0],[746,9,746,27,0,0,4,0],[746,9,746,27,0,0,4,1],[748,9,748,20,0,0,4,0],[748,9,748,20,0,0,4,1],[750,16,750,29,0,0,4,0],[750,16,750,29,0,0,4,1],[752,16,752,29,0,0,4,0],[752,16,752,29,0,0,4,1],[763,17,763,32,0,0,4,0],[763,17,763,32,0,0,4,1],[768,17,768,32,0,0,4,0],[768,17,768,32,0,0,4,1],[776,17,776,32,0,0,4,0],[776,17,776,32,0,0,4,1],[784,17,784,32,0,0,4,0],[784,17,784,32,0,0,4,1],[793,17,793,32,0,0,4,0],[793,17,793,32,0,0,4,1],[798,17,798,32,0,0,4,0],[798,17,798,32,0,0,4,1],[803,17,803,32,0,0,4,0],[803,17,803,32,0,0,4,1],[440,9,440,16,1,0,4,1],[440,9,440,16,3,0,4,1],[440,9,440,16,6,0,4,1],[440,9,440,16,9,0,4,1],[440,9,440,16,12,0,4,1],[440,9,440,16,14,0,4,1],[440,9,440,16,16,0,4,1],[440,9,440,16,17,0,4,1],[440,9,440,16,18,0,4,1],[440,9,440,16,19,0,4,1],[440,9,440,16,20,0,4,1],[440,9,440,16,21,0,4,1],[440,9,440,16,22,0,4,1]],"vp8mt_de_alloc_temp_buffers":[[681,7,681,25,0,0,4,0],[681,7,681,25,0,0,4,1],[682,17,682,28,0,0,4,0],[682,17,682,28,0,0,4,1],[690,7,690,25,0,0,4,0],[690,7,690,25,0,0,4,1],[691,17,691,28,0,0,4,0],[691,17,691,28,0,0,4,1],[699,7,699,25,0,0,4,0],[699,7,699,25,0,0,4,1],[700,17,700,28,0,0,4,0],[700,17,700,28,0,0,4,1],[709,7,709,24,0,0,4,0],[709,7,709,24,0,0,4,1],[710,17,710,28,0,0,4,0],[710,17,710,28,0,0,4,1],[718,7,718,24,0,0,4,0],[718,7,718,24,0,0,4,1],[719,17,719,28,0,0,4,0],[719,17,719,28,0,0,4,1],[727,7,727,24,0,0,4,0],[727,7,727,24,0,0,4,1],[728,17,728,28,0,0,4,0],[728,17,728,28,0,0,4,1]],"vp8_decoder_remove_threads":[[811,7,811,56,0,0,4,0],[816,17,816,57,0,0,4,0],[816,17,816,57,0,0,4,1],[821,17,821,57,0,0,4,0],[821,17,821,57,0,0,4,1],[825,9,825,45,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use input data as compressed video bitstream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Determine codec type from first byte (simplified heuristic)
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (data[0] & 0x01) { // Simple heuristic
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    }

    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream info
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    err = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    
    if (err == VPX_CODEC_OK) {
        err = vpx_codec_get_stream_info(&decoder, &si);
    }

    // Decode the input frame
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);

    if (decoded_img) {
        // Create output image for encoding
        vpx_image_t output_img;
        memset(&output_img, 0, sizeof(output_img));
        
        // Try both image allocation methods
        vpx_image_t *wrapped_img = vpx_img_wrap(&output_img, decoded_img->fmt, 
                                                 decoded_img->d_w, decoded_img->d_h, 
                                                 fuzz_uint32_t_3, decoded_img->planes[0]);
        
        // Also try vpx_img_alloc
        vpx_image_t *allocated_img = vpx_img_alloc(nullptr, decoded_img->fmt,
                                                   decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_4);

        if (allocated_img) {
            // Set a rectangle on the allocated image
            vpx_img_set_rect(allocated_img, fuzz_uint32_t_5, fuzz_uint32_t_6, 
                            decoded_img->d_w / 2, decoded_img->d_h / 2);
            
            // Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_enc_cfg_t enc_cfg;
            
            err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_7);
            if (err == VPX_CODEC_OK) {
                enc_cfg.g_w = decoded_img->d_w;
                enc_cfg.g_h = decoded_img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_error_resilient = 0;
                
                err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_8, 
                                           VPX_ENCODER_ABI_VERSION);
                
                if (err == VPX_CODEC_OK) {
                    // Encode the frame
                    err = vpx_codec_encode(&encoder, allocated_img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t *pkt;
                    
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Process encoded frame
                            break;
                        }
                    }
                    
                    // Try to get preview frame
                    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                    
                    // Destroy encoder
                    vpx_codec_destroy(&encoder);
                }
            }
            
            // Free allocated image
            vpx_img_free(allocated_img);
        }
    }

    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    // Also test with output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size, out_file);
        }
        assert_file_closed(&out_file);;
        
        // Test file descriptor usage
        out_file = fopen("output_file", "rb");
        if (out_file) {
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use fd to avoid unused variable warning
            assert_file_closed(&out_file);;
        }
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}