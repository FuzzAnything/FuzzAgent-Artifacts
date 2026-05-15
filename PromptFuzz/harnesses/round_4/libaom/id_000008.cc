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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode and re-encode AV1 video:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




  // Early return if data is too small
  if (size < 100) {
	return 0;
	}
	
  
  // Get libaom version (API 1)
  int version = aom_codec_version();
  (void)version; // Mark as used to avoid compiler warning
  
  // Create input file from fuzzer data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
  
  // Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = 128;
  dec_cfg.h = 128;
  dec_cfg.allow_lowbitdepth = 1;
  
  // Initialize AV1 decoder (API 2)
  if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, fuzz_int64_t_1, 
                             AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Read data into buffer
  std::vector<uint8_t> buffer(size);
  fread(buffer.data(), 1, size, in_file);
  assert_file_closed(&in_file);;
  
  // Decode the input data (API 5)
  aom_codec_decode(&decoder, buffer.data(), size, nullptr);
  
  // Get decoded frame (API 4)
  aom_codec_iter_t iter = nullptr;
  const aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
  
  if (decoded_img) {
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, fuzz_uint32_t_2) != AOM_CODEC_OK) {
      aom_codec_destroy(&decoder);
      assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_threads = 1;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &enc_cfg, fuzz_int64_t_3, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
      aom_codec_destroy(&decoder);
      assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration (API 3)
    aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Create output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.sz = size * 2; // Allocate enough space
    output_buf.buf = malloc(output_buf.sz);
    
    if (output_buf.buf) {
      // Set output buffer for encoder (API 6)
      aom_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_4, fuzz_uint32_t_5);
      
      // Create a copy of the decoded image data for encoding
      size_t img_data_size = decoded_img->sz;
      unsigned char *img_data = (unsigned char *)malloc(img_data_size);
      if (img_data) {
        // Copy image planes
        for (int plane = 0; plane < 3; plane++) {
          if (decoded_img->planes[plane]) {
            int plane_width = aom_img_plane_width(decoded_img, fuzz_int32_t_6);
            int plane_height = aom_img_plane_height(decoded_img, fuzz_int32_t_7);
            size_t plane_size = plane_width * plane_height;
            
            if (plane == 0) {
              memcpy(img_data, decoded_img->planes[plane], plane_size);
            }
          }
        }
        
        // Wrap image data (API 1)
        aom_image_t img_to_encode;
        if (aom_img_wrap(&img_to_encode, decoded_img->fmt, 
                        decoded_img->d_w, decoded_img->d_h,
                        fuzz_uint32_t_8, img_data) != nullptr) {
          
          // Copy strides
          for (int i = 0; i < 3; i++) {
            img_to_encode.stride[i] = decoded_img->stride[i];
          }
          
          // Encode the frame
          aom_codec_encode(&encoder, &img_to_encode, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11);
          
          // Get encoded data
          aom_codec_iter_t enc_iter = nullptr;
          const aom_codec_cx_pkt_t *pkt;
          while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
              // Successfully encoded frame
              break;
            }
          }
          
          // Free wrapped image (doesn't free img_data)
        }
        
        free(img_data);
      }
      
      free(output_buf.buf);
    }
    
    // Destroy encoder
    aom_codec_destroy(&encoder);
  }
  
  // Destroy decoder
  aom_codec_destroy(&decoder);
  
  assert_file_closed(&in_file);
	return 0;
}