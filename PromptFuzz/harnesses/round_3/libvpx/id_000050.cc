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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 6
//<Quality> {"density":8,"unique_branches":{"update_initial_width":[[2368,9,2368,39,0,0,4,0]],"alloc_util_frame_buffers":[[1305,7,1311,49,0,0,4,0]],"encoder_encode":[[1461,7,1461,38,0,0,4,1],[1483,27,1483,70,0,0,4,0]],"vpx_realloc_frame_buffer":[[162,7,162,33,0,0,4,0],[162,37,162,65,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000050_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Do nothing - just satisfy API requirement
    (void)user_priv;
    (void)img;
}

// Callback function for put_slice
static void  id_000050_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, 
                               const vpx_image_rect_t* rect2) {
    // Do nothing - just satisfy API requirement
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t* img = nullptr;
    vpx_codec_stream_info_t si = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_codec_iter_t iter = nullptr;
    
    // Initialize decoder
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) goto cleanup;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_1, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        goto cleanup;
    }
    
    // Register callbacks
    vpx_codec_register_put_frame_cb(&decoder,  id_000050_put_frame_callback, nullptr);
    vpx_codec_register_put_slice_cb(&decoder,  id_000050_put_slice_callback, nullptr);
    
    // Decode input data
    if (vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&decoder);
        (void)error; // Use error to prevent unused warning
    }
    
    // Get stream info
    si.sz = sizeof(si);
    if (vpx_codec_get_stream_info(&decoder, &si) == VPX_CODEC_OK) {
        // Initialize encoder
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        if (encoder_iface) {
            // Get default config
            if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
                cfg.g_w = si.w > 0 ? si.w : 128;
                cfg.g_h = si.h > 0 ? si.h : 128;
                
                if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4,
                                          VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                    
                    // Allocate image
                    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_5);
                    if (img) {
                        // Set rectangle on image
                        vpx_img_set_rect(img, fuzz_uint32_t_6, fuzz_uint32_t_7, cfg.g_w, cfg.g_h);
                        
                        // Encode the image
                        if (vpx_codec_encode(&encoder, img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11) == VPX_CODEC_OK) {
                            // Get encoded data
                            const vpx_codec_cx_pkt_t* pkt;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                                // Process packet if needed
                                (void)pkt;
                            }
                        }
                        
                        // Free image
                        vpx_img_free(img);
                    }
                }
            }
        }
    }

cleanup:
    // Clean up resources
    if (encoder.iface) vpx_codec_destroy(&encoder);
    if (decoder.iface) vpx_codec_destroy(&decoder);
    
    return 0;
}