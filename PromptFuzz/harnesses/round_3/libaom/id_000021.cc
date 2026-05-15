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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_destroy","aom_img_alloc_with_border","aom_img_alloc","aom_img_free","aom_img_free","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 16) {
    return 0;
  }

  // Create a FILE* for input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Open output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 1: Parse stream info from input data
  aom_codec_stream_info_t stream_info;
  memset(&stream_info, 0, sizeof(stream_info));
  
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  aom_codec_err_t peek_result = aom_codec_peek_stream_info(
      decoder_iface, data, size, &stream_info);
  
  // Step 2: Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  
  aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
      &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
  
  if (dec_init_result != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 3: Decode the input frame
  aom_codec_err_t decode_result = aom_codec_decode(
      &decoder, data, size, NULL);
  
  // Get stream info from decoder context
  aom_codec_stream_info_t decoder_stream_info;
  memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
  aom_codec_err_t get_stream_info_result = aom_codec_get_stream_info(
      &decoder, &decoder_stream_info);

  // Step 4: Get decoded frame
  aom_codec_iter_t iter = NULL;
  aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
  
  if (!decoded_img) {
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Create images for encoding
  unsigned int width = decoded_img->d_w;
  unsigned int height = decoded_img->d_h;
  
  // Allocate image with border
  aom_image_t *img_with_border = aom_img_alloc_with_border(
      NULL, AOM_IMG_FMT_I420, width, height, 1, 1, 16);
  
  // Allocate regular image
  aom_image_t *regular_img = aom_img_alloc(
      NULL, AOM_IMG_FMT_I420, width, height, 1);
  
  if (!img_with_border || !regular_img) {
    if (img_with_border) aom_img_free(img_with_border);
    if (regular_img) aom_img_free(regular_img);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 6: Copy decoded data to regular image
  for (int plane = 0; plane < 3; ++plane) {
    int plane_width = aom_img_plane_width(decoded_img, plane);
    int plane_height = aom_img_plane_height(decoded_img, plane);
    
    for (int y = 0; y < plane_height; ++y) {
      memcpy(regular_img->planes[plane] + y * regular_img->stride[plane],
             decoded_img->planes[plane] + y * decoded_img->stride[plane],
             plane_width);
    }
  }

  // Step 7: Set rectangle in image with border
  // Use safe rectangle dimensions based on image size
  unsigned int rect_x = width > 32 ? 16 : 0;
  unsigned int rect_y = height > 32 ? 16 : 0;
  unsigned int rect_w = width > 64 ? 32 : (width / 2);
  unsigned int rect_h = height > 64 ? 32 : (height / 2);
  
  aom_img_set_rect(img_with_border, rect_x, rect_y, rect_w, rect_h, 8);

  // Step 8: Initialize encoder
  aom_codec_ctx_t encoder;
  aom_codec_enc_cfg_t enc_cfg;
  
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_err_t enc_cfg_result = aom_codec_enc_config_default(
      encoder_iface, &enc_cfg, 0);
  
  if (enc_cfg_result != AOM_CODEC_OK) {
    aom_img_free(img_with_border);
    aom_img_free(regular_img);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Configure encoder
  enc_cfg.g_w = width;
  enc_cfg.g_h = height;
  enc_cfg.g_timebase.num = 1;
  enc_cfg.g_timebase.den = 30;
  enc_cfg.g_threads = 1;
  enc_cfg.rc_end_usage = AOM_Q;
  enc_cfg.rc_min_quantizer = 20;
  enc_cfg.rc_max_quantizer = 40;
  
  aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
      &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
  
  if (enc_init_result != AOM_CODEC_OK) {
    aom_img_free(img_with_border);
    aom_img_free(regular_img);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 9: Encode the regular image
  aom_codec_err_t encode_result = aom_codec_encode(
      &encoder, regular_img, 0, 1, 0);
  
  // Step 10: Get encoded data
  aom_codec_iter_t enc_iter = NULL;
  const aom_codec_cx_pkt_t *pkt = NULL;
  
  while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
  }

  // Step 11: Check for metadata and free if present
  size_t metadata_count = aom_img_num_metadata(regular_img);
  for (size_t i = 0; i < metadata_count; ++i) {
    const aom_metadata_t *metadata = aom_img_get_metadata(regular_img, i);
    if (metadata) {
      // Allocate a copy to free (since we can't free const pointer)
      aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
          metadata->type, metadata->payload, metadata->sz, metadata->insert_flag);
      aom_img_metadata_free(metadata_copy);
    }
  }

  // Cleanup
  aom_img_free(img_with_border);
  aom_img_free(regular_img);
  aom_codec_destroy(&encoder);
  aom_codec_destroy(&decoder);
  
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}