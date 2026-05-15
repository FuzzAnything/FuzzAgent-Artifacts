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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 117
//<Quality> {"density":9,"unique_branches":{"vp8_update_rate_correction_factors":[[1105,5,1105,11,0,0,4,0],[1107,5,1107,11,0,0,4,1]],"recode_loop_test":[[2830,7,2830,33,0,0,4,0],[2835,10,2835,50,0,0,4,1],[2836,10,2836,49,0,0,4,0],[2836,53,2836,63,0,0,4,0],[2836,53,2836,63,0,0,4,1],[2840,14,2840,62,0,0,4,1]],"vp8cx_encode_intra_macroblock":[[1098,21,1098,47,0,0,4,0]],"vp8_get_compressed_data":[[5030,7,5030,33,0,0,4,1],[5103,7,5103,33,0,0,4,1]],"check_reset_2nd_coeffs":[[364,7,364,27,0,0,4,0],[364,7,364,27,0,0,4,1],[364,31,364,51,0,0,4,0],[366,15,366,29,0,0,4,0],[366,15,366,29,0,0,4,1],[368,12,368,23,0,0,4,0],[368,12,368,23,0,0,4,1],[369,9,369,18,0,0,4,0],[369,9,369,18,0,0,4,1],[379,7,379,15,0,0,4,0],[380,17,380,31,0,0,4,0],[380,17,380,31,0,0,4,1]],"vp8_regular_quantize_b_sse4_1":[[111,9,111,14,0,0,4,0],[111,9,111,14,0,0,4,1]],"vp8_optimize_mbuv":[[469,7,469,30,0,0,4,1],[471,7,471,29,0,0,4,1],[479,16,479,22,0,0,4,0],[479,16,479,22,0,0,4,1]],"optimize_b":[[188,7,188,65,0,0,4,0],[202,17,202,25,0,0,4,0],[202,17,202,25,0,0,4,1],[210,9,210,10,0,0,4,0],[210,9,210,10,0,0,4,1],[219,11,219,20,0,0,4,0],[219,11,219,20,0,0,4,1],[227,11,227,31,0,0,4,0],[227,11,227,31,0,0,4,1],[236,40,236,44,0,0,4,0],[236,40,236,44,0,0,4,1],[237,34,237,38,0,0,4,0],[237,34,237,38,0,0,4,1],[246,11,246,58,0,0,4,0],[246,11,246,58,0,0,4,1],[247,11,247,76,0,0,4,0],[253,11,253,19,0,0,4,0],[253,11,253,19,0,0,4,1],[259,11,259,13,0,0,4,0],[259,11,259,13,0,0,4,1],[264,13,264,51,0,0,4,0],[264,13,264,51,0,0,4,1],[266,13,266,51,0,0,4,0],[266,13,266,51,0,0,4,1],[270,11,270,20,0,0,4,0],[270,11,270,20,0,0,4,1],[272,13,272,32,0,0,4,0],[272,13,272,32,0,0,4,1],[276,13,276,32,0,0,4,0],[276,13,276,32,0,0,4,1],[284,11,284,31,0,0,4,0],[284,11,284,31,0,0,4,1],[292,11,292,19,0,0,4,0],[292,11,292,19,0,0,4,1],[296,40,296,44,0,0,4,0],[296,40,296,44,0,0,4,1],[297,34,297,38,0,0,4,0],[297,34,297,38,0,0,4,1],[299,28,299,32,0,0,4,0],[299,28,299,32,0,0,4,1],[313,11,313,30,0,0,4,0],[317,11,317,30,0,0,4,0],[317,11,317,30,0,0,4,1],[338,7,338,27,0,0,4,0],[338,7,338,27,0,0,4,1],[344,18,344,25,0,0,4,0],[344,18,344,25,0,0,4,1],[346,9,346,10,0,0,4,0],[346,9,346,10,0,0,4,1]],"pick_quickcompress_mode":[[818,7,818,15,0,0,4,1],[841,7,841,34,0,0,4,1],[843,14,843,50,0,0,4,1],[845,14,845,49,0,0,4,1]],"vp8_optimize_mby":[[436,7,436,30,0,0,4,1],[438,7,438,29,0,0,4,1],[446,20,446,67,0,0,4,0],[447,20,447,68,0,0,4,0],[448,10,448,23,0,0,4,0],[450,15,450,21,0,0,4,0],[450,15,450,21,0,0,4,1],[454,7,454,20,0,0,4,0]],"vp8_rd_pick_intra_mode":[[2397,7,2397,28,0,0,4,0],[2397,7,2397,28,0,0,4,1]],"vp8_encode_intra16x16mbuv":[[115,7,115,18,0,0,4,0]],"rd_pick_intra4x4mby_modes":[[610,15,610,21,0,0,4,1],[616,9,616,42,0,0,4,0],[637,7,637,35,0,0,4,1]],"rd_pick_intra16x16mby_mode":[[657,24,657,39,0,0,4,0],[657,24,657,39,0,0,4,1],[670,9,670,26,0,0,4,0],[670,9,670,26,0,0,4,1]],"vp8_encode_frame":[[684,7,684,33,0,0,4,1]],"vp8_encode_intra16x16mby":[[95,7,95,18,0,0,4,0]],"encode_frame_to_data_rate":[[4099,14,4101,44,0,0,4,0],[4110,11,4110,61,0,0,4,1],[4164,13,4164,41,0,0,4,0],[4166,20,4166,31,0,0,4,0],[4174,13,4174,27,0,0,4,1],[4196,15,4196,37,0,0,4,0],[4207,15,4207,65,0,0,4,1],[4212,19,4212,31,0,0,4,1],[4212,35,4212,75,0,0,4,1],[4224,11,4224,21,0,0,4,1],[4226,18,4226,27,0,0,4,1],[4232,11,4232,50,0,0,4,1],[4233,13,4233,53,0,0,4,1],[4244,9,4244,18,0,0,4,0],[4250,12,4250,21,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_err_to_string","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_err_to_string","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


