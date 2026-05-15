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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"init_seq_coding_tools":[[604,7,604,37,0,0,4,0],[604,41,604,66,0,0,4,0]]},"library_calls":["aom_uleb_encode","aom_codec_err_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_codec_err_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve an event where encoded video data is processed, including ULEB encoding of metadata, image manipulation, and encoding:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_16, fdp);
	//fuzzer shim end}




  if (size < 32) {
	return 0;
	}
	  // Need minimum data for basic operations

  // 1. ULEB encoding of a value from input data
  uint64_t uleb_value = 0;
  if (size >= sizeof(uint64_t)) {
    memcpy(&uleb_value, data, sizeof(uint64_t));
  }
  
  uint8_t coded_value[16];
  size_t coded_size = 0;
  int uleb_result = aom_uleb_encode(fuzz_uint64_t_1, sizeof(coded_value), 
                                     coded_value, &coded_size);
  // Use error string for ULEB encoding result
  const char* uleb_error_str = aom_codec_err_to_string(
    static_cast<aom_codec_err_t>(uleb_result > 0 ? 0 : AOM_CODEC_ERROR));

  // 2. Create and initialize encoder context
  aom_codec_ctx_t encoder;
  aom_codec_iface_t* iface = aom_codec_av1_cx();
  
  // Configure encoder
  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_2);
  cfg.g_w = 64;
  cfg.g_h = 64;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 256;
  cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;

  // Initialize encoder
  aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder, iface, &cfg, 
                                                          fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION);
  const char* init_error_str = aom_codec_err_to_string(enc_init_result);
  if (enc_init_result != AOM_CODEC_OK) {
    return 0;  // Can't proceed without encoder
  }

  // 3. Create and manipulate image
  aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint32_t_6);
  if (!img) {
    aom_codec_destroy(&encoder);
    return 0;
  }

  // Fill image planes with data from input
  size_t data_offset = 0;
  for (int plane = 0; plane < 3; ++plane) {
    int plane_width = aom_img_plane_width(img, fuzz_int32_t_7);
    int plane_height = aom_img_plane_height(img, fuzz_int32_t_8);
    size_t plane_size = plane_width * plane_height;
    
    if (data_offset + plane_size <= size) {
      for (int y = 0; y < plane_height; ++y) {
        size_t copy_size = std::min<size_t>(plane_width, size - data_offset);
        if (copy_size > 0) {
          memcpy(&img->planes[plane][y * img->stride[plane]], 
                 &data[data_offset], copy_size);
          data_offset += copy_size;
        }
      }
    }
  }

  // Set a rectangular region on the image using input dimensions
  unsigned int rect_x = 0;
  unsigned int rect_y = 0;
  unsigned int rect_w = 32;
  unsigned int rect_h = 32;
  
  if (size >= 4) {
    rect_x = data[0] % 32;
    rect_y = data[1] % 32;
    rect_w = 8 + (data[2] % 24);
    rect_h = 8 + (data[3] % 24);
  }
  
  int set_rect_result = aom_img_set_rect(img, fuzz_uint32_t_9, fuzz_uint32_t_10, fuzz_uint32_t_11, fuzz_uint32_t_12, fuzz_uint32_t_13);

  // 4. Encode the image
  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;
  aom_enc_frame_flags_t flags = 0;
  
  // Use the ULEB encoded value as PTS if encoding was successful
  if (uleb_result == 0) {
    pts = static_cast<aom_codec_pts_t>(uleb_value);
  }
  
  aom_codec_err_t encode_result = aom_codec_encode(&encoder, img, fuzz_int64_t_14, 
                                                   fuzz_uint64_t_15, fuzz_int64_t_16);
  const char* encode_error_str = aom_codec_err_to_string(encode_result);

  // 5. Get encoded data
  aom_codec_iter_t iter = nullptr;
  const aom_codec_cx_pkt_t* pkt = nullptr;
  while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      // Process encoded frame if needed
      break;
    }
  }

  // 6. Cleanup
  aom_img_free(img);
  aom_codec_destroy(&encoder);

  // Force usage of error strings to prevent optimization
  volatile const char* dummy1 = uleb_error_str;
  volatile const char* dummy2 = init_error_str;
  volatile const char* dummy3 = encode_error_str;
  (void)dummy1;
  (void)dummy2;
  (void)dummy3;

  return 0;
}