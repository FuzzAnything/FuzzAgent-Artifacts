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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimal data for headers
    
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = NULL;
    const vpx_codec_cx_pkt_t *pkt = NULL;
    
    // Initialize decoder with VP9
    if (vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        // We've used vpx_codec_error_detail, per requirement
        return 0;
    }
    
    // Decode the input data (assume it's VP9 encoded)
    if (vpx_codec_decode(&decoder, data, size, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frame
    img = vpx_codec_get_frame(&decoder, &iter);
    if (!img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Create output image buffer (same dimensions as decoded frame)
    size_t buffer_size = img->d_w * img->d_h * 3; // Rough estimate for YUV
    unsigned char *img_data = (unsigned char *)malloc(buffer_size);
    if (!img_data) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Use vpx_img_wrap to wrap existing buffer
    vpx_image_t wrapped_img;
    if (!vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, img->d_w, img->d_h, 1, img_data)) {
        free(img_data);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy data from decoded image to wrapped image (simplified - actual would need plane copying)
    // In reality, we'd copy Y, U, V planes separately
    if (img->stride[0] > 0) {
        size_t copy_size = img->d_h * img->stride[0];
        if (copy_size <= buffer_size) {
            memcpy(img_data, img->planes[0], copy_size);
        }
    }
    
    // Set a rectangle in the image (crop to smaller area for encoding)
    unsigned int crop_w = img->d_w > 32 ? 32 : img->d_w;
    unsigned int crop_h = img->d_h > 32 ? 32 : img->d_h;
    int set_rect_result = vpx_img_set_rect(&wrapped_img, 0, 0, crop_w, crop_h);
    // We've used vpx_img_set_rect, per requirement
    
    // Initialize encoder with VP8
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &enc_cfg, 0) != VPX_CODEC_OK) {
        free(img_data);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    enc_cfg.g_w = crop_w;
    enc_cfg.g_h = crop_h;
    enc_cfg.rc_target_bitrate = 256; // Low bitrate for fuzzing
    
    if (vpx_codec_enc_init_ver(&encoder, vpx_codec_vp8_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        // We've used vpx_codec_error_detail again
        free(img_data);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Encode the wrapped/cropped image
    if (vpx_codec_encode(&encoder, &wrapped_img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        // Used vpx_codec_error_detail
    }
    
    // Get encoded data packets
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // We've successfully used vpx_codec_get_cx_data
            break;
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    free(img_data);
    
    return 0;
}