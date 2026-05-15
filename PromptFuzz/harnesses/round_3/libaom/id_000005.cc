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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"read_tile_info_max_tile":[[2195,48,2195,65,0,0,4,1]]},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_set_rect","aom_img_plane_height","aom_codec_control","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_set_rect","aom_img_plane_height","aom_codec_control","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
    return 0;
  }

  // 1. Get version info
  const char *extra_version = aom_codec_version_extra_str();
  (void)extra_version; // Use to avoid unused variable warning

  // 2. Create a FILE* from input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // 3. Peek at stream info using the AV1 decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  aom_codec_stream_info_t stream_info;
  memset(&stream_info, 0, sizeof(stream_info));
  
  // Read some data for peeking
  uint8_t peek_buffer[4096];
  size_t peek_size = fread(peek_buffer, 1, sizeof(peek_buffer), in_file);
  fseek(in_file, 0, SEEK_SET); // Reset for later use
  
  if (peek_size > 0) {
    aom_codec_err_t peek_err = aom_codec_peek_stream_info(
        decoder_iface, peek_buffer, peek_size, &stream_info);
    (void)peek_err; // Result may be error if data invalid
  }

  // 4. Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  
  aom_codec_err_t init_err = aom_codec_dec_init_ver(
      &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
  
  if (init_err != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // 5. Get stream info using decoder context
  aom_codec_stream_info_t stream_info2;
  memset(&stream_info2, 0, sizeof(stream_info2));
  aom_codec_get_stream_info(&decoder, &stream_info2);

  // 6. Decode the input data
  // Read the entire input for decoding
  uint8_t *decode_data = (uint8_t *)malloc(size);
  if (!decode_data) {
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }
  
  size_t read_size = fread(decode_data, 1, size, in_file);
  if (read_size > 0) {
    aom_codec_decode(&decoder, decode_data, read_size, nullptr);
  }

  // 7. Try to get a frame (optional, for completeness)
  aom_codec_iter_t iter = nullptr;
  aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);

  // 8. Create an image for testing image APIs
  const unsigned int img_width = 64;
  const unsigned int img_height = 64;
  const unsigned int align = 16;
  
  aom_image_t *test_img = aom_img_alloc(
      nullptr, AOM_IMG_FMT_I420, img_width, img_height, align);
  
  if (test_img) {
    // 9. Set a rectangle on the image
    unsigned int rect_x = 10;
    unsigned int rect_y = 10;
    unsigned int rect_w = 20;
    unsigned int rect_h = 20;
    unsigned int border = 0;
    
    aom_img_set_rect(test_img, rect_x, rect_y, rect_w, rect_h, border);
    
    // 10. Get plane height
    int plane = 0; // Y plane
    int plane_height = aom_img_plane_height(test_img, plane);
    (void)plane_height; // Use to avoid unused variable warning
  }

  // 11. Use codec control with a simple control ID
  // Use AOMD_GET_LAST_REF_UPDATES (control ID 9) which is safe for decoder
  int ctrl_id = 9; // AOMD_GET_LAST_REF_UPDATES
  int ref_update = 0;
  aom_codec_control(&decoder, ctrl_id, &ref_update);

  // 12. Initialize encoder config for testing
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_enc_cfg_t enc_cfg;
  
  // Usage 0 = GOOD_QUALITY
  aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  
  // 13. Create output file for testing
  FILE *out_file = fopen("output_file", "wb");
  if (out_file) {
    // Write some data if we have a decoded image
    if (decoded_img && test_img) {
      // Write minimal data - just for API demonstration
      uint8_t dummy_data[] = {0x01, 0x02, 0x03};
      fwrite(dummy_data, 1, sizeof(dummy_data), out_file);
    }
    assert_file_closed(&out_file);;
  }

  // 14. Cleanup
  if (test_img) {
    aom_img_free(test_img);
  }
  
  free(decode_data);
  aom_codec_destroy(&decoder);
  assert_file_closed(&in_file);;
  
  // Remove output file if created
  remove("output_file");

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}