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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 59
//<Quality> {"density":20,"unique_branches":{"write_mb_features":[[360,7,360,30,0,0,4,0],[360,34,360,63,0,0,4,0],[362,7,362,13,0,0,4,0],[366,7,366,13,0,0,4,1],[370,7,370,13,0,0,4,1],[374,7,374,13,0,0,4,1],[380,7,380,14,0,0,4,1]],"validate_config":[[125,12,125,29,26,0,4,1],[125,33,125,49,26,0,4,0]],"vp8_new_framerate":[[1294,7,1294,31,0,0,4,0],[1294,35,1294,58,0,0,4,1]],"vp8_encode_frame":[[906,7,906,31,0,0,4,0],[906,35,906,65,0,0,4,0],[916,9,916,18,0,0,4,0],[922,11,922,24,0,0,4,0],[928,11,928,24,0,0,4,1],[933,19,933,44,0,0,4,0],[933,19,933,44,0,0,4,1],[934,13,934,46,0,0,4,1]],"write_kfmodes":[[591,11,591,51,0,0,4,0]],"vp8_get_compressed_data":[[4862,7,4862,42,0,0,4,1],[5130,7,5130,37,0,0,4,0],[5138,7,5138,31,0,0,4,0]],"vp8_pack_bitstream":[[1075,7,1075,31,0,0,4,0],[1080,9,1080,40,0,0,4,0],[1086,19,1086,33,0,0,4,0],[1086,19,1086,33,0,0,4,1],[1088,21,1088,40,0,0,4,0],[1088,21,1088,40,0,0,4,1],[1092,15,1092,19,0,0,4,0],[1092,15,1092,19,0,0,4,1],[1095,17,1095,25,0,0,4,0],[1109,9,1109,39,0,0,4,0],[1111,19,1111,44,0,0,4,0],[1111,19,1111,44,0,0,4,1],[1114,13,1114,24,0,0,4,1],[1147,13,1147,43,0,0,4,0],[1244,7,1244,37,0,0,4,0]],"cyclic_background_refresh":[[509,7,509,36,0,0,4,1],[534,7,534,42,0,0,4,1]],"vp8cx_mb_init_quantizer":[[298,7,298,31,0,0,4,0],[300,9,300,52,0,0,4,1],[309,16,309,29,0,0,4,0],[309,33,309,49,0,0,4,0],[321,7,321,18,0,0,4,1],[321,22,321,42,0,0,4,1],[387,14,387,59,0,0,4,1],[388,14,388,59,0,0,4,1],[389,14,389,53,0,0,4,1]],"encode_frame_to_data_rate":[[3774,7,3774,39,0,0,4,0],[3777,10,3777,44,0,0,4,1],[3778,9,3778,32,0,0,4,0],[3778,36,3778,57,0,0,4,0],[3778,61,3778,75,0,0,4,0],[4370,7,4370,38,0,0,4,1],[4438,7,4438,37,0,0,4,0],[4724,7,4724,38,0,0,4,1]],"encode_mb_row":[[426,9,426,33,0,0,4,0],[430,11,430,57,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000020_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, 
                               const vpx_image_rect_t* rect2) {
    // Empty callback for demonstration
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

// Frame buffer callbacks
static int  id_000020_get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    // Allocate buffer
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000020_release_frame_buffer_callback(void* cb_priv,
                                         vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful processing
    
    // Create a FILE* for input using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read encoded data from input file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    std::vector<uint8_t> encoded_data(input_size);
    fread(encoded_data.data(), 1, input_size, in_file);
    assert_file_closed(&in_file);;
    
    // PART 1: DECODING
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                 &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for decoder
    res = vpx_codec_set_frame_buffer_functions(&decoder,
                                               id_000020_get_frame_buffer_callback,
                                               id_000020_release_frame_buffer_callback,
                                              nullptr);
    
    // Get stream info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    res = vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, encoded_data.data(), encoded_data.size(),
                          nullptr, 0);
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_frame) {
        // No frame decoded, but continue with encoding part
        // Create a dummy image for encoding
        const int width = 320;
        const int height = 240;
        const int stride_align = 1;
        unsigned char* img_data = (unsigned char*)malloc(width * height * 3 / 2);
        
        vpx_image_t dummy_img;
        vpx_img_wrap(&dummy_img, VPX_IMG_FMT_I420, width, height,
                    stride_align, img_data);
        
        // PART 2: ENCODING
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
        vpx_codec_enc_cfg_t enc_cfg;
        
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        enc_cfg.g_w = width;
        enc_cfg.g_h = height;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 1;
        
        // Try both init functions
        vpx_rational_t dsf = {1, 1};
        res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg,
                                          1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
        
        if (res != VPX_CODEC_OK) {
            // Fall back to single encoder init
            res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg,
                                        0, VPX_ENCODER_ABI_VERSION);
        }
        
        if (res == VPX_CODEC_OK) {
            // Set control parameters
            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
            vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 1);
            vpx_codec_control_(&encoder, VP8E_SET_ARNR_MAXFRAMES, 7);
            vpx_codec_control_(&encoder, VP8E_SET_ARNR_STRENGTH, 5);
            vpx_codec_control_(&encoder, VP8E_SET_ARNR_TYPE, 3);
            
            // Register put slice callback
            vpx_codec_register_put_slice_cb(&encoder,  id_000020_put_slice_callback, nullptr);
            
            // Prepare output buffer for encoded data
            vpx_fixed_buf_t output_buffer;
            output_buffer.buf = malloc(size * 2);
            output_buffer.sz = size * 2;
            
            // Set external buffer for encoded data
            vpx_codec_set_cx_data_buf(&encoder, &output_buffer, 0, 0);
            
            // Encode the dummy image
            res = vpx_codec_encode(&encoder, &dummy_img, 0, 1, 0, 0);
            
            // Get global headers
            vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
            
            // Get encoded data packets
            const vpx_codec_cx_pkt_t* pkt;
            vpx_codec_iter_t enc_iter = nullptr;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Clean up encoder resources
            free(output_buffer.buf);
            if (global_headers) {
                free(global_headers->buf);
            }
            vpx_codec_destroy(&encoder);
            
            // Free dummy image data
            free(img_data);
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        
        // Close output file
        assert_file_closed(&out_file);;
        
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // We have a decoded frame - use it for encoding
    const int width = decoded_frame->d_w;
    const int height = decoded_frame->d_h;
    
    // PART 2: ENCODING with decoded frame
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 1;
    
    // Try both init functions
    vpx_rational_t dsf = {1, 1};
    res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg,
                                      1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        // Fall back to single encoder init
        res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg,
                                    0, VPX_ENCODER_ABI_VERSION);
    }
    
    if (res == VPX_CODEC_OK) {
        // Set control parameters
        vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
        vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 1);
        vpx_codec_control_(&encoder, VP8E_SET_ARNR_MAXFRAMES, 7);
        vpx_codec_control_(&encoder, VP8E_SET_ARNR_STRENGTH, 5);
        vpx_codec_control_(&encoder, VP8E_SET_ARNR_TYPE, 3);
        
        // Register put slice callback
        vpx_codec_register_put_slice_cb(&encoder,  id_000020_put_slice_callback, nullptr);
        
        // Prepare output buffer for encoded data
        vpx_fixed_buf_t output_buffer;
        output_buffer.buf = malloc(size * 2);
        output_buffer.sz = size * 2;
        
        // Set external buffer for encoded data
        vpx_codec_set_cx_data_buf(&encoder, &output_buffer, 0, 0);
        
        // Encode the decoded frame
        res = vpx_codec_encode(&encoder, decoded_frame, 0, 1, 0, 0);
        
        // Get global headers
        vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
        
        // Get encoded data packets
        const vpx_codec_cx_pkt_t* pkt;
        vpx_codec_iter_t enc_iter = nullptr;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Get error details if any
        const char* error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail; // Use variable to avoid warning
        
        // Clean up encoder resources
        free(output_buffer.buf);
        if (global_headers) {
            free(global_headers->buf);
        }
        vpx_codec_destroy(&encoder);
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // Close output file
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}