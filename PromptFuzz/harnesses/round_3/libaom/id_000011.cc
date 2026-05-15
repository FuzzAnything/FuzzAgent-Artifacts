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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc_with_border","aom_img_free","aom_codec_destroy","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc_with_border","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

  // 1. Check encoder capabilities
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_caps_t caps = aom_codec_get_caps(encoder_iface);
  
  // 2. Initialize encoder with configuration
  aom_codec_ctx_t encoder;
  aom_codec_enc_cfg_t enc_cfg;
  aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  
  // Use input data to determine some encoder parameters
  enc_cfg.g_w = (data[0] % 128 + 1) * 16;
  enc_cfg.g_h = (data[1] % 128 + 1) * 16;
  enc_cfg.rc_target_bitrate = (data[2] % 1000) + 100;
  
  aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
  
  // 3. Set encoder options based on input data
  char cpu_used[3];
  snprintf(cpu_used, sizeof(cpu_used), "%d", data[3] % 9);
  aom_codec_set_option(&encoder, "cpu-used", cpu_used);
  
  // 4. Create images using different methods
  aom_image_t img1;
  unsigned int width = enc_cfg.g_w;
  unsigned int height = enc_cfg.g_h;
  
  // Allocate image with border
  unsigned int border = data[4] % 32;
  aom_img_alloc_with_border(&img1, AOM_IMG_FMT_I420, width, height, 1, 1, border);
  
  // Wrap existing data as image
  size_t img_data_size = width * height * 3 / 2;
  if (img_data_size > size - 10) img_data_size = size - 10;
  
  unsigned char *img_data = static_cast<unsigned char*>(malloc(img_data_size));
  if (!img_data) {
    aom_img_free(&img1);
    aom_codec_destroy(&encoder);
    return 0;
  }
  
  memcpy(img_data, data + 10, std::min(img_data_size, size - 10));
  
  aom_image_t img2;
  aom_img_wrap(&img2, AOM_IMG_FMT_I420, width, height, 1, img_data);
  
  // 5. Add metadata to image
  uint32_t metadata_type = 1; // Example metadata type
  size_t metadata_size = std::min(size_t(100), size - 20);
  aom_img_add_metadata(&img1, metadata_type, data + 20, metadata_size, AOM_MIF_ANY_FRAME);
  
  // 6. Encode the image
  aom_codec_encode(&encoder, &img1, 0, 1, 0);
  
  // 7. Try to get preview frame
  const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
  
  // 8. Cleanup
  free(img_data);
  aom_img_free(&img1);
  // Note: img2 doesn't need freeing as it wraps existing data
  aom_codec_destroy(&encoder);
  
  return 0;
}