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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_global_headers","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc_with_border","aom_img_set_rect","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_err_to_string","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_err_to_string","aom_codec_error_detail","aom_codec_get_global_headers","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_alloc_with_border","aom_img_set_rect","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Custom frame buffer callbacks
static int  id_000037_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
  // Simple allocation - in real fuzzing we'd want more sophisticated handling
  fb->data = (uint8_t*)malloc(min_size);
  if (!fb->data) return -1;
  fb->size = min_size;
  return 0;
}

static int  id_000037_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
  if (fb->data) {
    free(fb->data);
    fb->data = nullptr;
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
	return 0;
	}
	
  
  // Initialize variables
  aom_codec_ctx_t codec_ctx;
  aom_codec_dec_cfg_t dec_cfg = {0};
  aom_codec_iter_t iter = nullptr;
  aom_image_t dummy_img;
  const aom_metadata_t *metadata = nullptr;
  aom_fixed_buf_t *global_headers = nullptr;
  const char *iface_name = nullptr;
  const char *build_config = nullptr;
  const char *error_detail = nullptr;
  const char *error_string = nullptr;
  aom_codec_err_t res;
  
  // 1. Get AV1 decoder interface
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) {
	return 0;
	}
	
  
  // 2. Get codec interface name
  iface_name = aom_codec_iface_name(iface);
  
  // 3. Get build configuration
  build_config = aom_codec_build_config();
  
  // 4. Initialize decoder with default configuration
  dec_cfg.threads = 1;
  dec_cfg.w = 640;
  dec_cfg.h = 480;
  dec_cfg.allow_lowbitdepth = 1;
  
  res = aom_codec_dec_init_ver(&codec_ctx, iface, &dec_cfg, 0, 
                               AOM_DECODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
    error_string = aom_codec_err_to_string(res);
    error_detail = aom_codec_error_detail(&codec_ctx);
    aom_codec_destroy(&codec_ctx);
    return 0;
  }
  
  // 5. Set frame buffer functions
  res = aom_codec_set_frame_buffer_functions(&codec_ctx, 
                                             id_000037_get_frame_buffer,
                                             id_000037_release_frame_buffer,
                                            nullptr);
  if (res != AOM_CODEC_OK) {
    error_string = aom_codec_err_to_string(res);
  }
  
  // 6. Try to set compressed data buffer (may fail for decoder, that's OK)
  aom_fixed_buf_t cx_buf;
  cx_buf.buf = (void*)data;
  cx_buf.sz = size;
  res = aom_codec_set_cx_data_buf(&codec_ctx, &cx_buf, 0, 0);
  // Don't check error - this is expected to fail for decoder
  
  // 7. Decode the input data
  res = aom_codec_decode(&codec_ctx, data, size, nullptr);
  if (res != AOM_CODEC_OK) {
    error_string = aom_codec_err_to_string(res);
    error_detail = aom_codec_error_detail(&codec_ctx);
  }
  
  // 8. Try to get global headers (may return nullptr for decoder)
  global_headers = aom_codec_get_global_headers(&codec_ctx);
  
  // 9. Get decoded frames
  aom_image_t *img;
  while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
    // 10. Get image plane dimensions
    int plane_width = aom_img_plane_width(img, 0);
    int plane_height = aom_img_plane_height(img, 0);
    
    // 11. Try to get metadata (if any)
    if (aom_img_num_metadata(img) > 0) {
      metadata = aom_img_get_metadata(img, 0);
    }
    
    // 12. Create a dummy image and set rectangle
    // Allocate dummy image with border
    aom_image_t *dummy = aom_img_alloc_with_border(&dummy_img, 
                                                   AOM_IMG_FMT_I420,
                                                   100, 100,  // dimensions
                                                   16,        // align
                                                   16,        // size_align
                                                   8);        // border
    if (dummy) {
      // Try to set a rectangle on the dummy image
      // Use fuzz data to determine rectangle parameters, but clamp to valid ranges
      unsigned int x = data[0] % 50;
      unsigned int y = data[1 % size] % 50;
      unsigned int w = 10 + (data[2 % size] % 40);
      unsigned int h = 10 + (data[3 % size] % 40);
      
      aom_img_set_rect(dummy, x, y, w, h, 4);
      aom_img_free(dummy);
    }
  }
  
  // 13. Clean up
  aom_codec_destroy(&codec_ctx);
  
  // Clean up global headers if allocated
  if (global_headers && global_headers->buf) {
    free(global_headers->buf);
  }
  
  return 0;
}