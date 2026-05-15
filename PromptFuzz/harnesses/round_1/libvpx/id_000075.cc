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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 168, nr_unique_branch: 62
//<Quality> {"density":21,"unique_branches":{"vp8_build_inter16x16_predictors_mb":[[319,7,319,35,0,0,4,0],[348,7,348,35,0,0,4,0]],"evaluate_inter_mode":[[486,7,486,27,0,0,4,1],[486,31,486,59,0,0,4,1],[487,7,487,34,0,0,4,1]],"encode_mvcomponent":[[22,17,22,22,0,0,4,0],[24,7,24,22,0,0,4,1],[28,9,28,11,0,0,4,1],[36,14,36,21,0,0,4,0],[36,14,36,21,0,0,4,1],[42,14,42,21,0,0,4,0],[42,14,42,21,0,0,4,1],[44,9,44,19,0,0,4,0],[44,9,44,19,0,0,4,1]],"vp8_write_mvprobs":[[316,7,316,15,0,0,4,0],[316,19,316,27,0,0,4,0]],"vp8_find_near_mvs":[[41,9,41,30,0,0,4,0],[53,9,53,29,0,0,4,0],[60,11,60,39,0,0,4,0],[60,11,60,39,0,0,4,1],[73,9,73,34,0,0,4,0],[80,11,80,39,0,0,4,0],[80,11,80,39,0,0,4,1],[92,7,92,36,0,0,4,0],[94,9,94,51,0,0,4,0],[94,9,94,51,0,0,4,1],[103,7,103,65,0,0,4,0]],"calc_prob":[[162,10,162,11,0,0,4,1]],"vp8_find_best_sub_pixel_step_iteratively":[[266,12,266,53,0,0,4,0],[269,12,269,53,0,0,4,0],[272,12,272,53,0,0,4,0]],"vp8_diamond_search_sadx4":[[1231,17,1231,39,0,0,4,0],[1257,17,1257,34,0,0,4,0],[1267,9,1267,31,0,0,4,0]],"update":[[175,7,175,27,0,0,4,0]],"vp8_pick_inter_mode":[[827,13,827,59,0,0,4,0],[1000,15,1000,38,0,0,4,0],[1002,15,1002,38,0,0,4,0],[1039,20,1039,37,0,0,4,0],[1042,19,1042,24,0,0,4,0],[1042,19,1042,24,0,0,4,1],[1048,21,1048,38,0,0,4,0],[1048,21,1048,38,0,0,4,1],[1184,9,1185,45,0,0,4,1]],"update_mvcount":[[382,7,382,48,0,0,4,0],[391,9,391,21,0,0,4,0],[391,25,391,41,0,0,4,0],[391,45,391,57,0,0,4,0],[391,61,391,77,0,0,4,0]],"vp8_get_inter_mbpred_error":[[157,7,157,24,0,0,4,0]],"vp8_sixtap_predict16x16_ssse3":[[239,7,239,14,0,0,4,0],[239,7,239,14,0,0,4,1],[240,9,240,16,0,0,4,0],[240,9,240,16,0,0,4,1],[252,9,252,16,0,0,4,0]],"vp8_build_component_cost_table":[[126,7,126,18,0,0,4,1],[139,7,139,18,0,0,4,1]],"vp8_sixtap_predict8x8_ssse3":[[272,7,272,14,0,0,4,0],[272,7,272,14,0,0,4,1],[273,9,273,16,0,0,4,0],[273,9,273,16,0,0,4,1],[282,9,282,16,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_flip","vpx_codec_encode","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_flip","vpx_codec_encode","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


// Callback function for frame output
static void  id_000075_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This is just a stub callback for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful fuzzing
    
    // 1. First use the input data as VP8 encoded stream to get stream info
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get codec interface for VP8 encoder
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Mark as used
    
    // 3. Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create encoder context
    vpx_codec_ctx_t codec;
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&codec);
        (void)error_detail; // Mark as used
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Set encoder configuration
    res = vpx_codec_enc_config_set(&codec, &cfg);
    (void)res; // Result may or may not be OK for fuzzing
    
    // 6. Call control API (set CPU used to 0 for fastest encoding)
    vpx_codec_control_(&codec, VP8E_SET_CPUUSED, 0);
    
    // 7. Get global headers
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&codec);
    (void)headers; // Mark as used
    
    // 8. Register put frame callback
    vpx_codec_register_put_frame_cb(&codec,  id_000075_put_frame_callback, NULL);
    
    // 9. Create image buffers for encoding
    // Allocate first image
    vpx_image_t *img1 = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 320, 240, 1);
    
    // Wrap second image using input data
    // Use part of input data as image data (ensure we don't overflow)
    size_t img_data_size = 320 * 240 * 3 / 2; // I420 format size
    unsigned char *img_data = (unsigned char *)malloc(img_data_size);
    if (img_data) {
        // Copy from input data, truncate if needed
        size_t copy_size = (size < img_data_size) ? size : img_data_size;
        memcpy(img_data, data, copy_size);
        
        vpx_image_t *img2 = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, 320, 240, 1, img_data);
        
        // 10. Set rectangle on image
        vpx_img_set_rect(img2, 0, 0, 160, 120);
        
        // 11. Encode images
        // Encode first image (allocated)
        if (img1) {
            // Fill image with some data from input
            size_t offset = 0;
            for (int i = 0; i < 3; i++) {
                if (offset < size) {
                    size_t plane_size = img1->stride[i] * (i == 0 ? 240 : 120);
                    size_t copy_len = (size - offset < plane_size) ? size - offset : plane_size;
                    if (copy_len > 0) {
                        memcpy(img1->planes[i], data + offset, copy_len);
                        offset += copy_len;
                    }
                }
            }
            
            vpx_codec_encode(&codec, img1, 0, 1, 0, VPX_DL_REALTIME);
            
            // 12. Get encoded data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process packet (just for fuzzing)
                (void)pkt;
            }
        }
        
        // Encode second image (wrapped)
        vpx_codec_encode(&codec, img2, 1, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data again
        vpx_codec_iter_t iter2 = NULL;
        const vpx_codec_cx_pkt_t *pkt2;
        while ((pkt2 = vpx_codec_get_cx_data(&codec, &iter2)) != NULL) {
            // Process packet (just for fuzzing)
            (void)pkt2;
        }
        
        // Free wrapped image
        vpx_img_free(img2);
        free(img_data);
    }
    
    // 13. Flip image (if we had one)
    if (img1) {
        vpx_img_flip(img1);
        
        // Encode flipped image
        vpx_codec_encode(&codec, img1, 2, 1, 0, VPX_DL_REALTIME);
        
        // Free allocated image
        vpx_img_free(img1);
    }
    
    // 14. Cleanup
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}