// Forward declaration for frame buffer callbacks
struct vpx_codec_frame_buffer;
typedef struct vpx_codec_frame_buffer vpx_codec_frame_buffer_t;

// Callback functions for frame buffer allocation
static int  id_000042_get_frame_buffer(void* cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    uint8_t* buf = (uint8_t*)malloc(min_size);
    if (!buf) return -1;
    fb->data = buf;
    fb->size = min_size;
    fb->priv = buf;
    return 0;
}

static int  id_000042_release_frame_buffer(void* cb_priv, 
                               vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    if (fb->priv) {
        free(fb->priv);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Limit input size to prevent excessive memory usage
    if (size < 10 || size > 10000) {
        return 0;
    }
    
    // Try both VP8 and VP9 decoder interfaces
    vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    vpx_codec_stream_info_t si;
    vpx_codec_err_t res = VPX_CODEC_UNSUP_BITSTREAM;
    
    // Try to peek stream info with each decoder interface
    for (int i = 0; i < 2; i++) {
        si.sz = sizeof(si);
        res = vpx_codec_peek_stream_info(decoder_ifaces[i], data, 
                                         (unsigned int)std::min(size, (size_t)1000), &si);
        
        // Get interface name for debugging
        const char* iface_name = vpx_codec_iface_name(decoder_ifaces[i]);
        (void)iface_name; // Prevent unused warning
        
        // Get capabilities
        vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_ifaces[i]);
        (void)caps; // Prevent unused warning
        
        if (res == VPX_CODEC_OK) {
            break;
        }
        
        // Convert error to string (demonstrate API usage)
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str; // Prevent unused warning
    }
    
    // If we couldn't identify the stream, try encoder interfaces
    vpx_codec_iface_t* encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    for (int i = 0; i < 2; i++) {
        // Get interface name
        const char* enc_iface_name = vpx_codec_iface_name(encoder_ifaces[i]);
        (void)enc_iface_name;
        
        // Get encoder capabilities
        vpx_codec_caps_t enc_caps = vpx_codec_get_caps(encoder_ifaces[i]);
        (void)enc_caps;
    }
    
    // Create a decoder context
    vpx_codec_ctx_t decoder;
    memset(&decoder, 0, sizeof(decoder));
    
    // Try to initialize decoder with VP8 (simplest case)
    res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(), NULL, 0, 
                                 VPX_DECODER_ABI_VERSION);
    
    if (res == VPX_CODEC_OK) {
        // Set frame buffer functions
        res = vpx_codec_set_frame_buffer_functions(&decoder, 
                                                    id_000042_get_frame_buffer,
                                                    id_000042_release_frame_buffer,
                                                   NULL);
        
        // Convert any error to string
        const char* setfb_err = vpx_codec_err_to_string(res);
        (void)setfb_err;
        
        // Create encoder context
        vpx_codec_ctx_t encoder;
        memset(&encoder, 0, sizeof(encoder));
        
        vpx_codec_enc_cfg_t cfg;
        vpx_rational_t dsf = {1, 1}; // No downsampling
        
        // Get default encoder config
        vpx_codec_err_t cfg_res = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), 
                                                              &cfg, 0);
        if (cfg_res == VPX_CODEC_OK) {
            // Set basic configuration
            cfg.g_w = 640;
            cfg.g_h = 480;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.rc_target_bitrate = 256;
            
            // Initialize multi-encoder (num_enc = 1 for single encoder)
            res = vpx_codec_enc_init_multi_ver(&encoder, vpx_codec_vp8_cx(),
                                              &cfg, 1, 0, &dsf,
                                              VPX_ENCODER_ABI_VERSION);
            
            // Convert error to string if any
            const char* enc_init_err = vpx_codec_err_to_string(res);
            (void)enc_init_err;
            
            if (res == VPX_CODEC_OK) {
                // Create a dummy image for encoding
                vpx_image_t img;
                memset(&img, 0, sizeof(img));
                
                // Allocate image with random data from fuzzer input
                unsigned int align = 16;
                vpx_img_alloc(&img, VPX_IMG_FMT_I420, 
                             cfg.g_w, cfg.g_h, align);
                
                if (img.planes[0]) {
                    // Fill image planes with fuzzer data
                    size_t bytes_to_copy = std::min(size, 
                        (size_t)(img.stride[0] * cfg.g_h * 3 / 2));
                    
                    if (bytes_to_copy > 0) {
                        memcpy(img.planes[0], data, 
                               std::min(bytes_to_copy, size));
                    }
                    
                    // Encode the image
                    vpx_codec_encode(&encoder, &img, 0, 1, 0, 0);
                    
                    // Free the image
                    vpx_img_free(&img);
                }
                
                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
        
        // Destroy decoder
        vpx_codec_destroy(&decoder);
    }
    
    // Create output file (as per instructions)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to demonstrate file usage
        if (size > 0) {
            fwrite(data, 1, std::min(size, (size_t)100), out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Also demonstrate input file usage with fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file descriptor (as per instructions)
        int fd = fuzz_fileno(in_file);
        (void)fd; // Prevent unused warning
        
        // Read and discard some data
        uint8_t buf[100];
        fread(buf, 1, std::min(size, (size_t)100), in_file);
        
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}