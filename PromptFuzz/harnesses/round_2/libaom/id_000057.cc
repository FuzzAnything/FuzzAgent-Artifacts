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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_img_wrap","aom_img_set_rect","aom_img_plane_width","aom_img_plane_height","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_img_wrap","aom_img_set_rect","aom_img_plane_width","aom_img_plane_height","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
	return 0;
	}
	  // Need minimal data for meaningful fuzzing
  
  // Create input FILE* from fuzzer data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
  
  // 1. Get decoder interface name
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  const char *decoder_name = aom_codec_iface_name(decoder_iface);
  (void)decoder_name; // Prevent unused variable warning
  
  // 2. Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.allow_lowbitdepth = 1;
  
  aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                              AOM_DECODER_ABI_VERSION);
  if (err != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }
  
  // 3. Decode input data
  err = aom_codec_decode(&decoder, data, size, nullptr);
  
  // 4. Get stream information
  aom_codec_stream_info_t stream_info;
  stream_info.is_annexb = 0;
  if (aom_codec_get_stream_info(&decoder, &stream_info) == AOM_CODEC_OK) {
    // Use stream info to validate dimensions
    (void)stream_info.w;
    (void)stream_info.h;
  }
  
  // 5. Get decoded frame
  aom_codec_iter_t iter = nullptr;
  aom_image_t *decoded_image = aom_codec_get_frame(&decoder, &iter);
  
  // 6. Process image if decoding was successful
  if (decoded_image) {
    // Get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    (void)encoder_name;
    
    // 7. Create a test image for encoding
    aom_image_t test_image;
    unsigned char img_data[100 * 100 * 3] = {0}; // Small buffer
    aom_img_wrap(&test_image, AOM_IMG_FMT_I420, 100, 100, 1, img_data);
    
    // 8. Set rectangle on test image
    unsigned int border = 2;
    if (test_image.w > 10 && test_image.h > 10) {
      aom_img_set_rect(&test_image, 5, 5, 10, 10, border);
    }
    
    // 9. Get plane dimensions
    int plane_width = aom_img_plane_width(&test_image, 0);
    int plane_height = aom_img_plane_height(&test_image, 0);
    (void)plane_width;
    (void)plane_height;
    
    // 10. Add metadata to test image
    uint8_t metadata_data[] = {1, 2, 3, 4};
    aom_metadata_t *metadata = aom_img_metadata_alloc(1, metadata_data, 
                                                     sizeof(metadata_data),
                                                     AOM_MIF_ANY_FRAME);
    if (metadata) {
      aom_img_add_metadata(&test_image, metadata->type, metadata->payload, 
                          metadata->sz, metadata->insert_flag);
      aom_img_metadata_free(metadata);
    }
    
    // 11. Initialize encoder with default config
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err == AOM_CODEC_OK) {
      enc_cfg.g_w = 100;
      enc_cfg.g_h = 100;
      enc_cfg.g_threads = 1;
      enc_cfg.rc_end_usage = AOM_Q;
      enc_cfg.rc_min_quantizer = 0;
      enc_cfg.rc_max_quantizer = 63;
      enc_cfg.g_error_resilient = 0;
      enc_cfg.g_lag_in_frames = 0;
      
      err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                   AOM_ENCODER_ABI_VERSION);
      
      if (err == AOM_CODEC_OK) {
        // 12. Get global headers
        aom_fixed_buf_t *headers = aom_codec_get_global_headers(&encoder);
        if (headers) {
          free(headers->buf);
          free(headers);
        }
        
        // 13. Set cx data buffer
        uint8_t cx_buffer[1024];
        aom_fixed_buf_t cx_buf = {cx_buffer, sizeof(cx_buffer)};
        aom_codec_set_cx_data_buf(&encoder, &cx_buf, 0, 0);
        
        // Clean up encoder
        aom_codec_destroy(&encoder);
      }
    }
    
    // Clean up test image
    aom_img_free(&test_image);
  }
  
  // 14. Clean up decoder
  aom_codec_destroy(&decoder);
  
  // Close file
  assert_file_closed(&in_file);;
  
  assert_file_closed(&in_file);
	return 0;
}