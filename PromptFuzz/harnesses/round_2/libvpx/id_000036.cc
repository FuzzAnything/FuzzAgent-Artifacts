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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"vpx_codec_register_put_slice_cb":[[158,15,158,18,0,0,4,0]],"vpx_codec_register_put_frame_cb":[[138,15,138,18,0,0,4,0]],"vpx_codec_set_frame_buffer_functions":[[178,15,178,22,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First get version and build info
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    (void)version_str;  // Prevent unused variable warning
    (void)build_config; // Prevent unused variable warning

    // 2. Create image from input data (simulate raw image input)
    // Use a fixed resolution - 64x64 I420 format (YUV 4:2:0)
    const unsigned int width = 64;
    const unsigned int height = 64;
    
    // Calculate required buffer size for I420 format
    // Y plane: width * height
    // U and V planes: (width/2) * (height/2) each
    size_t y_size = width * height;
    size_t uv_size = (width/2) * (height/2);
    size_t total_img_size = y_size + 2 * uv_size;
    
    if (size < total_img_size) {
        // Not enough data for image, skip
        return 0;
    }
    
    // Create image structure
    vpx_image_t raw_img;
    memset(&raw_img, 0, sizeof(raw_img));
    
    // Wrap input data as image
    vpx_image_t *img = vpx_img_wrap(&raw_img, VPX_IMG_FMT_I420, 
                                    width, height, 1, (unsigned char*)data);
    if (!img) {
        return 0;
    }
    
    // 3. Set image rectangle (process a sub-region)
    vpx_img_set_rect(img, 0, 0, width/2, height/2);
    
    // 4. Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
    
    // Initialize single encoder
    if (vpx_codec_enc_init_ver(&encoder, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Also test multi-encoder initialization (even though we won't use it fully)
    vpx_codec_ctx_t multi_encoder;
    vpx_rational_t dsf = {1, 1};
    vpx_codec_enc_init_multi_ver(&multi_encoder, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    // Don't use multi_encoder for actual encoding, just demonstrate API call
    vpx_codec_destroy(&multi_encoder);
    
    // 5. Set encoder configuration (update after init)
    cfg.rc_target_bitrate = 512;  // Change bitrate
    vpx_codec_enc_config_set(&encoder, &cfg);
    
    // 6. Register callbacks (use NULL callbacks for fuzzing)
    vpx_codec_register_put_slice_cb(&encoder, nullptr, nullptr);
    vpx_codec_register_put_frame_cb(&encoder, nullptr, nullptr);
    
    // 7. Register frame buffer functions (use NULL for fuzzing)
    vpx_codec_set_frame_buffer_functions(&encoder, nullptr, nullptr, nullptr);
    
    // 8. Get global headers
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&encoder);
    if (headers) {
        free(headers->buf);  // Free the buffer if allocated
        free(headers);       // Free the structure
    }
    
    // 9. Encode the image
    vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // 10. Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process different packet types
        switch (pkt->kind) {
            case VPX_CODEC_CX_FRAME_PKT:
                // Frame data available
                break;
            case VPX_CODEC_STATS_PKT:
                // Stats packet
                break;
            case VPX_CODEC_PSNR_PKT:
                // PSNR packet
                break;
        }
    }
    
    // 11. Get error information (even if no error, demonstrate API)
    const char *error = vpx_codec_error(&encoder);
    const char *error_detail = vpx_codec_error_detail(&encoder);
    (void)error;
    (void)error_detail;
    
    // 12. Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    
    return 0;
}