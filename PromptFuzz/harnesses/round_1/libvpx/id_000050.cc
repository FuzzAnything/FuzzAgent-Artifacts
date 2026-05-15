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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_control_","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_control_","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
    return 0;
  }

  // Create a FILE pointer to read input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptor for input file
  int in_fd = fuzz_fileno(in_file);
  (void)in_fd; // Use to avoid unused variable warning

  // DECODING PATH: Decode VP9 video from input data
  vpx_codec_ctx_t decoder;
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
  vpx_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = 640;
  dec_cfg.h = 480;

  // Initialize decoder
  vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Decode the input data
  err = vpx_codec_decode(&decoder, data, size, nullptr, 0);
  if (err != VPX_CODEC_OK) {
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Get decoded frames
  vpx_codec_iter_t iter = nullptr;
  vpx_image_t *decoded_image = nullptr;
  while ((decoded_image = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
    // Use decoder control API
    int ctrl_id = VP8D_GET_LAST_REF_UPDATES;
    vpx_codec_control_(&decoder, ctrl_id);
    break; // Process only first frame for fuzzing
  }

  // ENCODING PATH: Re-encode the decoded frame
  if (decoded_image) {
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0; // Good quality

    // Get encoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    (void)caps; // Use to avoid unused variable warning

    // Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    if (err == VPX_CODEC_OK) {
      // Configure encoder settings
      enc_cfg.g_w = decoded_image->d_w;
      enc_cfg.g_h = decoded_image->d_h;
      enc_cfg.g_timebase.num = 1;
      enc_cfg.g_timebase.den = 30;
      enc_cfg.rc_target_bitrate = 1000;
      enc_cfg.g_pass = VPX_RC_ONE_PASS;

      // Initialize encoder
      err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        // Set encoder configuration
        err = vpx_codec_enc_config_set(&encoder, &enc_cfg);

        // Allocate image for encoder input
        vpx_image_t *enc_image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                                                decoded_image->d_w,
                                                decoded_image->d_h, 16);
        if (enc_image) {
          // Copy data from decoded image (simplified - actual conversion needed)
          // This is a simplified copy - real code would need proper format conversion
          if (decoded_image->fmt == VPX_IMG_FMT_I420 &&
              enc_image->fmt == VPX_IMG_FMT_I420) {
            for (int plane = 0; plane < 3; plane++) {
              int src_stride = decoded_image->stride[plane];
              int dst_stride = enc_image->stride[plane];
              int height = (plane == 0) ? decoded_image->d_h 
                          : decoded_image->d_h / 2;
              int width = (plane == 0) ? decoded_image->d_w 
                          : decoded_image->d_w / 2;
              
              for (int y = 0; y < height; y++) {
                memcpy(enc_image->planes[plane] + y * dst_stride,
                       decoded_image->planes[plane] + y * src_stride,
                       width);
              }
            }

            // Flip image vertically
            vpx_img_flip(enc_image);

            // Encode the image
            err = vpx_codec_encode(&encoder, enc_image, 0, 1, 0, 0);
            (void)err; // Use to avoid unused variable warning

            // Get preview frame from encoder
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
            (void)preview; // Use to avoid unused variable warning

            // Free allocated image
            vpx_img_free(enc_image);
          }
        }

        // Destroy encoder
        vpx_codec_destroy(&encoder);
      }
    }
  }

  // Clean up decoder
  vpx_codec_destroy(&decoder);
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}