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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 117, nr_unique_branch: 15
//<Quality> {"density":13,"unique_branches":{"choose_partitioning":[[1677,7,1677,22,0,0,4,0],[1683,9,1683,22,0,0,4,1],[1690,14,1690,27,0,0,4,1]],"vp9_rc_update_rate_correction_factors":[[825,9,825,48,0,0,4,0]],"block_yrd":[[742,20,742,45,0,0,4,1],[744,20,744,46,0,0,4,1],[785,11,785,30,0,0,4,1],[836,11,836,30,0,0,4,1]],"set_vt_partitioning":[[502,9,502,32,0,0,4,1],[512,9,512,53,0,0,4,1]],"vp9_tokenize_sb":[[479,21,479,30,0,0,4,1]],"update_coef_probs_common":[[591,11,591,25,0,0,4,0]],"vp9_encode_block_intra":[[1037,11,1037,26,0,0,4,1],[1045,11,1045,26,0,0,4,1]],"estimate_block_intra":[[1083,7,1083,17,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000059_get_frame_buffer(void *cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = (uint8_t*)malloc(4096); // Simple allocation
    fb->size = 4096;
    return fb->data ? 0 : -1;
}

static int  id_000059_release_frame_buffer(void *cb_priv, 
                               vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
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
    
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    const vpx_image_t *preview_frame = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Initialize stream info
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try to decode as VP9 first, then VP8
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, dec_iface, nullptr, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        dec_iface = vpx_codec_vp8_dx();
        res = vpx_codec_dec_init_ver(&decoder, dec_iface, nullptr, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
    }
    
    if (res == VPX_CODEC_OK) {
        // Set frame buffer functions
        vpx_codec_set_frame_buffer_functions(&decoder,  id_000059_get_frame_buffer, 
                                            id_000059_release_frame_buffer, nullptr);
        
        // Read and decode the input data
        std::vector<uint8_t> buffer(size);
        fread(buffer.data(), 1, size, in_file);
        
        vpx_codec_decode(&decoder, buffer.data(), buffer.size(), nullptr, fuzz_int64_t_3);
        
        // Get stream information
        vpx_codec_get_stream_info(&decoder, &stream_info);
        
        // Get preview frame
        preview_frame = vpx_codec_get_preview_frame(&decoder);
        
        // Initialize encoder with same codec type
        vpx_codec_iface_t *enc_iface = (dec_iface == vpx_codec_vp9_dx()) ? 
                                      vpx_codec_vp9_cx() : vpx_codec_vp8_cx();
        
        if (vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_4) == VPX_CODEC_OK) {
            // Set encoder configuration
            if (preview_frame) {
                enc_cfg.g_w = preview_frame->d_w;
                enc_cfg.g_h = preview_frame->d_h;
            } else {
                enc_cfg.g_w = 640;
                enc_cfg.g_h = 480;
            }
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            // Initialize encoder
            if (vpx_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, fuzz_int64_t_5, 
                                      VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                
                // Get global headers
                global_headers = vpx_codec_get_global_headers(&encoder);
                if (global_headers && global_headers->buf) {
                    fwrite(global_headers->buf, 1, global_headers->sz, out_file);
                }
                
                // Create an image for encoding
                if (!preview_frame) {
                    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                       enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_6);
                    if (img) {
                        // Fill with dummy data
                        memset(img->planes[0], 128, img->stride[0] * img->d_h);
                        memset(img->planes[1], 128, img->stride[1] * img->d_h / 2);
                        memset(img->planes[2], 128, img->stride[2] * img->d_h / 2);
                    }
                }
                
                // Encode frame
                vpx_codec_encode(&encoder, preview_frame ? preview_frame : img, 
                               fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
                
                // Get encoded data
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    // Cleanup
    if (img) vpx_img_free(img);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}