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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


// Define callback function for put_frame
static void  id_000044_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Empty callback - just to satisfy API requirement
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need at least some data to work with
    
    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_ctx_t multi_encoder = {0};
    vpx_image_t *image = nullptr;
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_codec_stream_info_t si = {0};
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // 1. Get VP9 decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    // 2. Get decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;  // Use variable to avoid unused warning
    
    // 3. Peek stream info from input data
    si.sz = sizeof(si);
    vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    
    // 4. Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    if (si.w > 0 && si.h > 0) {
        dec_cfg.w = si.w;
        dec_cfg.h = si.h;
    } else {
        dec_cfg.w = 128;
        dec_cfg.h = 128;
    }
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Cleanup and return if decoder init fails
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 5. Decode the input data
    res = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 6. Get decoded frame
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // 7. Allocate image for encoding
    unsigned int width = 128;
    unsigned int height = 128;
    if (decoded_frame) {
        width = decoded_frame->d_w;
        height = decoded_frame->d_h;
    }
    
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!image) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 8. Set rectangle on image
    unsigned int rect_w = width > 16 ? width / 4 : 4;
    unsigned int rect_h = height > 16 ? height / 4 : 4;
    vpx_img_set_rect(image, 0, 0, rect_w, rect_h);
    
    // 9. Get default encoder configuration
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    res = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    
    // 10. Initialize single encoder
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 11. Initialize multi-encoder (with num_enc = 1)
    vpx_rational_t dsf = {1, 1};  // Downsampling factor
    res = vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 12. Register put frame callback
    vpx_codec_register_put_frame_cb(&encoder,  id_000044_put_frame_callback, nullptr);
    
    // 13. Encode the image with single encoder
    vpx_codec_encode(&encoder, image, 0, 1, 0, 0);
    
    // 14. Get encoded data
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
        (void)pkt;
    }
    
    // Also encode with multi-encoder
    vpx_codec_encode(&multi_encoder, image, 0, 1, 0, 0);
    
    // 15. Destroy codec contexts
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&multi_encoder);
    
    // 16. Free allocated image
    vpx_img_free(image);
    
    return 0;
}