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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_set_cx_data_buf","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_set_cx_data_buf","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/



// Dummy callback function for put_slice
static void  id_000035_dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *clipping) {
  // Do nothing - just for API usage
  (void)user_priv;
  (void)img;
  (void)slice;
  (void)clipping;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
    return 0;
  }

  // Create a FILE* for reading input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
  (void)in_fd; // Use if needed

  // Step 1: Get codec interfaces
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
  
  if (!encoder_iface || !decoder_iface) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 2: Get codec capabilities
  vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
  vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
  (void)encoder_caps;
  (void)decoder_caps;

  // Step 3: Peek stream info from input data
  vpx_codec_stream_info_t stream_info;
  memset(&stream_info, 0, sizeof(stream_info));
  stream_info.sz = sizeof(stream_info);
  
  vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
      decoder_iface, data, static_cast<unsigned int>(size), &stream_info);
  (void)peek_result;

  // Step 4: Initialize encoder
  vpx_codec_ctx_t encoder;
  memset(&encoder, 0, sizeof(encoder));
  
  vpx_codec_enc_cfg_t encoder_config;
  vpx_codec_enc_config_default(encoder_iface, &encoder_config, 0);
  
  // Set some reasonable values for fuzzing
  encoder_config.g_w = 64;
  encoder_config.g_h = 64;
  encoder_config.g_timebase.num = 1;
  encoder_config.g_timebase.den = 30;
  encoder_config.rc_target_bitrate = 256;
  encoder_config.g_error_resilient = 0;
  
  vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
      &encoder, encoder_iface, &encoder_config, 0, VPX_ENCODER_ABI_VERSION);
  
  if (enc_init_result != VPX_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 5: Register put slice callback
  vpx_codec_register_put_slice_cb(&encoder,  id_000035_dummy_put_slice_cb, nullptr);

  // Step 6: Create an image for encoding
  vpx_image_t *image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                      encoder_config.g_w, 
                                      encoder_config.g_h, 16);
  if (!image) {
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Fill image with some data from input
  size_t bytes_to_copy = size;
  if (bytes_to_copy > static_cast<size_t>(image->stride[0] * image->d_h)) {
    bytes_to_copy = static_cast<size_t>(image->stride[0] * image->d_h);
  }
  
  if (bytes_to_copy > 0) {
    // Read from input file into image plane
    fread(image->planes[0], 1, bytes_to_copy, in_file);
    rewind(in_file); // Rewind for potential reuse
  }

  // Step 7: Encode the image
  vpx_codec_encode(&encoder, image, 0, 1, 0, VPX_DL_REALTIME);

  // Step 8: Get encoded data
  vpx_codec_iter_t iter = nullptr;
  const vpx_codec_cx_pkt_t *pkt = nullptr;
  
  while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      // Step 9: Set cx data buffer
      vpx_fixed_buf_t buf;
      buf.buf = const_cast<void*>(pkt->data.frame.buf);
      buf.sz = pkt->data.frame.sz;
      
      vpx_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
      break;
    }
  }

  // Step 10: Initialize decoder
  vpx_codec_ctx_t decoder;
  memset(&decoder, 0, sizeof(decoder));
  
  vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
      &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
  
  if (dec_init_result == VPX_CODEC_OK) {
    // Step 11: Decode the original input data
    vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), 
                     nullptr, 0);

    // Step 12: Get decoded frames
    vpx_codec_iter_t dec_iter = nullptr;
    vpx_image_t *decoded_img = nullptr;
    
    while ((decoded_img = vpx_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
      // Step 13: Wrap the decoded image
      vpx_image_t wrapped_img;
      memset(&wrapped_img, 0, sizeof(wrapped_img));
      
      vpx_img_wrap(&wrapped_img, decoded_img->fmt, 
                   decoded_img->d_w, decoded_img->d_h, 
                   16, decoded_img->img_data);
      
      // Use the wrapped image for something
      (void)wrapped_img;
    }

    // Step 14: Try some control operations
    int ctrl_id = 0;
    vpx_codec_control_(&decoder, ctrl_id);
    vpx_codec_control_(&encoder, ctrl_id);

    // Clean up decoder
    vpx_codec_destroy(&decoder);
  }

  // Step 15: Clean up resources
  vpx_img_free(image);
  vpx_codec_destroy(&encoder);
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}