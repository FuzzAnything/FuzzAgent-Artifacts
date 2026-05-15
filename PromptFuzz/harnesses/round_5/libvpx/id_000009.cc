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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback for put_slice (required to use the API)
static void  id_000009_dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *update) {
  // Do nothing - just a placeholder to use the API
  (void)user_priv;
  (void)img;
  (void)slice;
  (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // 1. Get version strings (use required APIs)
  const char *version_str = vpx_codec_version_str();
  const char *build_config = vpx_codec_build_config();
  (void)version_str;  // Mark as used
  (void)build_config; // Mark as used
  
  // Create a FILE* for input data (as required)
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Create a FILE* for output data (as required)
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }
  
  // Get file descriptors (as required, though not strictly needed)
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  (void)in_fd;  // Mark as used
  (void)out_fd; // Mark as used
  
  // 2. Initialize decoder
  vpx_codec_ctx_t decoder;
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
  
  // Check decoder capabilities
  vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
  (void)caps; // Mark as used
  
  // Initialize decoder with default settings
  vpx_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
  
  if (res != VPX_CODEC_OK) {
    // Get error detail (use required API)
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail; // Mark as used
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // 3. Initialize encoder
  vpx_codec_ctx_t encoder;
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t enc_cfg;
  
  // Get default encoder configuration
  res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  if (res != VPX_CODEC_OK) {
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // Set some basic encoder parameters
  enc_cfg.g_w = 640;
  enc_cfg.g_h = 480;
  enc_cfg.g_timebase.num = 1;
  enc_cfg.g_timebase.den = 30;
  enc_cfg.rc_target_bitrate = 1000;
  enc_cfg.g_pass = VPX_RC_ONE_PASS;
  
  // Initialize encoder
  res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // 4. Register put_slice callback (use required API)
  res = vpx_codec_register_put_slice_cb(&encoder,  id_000009_dummy_put_slice_cb, nullptr);
  (void)res; // Result may fail if encoder doesn't support this, that's OK
  
  // 5. Get global headers (use required API)
  vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
  if (global_headers && global_headers->buf && global_headers->sz > 0) {
    fwrite(global_headers->buf, 1, global_headers->sz, out_file);
  }
  
  // 6. Allocate an image (use required API)
  vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 640, 480, 1);
  
  // 7. Decode input data
  res = vpx_codec_decode(&decoder, data, size, nullptr, 0);
  
  // Get error detail if decode failed (use required API)
  if (res != VPX_CODEC_OK) {
    const char *decode_error = vpx_codec_error_detail(&decoder);
    (void)decode_error; // Mark as used
  }
  
  // 8. Get decoded frames
  vpx_codec_iter_t iter = nullptr;
  vpx_image_t *decoded_frame;
  
  while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
    // If we have a valid image allocated, copy the decoded frame
    if (img && decoded_frame) {
      // Simple copy of first plane (Y plane) for demonstration
      // In real usage, would need to copy all planes properly
      if (img->stride[0] == decoded_frame->stride[0] &&
          img->d_w == decoded_frame->d_w &&
          img->d_h == decoded_frame->d_h) {
        for (unsigned int i = 0; i < decoded_frame->d_h; ++i) {
          memcpy(img->planes[0] + i * img->stride[0],
                 decoded_frame->planes[0] + i * decoded_frame->stride[0],
                 decoded_frame->d_w);
        }
      }
      
      // 9. Encode the frame (use required API)
      res = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
      
      // Get encoded packets
      const vpx_codec_cx_pkt_t *pkt;
      vpx_codec_iter_t enc_iter = nullptr;
      
      while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
          fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
      }
    }
    
    // Break after first frame for fuzzing efficiency
    break;
  }
  
  // 10. Free allocated image (use required API)
  if (img) {
    vpx_img_free(img);
  }
  
  // 11. Clean up (use required APIs)
  vpx_codec_destroy(&decoder);
  vpx_codec_destroy(&encoder);
  
  // Close files
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  // Remove output file to clean up
  remove("output_file");
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}