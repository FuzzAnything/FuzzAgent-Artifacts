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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 20
//<Quality> {"density":19,"unique_branches":{"encode_frame_to_data_rate":[[3407,41,3407,75,0,0,4,0]],"pick_quickcompress_mode":[[833,14,833,38,0,0,4,0]],"vp8_change_config":[[1469,5,1469,26,0,0,4,0],[1473,11,1473,34,0,0,4,1],[1477,11,1477,33,0,0,4,1]],"calc_iframe_target_size":[[312,12,312,48,0,0,4,1],[324,13,324,55,0,0,4,0],[329,9,329,40,0,0,4,0],[336,11,336,33,0,0,4,1],[346,9,346,58,0,0,4,0],[352,9,352,22,0,0,4,0],[25,23,25,34,1,0,4,1],[24,23,24,34,2,0,4,1]],"vp8_set_speed_features":[[816,14,816,23,0,0,4,1],[819,9,819,18,0,0,4,1],[895,5,895,11,0,0,4,0],[897,11,897,20,0,0,4,1],[906,11,906,20,0,0,4,1],[916,11,916,20,0,0,4,1],[922,11,922,20,0,0,4,1]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to perform VP8 encoding with a slice callback:

*/


extern "C" {
}

// Dummy callback functions
static void  id_000061_dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *total) {}

static void  id_000061_dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {}

static int  id_000061_dummy_get_frame_buffer_cb(void *priv, size_t min_size,
                                     vpx_codec_frame_buffer_t *fb) {
  return -1; // Return error to use internal allocation
}

static int  id_000061_dummy_release_frame_buffer_cb(void *priv,
                                         vpx_codec_frame_buffer_t *fb) {
  return -1; // Return error
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Create a FILE* from input data using fmemopen
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  (void)in_fd;  // Mark as used
  (void)out_fd; // Mark as used

  // 1. Get version strings
  const char *version_str = vpx_codec_version_str();
  const char *build_config = vpx_codec_build_config();
  (void)version_str; // Mark as used
  (void)build_config; // Mark as used

  // 2. Get VP8 encoder interface
  vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
  if (!iface) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // 3. Initialize encoder configuration with default values
  vpx_codec_enc_cfg_t cfg;
  if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Set small dimensions to avoid excessive memory usage
  cfg.g_w = 16;
  cfg.g_h = 16;
  cfg.rc_target_bitrate = 256;
  cfg.g_threads = 1;
  cfg.g_pass = VPX_RC_ONE_PASS;

  // 4. Create encoder context
  vpx_codec_ctx_t codec;
  vpx_codec_err_t res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0,
                                               VPX_ENCODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // 5. Apply configuration
  if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // 6. Register callbacks
  if (vpx_codec_register_put_slice_cb(&codec,  id_000061_dummy_put_slice_cb,
                                      nullptr) != VPX_CODEC_OK) {
    // Continue anyway - callback registration might fail
  }

  if (vpx_codec_register_put_frame_cb(&codec,  id_000061_dummy_put_frame_cb,
                                      nullptr) != VPX_CODEC_OK) {
    // Continue anyway
  }

  if (vpx_codec_set_frame_buffer_functions(&codec,  id_000061_dummy_get_frame_buffer_cb,
                                            id_000061_dummy_release_frame_buffer_cb,
                                           nullptr) != VPX_CODEC_OK) {
    // Continue anyway
  }

  // 7. Create a test image
  vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 16, 16, 1);
  if (!img) {
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Fill image with some data from input
  size_t bytes_to_copy = size < 384 ? size : 384; // I420 needs 384 bytes for 16x16
  if (bytes_to_copy > 0 && img->img_data) {
    memcpy(img->img_data, data, bytes_to_copy);
  }

  // 8. First encode call
  if (vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
    // Continue to get error information
  }

  // 9. Get encoded data
  vpx_codec_iter_t iter = nullptr;
  const vpx_codec_cx_pkt_t *pkt;
  while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      // Write to output file
      fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
  }

  // 10. Second encode call (with different parameters)
  if (vpx_codec_encode(&codec, img, 1, 1, VPX_EFLAG_FORCE_KF,
                       VPX_DL_GOOD_QUALITY) != VPX_CODEC_OK) {
    // Continue
  }

  // 11. Get second batch of encoded data
  iter = nullptr;
  while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
  }

  // 12. Try to get preview frame
  const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
  (void)preview; // Mark as used

  // 13. Check for errors
  const char *error = vpx_codec_error(&codec);
  const char *error_detail = vpx_codec_error_detail(&codec);
  (void)error;      // Mark as used
  (void)error_detail; // Mark as used

  // 14. Cleanup
  vpx_img_free(img);
  vpx_codec_destroy(&codec);

  // Close files
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  // Remove output file
  remove("output_file");

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}