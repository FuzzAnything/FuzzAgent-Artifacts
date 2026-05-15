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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 4
//<Quality> {"density":26,"unique_branches":{"vpx_img_set_rect":[[194,28,194,43,0,0,4,1]],"recode_loop_test":[[2836,10,2836,49,0,0,4,1]],"check_reset_2nd_coeffs":[[364,7,364,27,0,0,4,0],[364,31,364,51,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_control_","vpx_codec_error","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_control_","vpx_codec_error","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":1}
/**/


// Include necessary libvpx headers

// Dummy callback function for vpx_codec_register_put_slice_cb
static void  id_000045_dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *rect1,
                               const vpx_image_rect_t *rect2) {
  // Do nothing - dummy callback
  (void)user_priv;
  (void)img;
  (void)rect1;
  (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
    return 0;
  }

  // Create encoder and decoder interfaces
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
  
  // Get capabilities and names
  vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
  vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
  const char *encoder_name = vpx_codec_iface_name(encoder_iface);
  const char *decoder_name = vpx_codec_iface_name(decoder_iface);
  (void)encoder_caps;
  (void)decoder_caps;
  (void)encoder_name;
  (void)decoder_name;

  // Initialize decoder context
  vpx_codec_ctx_t decoder_ctx;
  vpx_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  vpx_codec_err_t dec_init_res = vpx_codec_dec_init_ver(
      &decoder_ctx, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

  // Try to peek stream info
  vpx_codec_stream_info_t stream_info;
  stream_info.sz = sizeof(stream_info);
  vpx_codec_peek_stream_info(decoder_iface, data,
                              (unsigned int)(size > 1024 ? 1024 : size),
                              &stream_info);

  // Initialize encoder context
  vpx_codec_ctx_t encoder_ctx;
  vpx_codec_enc_cfg_t enc_cfg;
  vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  
  // Set encoder configuration based on stream info if available
  if (stream_info.w > 0 && stream_info.h > 0) {
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
  } else {
    // Use default values if stream info not available
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
  }
  enc_cfg.g_threads = 1;
  enc_cfg.rc_target_bitrate = 1000;
  enc_cfg.g_timebase.num = 1;
  enc_cfg.g_timebase.den = 30;

  vpx_codec_err_t enc_init_res = vpx_codec_enc_init_ver(
      &encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);

  // Allocate image for encoding
  vpx_image_t *img = vpx_img_alloc(
      nullptr, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 1);
  
  // Wrap some data in an image (dummy operation)
  uint8_t dummy_img_data[1000] = {0};
  vpx_image_t wrapped_img;
  vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 10, 10, 1, dummy_img_data);

  // Set rectangle on image (dummy operation)
  vpx_img_set_rect(img, 0, 0, 10, 10);

  // Register put slice callback
  vpx_codec_register_put_slice_cb(&encoder_ctx,  id_000045_dummy_put_slice_cb, nullptr);

  // Set output buffer for encoded data
  vpx_fixed_buf_t output_buf;
  output_buf.buf = malloc(10000);
  output_buf.sz = 10000;
  vpx_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, 0);

  // Try to decode input data if decoder initialized successfully
  if (dec_init_res == VPX_CODEC_OK) {
    vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, nullptr, 0);
    
    // Try to get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    // If we got a decoded frame, try to encode it
    if (decoded_frame && enc_init_res == VPX_CODEC_OK) {
      // Try to encode the frame
      vpx_codec_encode(&encoder_ctx, decoded_frame, 0, 1, 0, 0);
      
      // Get encoded data
      vpx_codec_iter_t enc_iter = nullptr;
      const vpx_codec_cx_pkt_t *pkt;
      while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
        // Process packet if needed
        (void)pkt;
      }
    }
  }

  // Try to encode a dummy frame even if decode failed
  if (enc_init_res == VPX_CODEC_OK && img) {
    vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, 0);
    
    // Get encoded data again
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
      // Process packet if needed
      (void)pkt;
    }
  }

  // Try to use control function (dummy operation with 0 as control ID)
  if (enc_init_res == VPX_CODEC_OK) {
    vpx_codec_control_(&encoder_ctx, 0);
  }
  if (dec_init_res == VPX_CODEC_OK) {
    vpx_codec_control_(&decoder_ctx, 0);
  }

  // Get error messages (even if no error)
  const char *enc_error = vpx_codec_error(&encoder_ctx);
  const char *dec_error = vpx_codec_error(&decoder_ctx);
  (void)enc_error;
  (void)dec_error;

  // Cleanup
  if (enc_init_res == VPX_CODEC_OK) {
    vpx_codec_destroy(&encoder_ctx);
  }
  if (dec_init_res == VPX_CODEC_OK) {
    vpx_codec_destroy(&decoder_ctx);
  }
  
  if (img) {
    vpx_img_free(img);
  }
  
  free(output_buf.buf);

  return 0;
}