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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"vp9_set_row_mt":[[6857,35,6857,57,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_alloc","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback function for vpx_codec_register_put_slice_cb
static void  id_000008_dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
  (void)user_priv;
  (void)img;
  (void)valid;
  (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Early exit if input is too small
  if (size < 1) {
	return 0;
	}
	
  
  // 1. Get version strings (for coverage)
  const char *version_str = vpx_codec_version_str();
  const char *build_config = vpx_codec_build_config();
  (void)version_str;
  (void)build_config;
  
  // 2. Create decoder interface and check capabilities
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
  vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
  (void)decoder_caps;
  
  // 3. Initialize decoder
  vpx_codec_ctx_t decoder;
  vpx_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = 0;
  dec_cfg.h = 0;
  
  vpx_codec_err_t ret = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                0, VPX_DECODER_ABI_VERSION);
  if (ret != VPX_CODEC_OK) {
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail;
    return 0;
  }
  
  // 4. Decode the input data
  ret = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
  if (ret != VPX_CODEC_OK) {
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail;
    vpx_codec_destroy(&decoder);
    return 0;
  }
  
  // 5. Try to get decoded frame
  vpx_codec_iter_t iter = nullptr;
  vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
  
  // 6. Create encoder interface and get default config
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t enc_cfg;
  ret = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  if (ret != VPX_CODEC_OK) {
    vpx_codec_destroy(&decoder);
    return 0;
  }
  
  // 7. Allocate image for encoding (use decoded frame dimensions or defaults)
  unsigned int width = 320;
  unsigned int height = 240;
  if (decoded_img) {
    width = decoded_img->d_w;
    height = decoded_img->d_h;
  }
  
  vpx_image_t *img_to_encode = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
  if (!img_to_encode) {
    vpx_codec_destroy(&decoder);
    return 0;
  }
  
  // Fill the image with some data for encoding
  // Use input data to seed the image content
  size_t data_used = 0;
  for (unsigned int plane = 0; plane < 3; ++plane) {
    unsigned int plane_height = (plane == 0) ? height : height / 2;
    unsigned int plane_width = (plane == 0) ? width : width / 2;
    
    for (unsigned int y = 0; y < plane_height; ++y) {
      for (unsigned int x = 0; x < plane_width; ++x) {
        if (data_used < size) {
          img_to_encode->planes[plane][y * img_to_encode->stride[plane] + x] = 
            data[data_used++];
        } else {
          img_to_encode->planes[plane][y * img_to_encode->stride[plane] + x] = 
            128; // Default gray value
        }
      }
    }
  }
  
  // 8. Initialize encoder
  enc_cfg.g_w = width;
  enc_cfg.g_h = height;
  enc_cfg.rc_target_bitrate = 1000;
  enc_cfg.g_timebase.num = 1;
  enc_cfg.g_timebase.den = 30;
  
  vpx_codec_ctx_t encoder;
  ret = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
  if (ret != VPX_CODEC_OK) {
    vpx_img_free(img_to_encode);
    vpx_codec_destroy(&decoder);
    return 0;
  }
  
  // 9. Register dummy put_slice callback
  ret = vpx_codec_register_put_slice_cb(&encoder,  id_000008_dummy_put_slice_cb, nullptr);
  (void)ret; // Ignore error for fuzzing
  
  // 10. Get global headers (though not typically used for VP9)
  vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
  (void)global_headers;
  
  // 11. Encode the image
  ret = vpx_codec_encode(&encoder, img_to_encode, 0, 1, 0, 0);
  if (ret != VPX_CODEC_OK) {
    const char *error_detail = vpx_codec_error_detail(&encoder);
    (void)error_detail;
  }
  
  // 12. Cleanup
  vpx_img_free(img_to_encode);
  vpx_codec_destroy(&encoder);
  vpx_codec_destroy(&decoder);
  
  return 0;
}